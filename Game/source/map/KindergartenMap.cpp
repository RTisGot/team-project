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
        MV1LoadModel("Game/assets/models/map/Kindergarten/Kindergarten.mv1");

    // モデルのスケーリング
    if (m_modelHandle != -1)
    {
        MV1SetScale(
            m_modelHandle,
            VGet(0.005f, 0.005f, 0.005f));
    }

    // モデルの読み込みに失敗した場合はエラーメッセージを出力

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
