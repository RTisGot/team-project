#ifndef DEF_ROOF_TOP_H
#define DEF_ROOF_TOP_H

#include "follower/follower.h"
#include "map/MapDataLoader.h"
#include "collision/CollisionMap.h"
#include "map/MapBase.h"
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
class RoofTop : public MapBase
{
public:
    RoofTop();
    ~RoofTop();
    bool Init() override;

    /**
     * @brief 更新処理
     * @param playerPos プレイヤーの座標
     * @param playerAngle プレイヤーの向き
     */
    void Update() override;
    void UpdateFollower(const VECTOR& playerPos, float playerAngle);
    void Draw() override;

    const MapData& GetMapData() const override;

    int GetModelHandle() const override { return m_modelHandle; }

private:
    int m_modelHandle;  ///< 屋上のモデルハンドル

    MapData m_MapData;  ///< マップデータ

    std::unique_ptr<Follower> m_follower;   ///< お供インスタンス
};

#endif // DEF_ROOF_TOP_H
