#include "item/OrbActor.h"
#include "collision/CollisionManager.h"
#include <DxLib.h>

namespace
{
    constexpr float ROTATE_SPEED = 0.01f;
    constexpr float ORB_SCALE = 0.02f;
    constexpr float GRAVITY = -0.1f;
    constexpr float MAX_FALL_SPEED = -10.0f;
    constexpr float GROUND_Y = 0.0f;
}

OrbActor::OrbActor()
    : m_ModelHandle(-1)
{
}

OrbActor::~OrbActor()
{
    if (m_ModelHandle >= 0)
    {
        MV1DeleteModel(m_ModelHandle);
        m_ModelHandle = -1;
    }
}

bool OrbActor::Init(uint32_t id, const VECTOR& position, int modelHandle)
{
    m_Data.m_Id = id;

    m_Position = position;

    m_ModelHandle = MV1DuplicateModel(modelHandle);

    if (m_ModelHandle < 0)
    {
        return false;
    }

    MV1SetScale(m_ModelHandle, VGet(ORB_SCALE, ORB_SCALE, ORB_SCALE));

    return true;
}

void OrbActor::Update(CollisionManager* collisionManager)
{
    if (m_Data.m_State != OrbState::World)
    {
        return;
    }

    if (m_Data.m_State == OrbState::World)
    {
        UpdateGravity();
    }

    // ステージとの当たり判定を処理
    if (collisionManager)
    {
        collisionManager->ResolveStageCollision(
            m_Position,
            m_VelocityY,
            m_IsGround,
            1.0f,
            0.5f);
    }

    m_Rotation.y += ROTATE_SPEED;
}

void OrbActor::Draw() const
{
    if (m_ModelHandle < 0)
    {
        return;
    }

    if (m_Data.m_IsCollected)
    {
        return;
    }

    MV1SetPosition(m_ModelHandle, m_Position);

    MV1SetRotationXYZ(m_ModelHandle, m_Rotation);

    MV1DrawModel(m_ModelHandle);
}

bool OrbActor::CanPickup(const VECTOR& playerPos, float range) const
{
    VECTOR diff;
    
    // プレイヤーとオーブの距離を計算
    diff.x = playerPos.x - m_Position.x;
    diff.y = playerPos.y - m_Position.y;
    diff.z = playerPos.z - m_Position.z;

    // 距離の二乗を計算
    const float distSq =
        diff.x * diff.x +
        diff.y * diff.y +
        diff.z * diff.z;

    // 距離の二乗が範囲の二乗以下なら拾える
    return distSq <= range * range;
}

void OrbActor::UpdateGravity()
{
    if (m_IsGround)
    {
        return;
    }

    // 重力を加算
    m_VelocityY += GRAVITY;

    // 落下速度の上限を設定
    if (m_VelocityY < MAX_FALL_SPEED)
    {
        m_VelocityY = MAX_FALL_SPEED;
    }

    // Y方向の位置を更新
    m_Position.y += m_VelocityY;

    // 地面に接しているかの判定
    if (m_Position.y <= GROUND_Y)
    {
        m_Position.y = GROUND_Y;
        m_VelocityY = 0.0f;
        m_IsGround = true;
    }
}

const OrbData& OrbActor::GetData() const
{
    return m_Data;
}

OrbData& OrbActor::GetData()
{
    return m_Data;
}

const VECTOR& OrbActor::GetPosition() const
{
    return m_Position;
}

void OrbActor::SetPosition(const VECTOR& position)
{
    m_Position = position;
}

void OrbActor::SetGround(bool isGround)
{
    m_IsGround = isGround;
}

void OrbActor::SetVelocityY(float velocityY)
{
    m_VelocityY = velocityY;
}
