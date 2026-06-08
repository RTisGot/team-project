#ifndef DEF_COLLISIONMANAGER_H
#define DEF_COLLISIONMANAGER_H

#include "collision/StageCollider.h"
#include <vector>
#include <memory>
#include <DxLib.h>

class CollisionManager
{
public:
    CollisionManager();
    ~CollisionManager() = default;

    void Init(int stageModelHandle);

    void Update();

    /**
    * @brief キャラとステージの当たり判定処理
    * @param position 現在の座標
    * @param velocityY Y方向の速度
    * @param isGround 地面に接しているか
    * @param height 高さ
    * @param radius 半径
    */
    void ResolveStageCollision(VECTOR& position, float& velocityY, bool& isGround, float height, float radius);

private:
    std::unique_ptr<StageCollider> m_stageCollider; ///< ステージコライダー
};

#endif // DEF_COLLISIONMANAGER_H
