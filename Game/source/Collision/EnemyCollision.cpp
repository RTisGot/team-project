#include "collision/EnemyCollision.h"

EnemyCollision::EnemyCollision()
    : m_StageModelHandle(-1)
{
}

void EnemyCollision::SetStageModel(int modelHandle)
{
    m_StageModelHandle = modelHandle;
}

void EnemyCollision::ResolveCollision(
    VECTOR& position,
    float& velocityY,
    bool& isGround)
{
    if (m_StageModelHandle == -1)
    {
        return;
    }

    VECTOR lineStart = VAdd(
        position,
        VGet(0.0f, 20.0f, 0.0f));

    VECTOR lineEnd = VAdd(
        position,
        VGet(0.0f, -10.0f, 0.0f));

    auto hit = MV1CollCheck_Line(
        m_StageModelHandle,
        -1,
        lineStart,
        lineEnd);

    if (hit.HitFlag)
    {
        if (velocityY <= 0.0f)
        {
            position.y = hit.HitPosition.y;
            velocityY = 0.0f;
            isGround = true;
        }
    }
    else
    {
        isGround = false;
    }
}
