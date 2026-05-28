#include "player/player.h"
#include "Collision/CollisionManager.h"
#include <math.h>
#include<Dxlib.h>

#define MOVE_SPEED 10.0f


// コンストラクタ
// プレイヤーとカメラ情報を初期化
Player::Player()
{
    
    m_Modelhandle = -1;

    m_AnimAttachIndex = -1;
    m_AnimTotalTime = 0.0f;
    m_AnimTime = 0.0f;
	// 座標
	m_Position = VGet(200.0f, 400.0f, 0.0f);

	// 向き
	m_PlayerAngle = 0.0f;

// カメラ制御
	m_CameraYaw = 0.0f; //横回転
	m_CameraPitch = 0.3f;//縦回転

	m_CameraDistance = 25.0f; //距離

// マウス感度
	m_MouseSensitivity = 0.005f;

	// ジャンプ・物理
	m_VelocityY = 0.0f;		// Y速度
	m_Gravity = -0.2f;		// 重力
	m_JumpPower = 3.0f;		// ジャンプ力
	m_IsGround = true;		// 接地フラグ

	// ダッシュ関連
	m_MoveSpeed = 1.0f;
	m_DashMultiplier = 2.0f;  // 2倍速
	m_IsDashing = false;

    LoadModel();
}


void Player::LoadModel()
{
    m_Modelhandle = MV1LoadModel("Game/assets/models/Character/Character.mv1");
    //animationをアタッチ
    m_AnimAttachIndex = MV1AttachAnim(m_Modelhandle, 0);
    //アニメーションの総時間を取得
    m_AnimTotalTime = MV1GetAttachAnimTotalTime(m_Modelhandle, m_AnimAttachIndex);
    //現在のアニメーションの再生時間管理
    m_AnimTime = 0.0f;
    MV1SetAttachAnimTime(m_Modelhandle, m_AnimAttachIndex, m_AnimTime);
  
    m_PlayerRadius = 5.0f;  // キャラクターの半径
    m_PlayerHeight = 10.0f;  // キャラクターの全体高さ
}

