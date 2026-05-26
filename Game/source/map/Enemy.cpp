#include "enemy/Enemy.h"
#include <math.h>
#include <system/Define.h>

Enemy::Enemy()
:m_ModelHandle(-1),
m_Position(VGet(0.0f, 0.0f, 0.0f)),
m_TargetMoveDirection(VGet(0.0f, 0.0f, 1.0f)),
m_MoveVec(VGet(0.0f, 0.0f, 0.0f)),
m_Angle(0.0f),
m_JumpPower(0.0f),
m_JumpStatus(OBJ_NO_JUMP),
m_Action(0),
m_PrevAction(0),
m_AnimIndex(0),
m_AttachIndex(0),
m_TotalTime(0.0f),
m_PlayTime(0.0f),
m_MoveTime(0)
{
			   
}

Enemy::~Enemy()
{
	// モデルの解放
	if (m_ModelHandle != -1)
	{
		MV1DeleteModel(m_ModelHandle);
		m_ModelHandle = -1;
	}
}

const static float ENEMY_MOVE_SPEED = 0.05f; //敵の移動速度
const static float ENEMY_ANGLE_SPEED = 0.2f; //敵の角度変化速度
const static float ENEMY_JUMP_POWER = 0.25f; //ジャンプ力
const static float ENEMY_GRAVITY = 0.01f; //敵の重力
const static float ENEMY_MAX_FALL_SPEED = -1.5f; //敵の落下速度の下限
const static float ENEMY_HIT_HEIGHT = 1.3f; //当たり判定用の高さ
const static int ENEMY_ANIMATION_NUM = 4; //敵のアニメーション総数

void Enemy::Init()
{
    // 座標
    m_Position = VGet(300.0f, 400.0f, 0.0f);

	// モデルの読み込み
	m_ModelHandle = MV1LoadModel("Game/assets/models/enemy/enemy.mv1");
	if (m_ModelHandle != -1)
	{
		MV1SetScale(m_ModelHandle, VGet(0.01f, 0.01f, 0.01f));
	}

    // 向く方向の初期化
    m_TargetMoveDirection = VGet(0.0f, 0.0f, 1.0f);

    // 移動ベクトルの初期化
    m_MoveVec = VGet(0.0f, 0.0f, 0.0f);

    // 角度の初期化
    m_Angle = 0.0f;

    // ジャンプの初期化
    m_JumpPower = 0.0f;
    m_JumpStatus = OBJ_NO_JUMP;

    // アクションの初期化
    m_Action = 0;
    m_PrevAction = 0;

    m_AttachIndex = 0;

    m_PlayTime = 0.0f;

    m_MoveTime = 0;

}

// 敵の向き更新
void Enemy::AngleUpdate()
{
    float TargetAngle;
    float SaAngle;
    TargetAngle = (float)atan2f(m_TargetMoveDirection.x, m_TargetMoveDirection.z);

    // 目標の角度と現在の角度との差を割り出す
    {
        // 最初は単純に引き算
        SaAngle = TargetAngle - m_Angle;

        // ある方向からある方向の差が１８０度以上になることは無いので
        // 差の値が１８０度以上になっていたら修正する
        if (SaAngle < -DX_PI_F)
        {
            SaAngle += DX_TWO_PI_F;
        }
        else
            if (SaAngle > DX_PI_F)
            {
                SaAngle -= DX_TWO_PI_F;
            }
    }

    // 角度の差を０に近づける
    if(SaAngle>0.0f)
    {
        // 差がプラスの場合は引く
        SaAngle -= ENEMY_ANGLE_SPEED;
        if (SaAngle < 0.0f)
        {
            SaAngle = 0.0f;
        }
    }
    else
    {
        // 差がマイナスの場合は足す
        SaAngle += ENEMY_ANGLE_SPEED;
        if (SaAngle > 0.0f)
        {
            SaAngle = 0.0f;
        }
    }

    // モデルの角度を更新
    m_Angle = TargetAngle - SaAngle;
    MV1SetRotationXYZ(m_ModelHandle, VGet(0.0f, m_Angle + DX_PI_F, 0.0f));
}

// 敵の動きの更新
void Enemy::MoveUpdate() {
    m_MoveVec = VGet(0.0f, 0.0f, 0.0f);

    //一定時間X軸正方向に移動→X軸負の方向に移動を繰り返す
    if (m_MoveTime < 180) {
        m_MoveVec = VGet(ENEMY_MOVE_SPEED, 0.0f, 0.0f);
    }
    else {
        m_MoveVec = VGet(-ENEMY_MOVE_SPEED, 0.0f, 0.0f);
    }

    m_MoveTime++;
    if (m_MoveTime > 360) m_MoveTime = 0;

    //落下状態の計算
    if (m_JumpStatus == OBJ_FALL) {
        // Ｙ軸方向の速度を重力分減算する
        m_JumpPower -= ENEMY_GRAVITY;

        // 落下速度が下限を超えていたら修正する
        if (m_JumpPower < ENEMY_MAX_FALL_SPEED) m_JumpPower = ENEMY_MAX_FALL_SPEED;

        // 移動ベクトルのＹ成分をＹ軸方向の速度にする
        m_MoveVec.y = m_JumpPower;
    }

    //マップとの当たり判定
    //{
    //    VECTOR PolyPos1, PolyPos2;
    //    PolyPos1 = m_Position;
    //    PolyPos2 = VGet(PolyPos1.x, PolyPos1.y + 1.0f, PolyPos1.z);

    //    Map_CheckCollision(&m_Position, PolyPos1, PolyPos2, m_MoveVec, &m_JumpStatus, &m_JumpPower);
    //}

    //敵の座標の更新
    MV1SetPosition(m_ModelHandle, m_Position);
}

//敵のアニメーション
void Enemy::Animation() {
 
}

void Enemy::Update()
{
    //敵の移動処理
    MoveUpdate();

    //敵の方向を変える
    AngleUpdate();

    //敵のアニメーション
    Animation();
}

void Enemy::Draw()
{
    if (m_ModelHandle == -1)
    {
        return;
    }

    MV1DrawModel(m_ModelHandle);
}

//終了処理
void Enemy::Finalize() {
    MV1DeleteModel(m_ModelHandle);
}
