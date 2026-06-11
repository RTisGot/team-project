#ifndef PLAYER_H_
#define PLAYER_H_

#include "collision/CollisionManager.h"
#include "camera/CameraController.h"
#include "audio/AudioManager.h"
#include "hp/HP.h"
#include <memory>
#include <DxLib.h>

class OrbManager;
class Follower;

//アニメーションの種類
enum AnimType {
    ANIM_IDLE,
    ANIM_RUN,
    ANIM_WALK,
    ANIM_JUMP,
    ANIM_ITEMWALK,
    ANIM_ITEMRUN,
    ANIM_WALKTOIDLE,
    ANIM_WALKTOIDLE_ITEM,
};

struct AnimMap {
    const char* nameInBlender;
    AnimType type;
}; 

extern AnimMap animList[];


/**
 *	@brief プレイヤークラス
 */
class Player
{
public:

    //キャラクターのモデルを読み込み
    int   m_Modelhandle;
    int   m_AnimAttachIndex;  // アタッチしたアニメーションの識別番号
    float m_AnimTotalTime;    // アニメーションの総時間
    float m_AnimTime;         //現在のアニメーションの再生時間管理
    float m_PlayerHeight; // キャラクターの頭から足元までの高さ
    float m_PlayerRadius; // キャラクターの横幅の半径

    int animIndices[8]; // アニメーションの識別番号を格納する配列
    bool m_isHolding = false;  //アイテムを持っているか
    bool IsHoldingOrb() const;
	// コンストラクタ
	Player();

    //HPの更新
    HP* GetHP() { return &m_PlayerHP; }
	// 更新処理
    void Update(float deltaTime, CollisionManager* collisionManager);

	// 描画処理
	void Draw();

    void LoadModel();

    // カメラの設定
    void SetCameraSpawn(float yaw, float pitch, float distance);

    // プレイヤー座標の設定
    void SetPosition(const VECTOR& position);
	// プレイヤー座標の取得
	VECTOR GetPosition() const { return m_Position; }
    // プレイヤーの向きの設定
    void SetRotation(float angle);
    // プレイヤーの向きの取得
    float GetAngle() const { return m_PlayerAngle; }

    // オーブの数の取得
    int GetOrbCount() const;

    //キャラクターのモデルのサイズを計算
    VECTOR minPos, maxPos;

    //---アニメーション関連----------------
    void InitAnimations();      // 初期化
    void PlayAnim(AnimType type); // 指定したAnimTypeを再生する
   

    // オーブマネージャーのセット
    void SetOrbManager(OrbManager* orbManager);
    // オーブを放す処理
    void DropOrb();

private:

	// プレイヤー情報
	VECTOR m_Position;		// プレイヤー座標
	float m_PlayerAngle;	// プレイヤーの向き
    uint32_t m_HoldingOrbId = 0;    // プレイヤーが持っているオーブのID
    OrbManager* m_OrbManager; // オーブマネージャーへのポインタ
    AudioManager m_AudioManager;
	// カメラ情報
    std::unique_ptr<CameraController> m_pCamera;

	// ジャンプ・物理
	float m_VelocityY;	// Y速度
	float m_Gravity;	// 重力
	float m_JumpPower;	// ジャンプ力
	bool  m_IsGround;	// 接地フラグ
    float m_StunTimer;  //硬直時間
    int m_PrevJumpKeyState;//ジャンプの前のフレームのキー入力状態(押されているか)

	// ダッシュ関連
	float m_MoveSpeed;		// 通常速度
	//float m_DashMultiplier;	// ダッシュ倍率
	//bool  m_IsDashing;		// ダッシュ中フラグ

    //HP------------
    HP m_PlayerHP; // プレイヤーのHP管理

    int m_VSHandle; // 頂点シェーダーのハンドル
    int m_PSHandle; // ピクセルシェーダーのハンドル

    int m_OutlineVSHandle; // 輪郭線用頂点シェーダー
    int m_OutlinePSHandle; // 輪郭線用ピクセルシェーダー
    
    int m_CBufferHandle; // 定数バッファのハンドル

    int m_OrbCount; // プレイヤーが持っているオーブの数

    bool m_WasMoving;
    float m_FootstepTimer;
    float m_PrevHP;
    //void GetShaderConstantBufferAddress();

    Follower* m_pFollower;
};

#endif // PLAYER_H_
