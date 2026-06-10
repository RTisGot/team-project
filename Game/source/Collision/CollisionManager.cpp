#include "collision/CollisionManager.h"

CollisionManager::CollisionManager()
{
    m_PlayerCollider = std::make_unique<PlayerStageCollider>();
    m_CollisionMap = std::make_unique<CollisionMap>();
    m_stageCollider = std::make_unique<StageCollider>();
}

void CollisionManager::Init(int stageModelHandle)
{
    if(m_PlayerCollider)
    {
        m_PlayerCollider->SetStageModel(stageModelHandle);
        m_PlayerCollider->SetCollisionMap(m_CollisionMap.get());
    }

    if (m_stageCollider)
    {
        m_stageCollider->SetStageModel(stageModelHandle);
    }
}

void CollisionManager::Update()
{
    // TODO: 将来的に複数のコライダーを管理する場合は、ここで各コライダーの状態を更新する処理を追加
}

void CollisionManager::ResolvePlayerCollision(
    VECTOR& position,
    const VECTOR& previousPosition,
    float& velocityY,
    bool& isGround,
    float height,
    float radius)
{
    if (!m_PlayerCollider)
    {
        return;
    }

    // プレイヤーとステージの当たり判定処理を呼び出す
    m_PlayerCollider->ResolvePlayerCollision(
        position,
        previousPosition,
        velocityY,
        isGround,
        height,
        radius);
}

void CollisionManager::ResolveStageCollision(VECTOR& position, float& velocityY, bool& isGround, float height, float radius)
{
    if (m_stageCollider)
    {
        m_stageCollider->ResolveStageCollision(position, velocityY, isGround, height, radius);
    }
}

CollisionMap* CollisionManager::GetCollisionMap()
{
    return m_CollisionMap.get();
}
