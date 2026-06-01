#ifndef PLAYER_H_
#define PLAYER_H_

#include <DxLib.h>
#include "collision/CollisionManager.h"
#include "../HP/HP.h"

/**
 *	@brief プレイヤークラス
 */
class Player
{
public:

    //キャラクターのモデルを読み込み
    int m_Modelhandle;
    int   m_AnimAttachIndex;  // アタッチしたアニメーションの識別番号
    float m_AnimTotalTime;    // アニメーションの総時間
    float m_AnimTime;         //現在のアニメーションの再生時間管理
    float m_PlayerHeight; // キャラクターの頭から足元までの高さ
    float m_PlayerRadius; // キャラクターの横幅の半径
    float targetDistance = 30.0f;//通常時のキャラとカメラの距離
	// コンストラクタ
	Player();

    //HPの更新
    HP* GetHP() { return &m_PlayerHP; }
	// 更新処理
	void Update(CollisionManager* collisionManager);

	// 描画処理
	void Draw();

    void LoadModel();
	// プレイヤー座標の取得
	VECTOR GetPosition() const { return m_Position; }

    //キャラクターのモデルのサイズを計算
    VECTOR minPos, maxPos;

private:

	// プレイヤー情報
	VECTOR m_Position;		// プレイヤー座標
	float m_PlayerAngle;	// プレイヤーの向き

	// カメラ情報
	float m_CameraYaw;		// カメラ横回転
	float m_CameraPitch;	// カメラ縦回転
	float m_CameraDistance;	// カメラ距離
	float m_MouseSensitivity;	// マウス感度
    float m_TargetCameraDistance; // カメラ距離
    float m_BaseCameraDistance; // ホイールで変更する基準のカメラ距離
    VECTOR m_CameraTargetActual; // 実際のカメラ注視点
    int m_LastWheelRot; // マウスホイールの回転量

    // 注視点
    VECTOR targetPos =
    {
        m_Position.x,
        m_Position.y + 10.0f,
        m_Position.z
    };

	// ジャンプ・物理
	float m_VelocityY;	// Y速度
	float m_Gravity;	// 重力
	float m_JumpPower;	// ジャンプ力
	bool  m_IsGround;	// 接地フラグ
    float m_StunTimer;  //硬直時間
    int m_PrevJumpKeyState;//ジャンプの前のフレームのキー入力状態(押されているか)

	// ダッシュ関連
	float m_MoveSpeed;		// 通常速度
	float m_DashMultiplier;	// ダッシュ倍率
	bool  m_IsDashing;		// ダッシュ中フラグ

    //HP------------
    HP m_PlayerHP; // プレイヤーのHP管理

    int m_VSHandle; // 頂点シェーダーのハンドル
    int m_PSHandle; // ピクセルシェーダーのハンドル

    int m_OutlineVSHandle; // 輪郭線用頂点シェーダー
    int m_OutlinePSHandle; // 輪郭線用ピクセルシェーダー
    
    int m_CBufferHandle; // 定数バッファのハンドル

    void GetShaderConstantBufferAddress();

};

#endif