// 更新処理
// 入力・移動・カメラ更新
void Player::Update(CollisionManager* collisionManager)
{
	// 画面サイズ
	int screenX;
	int screenY;

	GetDrawScreenSize(&screenX, &screenY);

	// マウス座標取得
	int mouseX;
	int mouseY;

	GetMousePoint(&mouseX, &mouseY);

	// 画面中央
	int centerX = screenX / 2;
	int centerY = screenY / 2;

	// マウス移動量
	int moveX = mouseX - centerX;
	int moveY = mouseY - centerY;

	// マウスを中央へ戻す
	SetMousePoint(centerX, centerY);

	// カメラ回転

	// 横回転
	m_CameraYaw += moveX * m_MouseSensitivity;

	// 縦回転
	m_CameraPitch += moveY * m_MouseSensitivity;

	// カメラ縦回転制限
	// 真上・真下防止
	if (m_CameraPitch > 1.0f)
	{
		m_CameraPitch = 1.0f;
	}

	if (m_CameraPitch < -1.0f)
	{
		m_CameraPitch = -1.0f;
	}

	// カメラ前方向
	VECTOR forward =
	{
		cosf(m_CameraPitch) * sinf(m_CameraYaw),
		-sinf(m_CameraPitch),
		cosf(m_CameraPitch) * cosf(m_CameraYaw)
	};

	// カメラ右方向
	VECTOR right =
	{
		cosf(m_CameraYaw),
		0.0f,
		-sinf(m_CameraYaw)
	};

	// 入力方向
	VECTOR move = VGet(0.0f, 0.0f, 0.0f);

	// 前進
	if (CheckHitKey(KEY_INPUT_W))
	{
		move = VAdd(move, forward);
        
	}

	// 後退
	if (CheckHitKey(KEY_INPUT_S))
	{
		move = VSub(move, forward);
	}

	// 右移動
	if (CheckHitKey(KEY_INPUT_D))
	{
		move = VAdd(move, right);
	}

	// 左移動
	if (CheckHitKey(KEY_INPUT_A))
	{
		move = VSub(move, right);
	}

	/*// 上昇
	if (CheckHitKey(KEY_INPUT_E))
	{
		move.y += 1.0f;
	}

	// 下降
	if (CheckHitKey(KEY_INPUT_Q))
	{
		move.y -= 1.0f;
	}
    */
	// プレイヤー移動
	if (VSize(move) > 0.0f)
	{
		// 正規化
		move = VNorm(move);

		// 移動
		float speed = m_MoveSpeed;

        //ダッシュ中の速度管理
		if (m_IsDashing)
		{
			speed *= m_DashMultiplier;
		}

      // アニメーションの更新
        if (m_Modelhandle != -1 && m_AnimAttachIndex != -1)
        {
            // 0.5f ずつ時間を進める
            m_AnimTime += 0.5f;

            // アニメーションのループ処理
            if (m_AnimTime >= m_AnimTotalTime)
            {
                m_AnimTime -= m_AnimTotalTime;
            }

            //3Dモデルに反映
            MV1SetAttachAnimTime(m_Modelhandle, m_AnimAttachIndex, m_AnimTime);
        }
        //プレイヤーの位置を更新
		m_Position = VAdd(m_Position, VScale(move, speed));

    // プレイヤーの移動に合わせてカメラも移動---------------
        // プレイヤーの向きを移動方向に合わせる
        float targetAngle = atan2f(move.x, move.z);
        // プレイヤーの向きを徐々に目標角度に近づける
        float diff = targetAngle - m_PlayerAngle;

        //(DX_PI_F = 180)
        while (diff < -DX_PI_F) diff += DX_PI_F * 2.0f;
        while (diff > DX_PI_F) diff -= DX_PI_F * 2.0f;
        float rotateSpeed = 0.15f; // 回転速度
        // プレイヤーの向きを更新
        m_PlayerAngle += diff * rotateSpeed;
	}

	// ジャンプ入力
	if (CheckHitKey(KEY_INPUT_SPACE))
	{
		if (m_IsGround)
		{
			m_VelocityY = m_JumpPower;
			m_IsGround = false;
		}
	}

	//重力
	m_VelocityY += m_Gravity;
	m_Position.y += m_VelocityY;

	// 外部マネージャーへ当たり判定を委譲
	if (collisionManager != nullptr)
	{
		float halfheight =m_PlayerHeight;
		float playerRadius = m_PlayerRadius;
		
		collisionManager->ResolveStageCollision(m_Position, m_VelocityY, m_IsGround, halfheight, playerRadius);
	}

	// 場外落下時の復帰処理
	if (m_Position.y <= 0.0f)
	{
		m_Position.y = 400.0f;
		m_Position.x = 200.0f;
		m_Position.z = 0.0f;
		m_VelocityY = 0.0f;
	}

	// ダッシュ入力
	if (CheckHitKey(KEY_INPUT_LSHIFT))
	{
		m_IsDashing = true;
	}
	else
	{
		m_IsDashing = false;
	}
    
	// カメラ位置
	VECTOR cameraPos =
	{
		m_Position.x -
		cosf(m_CameraPitch) *
		sinf(m_CameraYaw) *
		m_CameraDistance,

		m_Position.y +
		sinf(m_CameraPitch) *
		m_CameraDistance,

		m_Position.z -
		cosf(m_CameraPitch) *
		cosf(m_CameraYaw) *
		m_CameraDistance
	};

	// 注視点
	VECTOR targetPos =
	{
		m_Position.x,
		m_Position.y -8.0f,
		m_Position.z
	};

	// カメラ設定
	SetCameraPositionAndTarget_UpVecY(cameraPos,targetPos);
}

// 描画処理(キャラクター描画)
void Player::Draw()
{
    // 新しい向きをセット
    MV1SetRotationXYZ(m_Modelhandle, VGet(0.0f, m_PlayerAngle, 0.0f));

    // 3Dモデルに新しい座標をセット
    VECTOR drawPos = m_Position;
    drawPos.y += (m_PlayerHeight * 0.9f);

    MV1SetPosition(m_Modelhandle, drawPos);

    // 3Dモデルの描画
    MV1DrawModel(m_Modelhandle);

    int color = GetColor(255, 255, 255);

    VECTOR bottomSphere = VAdd(m_Position, VGet(0.0f, m_PlayerRadius - m_PlayerHeight, 0.0f));
    VECTOR topSphere = VAdd(m_Position, VGet(0.0f, m_PlayerHeight - m_PlayerRadius, 0.0f));

    // DxLibの組み込み3D描画関数を使ってカプセル風に描画
    //DrawCapsule3D(bottomSphere, topSphere, m_PlayerRadius, 16, color, color, TRUE);
}
