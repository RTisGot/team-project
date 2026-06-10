#ifndef ENEMY_MANAGER_H_
#define ENEMY_MANAGER_H_

#include <vector>
#include <memory>

#include "enemy/Enemy.h"

class EnemyManager
{
public:

    EnemyManager();
    ~EnemyManager();

    bool Load(const char* jsonPath);

    void Update(
        CollisionManager* collisionManager,
        Player* player);

    void Draw();

    void Finalize();

private:

    std::vector<
        std::unique_ptr<Enemy>
    > m_Enemies;
};

#endif
