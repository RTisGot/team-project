#include "scene/ExploreScene.h"
#include "scene/SceneManager.h"
#include "map/KindergartenMap.h"

#include <DxLib.h>

ExploreScene::ExploreScene(SceneManager* manager)
    : m_manager(manager)
{
}

ExploreScene::~ExploreScene() = default;

void ExploreScene::Init()
{
   
    // カメラのクリップ距離を設定
    SetCameraNearFar(16.0f, 5000.0f);

    // TPS視点用にマウスカーソルを非表示
    SetMouseDispFlag(FALSE);

    m_kindergartenMap = std::make_unique<KindergartenMap>();
    m_kindergartenMap->Init();

    // プレイヤーの生成
    m_player = std::make_unique<Player>();

    // 当たり判定マネージャーの生成とステージモデル登録
    m_collisionManager = std::make_unique<CollisionManager>();
    m_collisionManager->Init(m_kindergartenMap->GetModelHandle());

    m_enemy = std::make_unique<Enemy>();
    m_enemy->Init(); // 敵の初期位置を設定

    // ライトマネージャーの生成と初期化
    m_lightManager = std::make_unique<LightManager>();
    m_lightManager->Init();
}

void ExploreScene::Update()
{

    // プレイヤー更新（入力・移動・カメラ更新を含む）
    if (m_player && m_kindergartenMap)
    {
        m_player->Update(m_collisionManager.get());
    }

    if (m_enemy)
    {
        m_enemy->Update(m_collisionManager.get());
    }


    if (m_kindergartenMap)
    {
        m_kindergartenMap->Update();
    }
}

void ExploreScene::Draw()
{
    if (m_kindergartenMap)
    {
        m_kindergartenMap->Draw();
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

    DrawString(
        10,
        10,
        "Explore Scene",
        GetColor(255, 255, 255));
}
