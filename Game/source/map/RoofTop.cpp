#include "map/RoofTop.h"
#include "follower/follower.h"
#include "item/OrbLoader.h"
#include <DxLib.h>

RoofTop::RoofTop()
	: m_modelHandle(-1)
    , m_follower(std::make_unique<Follower>())
{
}

RoofTop::~RoofTop()
{
	// モデルの解放
	if (m_modelHandle != -1)
	{
		MV1DeleteModel(m_modelHandle);
		m_modelHandle = -1;
	}
}

bool RoofTop::Init()
{
    if (!MapDataLoader::Load("Game/data/maps/Rooftop.json", m_MapData))
    {
        return false;
    }

    m_modelHandle = MV1LoadModel("Game/assets/models/map/RoofTop.mv1");

    if (m_modelHandle == -1)
    {
        return false;
    }

    MV1SetScale(m_modelHandle, VGet(1.6f, 1.6f, 1.6f));

    MV1SetupCollInfo(m_modelHandle, -1, 8, 8, 8);

    if (m_follower)
    {
        m_follower->LoadModel();
    }

    return true;
}

void RoofTop::Update()
{
}

void RoofTop::UpdateFollower(const VECTOR& playerPos, float playerAngle)
{
    if (m_follower)
    {
        m_follower->SetTargetPosition(playerPos);
        m_follower->SetTargetAngle(playerAngle);
        m_follower->Update();
    }
}

void RoofTop::Draw()
{
	if (m_modelHandle != -1)
	{
		MV1DrawModel(m_modelHandle);
	}
	else
	{
		DrawString(100, 100, "屋上モデルが読み込まれていません", GetColor(255, 0, 0));
	}

    if (m_follower)
    {
        m_follower->Draw();
    }
}

const MapData& RoofTop::GetMapData() const
{
    return m_MapData;
}
