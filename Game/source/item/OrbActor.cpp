#include "item/OrbActor.h"
#include <DxLib.h>

namespace
{
    constexpr float ROTATE_SPEED = 0.01f;
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

    return true;
}

void OrbActor::Update()
{
    if (m_Data.m_IsCollected)
    {
        return;
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

bool OrbActor::CanPickup(
    const VECTOR& playerPos,
    float range) const
{
    VECTOR diff;

    diff.x = playerPos.x - m_Position.x;
    diff.y = playerPos.y - m_Position.y;
    diff.z = playerPos.z - m_Position.z;

    const float distSq =
        diff.x * diff.x +
        diff.y * diff.y +
        diff.z * diff.z;

    return distSq <= range * range;
}

const OrbData& OrbActor::GetData() const
{
    return m_Data;
}

OrbData& OrbActor::GetData()
{
    return m_Data;
}

void OrbActor::SetCollected(bool collected)
{
    m_Data.m_IsCollected = collected;
}

bool OrbActor::IsCollected() const
{
    return m_Data.m_IsCollected;
}

const VECTOR& OrbActor::GetPosition() const
{
    return m_Position;
}
