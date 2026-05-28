#include "system/LightManager.h"
#include <DxLib.h>

LightManager::LightManager()
	:m_sunLightHandle(-1)
{
}

LightManager::~LightManager()
{

}

bool LightManager::Init()
{
	//SetUseLighting(FALSE);
	return true;
}

void LightManager::Update()
{

}
