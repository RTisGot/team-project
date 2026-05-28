#include "scene/ExploreScene.h"
#include "player/player.h" // Player の完全型が必要な箇所（デストラクタ定義）でインクルード

ExploreScene::ExploreScene(SceneManager* manager)
    : m_manager(manager)
{
}

ExploreScene::~ExploreScene() = default; // Player が定義済みの翻訳単位でデフォルトデストラクタを生成

void ExploreScene::Init()
{
    // 探索シーンの初期化処理
}

void ExploreScene::Update()
{
    // 探索シーンの更新処理
}

void ExploreScene::Draw()
{
    // 探索シーンの描画処理
}
