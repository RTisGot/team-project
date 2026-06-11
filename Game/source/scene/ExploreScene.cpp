#include "scene/ExploreScene.h"
#include "thirdparty/json.hpp"
#include <fstream>
#include <item/OrbLoader.h>
#include <scene/LoadingScene.h>
#include <scene/GameOverScene.h>

using json = nlohmann::json;
ExploreScene::ExploreScene(SceneManager* manager)
    : m_manager(manager)
    , m_IsGameOver(false),
    m_StartFade(false),
    m_GameOverTimer(0.0f),
    m_FadeAlpha(0.0f)
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

    m_follower->SetOrbManager(m_OrbManager.get());
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
    m_InteractionUI =
        std::make_unique<InteractionUI>();
        m_IsGameOver = false;
        m_StartFade = false;
        m_GameOverTimer = 0.0f;
        m_FadeAlpha = 0.0f;
}

void ExploreScene::Update(float deltaTime)
{
    // プレイヤー更新（入力・移動・カメラ更新を含む）
    if (!m_IsGameOver)
    {
        if (m_player && m_CurrentMap)
        {
            m_player->Update(
                deltaTime,
                m_collisionManager.get());
        }
    }

    if (!m_IsGameOver &&
        m_player->GetHP()->GetCurrentHP() <= 0.0f)
    {
        m_IsGameOver = true;
        m_GameOverTimer = 0.0f;
    }

    if (m_IsGameOver)
    {
        m_GameOverTimer += deltaTime;

        // 1秒停止
        if (m_GameOverTimer >= 1.0f)
        {
            m_StartFade = true;
        }

        // 暗転
        if (m_StartFade)
        {
            m_FadeAlpha += 255.0f * deltaTime;

            if (m_FadeAlpha > 255.0f)
            {
                m_FadeAlpha = 255.0f;
            }
        }

        // 完全に暗転
        if (m_FadeAlpha >= 255.0f)
        {
            m_manager->ChangeScene(
                std::make_shared<GameOverScene>(m_manager));

            return;
        }

        // プレイヤーや敵の更新を止める
        return;
    }
    /*if (m_player->GetHP()->GetCurrentHP() <= 0.0f)
    {
        auto gameOver = std::make_shared<GameOverScene>(m_manager);

        m_manager->ChangeScene((m_manager, gameOver));

        return;
    }*/

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

    if (m_InteractionUI &&
        m_player &&
        m_OrbManager)
    {
        // オーブ所持中
        if (m_player->IsHoldingOrb())
        {
            m_InteractionUI->SetText(
                true,
                "G : 捨てる");
        }
        else
        {
            auto orb =
                m_OrbManager->FindNearestOrb(
                    m_player->GetPosition(),
                    OrbManager::ORB_PICKUP_RANGE);

            if (orb)
            {
                m_InteractionUI->SetText(
                    true,
                    "E : 拾う");
            }
            else
            {
                m_InteractionUI->SetText(
                    false,
                    "");
            }
        }
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

    if (m_follower)
    {
        m_follower->Draw();
    }

    /*if (m_enemy)
   

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

    if (m_InteractionUI)
    {
        m_InteractionUI->Draw();
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

    if (m_FadeAlpha > 0.0f)
    {
        SetDrawBlendMode(
            DX_BLENDMODE_ALPHA,
            (int)m_FadeAlpha);

        DrawBox(
            0,
            0,
            1920,
            1080,
            GetColor(0, 0, 0),
            TRUE);

        SetDrawBlendMode(
            DX_BLENDMODE_NOBLEND,
            255);
    }

#ifdef _DEBUG

    m_collisionManager->GetCollisionMap()->DrawDebug();

#endif

}
