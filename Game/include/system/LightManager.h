#ifndef DEF_LIGHTMANAGER_H
#define DEF_LIGHTMANAGER_H

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

#endif // DEF_LIGHTMANAGER_H
