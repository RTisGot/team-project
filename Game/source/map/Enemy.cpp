#include "enemy/Enemy.h"

void Enemy::Init(VECTOR startPos)
{
    modelHandle = MV1LoadModel("Game/assets/models/map/enemy/Enemy.mv1");
    pos = startPos;
    angle = 0.0f;
}

void Enemy::Update()
{
    // ç°ÇÕâΩÇ‡ÇµÇ»Ç¢Åiå„Ç≈AIì¸ÇÍÇÈÅj
}

void Enemy::Draw()
{
    MV1SetPosition(modelHandle, pos);
    MV1SetRotationXYZ(modelHandle, VGet(0, angle, 0));
    MV1DrawModel(modelHandle);
}