#include "scene/TitleScene.h"
#include "scene/LobbyScene.h"
#include <DxLib.h>
#include <memory>
#include <scene/LoadingScene.h>

TitleScene::TitleScene(SceneManager* manager)
    : m_manager(manager)
{
}

void TitleScene::Init()
{
    // タイトルシーンの初期化処理
}

void TitleScene::Update()
{
    // スペースキーが押されたか判定
    if (CheckHitKey(KEY_INPUT_SPACE) == 1)
    {
        auto lobbyScene =
            std::make_shared<LobbyScene>(m_manager);

        m_manager->ChangeScene(
            std::make_shared<LoadingScene>(
                m_manager,
                lobbyScene));
    }
}

void TitleScene::Draw()
{
    // タイトルの文字を描画a
    DrawString(100, 100, "ひろって、かえろう。", GetColor(255, 255, 255));
    DrawString(100, 150, "Press SPACE to Start", GetColor(200, 200, 200));
}
