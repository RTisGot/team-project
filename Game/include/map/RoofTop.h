#ifndef DEF_ROOF_TOP_H
#define DEF_ROOF_TOP_H

#include "follower/follower.h"
#include "map/MapDataLoader.h"
#include <DxLib.h>
#include <memory>

/**
 * @file RoofTop.h
 * @brief 屋上クラスの定義
 */

 /**
  * @class RoofTop
  * @brief 屋上クラス
  */
class RoofTop
{
public:
    RoofTop();
    ~RoofTop();
    bool Init();

    /**
     * @brief 更新処理
     * @param playerPos プレイヤーの座標
     * @param playerAngle プレイヤーの向き
     */
    void Update(const VECTOR& playerPos, float playerAngle);

    void Draw();

    int GetModelHandle() const { return m_modelHandle; }

    bool Load();

private:
    int m_modelHandle;  // 屋上のモデルハンドル
    SpawnPoint m_PlayerSpawnPoint; // プレイヤースポーン地点

    // お供キャラクター
    std::unique_ptr<Follower> m_follower;
};

#endif // DEF_ROOF_TOP_H
