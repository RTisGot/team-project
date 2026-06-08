#ifndef DEF_KINDERGARTENMAP_H
#define DEF_KINDERGARTENMAP_H

#include "map/MapDataLoader.h"
#include "map/MapBase.h"
#include <DxLib.h>

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

    int GetModelHandle() const override { return m_modelHandle; }

private:
    int m_modelHandle;    ///< 保育園マップモデル
};

#endif // DEF_KINDERGARTENMAP_H
