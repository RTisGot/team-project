#include "player/player.h"
#include "item/OrbManager.h"
#include "item/OrbActor.h"
#include <cmath>
#include <cstring>
#include <vector>
#include <fstream>

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
    m_Position = VGet(0.0f, 0.0f, 0.0f);

    // 向き
    m_PlayerAngle = 0.0f;

    SetUseLighting(FALSE);
    SetGlobalAmbientLight(GetColorF(1.0f, 1.0f, 1.0f, 1.0f));
    SetMaterialUseVertDifColor(FALSE);

    // カメラ制御
    m_pCamera = std::make_unique<CameraController>();
    m_pCamera->InitMouse();

    // ジャンプ・物理
    m_VelocityY = 0.0f;		// Y速度
    m_Gravity = -400.0f;	// 重力
    m_JumpPower = 110.0f;	// ジャンプ力
    m_IsGround = true;		// 接地フラグ
    m_StunTimer = 0.0f;     //着地硬直時間
    m_PrevJumpKeyState = 0; //ジャンプの前のフレームのキー入力状態(押されているか)

    // ダッシュ関連
    m_MoveSpeed = 50.0f;
    m_DashMultiplier = 2.0f;  // 2倍速
    m_IsDashing = false;

    // shaderのハンドルを初期化
    m_VSHandle = -1;
    m_PSHandle = -1;
    m_OutlineVSHandle = -1;
    m_OutlinePSHandle = -1;
    m_CBufferHandle = -1;
    LoadModel();

    // アイテム関係
    m_OrbManager = nullptr;
    m_OrbCount = 0;
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

    m_PlayerRadius = 4.0f;  // キャラクターの半径
    m_PlayerHeight = 20.0f;  // キャラクターの全体高さ

    // --- 頂点シェーダーの読み込み ---
    int vsFileHandle = FileRead_open("Game/assets/shaders/VertexShader.vso");
    if (vsFileHandle != 0) 
    {
        int64_t fileSize = FileRead_size("Game/assets/shaders/VertexShader.vso");
        std::vector<char> vsBuffer(fileSize);
        FileRead_read(vsBuffer.data(), (int)fileSize, vsFileHandle);
        FileRead_close(vsFileHandle);

        m_VSHandle = LoadVertexShaderFromMem(vsBuffer.data(), vsBuffer.size());
    }
    else 
    {
        m_VSHandle = -1;
    }

    // --- ピクセルシェーダーの読み込み ---
    int psFileHandle = FileRead_open("Game/assets/shaders/PixelShader.pso");
    if (psFileHandle != 0)
    {
        int64_t fileSize = FileRead_size("Game/assets/shaders/PixelShader.pso");
        std::vector<char> psBuffer(fileSize);
        FileRead_read(psBuffer.data(), (int)fileSize, psFileHandle);
        FileRead_close(psFileHandle);

        m_PSHandle = LoadPixelShaderFromMem(psBuffer.data(), psBuffer.size());
    }
    else 
    {
        m_PSHandle = -1;
    }
}

