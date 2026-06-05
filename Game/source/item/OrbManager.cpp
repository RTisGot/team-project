#include "item/OrbManager.h"
#include "item/OrbActor.h"
#include "player/player.h"

OrbManager::~OrbManager()
{
    if (m_OrbModelHandle >= 0)
    {
        MV1DeleteModel(m_OrbModelHandle);
        m_OrbModelHandle = -1;
    }
}

bool OrbManager::Init()
{
    m_OrbModelHandle = MV1LoadModel("Game/assets/models/item/blueOrb.mv1");

    return m_OrbModelHandle >= 0;
}

void OrbManager::Update(Player* player, CollisionManager* collisionManager)
{
    for (auto& orb : m_Orbs)
    {
        if (orb->GetData().m_State == OrbState::Player)
        {
            VECTOR pos =player->GetPosition();

            pos.y += 15.0f;

            orb->SetPosition(pos);
        }

        orb->Update(collisionManager);
    }
}

void OrbManager::Draw() const
{
    for (const auto& orb : m_Orbs)
    {
        orb->Draw();
    }
}

void OrbManager::CreateOrb(uint32_t id, const VECTOR& position)
{
    if (m_OrbModelHandle < 0)
    {
        return;
    }

    auto orb = std::make_shared<OrbActor>();

    if (!orb->Init(id, position, m_OrbModelHandle))
    {
        return;
    }

    m_Orbs.push_back(orb);
}

std::shared_ptr<OrbActor>
OrbManager::FindNearestOrb(const VECTOR& position, float range)
{
    std::shared_ptr<OrbActor> result;

    float nearestSq = range * range;

    for (auto& orb : m_Orbs)
    {
        if (orb->GetData().m_State != OrbState::World)
        {
            continue;
        }

        VECTOR diff;

        // 位置の差を計算
        diff.x =orb->GetPosition().x -position.x;
        diff.y =orb->GetPosition().y -position.y;
        diff.z =orb->GetPosition().z -position.z;

        // 2乗距離を計算
        float distSq =
            diff.x * diff.x +
            diff.y * diff.y +
            diff.z * diff.z;

        // 最も近いオーブを更新
        if (distSq < nearestSq)
        {
            nearestSq = distSq;
            result = orb;
        }
    }

    return result;
}

std::shared_ptr<OrbActor>
OrbManager::FindOrbById(uint32_t id)
{
    for (auto& orb : m_Orbs)
    {
        if (orb->GetData().m_Id == id)
        {
            return orb;
        }
    }

    return nullptr;
}
