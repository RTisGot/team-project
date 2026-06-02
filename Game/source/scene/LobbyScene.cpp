#include "scene/LobbyScene.h"
#include "scene/LoadingScene.h"
#include "scene/ExploreScene.h"

LobbyScene::LobbyScene(SceneManager* manager)
    : m_manager(manager)
    , m_isPlayerInRoom(false)
    , m_roomMin (VGet(0.0f, 0.0f, 0.0f))
    , m_roomMax (VGet(0.0f, 0.0f, 0.0f))
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

    // 部屋の範囲を定義
    m_roomMin = VGet(150.0f, 340.0f, -190.0f);
    m_roomMax = VGet(170.0f, 360.0f, -155.0f);
}

void LobbyScene::Update()
{
    // プレイヤー更新（入力・移動・カメラ更新を含む）
    if (m_player && m_roofTop)
    {
        m_player->Update(m_collisionManager.get());

        m_roofTop->Update(
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

#ifdef _DEBUG
    DrawRoomDebug();
#endif

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

void LobbyScene::DrawRoomDebug()
{
#ifdef _DEBUG

    unsigned int color =
        m_isPlayerInRoom ?
        GetColor(0, 255, 0) :
        GetColor(255, 0, 0);

    VECTOR p[8];

    p[0] = VGet(m_roomMin.x, m_roomMin.y, m_roomMin.z);
    p[1] = VGet(m_roomMax.x, m_roomMin.y, m_roomMin.z);
    p[2] = VGet(m_roomMax.x, m_roomMax.y, m_roomMin.z);
    p[3] = VGet(m_roomMin.x, m_roomMax.y, m_roomMin.z);

    p[4] = VGet(m_roomMin.x, m_roomMin.y, m_roomMax.z);
    p[5] = VGet(m_roomMax.x, m_roomMin.y, m_roomMax.z);
    p[6] = VGet(m_roomMax.x, m_roomMax.y, m_roomMax.z);
    p[7] = VGet(m_roomMin.x, m_roomMax.y, m_roomMax.z);

    // 底面
    DrawLine3D(p[0], p[1], color);
    DrawLine3D(p[1], p[2], color);
    DrawLine3D(p[2], p[3], color);
    DrawLine3D(p[3], p[0], color);

    // 上面
    DrawLine3D(p[4], p[5], color);
    DrawLine3D(p[5], p[6], color);
    DrawLine3D(p[6], p[7], color);
    DrawLine3D(p[7], p[4], color);

    // 側面
    DrawLine3D(p[0], p[4], color);
    DrawLine3D(p[1], p[5], color);
    DrawLine3D(p[2], p[6], color);
    DrawLine3D(p[3], p[7], color);

#endif
}
