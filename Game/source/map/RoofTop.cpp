#include "map/RoofTop.h"
#include "follower/follower.h"
#include <DxLib.h>

RoofTop::RoofTop()
	: m_modelHandle(-1)
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
	// モデルの読み込み
    m_modelHandle = MV1LoadModel("Game/assets/models/map/RoofTop.mv1");
	if (m_modelHandle == -1)
	{
		return false; // モデルの読み込みに失敗
	}

	return true;
}

void RoofTop::Update(const VECTOR& playerPos, float playerAngle)
{

	if (m_door)
	{
		m_door->Update(playerPos);
	}

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

    MV1SetPosition(m_itemModelHandle, VGet(250.0f, 400.0f, 0.0f));
    MV1DrawModel(m_itemModelHandle);

	if (m_door)
	{
		m_door->Draw();
	}

    if (m_follower)
    {
        m_follower->Draw();
    }
}
