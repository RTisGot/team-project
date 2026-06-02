#include "map/ShipMap.h"
#include <DxLib.h>

ShipMap::ShipMap()
    : m_modelHandle(-1)
{
    m_scale = VGet(0.1f, 0.1f, 0.1f);
}

ShipMap::~ShipMap()
{
    if (m_modelHandle != -1)
    {
        MV1DeleteModel(m_modelHandle);
        m_modelHandle = -1;
    }
}

bool ShipMap::Init()
{
    // モデル読み込み
    m_modelHandle =
        MV1LoadModel("Game/assets/models/map/Ship.mv1");

    if (m_modelHandle == -1)
    {
        OutputDebugString(
            TEXT("Ship.mv1 の読み込みに失敗しました\n"));

        return false;
    }

    MV1SetScale(m_modelHandle, m_scale);

    return true;
}

void ShipMap::Update()
{
    // 現状処理なし
}

void ShipMap::Draw()
{
    if (m_modelHandle != -1)
    {
        MV1DrawModel(m_modelHandle);
    }
    else
    {
        DrawString(
            100,
            100,
            "船マップが読み込まれていません",
            GetColor(255, 0, 0));
    }
}
