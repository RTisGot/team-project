#ifndef PLAYER_H_
#define PLAYER_H_

#include <DxLib.h>

/**
 *	@brief プレイヤークラス
 */
class Player
{
public:

	// コンストラクタ
	Player();

	// 更新処理
	void Update(int roofTopModelHandle);

	// 描画処理
	void Draw();

	// プレイヤー座標の取得
	VECTOR GetPosition() const { return m_Position; }

private:

	// プレイヤー情報
	VECTOR m_Position;		// プレイヤー座標
	float m_PlayerAngle;	// プレイヤーの向き

	// カメラ情報
	float m_CameraYaw;		// カメラ横回転
	float m_CameraPitch;	// カメラ縦回転
	float m_CameraDistance;	// カメラ距離
	float m_MouseSensitivity;	// マウス感度

	// ジャンプ・物理
	float m_VelocityY;	// Y速度
	float m_Gravity;	// 重力
	float m_JumpPower;	// ジャンプ力
	bool  m_IsGround;	// 接地フラグ

	// ダッシュ関連
	float m_MoveSpeed;		// 通常速度
	float m_DashMultiplier;	// ダッシュ倍率
	bool  m_IsDashing;		// ダッシュ中フラグ
};

#endif