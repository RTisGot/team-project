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

    if (!m_OrbManager->Init())
    {
        printfDx("OrbManager Init Failed\n");
    }

    std::ifstream file("Game/data/maps/KindergartenMap.json");

    if (!file.is_open())
    {
        printfDx("KindergartenMap.json Open Failed\n");
        return;
    }

    json root;

    file >> root;

    if (!root.contains("orbs"))
    {
        printfDx("OrbManager Init Failed\n");
        return;
    }
    auto& orbs = root["orbs"];

    for (const auto& orb : orbs)
    {
        uint32_t id = orb["id"];
        float x = orb["position"]["x"];
        float y = orb["position"]["y"];
        float z = orb["position"]["z"];

        m_OrbManager->CreateOrb(id, VGet(x, y, z));
    }

    // カメラのクリップ距離を設定
    SetCameraNearFar(16.0f, 5000.0f);

    // TPS視点用にマウスカーソルを非表示
    SetMouseDispFlag(FALSE);

    m_kindergartenMap = std::make_unique<KindergartenMap>();
    m_kindergartenMap->Init();

    m_playObject = std::make_unique<PlayObject>();
    m_playObject->Init();

    // プレイヤーの生成
    m_player = std::make_unique<Player>();
    m_player->SetOrbManager(m_OrbManager.get());

    // 当たり判定マネージャーの生成とステージモデル登録
    m_collisionManager = std::make_unique<CollisionManager>();
    m_collisionManager->Init(m_kindergartenMap->GetModelHandle());

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
    if (m_player && m_kindergartenMap)
    {
        m_player->Update(deltaTime, m_collisionManager.get());
    }

    if (m_enemy)
    {
        m_enemy->Update(m_collisionManager.get());
    }

    if (m_kindergartenMap)
    {
        m_kindergartenMap->Update();
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
    if (m_kindergartenMap)
    {
        m_kindergartenMap->Draw();
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
