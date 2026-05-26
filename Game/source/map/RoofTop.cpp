#include "map/RoofTop.h"
#include <DxLib.h>

RoofTop::RoofTop()
	: m_modelHandle(-1)
	, m_door(std::make_unique<Door>())
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
	m_modelHandle = MV1LoadModel("Game/assets/models/map/rooftop.mv1");
	if (m_modelHandle == -1)
	{
		return false; // モデルの読み込みに失敗
	}

	if (m_door && !m_door->Init())
	{
		return false;
	}

	return true;
}

void RoofTop::Update(const VECTOR& playerPos)
{
	if (m_door)
	{
		m_door->Update(playerPos);
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

	if (m_door)
	{
		m_door->Draw();
	}
}