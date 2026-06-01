#include <follower/follower.h>
#include <DxLib.h>
#include <math.h>

Follower::Follower()
{
    m_ModelHandle = -1;
    m_Position = VGet(250.0f, 350.0f, 0.0f);
    m_TargetPosition = m_Position;
    m_Angle = 0.0f;
}

Follower::~Follower()
{
    if (m_ModelHandle != -1)
    {
        MV1DeleteModel(m_ModelHandle);
        m_ModelHandle = -1;
    }
}

void Follower::LoadModel()
{
    if (m_ModelHandle != -1)
    {
        return;
    }

    m_ModelHandle = MV1LoadModel(
        "Game/assets/models/follower/cup.mv1"
    );

    if (m_ModelHandle != -1)
    {
        // 必要ならサイズ調整
        // MV1SetScale(m_ModelHandle, VGet(0.5f, 0.5f, 0.5f));
    }
}

void Follower::SetTargetPosition(const VECTOR& pos)
{
    m_TargetPosition = pos;
}

void Follower::SetTargetAngle(float angle)
{
    m_TargetAngle = angle;
}

void Follower::Update()
{
    VECTOR targetPos;

    targetPos.x =
        m_TargetPosition.x -
        sinf(m_TargetAngle) * 15.0f;

    targetPos.y =
        m_TargetPosition.y;

    targetPos.z =
        m_TargetPosition.z -
        cosf(m_TargetAngle) * 15.0f;

    VECTOR dir = VSub(targetPos, m_Position);

    float distance = VSize(dir);

    if (distance > 0.1f)
    {
        dir = VNorm(dir);

        m_Position = VAdd(
            m_Position,
            VScale(dir, 0.5f)
        );

        m_Angle = atan2f(dir.x, dir.z);
    }
}

void Follower::Draw()
{
    if (m_ModelHandle == -1)
    {
        return;
    }

    MV1SetPosition(m_ModelHandle, m_Position);

    MV1SetRotationXYZ(
        m_ModelHandle,
        VGet(0.0f, m_Angle, 0.0f)
    );

    MV1DrawModel(m_ModelHandle);
}
