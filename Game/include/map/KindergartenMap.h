#ifndef DEF_KINDERGARTENMAP_H
#define DEF_KINDERGARTENMAP_H

#include "map/MapDataLoader.h"
#include "map/MapBase.h"
#include <DxLib.h>
/**
 * @file KindergartenMap.h
 * @brief 保育園マップの定義
 */

/**
 * @class KindergartenMap
 * @brief 保育園マップクラス
 */
class KindergartenMap : public MapBase
{
public:
    KindergartenMap();
    ~KindergartenMap();

    bool Init() override;
    void Update() override;
    void Draw() override;

    const MapData& GetMapData() const override;

    int GetModelHandle() const override { return m_modelHandle; }

private:
    int m_modelHandle;    ///< マップの3Dモデルハンドル

    MapData m_MapData;      ///< マップデータ
};

#endif // DEF_KINDERGARTENMAP_H
