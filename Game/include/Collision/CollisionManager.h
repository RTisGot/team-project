#pragma once
#include <DxLib.h>

/**
 * @file CollisionManager.h
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
 * @class CollisionManager
 * @brief 
 */
class CollisionManager
{
public:
    CollisionManager();
    ~CollisionManager() = default;

    /**
     * @brief ステージモデルを設定する
     * @param modelHandle ステージモデルのハンドル
     */
    void SetStageModel(int modelHandle);
    
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
    int m_StageModelHandle; ///< ステージモデルのハンドル
};
