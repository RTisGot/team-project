#include "scene/ExploreScene.h"
#include "thirdparty/json.hpp"
#include <fstream>

using json = nlohmann::json;
ExploreScene::ExploreScene(SceneManager* manager)
    : m_manager(manager)
{
}

ExploreScene::~ExploreScene() = default; // Player が定義済みの翻訳単位でデフォルトデストラクタを生成

void ExploreScene::Init()
{
    m_OrbManager = std::make_unique<OrbManager>();


    // カメラのクリップ距離を設定
    SetCameraNearFar(16.0f, 5000.0f);

    // TPS視点用にマウスカーソルを非表示
    SetMouseDispFlag(FALSE);

    // マップの生成と初期化
    m_CurrentMap = std::make_unique<KindergartenMap>();

    if (!m_CurrentMap->Init())
    {
        return;
    }

    const auto& mapData = m_CurrentMap->GetMapData();

    // 遊具の生成と初期化
    m_playObject = std::make_unique<PlayObject>();
    m_playObject->Init();

    // プレイヤーの生成
    m_player = std::make_unique<Player>();
    m_player->SetPosition(mapData.PlayerSpawn.Position);
    m_player->SetRotation(mapData.PlayerSpawn.Rotation.y);

    m_player->SetOrbManager(m_OrbManager.get());

    // 当たり判定マネージャーの生成とステージモデル登録
    m_collisionManager = std::make_unique<CollisionManager>();
    m_collisionManager->Init(m_CurrentMap->GetModelHandle());

    m_enemy = std::make_unique<Enemy>();
    m_enemy->Init();

    // ライトマネージャーの生成と初期化
    m_lightManager = std::make_unique<LightManager>();
    m_lightManager->Init();

    m_UIManager = std::make_unique<UIManager>();
    m_UIManager->LoadResources();
}

void ExploreScene::Update(float deltaTime)
{
    // プレイヤー更新（入力・移動・カメラ更新を含む）
    if (m_player && m_CurrentMap)
    {
        m_player->Update(deltaTime, m_collisionManager.get());
    }

    if (m_enemy)
    {
        m_enemy->Update(m_collisionManager.get());
    }

    if (m_CurrentMap)
    {
        m_CurrentMap->Update();
    }

    if (m_playObject)
    {
        m_playObject->Update();
    }

    if (m_OrbManager)
    {
        m_OrbManager->Update(m_player.get(), m_collisionManager.get());
    }
}

void ExploreScene::Draw()
{
    if (m_CurrentMap)
    {
        m_CurrentMap->Draw();
    }

    if (m_playObject)
    {
        m_playObject->Draw();
    }

    if (m_OrbManager)
    {
        m_OrbManager->Draw();
    }

    if (m_player)
    {
        m_player->Draw();
    }

    if (m_enemy)
    {
        m_enemy->Draw();
    }

    if(m_UIManager)
    {
        m_UIManager->Draw(m_player.get());
    }  

    DrawString(
        10,
        10,
        "Explore Scene",
        GetColor(255, 255, 255));

    // プレイヤー座標を取得して表示
    VECTOR playerPos = { 0.0f, 0.0f, 0.0f };
    if (m_player)
    {
        playerPos = m_player->GetPosition();
    }
    DrawFormatString(
        10,
        100,
        GetColor(255, 255, 255),
        "Player Pos : %.1f %.1f %.1f",
        playerPos.x,
        playerPos.y,
        playerPos.z);
}
