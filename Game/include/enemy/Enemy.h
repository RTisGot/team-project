#ifndef ENEMY_H_
#define ENEMY_H_	
#include "DxLib.h"
#include <Collision/CollisionManager.h>

class Enemy
{
public:

	// コンストラクタ
	Enemy();

	// デストラクタ
	~Enemy();

	// 初期化処理
	void Init();

    // 敵の向き更新
    void AngleUpdate();
    
    // 敵の移動更新
    void MoveUpdate();

	// 更新処理
	void Update(CollisionManager* collisionManager);

    // アニメーション処理
    void Animation();

	// 描画処理
	void Draw();
  
    // 敵座標の取得
    void Finalize();

private:

    int m_ModelHandle;	            ///< モデルハンドル
    VECTOR m_Position;              ///< 敵座標
    VECTOR m_TargetMoveDirection;   ///< モデルが向くべき方向
    VECTOR m_MoveVec;               ///< 移動ベクトル

    float m_Angle;              ///< 敵の向き
    float m_JumpPower;          ///< ジャンプ力
    int m_JumpStatus;           ///< ジャンプ状態
    int m_Action;               ///< 現在の行動
    int m_PrevAction;           ///< 前回の行動
    int m_AnimIndex;            ///< アニメーションのインデックス
    int m_AttachIndex;          ///< モデルのアタッチメントインデックス
    float m_TotalTime;          ///< 経過時間
    float m_PlayTime;           ///< 行動開始からの時間

    int m_MoveTime;               ///< 移動時間

    float m_EnemyHeight;        ///< 敵の全体高さ
    float m_EnemyRadius;        ///< 敵の横幅の半径

    float m_VelocityY;          ///< Y方向の速度
    bool  m_IsGround;           ///< 接地フラグ
};
#endif
