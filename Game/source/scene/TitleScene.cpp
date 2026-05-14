#include "scene/TitleScene.h"
#include "scene/GameScene.h"
#include <DxLib.h>
#include <memory>

TitleScene::TitleScene(SceneManager* manager)
    : m_manager(manager)
{
}

void TitleScene::Initialize()
{
    // タイトルシーンの初期化処理
}

void TitleScene::Update()
{
    // スペースキーが押されたか判定
    if (CheckHitKey(KEY_INPUT_SPACE) == 1)
    {
        // 次のシーン(GameScene)を生成して遷移
        m_manager->ChangeScene(std::make_shared<GameScene>(m_manager));
    }
}

void TitleScene::Draw()
{
    // タイトルの文字を描画a
    DrawString(100, 100, "ひろって、かえろう。 ", GetColor(255, 255, 255));
    DrawString(100, 150, "Press SPACE to Start", GetColor(200, 200, 200));
}