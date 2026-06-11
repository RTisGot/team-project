#include "scene/ClearScene.h"
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
    
    //if (CheckHitKey(KEY_INPUT_RETURN))
    //{
    //    
    //}
}

void ClearScene::Draw()
{
    DrawFormatString(
        500,
        300,
        GetColor(255, 255, 0),
        "GAME CLEAR!");

  
}
