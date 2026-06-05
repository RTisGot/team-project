#include "player/player.h"
#include "item/OrbManager.h"
#include "item/OrbActor.h"
#include <math.h>
#include <Dxlib.h>
#include <cstring>
#include <vector>
#include <fstream>

// CelToonPS.fx の cbuffer CelParams : register(b4) と同じ並び
struct CelPSConstantBuffer
{
    DxLib::FLOAT4 LightDir;
    DxLib::FLOAT4 LightColor;
    DxLib::FLOAT4 ShadowParams;
    DxLib::FLOAT4 ShadeParams;
};

// ファイルをバイナリとして丸ごとメモリに読み込むヘルパー関数
static std::vector<char> LoadFileToMemory(const char* filepath)
{
    std::ifstream file(filepath, std::ios::binary | std::ios::ate);
    if (!file.is_open()) return std::vector<char>(); // 失敗時は空の配列

    std::streamsize size = file.tellg();
    std::vector<char> buffer(size);
    file.seekg(0, std::ios::beg);
    file.read(buffer.data(), size);

    return buffer;
}

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
	m_Position = VGet(300.0f, 580.0f, 0.0f);

	// 向き
	m_PlayerAngle = 0.0f;

    DxLib::SetLightDirection(DxLib::VGet(-0.4f, -0.92f, -0.35f)); // シェーダー内のLの逆方向（光の進む向き）
   

// カメラ制御
	m_CameraYaw = 0.0f; //横回転
	m_CameraPitch = 0.3f;//縦回転
    m_TargetCameraDistance = 30.0f;//通常時のキャラとカメラの距離
    m_BaseCameraDistance = 30.0f; //ホイール時の基準距離
    
	m_CameraDistance = 30.0f; //距離

    m_CameraHeightActual = m_Position.y; //カメラの高さ

    m_CameraTargetActual =
    {
        m_Position.x,
        m_Position.y + 10.0f,
        m_Position.z
    };

    // マウス感度
	m_MouseSensitivity = 0.005f;

	// ジャンプ・物理
	m_VelocityY = 0.0f;		// Y速度
	m_Gravity = -400.0f;		// 重力
	m_JumpPower = 110.0f;		// ジャンプ力
	m_IsGround = true;		// 接地フラグ
    m_StunTimer = 0.0f;    //着地硬直時間
    m_PrevJumpKeyState = 0;//ジャンプの前のフレームのキー入力状態(押されているか)

	// ダッシュ関連
	m_MoveSpeed = 50.0f;
	m_DashMultiplier = 2.0f;  // 2倍速
	m_IsDashing = false;

    // ホイールの回転量を初期化
    m_LastWheelRot = 0;
   
    // shaderのハンドルを初期化
    m_VSHandle = -1;
    m_PSHandle = -1;
    m_OutlineVSHandle = -1;
    m_OutlinePSHandle = -1;
    m_CBufferHandle = -1;
    LoadModel();

    // アイテム関係
    m_OrbManager = nullptr;
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

  
    // --- 頂点シェーダーの読み込み ---
    int vsFileHandle = FileRead_open("Game/assets/shaders/SkinMesh4_CelVS.vso");
    if (vsFileHandle != 0) {
        int64_t fileSize = FileRead_size("Game/assets/shaders/SkinMesh4_CelVS.vso");
        std::vector<char> vsBuffer(fileSize);
        FileRead_read(vsBuffer.data(), (int)fileSize, vsFileHandle);
        FileRead_close(vsFileHandle);

        m_VSHandle = LoadVertexShaderFromMem(vsBuffer.data(), vsBuffer.size());
    }
    else {
        m_VSHandle = -1;
    }

    // --- ピクセルシェーダーの読み込み ---
    int psFileHandle = FileRead_open("Game/assets/shaders/CelToonPS.pso");
    if (psFileHandle != 0) {
        int64_t fileSize = FileRead_size("Game/assets/shaders/CelToonPS.pso");
        std::vector<char> psBuffer(fileSize);
        FileRead_read(psBuffer.data(), (int)fileSize, psFileHandle);
        FileRead_close(psFileHandle);

        m_PSHandle = LoadPixelShaderFromMem(psBuffer.data(), psBuffer.size());
    }
    else {
        m_PSHandle = -1;
    }

    if (m_CBufferHandle != -1)
    {
        DeleteShaderConstantBuffer(m_CBufferHandle);
        m_CBufferHandle = -1;
    }
    if (m_PSHandle != -1)
    {
        m_CBufferHandle = CreateShaderConstantBuffer(sizeof(CelPSConstantBuffer));
    }
}

