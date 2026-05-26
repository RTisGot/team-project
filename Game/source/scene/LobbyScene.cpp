#include "scene/LobbyScene.h"

LobbyScene::LobbyScene(SceneManager* manager)
    : m_manager(manager)
{
}

void LobbyScene::Init()
{
    // カメラのクリップ距離を設定
    SetCameraNearFar(16.0f, 5000.0f);

    // TPS視点用にマウスカーソルを非表示
    SetMouseDispFlag(FALSE);

    m_roofTop = std::make_unique<RoofTop>();
    m_roofTop->Init();

    // プレイヤーの生成
    m_player = std::make_unique<Player>();

    m_enemy = std::make_unique<Enemy>();
    m_enemy->Init(); // 敵の初期位置を設定

    // ライトマネージャーの生成と初期化
    m_lightManager = std::make_unique<LightManager>();
    m_lightManager->Init();
}

void LobbyScene::Update()
{
    // プレイヤー更新（入力・移動・カメラ更新を含む）
    if (m_player && m_roofTop)
    {
        m_player->Update(m_roofTop->GetModelHandle());
    }

    if (m_enemy)
    {
        m_enemy->Update();
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

    // 敵描画
    if (m_enemy)
    {
        m_enemy->Draw();
    }

    // デバッグ用UI描画
    DrawString(10, 10, "Game Scene - WASD移動 / マウス視点移動 ", GetColor(255, 255, 255));
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