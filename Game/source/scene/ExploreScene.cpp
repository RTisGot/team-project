#include "scene/ExploreScene.h"
#include "thirdparty/json.hpp"
#include <fstream>
#include <item/OrbLoader.h>
#include <scene/LoadingScene.h>
#include <scene/GameOverScene.h>

using json = nlohmann::json;
ExploreScene::ExploreScene(SceneManager* manager)
    : m_manager(manager)
{
}

ExploreScene::~ExploreScene() = default; // Player が定義済みの翻訳単位でデフォルトデストラクタを生成

void ExploreScene::Init()
{
    m_AudioManager.Init();
    m_AudioManager.PlayBGM(BGMType::Game);

    m_OrbManager = std::make_unique<OrbManager>();

    m_OrbManager->Init();

    OrbLoader loader;
    loader.LoadFromJson("Game/data/maps/KindergartenMap.json", *m_OrbManager);

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
    /*m_playObject = std::make_unique<PlayObject>();
    m_playObject->Init();*/

    // プレイヤーの生成
    m_player = std::make_unique<Player>();
    m_player->SetPosition(mapData.PlayerSpawn.Position);
    m_player->SetRotation(mapData.PlayerSpawn.Rotation.y);
    m_player->SetCameraSpawn(mapData.PlayerSpawn.Camera.Yaw, mapData.PlayerSpawn.Camera.Pitch, mapData.PlayerSpawn.Camera.Distance);

    m_player->SetOrbManager(m_OrbManager.get());

    // お供の生成
    m_follower = std::make_unique<Follower>();
    m_follower->LoadModel();

    // 当たり判定マネージャーの生成とステージモデル登録
    m_collisionManager = std::make_unique<CollisionManager>();
    m_collisionManager->Init(m_CurrentMap->GetModelHandle());

    CollisionMap* collisionMap = m_collisionManager->GetCollisionMap();
    for (const auto& wall : mapData.BoxColliders)
    {
        collisionMap->AddBox(wall.m_Min, wall.m_Max);
    }
    
    m_EnemyManager =
        std::make_unique<EnemyManager>();

    m_EnemyManager->Load(
        "Game/data/maps/KindergartenMap.json");

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

    if (m_player->GetHP()->GetCurrentHP() <= 0.0f)
    {
        auto gameOver = std::make_shared<GameOverScene>(m_manager);

        m_manager->ChangeScene((m_manager, gameOver));

        return;
    }

    // お供更新
    if (m_follower && m_player)
    {
        m_follower->SetTargetPosition(
            m_player->GetPosition());

        m_follower->SetTargetAngle(
            m_player->GetAngle());

        m_follower->Update();
    }

    if (m_EnemyManager)
    {
        m_EnemyManager->Update(
            m_collisionManager.get(),
            m_player.get());
    }

    if (m_CurrentMap)
    {
        m_CurrentMap->Update();
    }

    //if (m_playObject)
    //{
    //    m_playObject->Update();
    //}

    if (CheckHitKey(KEY_INPUT_L))
    {
        m_manager->ChangeScene(
            std::make_shared<ClearScene>(m_manager)
        );
        return;
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

    //if (m_playObject)
    //{
    //    m_playObject->Draw();
    //}

    if (m_OrbManager)
    {
        m_OrbManager->Draw();
    }

    if (m_player)
    {
        m_player->Draw();
    }

    /*if (m_enemy)
    if (m_follower)
    {
        m_follower->Draw();
    }

    if (m_enemy)
    {
        m_enemy->Draw();
    }*/

    if (m_EnemyManager)
    {
        m_EnemyManager->Draw();
    }

    if(m_UIManager)
    {
        m_UIManager->Draw(m_player.get());
    }

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

#ifdef _DEBUG

    m_collisionManager->GetCollisionMap()->DrawDebug();

#endif

}
