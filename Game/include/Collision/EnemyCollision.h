#pragma once
#include <DxLib.h>

class EnemyCollision
{
public:
    EnemyCollision();
    ~EnemyCollision() = default;

    void SetStageModel(int modelHandle);

    void ResolveCollision(
        VECTOR& position,
        float& velocityY,
        bool& isGround);

private:
    int m_StageModelHandle;
};
