#include "scene/GameOverScene.h"
#include "scene/SceneManager.h"
#include "scene/TitleScene.h"

#include <DxLib.h>
#include <memory>

GameOverScene::GameOverScene(SceneManager* manager)
    : m_manager(manager)
{
}

void GameOverScene::Init()
{
}

void GameOverScene::Update(float deltaTime)
{
    if (CheckHitKey(KEY_INPUT_SPACE))
    {
        m_manager->ChangeScene(
            std::make_shared<TitleScene>(
                m_manager));
    }
}

void GameOverScene::Draw()
{
    DrawBox(
        0,
        0,
        1280,
        720,
        GetColor(0, 0, 0),
        TRUE);

    DrawString(
        520,
        300,
        "GAME OVER",
        GetColor(255, 0, 0));

    DrawString(
        470,
        360,
        "Press SPACE",
        GetColor(255, 255, 255));
}
