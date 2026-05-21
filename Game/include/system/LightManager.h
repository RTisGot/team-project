#pragma once

/**
 * @file LightManager.h
 * @brief ライト管理クラス
 */

/**
 * @class LightManager
 * @brief ライトの管理を行うクラス
 */
class LightManager
{
public:
	LightManager();
	~LightManager();
	bool Init();
	void Update();
private:
	int m_sunLightHandle; // 太陽光のライトハンドル
};