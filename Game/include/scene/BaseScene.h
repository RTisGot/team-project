#pragma once

/**
* @file BaseScene.h
* @brief シーンの基底クラス
*/

/**
* @class BaseScene
* @brief シーンの基底クラス
* @details シーンの基底クラス。すべてのシーンはこのクラスを継承して実装する。
*/
class BaseScene
{
public:
	virtual ~BaseScene() = default;

	virtual void Init() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;
};