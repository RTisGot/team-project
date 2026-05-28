#include "collision/CollisionManager.h"

CollisionManager::CollisionManager()
{
    m_stageCollider = std::make_unique<StageCollider>();
}

void CollisionManager::Init(int stageModelHandle)
{
    if (m_stageCollider)
    {
        m_stageCollider->SetStageModel(stageModelHandle);
    }
}

void CollisionManager::Update()
{
    // TODO: 将来的に複数のコライダーを管理する場合は、ここで各コライダーの状態を更新する処理を追加
}

void CollisionManager::ResolveStageCollision(VECTOR& position, float& velocityY, bool& isGround, float height, float radius)
{
    if (m_stageCollider)
    {
        m_stageCollider->ResolveStageCollision(position, velocityY, isGround, height, radius);
    }
}
