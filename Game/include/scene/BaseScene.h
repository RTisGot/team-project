#ifndef DEF_BASESCENE_H
#define DEF_BASESCENE_H

/**
* @file BaseScene.h
* @brief シーンの基底クラスの定義
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
	virtual void Update(float deltaTime) = 0;
	virtual void Draw() = 0;
};

#endif // DEF_BASESCENE_H
