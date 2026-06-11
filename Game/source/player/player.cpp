#include "player/player.h"
#include "follower/follower.h"
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

//リストでアニメーションの種類とBlenderでの名前を管理
AnimMap animList[] = {
    {"アーマチュア|Idle", ANIM_IDLE},
    {"アーマチュア|Run", ANIM_RUN},
    {"アーマチュア|Walk", ANIM_WALK},
    {"アーマチュア|Jump", ANIM_JUMP},
    {"アーマチュア|Item.Walk", ANIM_ITEMWALK},
    {"アーマチュア|Item.Run", ANIM_ITEMRUN},
    {"Walk->Idle", ANIM_WALKTOIDLE},
    {"Item.Walk->Idle", ANIM_WALKTOIDLE_ITEM},
};

// コンストラクタ
// プレイヤーとカメラ情報を初期化
Player::Player()
{
     m_AudioManager.Init();

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
    // TODO: アニメーションのバグが発生しているため一時的にダッシュ機能を無効化
    //m_DashMultiplier = 1.7f;  // 2倍速
    //m_IsDashing = false;

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
    m_pFollower = nullptr;

    //アニメーションの初期化
    InitAnimations();

    PlayAnim(ANIM_IDLE);
}

void Player::LoadModel()
{
    m_Modelhandle = MV1LoadModel("Game/assets/models/Character/Character.mv1");
   
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

// アニメーションの初期化
void Player::InitAnimations() {
    // 1. 配列をリセット
    for (int i = 0; i < 8; i++) animIndices[i] = -1;

    // 2. リストを走査して番号を埋める
    int totalAnims = MV1GetAnimNum(m_Modelhandle);
    //printfDx("--- モデルが持つアニメーション名一覧 ---\n");
    for (int i = 0; i < totalAnims; i++) {
        const char* name = MV1GetAnimName(m_Modelhandle, i);
        //printfDx("Index %d: '%s'\n", i, name);
        // animListはグローバルか、どこか参照できる場所に置いてください
        for (int j = 0; j < 8; j++) {
            if (strcmp(name, animList[j].nameInBlender) == 0) {
                animIndices[animList[j].type] = i;
            }
        }
    }
}

// 指定したAnimTypeを再生する
void Player::PlayAnim(AnimType type) {
    int index = animIndices[type];
  
    if (m_AnimAttachIndex != -1) {
        int currentPlayingAnimIndex = MV1GetAttachAnim(m_Modelhandle, m_AnimAttachIndex);
        if (currentPlayingAnimIndex == index) {
            return; // すでに再生中なら何もしない
        }
    }
    if (index != -1) {
        if (m_AnimAttachIndex != -1) {
            MV1DetachAnim(m_Modelhandle, m_AnimAttachIndex);
        }

      
        m_AnimAttachIndex = MV1AttachAnim(m_Modelhandle, index, -1, FALSE);

        // 3. アニメーションの長さを取得
        m_AnimTotalTime = MV1GetAttachAnimTotalTime(m_Modelhandle, m_AnimAttachIndex);
        m_AnimTime = 0.0f; // 再生時間を先頭に戻す
    }
}

void Player::SetFollower(Follower* pFollower)
{
    m_pFollower = pFollower;
}

// -----------------更新処理----------------------------------------------
void Player::Update(float deltaTime, CollisionManager * collisionManager)
{
    // フレームレート補正
    deltaTime = min(deltaTime, 0.05f);

    // ジャンプキーの更新用
    int currentJumpKeyState = CheckHitKey(KEY_INPUT_SPACE);

    // 着地硬直の処理
    if (m_StunTimer > 0.0f) m_StunTimer -= deltaTime;

    // --- カメラ・移動計算 ---
    float yaw = m_pCamera->GetYaw();
    VECTOR forward = { sinf(yaw), 0.0f, cosf(yaw) };
    VECTOR right = { cosf(yaw), 0.0f, -sinf(yaw) };
    VECTOR move = VGet(0.0f, 0.0f, 0.0f);
    VECTOR previousPosition = m_Position;

    if (m_StunTimer > 0.0f) {
        move = VGet(0.0f, 0.0f, 0.0f);
    }
    else {
        if (CheckHitKey(KEY_INPUT_W)) move = VAdd(move, forward);
        if (CheckHitKey(KEY_INPUT_S)) move = VSub(move, forward);
        if (CheckHitKey(KEY_INPUT_D)) move = VAdd(move, right);
        if (CheckHitKey(KEY_INPUT_A)) move = VSub(move, right);

        // ジャンプ判定
        if (currentJumpKeyState == 1 && m_PrevJumpKeyState == 0 && m_IsGround) {
            m_VelocityY = m_JumpPower;
            m_IsGround = false;
        }
    }

    // ダッシュ判定
    bool isMoving = (VSize(move) > 0.0f);
    //m_IsDashing = CheckHitKey(KEY_INPUT_LSHIFT) && isMoving;

    // プレイヤー移動処理
    if (isMoving) {
        move = VNorm(move);
        //float speed = m_MoveSpeed * (m_IsDashing ? m_DashMultiplier : 1.0f);
        float speed = m_MoveSpeed;
        m_Position = VAdd(m_Position, VScale(move, speed * deltaTime));
        float targetAngle = atan2f(move.x, move.z);
        float diff = targetAngle - m_PlayerAngle;
        while (diff < -DX_PI_F) diff += DX_PI_F * 2.0f;
        while (diff > DX_PI_F) diff -= DX_PI_F * 2.0f;
        m_PlayerAngle += diff * 5.0f * deltaTime;
    }

    // --- アニメーション状態の決定 (ここで nextAnim を決める) ---
    AnimType nextAnim = ANIM_IDLE;
    if (!m_IsGround) {
        nextAnim = ANIM_JUMP;
    }
    else if (isMoving) {
        if (m_HoldingOrbId)
        {
            //nextAnim = (m_IsDashing) ? ANIM_ITEMRUN : ANIM_ITEMWALK;
            nextAnim = ANIM_WALK;
        }
        else {
            //nextAnim = (m_IsDashing) ? ANIM_RUN : ANIM_WALK;
            nextAnim = ANIM_WALK;
        }
    }
    PlayAnim(nextAnim); // 

    // --- アニメーション時間進行 ---
    if (m_Modelhandle != -1 && m_AnimAttachIndex != -1) {
        m_AnimTime += 18.0f * deltaTime;
        if (m_AnimTime >= m_AnimTotalTime) m_AnimTime -= m_AnimTotalTime;
    }

    // --- 物理・衝突・その他 
    bool wasGround = m_IsGround;
    m_VelocityY += m_Gravity * deltaTime;
    m_Position.y += m_VelocityY * deltaTime;
    if (collisionManager) collisionManager->ResolvePlayerCollision(m_Position, previousPosition, m_VelocityY, m_IsGround, m_PlayerHeight, m_PlayerRadius);
    if (m_IsGround && !wasGround) { m_StunTimer = 0.05f; m_VelocityY = 0.0f; }
    if (m_Position.y <= -10.0f) { m_Position = VGet(300.0f, 580.0f, 0.0f); m_VelocityY = 0.0f; }

    m_PrevJumpKeyState = currentJumpKeyState;

    // --- アイテム・カメラ・HP処理 (既存処理) ---
    if (CheckHitKey(KEY_INPUT_E)) {
        if (m_OrbManager) {
            auto orb = m_OrbManager->FindNearestOrb(GetPosition(), OrbManager::ORB_PICKUP_RANGE);
            if (orb && m_HoldingOrbId == 0) {
                orb->GetData().m_State = OrbState::Player;
                m_HoldingOrbId = orb->GetData().m_Id;
                m_OrbCount++;
            }
        }
    }

    // Gキーでお供にオーブを渡す
    if (CheckHitKey(KEY_INPUT_G))
    {
        if (m_HoldingOrbId != 0 &&
            m_pFollower != nullptr)
        {
            m_pFollower->GetInventory().AddItem(
                m_HoldingOrbId);

            m_HoldingOrbId = 0;
        }
    }

    //m_pCamera->Update(deltaTime, m_Position, m_IsDashing, isMoving);
    m_pCamera->Update(deltaTime, m_Position, false, isMoving);
    m_pCamera->Apply();
    if (CheckHitKey(KEY_INPUT_G)) DropOrb();
    m_PlayerHP.Update();

    //----------HP処理---------------
        // HPの更新
    m_PlayerHP.Update();

    float currentHP = m_PlayerHP.GetCurrentHP();

    if (currentHP < m_PrevHP)
    {
        m_AudioManager.PlaySE(SEType::Damage);
    }

    m_PrevHP = currentHP;
}
// 描画処理(キャラクター描画)
void Player::Draw()
{
    //DrawFormatString(20, 20, GetColor(255, 255, 0), "VS=%d PS=%d Model=%d", m_VSHandle, m_PSHandle, m_Modelhandle);
    // 新しい向きをセット
    MV1SetRotationXYZ(m_Modelhandle, VGet(0.0f, m_PlayerAngle, 0.0f));

    // 3Dモデルに新しい座標をセット
    VECTOR drawPos = m_Position;
    drawPos.y += 0.0f;

    MV1SetPosition(m_Modelhandle, drawPos);

    if (m_AnimAttachIndex != -1) {
        MV1SetAttachAnimTime(m_Modelhandle, m_AnimAttachIndex, m_AnimTime);
    }

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
   /*if (m_OutlineVSHandle != -1 && m_OutlinePSHandle != -1)
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

     if (m_Modelhandle == -1) return;
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

void Player::SetStageModelHandle(int modelHandle)
{
    if (m_pCamera)
    {
        m_pCamera->SetStageModelHandle(modelHandle);
    }
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

bool Player::IsHoldingOrb() const
{
    return m_HoldingOrbId != 0;
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
