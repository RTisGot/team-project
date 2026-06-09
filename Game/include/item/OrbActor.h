#ifndef ORB_ACTOR_H
#define ORB_ACTOR_H

#include "item/OrbData.h"
#include <memory>
#include <DxLib.h>

/**
 * @file OrbActor.h
 * @brief オーブのアクタークラスを定義するヘッダーファイル
 */

class Player;
class CollisionManager;

/**
 * @class OrbActor
 * @brief オーブのアクタークラス
 */
class OrbActor
{
public:
    OrbActor();
    ~OrbActor();

    /**
     * @brief 初期化
     * @param id オーブの識別子
     * @param position オーブの初期位置
     */
    bool Init(uint32_t id, const VECTOR& position, int modelHandle);

    void Update(CollisionManager* collisionManager);
    void Draw() const;

    /**
     * @brief プレイヤーがオーブを拾えるかどうかを判定する
     * @param playerPos プレイヤーの座標
     * @param range 判定範囲
     * @return プレイヤーがオーブを拾える場合はtrue、それ以外はfalse
     */
    bool CanPickup(const VECTOR& playerPos, float range) const;

    // 重力の更新
    void UpdateGravity();

    // オーブのデータを取得する
    const OrbData& GetData() const;

    // オーブのデータを取得する（編集可能）
    OrbData& GetData();

    // オーブの位置を取得する
    const VECTOR& GetPosition() const;

    // オーブの位置を設定する
    void SetPosition(const VECTOR& position);

    // オーブが地面に接しているかどうかを設定する
    void SetGround(bool isGround);

    // Y方向の速度を設定する
    void SetVelocityY(float velocityY);

private:
    OrbData m_Data; // オーブのデータ
    VECTOR m_Position{};    // オーブの位置
    VECTOR m_Rotation{};    // オーブの回転
    int m_ModelHandle = -1; // オーブのモデルハンドル

    float m_VelocityY = 0.0f;   // Y方向の速度
    bool m_IsGround = false;    // 地面に接しているかどうか
};

#endif // ORB_ACTOR_H