// -----------------更新処理----------------------------------------------
void Player::Update(float deltaTime, CollisionManager* collisionManager)
{
    // フレームレートが極端に低い場合の補正
    deltaTime = min(deltaTime, 0.05f);

    // ジャンプキーの状態を取得
    int currentJumpKeyState = CheckHitKey(KEY_INPUT_SPACE);

    // 着地硬直の処理
    if (m_StunTimer > 0.0f)
    {
        m_StunTimer -= deltaTime;

    }

    float yaw = m_pCamera->GetYaw();

    // カメラ前方向
    VECTOR forward =
    {
        sinf(yaw),
        0.0f,
        cosf(yaw)
    };

    VECTOR right =
    {
        cosf(yaw),
        0.0f,
        -sinf(yaw)
    };

    // 入力方向
    VECTOR move = VGet(0.0f, 0.0f, 0.0f);

    // 前のフレームの位置を保存
    VECTOR previousPosition = m_Position;

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
    
    // ダッシュの判定
    bool isMoving = (VSize(move) > 0.0f);
    m_IsDashing = CheckHitKey(KEY_INPUT_LSHIFT) && isMoving;

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

        // ダッシュ中は速度を増加
        if (m_IsDashing) speed *= m_DashMultiplier;

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

        collisionManager->ResolvePlayerCollision(
            m_Position,
            previousPosition,
            m_VelocityY,
            m_IsGround,
            m_PlayerHeight,
            m_PlayerRadius);
    }

    // 着地した瞬間の処理(前フレで空中だった場合)
    if (m_IsGround == true && wasGround == false)
    {
        m_StunTimer = 0.05f;//落下硬直
        m_VelocityY = 0.0f;
    }

    // 場外落下時の復帰処理
    if (m_Position.y <= -10.0f)
    {
        m_Position.x = 300.0f;
        m_Position.y = 580.0f;
        m_Position.z = 0.0f;
        m_VelocityY = 0.0f;
    }

    m_PrevJumpKeyState = currentJumpKeyState;

    // アイテム処理
    if (CheckHitKey(KEY_INPUT_E))
    {
        if (m_OrbManager == nullptr)
        {
            return;
        }

        // プレイヤーの周囲にあるオーブを検索
        auto orb = m_OrbManager->FindNearestOrb(GetPosition(), OrbManager::ORB_PICKUP_RANGE);
 
        if (orb && m_HoldingOrbId == 0)
        {
            // オーブをプレイヤーが持っている状態にする
            orb->GetData().m_State = OrbState::Player;

            // オーブの位置をプレイヤーの位置に合わせる
            m_HoldingOrbId = orb->GetData().m_Id;

            m_OrbCount++;
        }
    }

    // カメラ設定
    m_pCamera->Update(deltaTime, m_Position, m_IsDashing, isMoving);
    m_pCamera->Apply();

    // オーブを放す
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
    DrawFormatString(20, 20, GetColor(255, 255, 0),
        "VS=%d PS=%d Model=%d", m_VSHandle, m_PSHandle, m_Modelhandle);
    // 新しい向きをセット
    MV1SetRotationXYZ(m_Modelhandle, VGet(0.0f, m_PlayerAngle, 0.0f));


    //デバッグ
    VECTOR bottomCenter =
        VAdd(m_Position, VGet(0.0f, m_PlayerRadius, 0.0f));

    VECTOR topCenter =
        VAdd(m_Position,
            VGet(0.0f,
                m_PlayerHeight - m_PlayerRadius,
                0.0f));
    DrawSphere3D(bottomCenter, m_PlayerRadius, 16, GetColor(255, 0, 0), GetColor(255, 0, 0), TRUE);
    DrawSphere3D(topCenter, m_PlayerRadius, 16, GetColor(0, 255, 0), GetColor(0, 255, 0), TRUE);



    // 3Dモデルに新しい座標をセット
    VECTOR drawPos = m_Position;
    drawPos.y += 0.0f;

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

    /* if (m_Modelhandle == -1) return;
     // モデル姿勢
     MV1SetRotationXYZ(m_Modelhandle, VGet(0.0f, m_PlayerAngle, 0.0f));
     VECTOR drawPos = m_Position;
     drawPos.y += (m_PlayerHeight * 1.6f);
     MV1SetPosition(m_Modelhandle, drawPos);
     // 毎フレーム描画状態をリセット（
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
     SetPSConstF(1, c1);s
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
     SetUseBackCulling(TRUE);*/
}

void Player::SetCameraSpawn(float yaw, float pitch, float distance)
{
    if (!m_pCamera) return;

    m_pCamera->SetCameraParameter(yaw, pitch, distance);

    m_pCamera->Warp(m_Position);
}

void Player::SetPosition(const VECTOR& position)
{
    m_Position = position;
}

void Player::SetRotation(float angle)
{
    m_PlayerAngle = angle;
}

int Player::GetOrbCount() const
{
    return m_OrbCount;
}

void Player::SetOrbManager(OrbManager* orbManager)
{
    m_OrbManager = orbManager;
}

void Player::DropOrb()
{
    if (m_HoldingOrbId == 0) return;

    auto orb = m_OrbManager->FindOrbById(m_HoldingOrbId);

    if (!orb) return;

    VECTOR dropPos = GetPosition();

    orb->SetPosition(dropPos);

    orb->GetData().m_State = OrbState::World;

    orb->SetGround(false);

    orb->SetVelocityY(0.0f);

    m_HoldingOrbId = 0;
}
