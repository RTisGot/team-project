#include "scene/LobbyScene.h"
#include "scene/LoadingScene.h"
#include "scene/ExploreScene.h"

LobbyScene::LobbyScene(SceneManager* manager)
    : m_manager(manager)
    , m_isPlayerInRoom(false)
{
}

void LobbyScene::Init()
{
    // カメラのクリップ距離を設定
    SetCameraNearFar(16.0f, 5000.0f);

    // TPS視点用にマウスカーソルを非表示
    SetMouseDispFlag(FALSE);

    // 屋上の生成と初期化
    m_roofTop = std::make_unique<RoofTop>();
    m_roofTop->Init();

    // プレイヤーの生成
    m_player = std::make_unique<Player>();

    // 当たり判定マネージャーの生成とステージモデル登録
    m_collisionManager = std::make_unique<CollisionManager>();
    m_collisionManager->Init(m_roofTop->GetModelHandle());

    // 敵の生成と初期化
    m_enemy = std::make_unique<Enemy>();
    m_enemy->Init();

    // ライトマネージャーの生成と初期化
    m_lightManager = std::make_unique<LightManager>();
    m_lightManager->Init();
}

void LobbyScene::Update()
{
    // プレイヤー更新（入力・移動・カメラ更新を含む）
    if (m_player && m_roofTop)
    {
        m_player->Update(m_collisionManager.get());

        // 部屋の範囲を定義
        VECTOR roomMin = VGet(-500.0f, -100.0f, -500.0f);
        VECTOR roomMax = VGet(500.0f, 1000.0f, 500.0f);

        // プレイヤーが部屋の中にいるかどうかを判定
        VECTOR pos = m_player->GetPosition();
        if (pos.x >= roomMin.x && pos.x <= roomMax.x &&
            pos.y >= roomMin.y && pos.y <= roomMax.y &&
            pos.z >= roomMin.z && pos.z <= roomMax.z)
        {
            m_isPlayerInRoom = true;

            if (CheckHitKey(KEY_INPUT_F) == 1)
            {
                m_manager->ChangeScene(std::make_shared<ExploreScene>(m_manager));
            }
        }
        else
        {
            m_isPlayerInRoom = false;
        }
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

    // デバッグ用UI描画
    DrawString(10, 10, "Lobby Scene - WASD移動 / マウス視点移動 ", GetColor(255, 255, 255));
}

void LobbyScene::DrawDebugGrid()
{
    const int gridSize = 1000;
    const int gridStep = 100;
    int gridColor = GetColor(80, 80, 80);

    // X方向
    for (int z = -gridSize; z <= gridSize; z += gridStep)
    {
        DrawLine3D(
            VGet((float)-gridSize, 0.0f, (float)z),
            VGet((float)gridSize, 0.0f, (float)z),
            gridColor
        );
    }

    // Z方向
    for (int x = -gridSize; x <= gridSize; x += gridStep)
    {
        DrawLine3D(
            VGet((float)x, 0.0f, (float)-gridSize),
            VGet((float)x, 0.0f, (float)gridSize),
            gridColor
        );
    }
}
