#include "map/KindergartenMap.h"
#include <DxLib.h>

KindergartenMap::KindergartenMap()
    : m_modelHandle(-1)
{
}

KindergartenMap::~KindergartenMap()
{
    if (m_modelHandle != -1)
    {
        MV1DeleteModel(m_modelHandle);
        m_modelHandle = -1;
    }
}

bool KindergartenMap::Init()
{
    m_modelHandle =
        MV1LoadModel("Game/assets/models/map/Kindergarten.fbm/KindergartenMap.mv1");

    if (m_modelHandle == -1)
    {
        return false;
    }

    return true;
}

void KindergartenMap::Update()
{
}

void KindergartenMap::Draw()
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
            "保育園マップが読み込まれていません",
            GetColor(255, 0, 0));
    }
}
