#ifndef ENEMY_H_
#define ENEMY_H_

#include "DxLib.h"
#include <Collision/CollisionManager.h>
#include <player/player.h>

class Enemy
{
public:
    Enemy();
    ~Enemy();

    void Init(const VECTOR& position);

    void Update(
        CollisionManager* collisionManager,
        Player* player);

    void Draw();

    void Finalize();

private:

    void MoveUpdate(Player* player);
    void AngleUpdate();
    void AttackPlayer(Player* player);
    void Animation();
    void DrawViewRange();

    bool IsPlayerInRange(
        const VECTOR& playerPos);

private:

    int m_ModelHandle;

    VECTOR m_Position;
    VECTOR m_TargetMoveDirection;
    VECTOR m_MoveVec;

    float m_Angle;

    float m_EnemyHeight;
    float m_EnemyRadius;

    float m_ViewRange;
    float m_ViewAngle;

    int m_DamagePower;
    int m_DamageInterval;
    int m_DamageTimer;

    float m_VelocityY;
    bool m_IsGround;
    bool m_IsChasing;
};

#endif
