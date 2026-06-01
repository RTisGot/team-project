#include "map/ShipMap.h"
#include <DxLib.h>

ShipMap::ShipMap()
    : m_modelHandle(-1)
{
}

ShipMap::~ShipMap()
{
}

bool ShipMap::Init()
{
    // モデルの読み込み
    m_modelHandle = MV1LoadModel("model/ship.mv1");
    if (m_modelHandle == -1)
    {
        return false;
    }
    return true;
}

void ShipMap::Update(const VECTOR& playerPos)
{
    // プレイヤーの位置に応じた処理をここに追加
}

void ShipMap::Draw()
{
    if (m_modelHandle != -1)
    {
        MV1DrawModel(m_modelHandle);
    }
}