// -----------------更新処理----------------------------------------------
void Player::Update(float deltaTime, CollisionManager* collisionManager)
{
    // フレームレートが極端に低い場合の補正
    deltaTime = min(deltaTime, 0.05f);

    // 着地硬直の処理
    if (m_StunTimer > 0.0f)
    {
        m_StunTimer -= deltaTime;

    }

    // 画面サイズ
    int screenX;
    int screenY;

    GetDrawScreenSize(&screenX, &screenY);

    // マウス座標取得
    int mouseX;
    int mouseY;

    GetMousePoint(&mouseX, &mouseY);

    //マウスホイールの回転量を取得
    m_LastWheelRot = GetMouseWheelRotVol();

    // ジャンプキーの状態を取得
    int currentJumpKeyState = CheckHitKey(KEY_INPUT_SPACE);

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

    // カメラの高さをプレイヤーの高さに近づける
    float targetY = m_Position.y;

    // 着地硬直中はカメラの高さを少し上げる
    m_CameraHeightActual += (targetY - m_CameraHeightActual) * 8.0f * deltaTime;

    // カメラ縦回転制限
    // 真上・真下防止
    if (m_CameraPitch > 1.0f)m_CameraPitch = 1.0f;
    if (m_CameraPitch < -1.0f)	m_CameraPitch = -1.0f;

    // カメラ前方向
    VECTOR forward =
    {
        sinf(m_CameraYaw),
        0.0f,
        cosf(m_CameraYaw)
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


    if (m_StunTimer > 0.0f)
    {
        move = VGet(0.0f, 0.0f, 0.0f); // W/A/S/Dを押していても、移動方向を強制的にリセット
    }
    else
    {
        //ジャンプキーの状態を取得
        int currentJumpKeyState = CheckHitKey(KEY_INPUT_SPACE);
        // ジャンプの入力があった場合、前のフレームでは押されていなかった場合にジャンプ処理を行う
        if (currentJumpKeyState == 1 && m_PrevJumpKeyState == 0)
        {
            if (m_IsGround)
            {
                m_VelocityY = m_JumpPower;
                m_IsGround = false;
            }
        }
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



    // ホイールでカメラ距離を変更
    if (m_LastWheelRot != 0)
    {
        m_BaseCameraDistance -= m_LastWheelRot * 1.5f;// ホイールの回転量に応じて基準距離を変更
    }
    // カメラ距離の制限
    if (m_BaseCameraDistance < 15.0f)  m_BaseCameraDistance = 15.0f;  // 最短距離
    if (m_BaseCameraDistance > 60.0f) m_BaseCameraDistance = 60.0f; // 最長距離

    // ダッシュ入力
    if (CheckHitKey(KEY_INPUT_LSHIFT))
    {
        m_IsDashing = true;
    }
    else
    {
        m_IsDashing = false;
    }

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
            m_TargetCameraDistance = m_BaseCameraDistance + 10.0f; //ダッシュ中のカメラ距離
        }
        else
        {
            m_TargetCameraDistance = m_BaseCameraDistance; //通常時のカメラ距離
        }


        // アニメーションの更新
        if (m_Modelhandle != -1 && m_AnimAttachIndex != -1)
        {
            // 0.5f ずつ時間を進める
            m_AnimTime += 18.0f * deltaTime;

            // アニメーションのループ処理
            if (m_AnimTime >= m_AnimTotalTime)
            {
                m_AnimTime -= m_AnimTotalTime;
            }

            //3Dモデルに反映
            MV1SetAttachAnimTime(m_Modelhandle, m_AnimAttachIndex, m_AnimTime);
        }
        //プレイヤーの位置を更新
        m_Position = VAdd(m_Position, VScale(move, speed * deltaTime));


        // プレイヤーの向きを移動方向に合わせる
        float targetAngle = atan2f(move.x, move.z);
        // プレイヤーの向きを徐々に目標角度に近づける
        float diff = targetAngle - m_PlayerAngle;

        //(DX_PI_F = 180)
        while (diff < -DX_PI_F) diff += DX_PI_F * 2.0f;
        while (diff > DX_PI_F) diff -= DX_PI_F * 2.0f;
        float rotateSpeed = 5.0f; // 回転速度

        // プレイヤーの向きを更新
        m_PlayerAngle += diff * rotateSpeed * deltaTime;


    }
    constexpr float cameraFollowSpeed = 6.0f;

    m_CameraDistance += (m_TargetCameraDistance - m_CameraDistance) * cameraFollowSpeed * deltaTime;

    //空中かどうか判定
    bool wasGround = m_IsGround;

    //重力
    m_VelocityY += m_Gravity * deltaTime;
    m_Position.y += m_VelocityY * deltaTime;

    // ステージとの当たり判定
    if (collisionManager != nullptr)
    {
        float halfheight = m_PlayerHeight;
        float playerRadius = m_PlayerRadius;

        collisionManager->ResolveStageCollision(m_Position, m_VelocityY, m_IsGround, halfheight, playerRadius);
    }

    // 着地した瞬間の処理(前フレで空中だった場合)
    if (m_IsGround == true && wasGround == false)
    {
        m_StunTimer = 0.05f;//落下硬直
        m_VelocityY = 0.0f;
    }

    // 場外落下時の復帰処理
    if (m_Position.y <= 0.0f)
    {
        m_Position.x = 300.0f;
        m_Position.y = 580.0f;
        m_Position.z = 0.0f;
        m_VelocityY = 0.0f;
    }

    //キャラの頭上を注視点とす
    VECTOR idealTargetPos = VGet(m_Position.x, m_Position.y + 10.0f, m_Position.z);

    //キャラに少し遅れてついてくる
    constexpr float targetFollowSpeed = 15.0f;

    // カメラの注視点をキャラの頭上に近づける
    m_CameraTargetActual = VAdd(m_CameraTargetActual, VScale(VSub(idealTargetPos, m_CameraTargetActual), targetFollowSpeed * deltaTime));

    // カメラ位置
    VECTOR cameraPos =
    {
        m_Position.x -
        cosf(m_CameraPitch) *
        sinf(m_CameraYaw) *
        m_CameraDistance,

        m_CameraHeightActual +
        sinf(m_CameraPitch) *
        m_CameraDistance,

        m_Position.z -
        cosf(m_CameraPitch) *
        cosf(m_CameraYaw) *
        m_CameraDistance
    };

    m_PrevJumpKeyState = currentJumpKeyState;

    // カメラ設定
    DxLib::SetCameraPositionAndTarget_UpVecY(cameraPos, m_CameraTargetActual);

    // アイテム処理
    if (CheckHitKey(KEY_INPUT_E))
    {
        if(m_OrbManager==nullptr)
        {
            return;
        }
        auto orb = m_OrbManager->FindNearestOrb(GetPosition(), 50.0f);

        if (orb && m_HoldingOrbId == 0)
        {
            orb->GetData().m_State = OrbState::Player;

            m_HoldingOrbId = orb->GetData().m_Id;
        }
    }

    // Gでオーブを放す
    if (CheckHitKey(KEY_INPUT_G))
    {
        DropOrb();
    }

    //----------HP処理---------------
        // HPの更新
    m_PlayerHP.Update();
}

