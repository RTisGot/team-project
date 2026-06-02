/**
 * @file follower.cpp
 * @brief お供キャラクターの制御処理
 *
 * プレイヤーの位置と向きを基準に、
 * 一定距離後方の目標地点を計算して追従する。
 *
 * 主な機能
 * ・MV1モデルの読み込みと解放
 * ・プレイヤーへの追従移動
 * ・プレイヤー右後方への位置補正
 * ・浮遊演出（上下移動）
 * ・プレイヤーと同じ向きへの回転
 * ・MV1モデルの描画
 */

#include <follower/follower.h>
#include <DxLib.h>
#include <math.h>

// お供キャラクター用定数
namespace
{
    constexpr float FOLLOW_DISTANCE = 3.0f; // プレイヤーとの距離
    constexpr float FOLLOW_HEIGHT = 15.0f;  // お供の高さ

    constexpr float FLOAT_AMPLITUDE = 2.0f; // 揺れ幅
    constexpr float FLOAT_SPEED = 0.005f;   // 揺れる速度

    constexpr float FOLLOW_SPEED = 1.0f;    // 追従速度
    constexpr float STOP_DISTANCE = 0.1f;   // 停止距離

    constexpr float SIDE_OFFSET = 5.0f;     // プレイヤー右後方へのオフセット

    constexpr float TELEPORT_DISTANCE = 50.0f;
}

/**
 * @class Follower
 * @brief プレイヤーを追従するお供キャラクター
 *
 * プレイヤーの位置と向きを受け取り、
 * 後方斜め位置へ移動する。
 *
 * 浮遊演出や向きの同期、
 * MV1モデルの描画も担当する。
 */
Follower::Follower()
{
    m_ModelHandle = -1;

    m_Position = VGet(250.0f, 350.0f, 0.0f);
    m_TargetPosition = m_Position;

    m_Angle = 0.0f;
    m_TargetAngle = 0.0f;
}

/**
 * @brief デストラクタ
 *
 * 読み込んだMV1モデルを解放し、
 * 使用していたリソースを破棄する。
 */
Follower::~Follower()
{
    // モデルが読み込まれている場合のみ解放
    if (m_ModelHandle != -1)
    {
        MV1DeleteModel(m_ModelHandle);
        m_ModelHandle = -1;
    }
}

/**
 * @brief お供モデルを読み込む
 *
 * MV1形式のモデルを読み込み、
 * モデルハンドルを取得する。
 * 既に読み込み済みの場合は処理を行わない。
 */
void Follower::LoadModel()
{
    // 既にモデルが読み込まれている場合は再読み込みしない
    if (m_ModelHandle != -1)
    {
        return;
    }
    // お供モデルを読み込む
    m_ModelHandle =
        MV1LoadModel("Game/assets/models/follower/cup.mv1");

    // モデル読み込み成功時の設定
    if (m_ModelHandle != -1)
    {
        // 必要ならサイズ調整
        // MV1SetScale(m_ModelHandle, VGet(0.5f, 0.5f, 0.5f));
    }
}

/**
 * @brief 追従対象の座標を設定する
 * @param pos プレイヤーの現在座標
 */
void Follower::SetTargetPosition(const VECTOR& pos)
{
    m_TargetPosition = pos;
}

/**
 * @brief 追従対象の向きを設定する
 * @param angle プレイヤーの向き(ラジアン)
 */
void Follower::SetTargetAngle(float angle)
{
    m_TargetAngle = angle;
}

/**
 * @brief 更新処理
 *
 * プレイヤーの位置と向きをもとに
 * お供の追従位置を計算する。
 *
 * また、ワープ機能や浮遊演出や向きの同期も行う。
 */
void Follower::Update()
{
    // 浮遊演出用の上下オフセットを計算
    float floatOffset =
        sinf(GetNowCount() * FLOAT_SPEED) *
        FLOAT_AMPLITUDE;

    // プレイヤーの後方位置を計算
    VECTOR targetPos =
    {
        // プレイヤー後方のX座標
        m_TargetPosition.x -
        sinf(m_TargetAngle) * FOLLOW_DISTANCE,

        // プレイヤーより少し高い位置+浮遊量
        m_TargetPosition.y +
        FOLLOW_HEIGHT +
        floatOffset,

        // プレイヤー後方のZ座標
        m_TargetPosition.z -
        cosf(m_TargetAngle) * FOLLOW_DISTANCE
    };

    // プレイヤーの右後ろへずらす
    targetPos.x +=
        cosf(m_TargetAngle) * SIDE_OFFSET;

    targetPos.z -=
        sinf(m_TargetAngle) * SIDE_OFFSET;

    // 現在位置から目標位置への方向ベクトルを計算
    VECTOR dir = VSub(targetPos, m_Position);

    // 目標地点までの距離を取得
    float distance = VSize(dir);

    // 一定距離以上離れた場合は瞬間移動
    if (distance >= TELEPORT_DISTANCE)
    {
        m_Position = targetPos;
        m_Angle = m_TargetAngle;
        return;
    }

    // 停止距離以上離れている場合のみ移動
    if (distance > STOP_DISTANCE)
    {
        // 方向ベクトルを正規化
        dir = VNorm(dir);

        // 目標地点を通り過ぎないよう移動量を制限
        float moveAmount = (distance < FOLLOW_SPEED) ? distance : FOLLOW_SPEED;
            
        // 目標地点へ向かって移動
        m_Position = VAdd(m_Position, VScale(dir, moveAmount));
    }
    m_Angle = m_TargetAngle;
}

/**
 * @brief 描画処理
 *
 * 現在の座標と向きをモデルへ反映し、
 * お供キャラクターを描画する。
 */
void Follower::Draw()
{
    // モデルが読み込まれていない場合は描画しない
    if (m_ModelHandle == -1)
    {
        return;
    }

    // モデルの座標を設定
    MV1SetPosition(m_ModelHandle, m_Position);

    // モデルの向きを設定
    MV1SetRotationXYZ(m_ModelHandle, VGet(0.0f, m_Angle, 0.0f));

    // モデルを描画
    MV1DrawModel(m_ModelHandle);
}
