#include "player/player.h"
#include "Collision/CollisionManager.h"
#include <math.h>
#include<Dxlib.h>
#include <cstring>
#include<vector>

#define MOVE_SPEED 10.0f


// コンストラクタ
// プレイヤーとカメラ情報を初期化
Player::Player()
{
    SetUseDirect3DVersion(DX_DIRECT3D_11);
    m_Modelhandle = -1;

    m_AnimAttachIndex = -1;
    m_AnimTotalTime = 0.0f;
    m_AnimTime = 0.0f;
	// 座標
	m_Position = VGet(200.0f, 400.0f, 0.0f);

	// 向き
	m_PlayerAngle = 0.0f;

    SetUseLighting(FALSE);
    SetGlobalAmbientLight(GetColorF(1.0f, 1.0f, 1.0f, 1.0f));
    SetMaterialUseVertDifColor(FALSE);

// カメラ制御
	m_CameraYaw = 0.0f; //横回転
	m_CameraPitch = 0.3f;//縦回転
    float targetDistance = 30.0f;//通常時のキャラとカメラの距離

    
	m_CameraDistance = 30.0f; //距離

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

   
    //shaderのハンドルを初期化
    m_VSHandle = -1;
    m_PSHandle = -1;
    m_OutlineVSHandle = -1;
    m_OutlinePSHandle = -1;
    m_CBufferHandle = -1;
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

    // 頂点シェーダーの読み込み（コンパイル）
    m_VSHandle = LoadVertexShader("Game/assets/shaders/VertexShader.fx");

    // ピクセルシェーダーの読み込み（コンパイル）
    m_PSHandle = LoadPixelShader("Game/assets/shaders/PixelShader.fx");

   /* m_OutlineVSHandle = LoadVertexShader("Game/assets/shaders/OutlineVS.fx");
    m_OutlinePSHandle = LoadPixelShader("Game/assets/shaders/OutlinePS.fx");
    */
    m_CBufferHandle = CreateShaderConstantBuffer(32);

   
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

	m_CameraYaw += moveX * m_MouseSensitivity;//横
	m_CameraPitch += moveY * m_MouseSensitivity;//縦

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

	if (CheckHitKey(KEY_INPUT_W))move = VAdd(move, forward);//前進
	if (CheckHitKey(KEY_INPUT_S))move = VSub(move, forward);//後退
	if (CheckHitKey(KEY_INPUT_D))move = VAdd(move, right);//右
	if (CheckHitKey(KEY_INPUT_A))move = VSub(move, right);//左
	

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
            targetDistance = 40.0f; //ダッシュ中のカメラ距離
		}
        else
        {
            targetDistance = 30.0f; //通常時のカメラ距離
        }

      // アニメーションの更新
        if (m_Modelhandle != -1 && m_AnimAttachIndex != -1)
        {
            // 0.5f ずつ時間を進める
            m_AnimTime += 0.3f;

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
        float rotateSpeed = 0.12f; // 回転速度
        // プレイヤーの向きを更新
        m_PlayerAngle += diff * rotateSpeed;

            m_CameraDistance += (targetDistance - m_CameraDistance) * 0.1f;
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
		m_Position.y + 10.0f,
		m_Position.z
	};

	// カメラ設定
	SetCameraPositionAndTarget_UpVecY(cameraPos,targetPos);
}