// 描画処理(キャラクター描画)
void Player::Draw()
{
    // 1. モデルのトランスフォーム設定
    MV1SetRotationXYZ(m_Modelhandle, VGet(0.0f, m_PlayerAngle, 0.0f));
    MV1SetPosition(m_Modelhandle, m_Position);

    // ハンドルチェック
    if (m_PSHandle == -1 || m_VSHandle == -1 || m_CBufferHandle == -1 || m_PSConstantBufferHandle == -1)
    {
        MV1DrawModel(m_Modelhandle);
        return;
    }

    // 1. 必要なデータを VECTOR 形式で用意
    VECTOR camPos = DxLib::GetCameraPosition(); // カメラ座標
    VECTOR lightDir = VNorm(VGet(-0.4f, -0.92f, -0.35f)); // ライト方向

    // 2. モデルのユーザー定数としてシェーダーに送る
    // 第2引数：シェーダー内のレジスタ番号（b0, b1...）
    // 第3引数：送るデータのポインタ, 第4引数：個数（float4単位）
    MV1SetShaderConstantFloat4(m_Modelhandle, 0, (float*)&camPos, 1);   // register(b0)へ
    MV1SetUserShaderConstantFloat4(m_Modelhandle, 1, (float*)&lightDir, 1); // register(b1)へ

    // 3. シェーダーの適用
    MV1SetUseOrigShader(TRUE);
    SetUseVertexShader(m_VSHandle);
    SetUsePixelShader(m_PSHandle);

    // 鳴潮流：キャラクターの自影を背景と分離するため標準ライトをオフにする [1, 2]
    SetUseLighting(FALSE);

    MV1DrawModel(m_Modelhandle);

    // 描画実行
    MV1DrawModel(m_Modelhandle);

    // ---------------------------------------------------------
    // 5. 後処理
    // ---------------------------------------------------------
    SetUseVertexShader(-1);
    SetUsePixelShader(-1);
    MV1SetUseOrigShader(FALSE);
    SetUseLighting(TRUE);
}
    
void Player::SetOrbManager(OrbManager* orbManager)
{
    m_OrbManager = orbManager;
}

void Player::DropOrb()
{
    if (m_HoldingOrbId == 0)
    {
        return;
    }

    auto orb = m_OrbManager->FindOrbById(m_HoldingOrbId);

    if (!orb)
    {
        return;
    }

    VECTOR dropPos = GetPosition();

    dropPos.x += 10.0f;

    orb->SetPosition(dropPos);

    orb->GetData().m_State = OrbState::World;

    orb->SetGround(false);

    orb->SetVelocityY(0.0f);

    m_HoldingOrbId = 0;
}
