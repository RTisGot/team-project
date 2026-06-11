#include "DxLib.h"
#include "../../include/ui/UIManager.h"
#include "../../include/player/Player.h"
#include "../../include/HP/HP.h" // HPクラスの関数を呼ぶのでインクルード
#include <system/Define.h>

UIManager::UIManager()
{
    m_UIBGHandle = -1;
    m_UIBarHandle = -1;
    m_SkillUIHandle = -1;
        for (int i = 0; i < 4; ++i)
        {
            m_CookieHandle[i] = -1;
        }
        for (int i = 0; i < 3; ++i)
        {
            m_CookieCount[i] = 0;
        }
        m_SkillCount = 0;
}

void UIManager::LoadResources()
{
    m_UIBGHandle = LoadGraph("Game/assets/ui/ui_bg.png");
    m_UIBarHandle = LoadGraph("Game/assets/ui/ui_bar.png");
    m_SkillUIHandle = LoadGraph("Game/assets/ui/skill_ui.png");

    m_CookieHandle[0] = LoadGraph("Game/assets/ui/cookie_0.png");
    m_CookieHandle[1] = LoadGraph("Game/assets/ui/cookie_1.png");
    m_CookieHandle[2] = LoadGraph("Game/assets/ui/cookie_2.png");
    m_CookieHandle[3] = LoadGraph("Game/assets/ui/cookie_3.png");

    m_SkillCount = 3;
}

void UIManager::Draw(Player* player)
{

    // 左下スキルUI
    if (m_SkillUIHandle != -1)
    {
        DrawGraph(10, 580, m_SkillUIHandle, TRUE);
        // クッキーアイコンの描画
        if (m_CookieHandle[0] != -1)
        {
            const int cookieY = 620;
            const int startX = 120;
            const int spaceX = 50;

            for (int i = 0; i < 3; i++)
            {
                DrawGraph(
                    startX + (spaceX * i),
                    cookieY,
                    m_CookieHandle[m_CookieCount[i]],
                    TRUE);
            }
        }
    }

    //playerを取得できなかったら返す
    if (player == nullptr) return;

    //playerからHPコンポーネントを取得
    HP* hpComp = player->GetHP();
    //HPコンポーネントを取得できなかったら返す
    if (hpComp == nullptr) return;

    const int hpBlockMax = 5;

    float hpRatio =
        hpComp->GetCurrentHP() / hpComp->GetMaxHP();

    int currentBlock =
        static_cast<int>(hpRatio * hpBlockMax + 0.5f);

    const int hpBoxSize = 25;
    const int hpSpace = 5;

    int screenX;
    int screenY;

    GetDrawScreenSize(&screenX, &screenY);

    //DrawFormatString(
    //    screenX - 200,
    //    30,
    //    GetColor(0, 255, 255),
    //    "Orb : %d",
    //    player->GetOrbCount());

    for (int i = 0; i < hpBlockMax; i++)
    {
        int x = 1050 + i * (hpBoxSize + hpSpace);
        int y = 650;

        if (i < currentBlock)
        {
            DrawBox(
                x,
                y,
                x + hpBoxSize,
                y + hpBoxSize,
                GetColor(0, 255, 0),
                TRUE);
        }
        else
        {
            DrawBox(
                x,
                y,
                x + hpBoxSize,
                y + hpBoxSize,
                GetColor(80, 80, 80),
                TRUE);
        }

        // 枠
        DrawBox(
            x,
            y,
            x + hpBoxSize,
            y + hpBoxSize,
            GetColor(255, 255, 255),
            FALSE);
    }
}

