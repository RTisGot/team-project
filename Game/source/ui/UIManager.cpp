#include "DxLib.h"
#include "../../include/ui/UIManager.h"
#include "../../include/player/Player.h"
#include "../../include/HP/HP.h" // HPクラスの関数を呼ぶのでインクルード

UIManager::UIManager()
{
    m_UIBGHandle = -1;
    m_UIBarHandle = -1;
}

void UIManager::LoadResources()
{
    m_UIBGHandle = LoadGraph("Game/assets/ui/ui_bg.png");
    m_UIBarHandle = LoadGraph("Game/assets/ui/ui_bar.png");
}

void UIManager::Draw(Player* player)
{
    //playerを取得できなかったら返す
    if (player == nullptr) return;

    //playerからHPコンポーネントを取得
    HP* hpComp = player->GetHP();
    //HPコンポーネントを取得できなかったら返す
    if (hpComp == nullptr) return;

  
    // HPゲージとテキスト表記の描画
    int sx = 80;
    int sy = 600;
    int w = 300;
    int h = 20;

    // 背景バー
    if (m_UIBGHandle != -1) DrawGraph(sx, sy, m_UIBGHandle, TRUE);

    // 遅れて減る白いゲージ（DisplayHP）
    float displayRatio = hpComp->GetHP() / hpComp->GetMaxHP();
    DrawRectGraph(sx, sy, 0, 0, (int)(w * displayRatio), h, m_UIBarHandle, TRUE); // 本来は白いバー画像

    // 現在の緑ゲージ（HP）
    float hpRatio = hpComp->GetHP() / hpComp->GetMaxHP();
    DrawRectGraph(sx, sy, 0, 0, (int)(w * hpRatio), h, m_UIBarHandle, TRUE);

    
    DrawFormatString(sx + 10, sy + 2, GetColor(255, 255, 255), "%d / %d", (int)hpComp->GetHP(), (int)hpComp->GetMaxHP());
}
