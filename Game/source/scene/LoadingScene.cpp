#include "scene/LoadingScene.h"
#include "scene/SceneManager.h"

#include <DxLib.h>
#include <system/Define.h>

LoadingScene::LoadingScene(
    SceneManager* manager,
    std::shared_ptr<BaseScene> nextScene)
    : m_Manager(manager),
    m_NextScene(nextScene),
    m_LogoHandle(-1),
    m_Frame(0)
{
}

void LoadingScene::Init()
{
    m_LogoHandle = LoadGraph("Game/assets/UI/TeamLogo.png");
}

void LoadingScene::Update()
{
    m_Frame++;

    // 2秒後
    if (m_Frame > 120)
    {
        m_Manager->ChangeScene(m_NextScene);
    }
}

void LoadingScene::Draw()
{
    DrawBox(
        0,
        0,
        1920,
        1080,
        GetColor(0, 0, 0),
        TRUE);

    // 点滅
    float alpha =
        (sinf(m_Frame * 0.1f) + 1.0f) * 0.5f;

    SetDrawBlendMode(
        DX_BLENDMODE_ALPHA,
        static_cast<int>(alpha * 255));

    int logoW, logoH;
    GetGraphSize(m_LogoHandle, &logoW, &logoH);

    DrawGraph(
        SCREEN_WIDTH - logoW - 30,
        SCREEN_HEIGHT - logoH - 30,
        m_LogoHandle,
        TRUE);

    SetDrawBlendMode(
        DX_BLENDMODE_NOBLEND,
        0);
}
