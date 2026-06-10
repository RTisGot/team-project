#include "enemy/EnemyManager.h"

#include <fstream>
#include "thirdparty/json.hpp"

using json = nlohmann::json;

EnemyManager::EnemyManager()
{
}

EnemyManager::~EnemyManager()
{
    Finalize();
}

bool EnemyManager::Load(
    const char* jsonPath)
{
    std::ifstream file(jsonPath);

    if (!file.is_open())
    {
        return false;
    }

    json root;
    file >> root;

    if (!root.contains("enemies"))
    {
        return false;
    }

    for (const auto& enemyData :
        root["enemies"])
    {
        VECTOR pos =
        {
            enemyData["position"]["x"],
            enemyData["position"]["y"],
            enemyData["position"]["z"]
        };

        auto enemy =
            std::make_unique<Enemy>();

        enemy->Init(pos);

        m_Enemies.push_back(
            std::move(enemy));
    }

    return true;
}

void EnemyManager::Update(
    CollisionManager* collisionManager,
    Player* player)
{
    for (auto& enemy : m_Enemies)
    {
        enemy->Update(
            collisionManager,
            player);
    }
}

void EnemyManager::Draw()
{
    for (auto& enemy : m_Enemies)
    {
        enemy->Draw();
    }
}

void EnemyManager::Finalize()
{
    m_Enemies.clear();
}
