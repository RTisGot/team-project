#pragma once
#include <DxLib.h>

/**
 * @file Door.h
 * @brief ドアクラスの定義
 */

/**
 * @enum DoorState
 * @brief ドアの状態を表す列挙型
 */
enum class DoorState
{
	Closed, ///< ドアが閉まっている状態
	Opening, ///< ドアが開いている途中の状態
	Opened, ///< ドアが完全に開いている状態
};

 /**
  * @class Door
  * @brief ドアクラス
  */
class Door
{
public:
	Door();
	~Door();
	bool Init();
	void Update(const VECTOR& playerPos);
	void Draw();

	int GetModelHandle() const { return m_modelHandle; }

private:
	int m_modelHandle;	///< ドアのモデルハンドル
	int m_animIndex;		///< ドアのアニメーションインデックス
	float m_animTime;		///< ドアのアニメーション時間
	float m_animTotalTime;	///< ドアのアニメーション全体時間
	DoorState m_state;		///< ドアの状態

	VECTOR m_interactionPos;	///< ドアとプレイヤーのインタラクション位置
	float m_interactionRadius;	///< ドアとプレイヤーのインタラクション半径
};
