#ifndef DEF_COLLISIONMANAGER_H
#define DEF_COLLISIONMANAGER_H

#include "collision/PlayerStageCollider.h"
#include "collision/StageCollider.h"
#include "collision/CollisionMap.h"
#include <vector>
#include <memory>
#include <DxLib.h>

/**
 * @file CollisionManager.h
 * @brief 衝突管理クラスの定義
 */

/**
 * @class CollisionManager
 * @brief ゲーム内の衝突判定を管理するクラス
 */
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

    /**
     * @brief プレイヤーとステージの当たり判定処理
     * @param position 現在の座標
     * @param oldPosition 前回の座標
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

    CollisionMap* GetCollisionMap();
private:
    std::unique_ptr<PlayerStageCollider> m_PlayerCollider;  ///< プレイヤー用のステージコライダー
    std::unique_ptr<CollisionMap> m_CollisionMap; ///< コリジョンマップ
    std::unique_ptr<StageCollider> m_stageCollider; ///< ステージコライダー
};

#endif // DEF_COLLISIONMANAGER_H
