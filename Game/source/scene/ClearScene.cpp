#include "scene/ClearScene.h"
#include "scene/TitleScene.h"
#include "scene/SceneManager.h"
#include <DxLib.h>

ClearScene::ClearScene(SceneManager* manager)
    : m_Manager(manager)
{
}

void ClearScene::Init()
{
}

void ClearScene::Update(float deltaTime)
{

    if (CheckHitKey(KEY_INPUT_RETURN))
    {
        m_Manager->ChangeScene(
            std::make_shared<TitleScene>(m_Manager)
        );
        return;
    }
}

void ClearScene::Draw()
{
    DrawFormatString(
        500,
        300,
        GetColor(255, 255, 0),
        "GAME CLEAR!");
}