// 描画処理(キャラクター描画)
void Player::Draw()
{
    DrawFormatString(20, 20, GetColor(255, 255, 0),
        "VS=%d PS=%d Model=%d", m_VSHandle, m_PSHandle, m_Modelhandle);
   /* // 新しい向きをセット
    MV1SetRotationXYZ(m_Modelhandle, VGet(0.0f, m_PlayerAngle, 0.0f));

    // 3Dモデルに新しい座標をセット
    VECTOR drawPos = m_Position;
    drawPos.y += (m_PlayerHeight * 1.6f);

    MV1SetPosition(m_Modelhandle, drawPos);

   // 3Dモデルの描画
    MV1DrawModel(m_Modelhandle);
    
    int color = GetColor(255, 255, 255);

    VECTOR bottomSphere = VAdd(m_Position, VGet(0.0f, m_PlayerRadius - m_PlayerHeight, 0.0f));
    VECTOR topSphere = VAdd(m_Position, VGet(0.0f, m_PlayerHeight - m_PlayerRadius, 0.0f));




    if (m_VSHandle != -1 && m_PSHandle != -1)
    {
        // シェーダー適用
        DxLib::SetUseVertexShader(m_VSHandle);
        DxLib::SetUsePixelShader(m_PSHandle);
        // モデルのテクスチャをスロット0へ
        int texHandle = DxLib::MV1GetTextureGraphHandle(m_Modelhandle, 0);
        DxLib::SetUseTextureToShader(0, texHandle);
        // ---- PixelShader 定数 (c0～c4) を 2引数版 SetPSConstF でセット ----
        // c0: LightDir.xyz + pad
        DxLib::FLOAT4 c0;
        c0.x = 0.0f;  c0.y = 1.0f;  c0.z = 0.0f;  c0.w = 0.0f;
        // c1 : LightColor.rgb + pad
        DxLib::FLOAT4 c1;
        c1.x = 1.0f;  c1.y = 1.0f;  c1.z = 1.0f;  c1.w = 1.0f;
        // c2 : RimColor.rgb + RimIntensity
        DxLib::FLOAT4 c2;
        c2.x = 0.90f; c2.y = 0.95f; c2.z = 1.00f; c2.w = 0.20f;
        // c3 : MidThreshold, DarkThreshold, Smooth, ShadowMin
        DxLib::FLOAT4 c3;
        c3.x = 0.55f; // 中間影開始
        c3.y = 0.30f; // 濃い影開始
        c3.z = 0.08f; // 境界のなめらかさ
        c3.w = 0.65f; // 最低明るさ（低いと濃くなる）
        // c4 : MidShadowStrength, DarkShadowStrength, SpecPower, SpecStrength
        DxLib::FLOAT4 c4;
        c4.x = 0.78f; // 中間影の暗さ
        c4.y = 0.55f; // 濃い影の暗さ
        c4.z = 48.0f; // スペキュラ鋭さ
        c4.w = 0.08f; // スペキュラ強さ
        // c5 : CameraPos.xyz + pad
        VECTOR cam = DxLib::GetCameraPosition();
        DxLib::FLOAT4 c5;
        c5.x = cam.x; c5.y = cam.y; c5.z = cam.z; c5.w = 0.0f;
        DxLib::SetPSConstF(0, c0);
        DxLib::SetPSConstF(1, c1);
        DxLib::SetPSConstF(2, c2);
        DxLib::SetPSConstF(3, c3);
        DxLib::SetPSConstF(4, c4);
        DxLib::SetPSConstF(5, c5);
        SetDrawBright(255, 255, 255);
        SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
        SetUseBackCulling(TRUE); // 一旦通常に戻す
        // 本体描画
        DxLib::MV1DrawModel(m_Modelhandle);
    }
    else
    {
        // フォールバック
        DxLib::MV1DrawModel(m_Modelhandle);
    }
    // =========================================================
    // Pass 2 : アウトライン描画
    // =========================================================
    if (m_OutlineVSHandle != -1 && m_OutlinePSHandle != -1)
    {
        // DxLibで安全に使えるカリング制御（環境差を回避）
        // FALSE: バックカリング無効（両面描画）
        DxLib::SetUseBackCulling(FALSE);
        DxLib::SetUseVertexShader(m_OutlineVSHandle);
        DxLib::SetUsePixelShader(m_OutlinePSHandle);
        // c0: OutlineWidth, OutlineColor.rgb
        DxLib::FLOAT4 oc0;
        oc0.x = 0.015f; // 輪郭の太さ（モデルサイズに応じて調整）
        oc0.y = 0.03f;  // R
        oc0.z = 0.03f;  // G
        oc0.w = 0.05f;  // B
        DxLib::SetPSConstF(0, oc0);
        DxLib::MV1DrawModel(m_Modelhandle);
        // 元に戻す
        DxLib::SetUseBackCulling(TRUE);
    }
    // 後片付け
    DxLib::SetUseVertexShader(-1);
    DxLib::SetUsePixelShader(-1);
    */
    if (m_Modelhandle == -1) return;
    // モデル姿勢
    MV1SetRotationXYZ(m_Modelhandle, VGet(0.0f, m_PlayerAngle, 0.0f));
    VECTOR drawPos = m_Position;
    drawPos.y += (m_PlayerHeight * 1.6f);
    MV1SetPosition(m_Modelhandle, drawPos);
    // 毎フレーム描画状態をリセット（他処理の影響を消す）
    SetDrawBright(255, 255, 255);
    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
    SetUseBackCulling(TRUE);
    SetUseTextureToShader(0, -1);
    SetUseVertexShader(-1);
    SetUsePixelShader(-1);
    // シェーダーが無ければ通常描画
    if (m_VSHandle == -1 || m_PSHandle == -1)
    {
        MV1DrawModel(m_Modelhandle);
        return;
    }
    // シェーダー適用
    SetUseVertexShader(m_VSHandle);
    SetUsePixelShader(m_PSHandle);
    // テクスチャをPSの t0 に設定
    int texHandle = MV1GetTextureGraphHandle(m_Modelhandle, 0);
    SetUseTextureToShader(0, texHandle);
    // ---------------------------------------------------------
    // PixelShader 定数セット（PixelShader.fx と一致）
    // c0 : LightDir.xyz + pad
    // c1 : LightColor.rgb + pad
    // c2 : RimColor.rgb + RimIntensity
    // c3 : MidThreshold, DarkThreshold, Smooth, ShadowMin
    // c4 : MidShadowStrength, DarkShadowStrength, SpecPower, SpecStrength
    // c5 : CameraPos.xyz + pad
    // ---------------------------------------------------------
    DxLib::FLOAT4 c0, c1, c2, c3, c4, c5;
    c0.x = 0.0f;  c0.y = 1.0f;  c0.z = 0.0f;  c0.w = 0.0f;
    c1.x = 1.0f;  c1.y = 1.0f;  c1.z = 1.0f;  c1.w = 1.0f;
    c2.x = 0.90f; c2.y = 0.95f; c2.z = 1.00f; c2.w = 0.20f;
    c3.x = 0.55f; // MidThreshold
    c3.y = 0.30f; // DarkThreshold
    c3.z = 0.08f; // Smooth
    c3.w = 0.65f; // ShadowMin（上げると明るい）
    c4.x = 0.78f; // MidShadowStrength
    c4.y = 0.55f; // DarkShadowStrength
    c4.z = 48.0f; // SpecPower
    c4.w = 0.08f; // SpecStrength
    VECTOR cam = GetCameraPosition();
    c5.x = cam.x; c5.y = cam.y; c5.z = cam.z; c5.w = 0.0f;
    SetPSConstF(0, c0);
    SetPSConstF(1, c1);
    SetPSConstF(2, c2);
    SetPSConstF(3, c3);
    SetPSConstF(4, c4);
    SetPSConstF(5, c5);
    // 本体描画（1回だけ）
    MV1DrawModel(m_Modelhandle);
    // 後片付け
    SetUseTextureToShader(0, -1);
    SetUseVertexShader(-1);
    SetUsePixelShader(-1);
    SetUseBackCulling(TRUE);
}
    

