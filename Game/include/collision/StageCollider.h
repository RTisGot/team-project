#ifndef DEF_STAGECOLLIDER_H
#define DEF_STAGECOLLIDER_H

#include "collision/StageColliderBase.h"
#include <DxLib.h>

/**
 * @file StageCollider.h
 * @brief 衝突管理クラスの定義
 */

 /**
  * @brief 衝突判定用のコライダー構造体
  */
struct Collider
{
    unsigned int m_id;  // 識別子
    int m_type;         // 種別
    VECTOR m_Center;    // 中心座標
    float m_Radius;     // 半径
};

/**
 * @class StageCollision
 * @brief ステージとの当たり判定を管理するクラス
 */
class StageCollider:public StageColliderBase
{
public:
    StageCollider();
    ~StageCollider() = default;

    /**
     * @brief キャラとステージの当たり判定処理
     * @param position 現在の座標
     * @param velocityY Y方向の速度
     * @param isGround 地面に接しているか
     * @param height 高さ
     * @param radius 半径
     */
    void ResolveStageCollision(VECTOR& position, float& velocityY, bool& isGround, float height, float radius);
};

#endif // DEF_STAGECOLLIDER_H
