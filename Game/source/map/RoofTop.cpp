#include "map/RoofTop.h"
#include <DxLib.h>

RoofTop::RoofTop()
	:m_modelHandle(-1)
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
	return true;
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
}