#include "scene/LobbyScene.h"
#include "scene/LoadingScene.h"
#include "scene/ExploreScene.h"

LobbyScene::LobbyScene(SceneManager* manager)
    : m_manager(manager)
    , m_isPlayerInRoom(false)    
    , m_IsLoading(false)
    , m_LoadTimer(0.0f)
    , m_roomMin(VGet(0.0f, 0.0f, 0.0f))
    , m_roomMax(VGet(0.0f, 0.0f, 0.0f))
{
}

void LobbyScene::Init()
{
    m_AudioManager.Init();
    m_AudioManager.PlayBGM(BGMType::Title);

    // カメラのクリップ距離を設定
    SetCameraNearFar(16.0f, 5000.0f);

    // TPS視点用にマウスカーソルを非表示
    SetMouseDispFlag(FALSE);

    // 屋上マップの生成と初期化
    m_roofTop = std::make_unique<RoofTop>();

    if (!m_roofTop->Init())
    {
        return;
    }

    // マップデータからプレイヤーのスポーン位置と向きを取得してプレイヤーを初期化
    const auto& mapData = m_roofTop->GetMapData();
    m_player = std::make_unique<Player>();
    m_player->SetPosition(mapData.PlayerSpawn.Position);
    m_player->SetRotation(mapData.PlayerSpawn.Rotation.y);
    m_player->SetCameraSpawn(mapData.PlayerSpawn.Camera.Yaw, mapData.PlayerSpawn.Camera.Pitch, mapData.PlayerSpawn.Camera.Distance);

    m_player->SetStageModelHandle(m_roofTop->GetModelHandle());

    // 当たり判定マネージャーの生成とステージモデル登録
    m_collisionManager = std::make_unique<CollisionManager>();
    m_collisionManager->Init(m_roofTop->GetModelHandle());

    // 屋上のフェンス部分の当たり判定を追加
    CollisionMap* collisionMap = m_collisionManager->GetCollisionMap();
    collisionMap->AddBox(VGet(180.0, 550.0f, -345.0f), VGet(182.0f, 570.0f, 213.0f));
    collisionMap->AddBox(VGet(180.0, 550.0f, -345.0f), VGet(497.0f, 570.0f, -342.0f));
    collisionMap->AddBox(VGet(495.0f, 550.0f, -342.0f), VGet(497.0f, 570.0f, 213.0f));
    collisionMap->AddBox(VGet(182.0f, 550.0f, 210.0f), VGet(495.0f, 570.0f, 213.0f));

    // 次のマップに入るための扉付近部分の当たり判定を追加
    collisionMap->AddBox(VGet(212.0f, 550.0f, -315.5f), VGet(262.0f, 600.0f, -248.0f));

    // ライトマネージャーの生成と初期化
    m_lightManager = std::make_unique<LightManager>();
    m_lightManager->Init();

    m_InteractionUI =
        std::make_unique<InteractionUI>();

    m_IsLoading = false;
    m_LoadTimer = 0.0f;

    // 部屋の範囲を定義
    m_roomMin = VGet(240.0f, 544.0f, -304.0f);
    m_roomMax = VGet(272.0f, 576.0f, -248.0f);
}

void LobbyScene::Update(float deltaTime)
{
    // プレイヤー更新（入力・移動・カメラ更新を含む）
    if (m_player && m_roofTop)
    {
        m_player->Update(deltaTime,m_collisionManager.get());
        m_roofTop->Update();
        m_roofTop->UpdateFollower(
            m_player->GetPosition(),
            m_player->GetAngle()
        );
    }

    // プレイヤーが部屋の中にいるかどうかを判定
    VECTOR pos = m_player->GetPosition();
    if (pos.x >= m_roomMin.x && pos.x <= m_roomMax.x &&
        pos.y >= m_roomMin.y && pos.y <= m_roomMax.y &&
        pos.z >= m_roomMin.z && pos.z <= m_roomMax.z)
    {
        m_isPlayerInRoom = true;

        m_InteractionUI->SetText(true, "F : 扉に入る");

        if (CheckHitKey(KEY_INPUT_F) == 1)
        {
            m_AudioManager.StopBGM();

            m_AudioManager.PlaySE(SEType::Elevator);

            m_manager->ChangeScene(std::make_shared<LoadingScene>(m_manager, std::make_shared<ExploreScene>(m_manager)));
        }
    }
    else
    {
        m_isPlayerInRoom = false;

        m_InteractionUI->SetText(
            false,
            "");
    }
}

void LobbyScene::Draw()
{
    // マップ描画
    if (m_roofTop)
    {
        m_roofTop->Draw();
    }

    // プレイヤー描画
    if (m_player)
    {
        m_player->Draw();
    }

    if (m_InteractionUI)
    {
        m_InteractionUI->Draw();
    }
}
