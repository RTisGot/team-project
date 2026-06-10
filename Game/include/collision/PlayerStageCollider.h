#ifndef DEF_PLAYERSTAGECOLLIDER_H
#define DEF_PLAYERSTAGECOLLIDER_H

#include "collision/StageColliderBase.h"
#include <DxLib.h>

/**
 * @file PlayerStageCollider.h
 * @brief プレイヤーとステージの当たり判定を管理するクラスの定義
 */

class CollisionMap;

/**
 * @class PlayerStageCollider
 * @brief プレイヤーとステージの当たり判定を管理するクラス
 */
class PlayerStageCollider : public StageColliderBase
{
public:
    /**
     * @brief プレイヤーとステージの当たり判定処理
     * @param position 現在の座標
     * @param previousPosition 前回の座標
     * @param velocityY Y方向の速度
     * @param isGround 地面に接しているか
     * @param height 高さ
     * @param radius 半径
     */
    void ResolvePlayerCollision(
        VECTOR& position,
        const VECTOR& previousPosition,
        float& velocityY,
        bool& isGround,
        float height,
        float radius);

    /**
     * @brief コリジョンマップを設定
     * @param collisionMap コリジョンマップのポインタ
     */
    void SetCollisionMap(CollisionMap* collisionMap);
   
private:
    /**
     * @brief 足元の当たり判定処理
     * @param position 現在の座標
     * @param velocityY Y方向の速度
     * @param isGround 地面に接しているか
     * @param height 高さ
     */
    void ResolveFloor(VECTOR& position, float& velocityY, bool& isGround, float height);

    /**
     * @brief カプセル形状の当たり判定処理
     * @param position 現在の座標
     * @param velocityY Y方向の速度
     * @param radius 半径
     * @param height 高さ
     */
    void ResolveCapsule(VECTOR& position, float& velocityY, float radius, float height);


    CollisionMap* m_CollisionMap = nullptr; ///< コリジョンマップへのポインタ
};

#endif // DEF_PLAYERSTAGECOLLIDER_H
