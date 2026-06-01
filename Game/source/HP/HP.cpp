#include "../include/HP/HP.h"

HP::HP()
{
    m_MAXHP = 100.0f;  //最大HP100
    m_currentHP = m_MAXHP; //現在のHPを最大HPで初期化
    m_DisplayHP = m_MAXHP;
}

void HP::Update()
{
    m_DisplayHP += (m_currentHP - m_DisplayHP) * 0.05; //UIように遅れて表示されるHP

    if (m_currentHP < 0) m_currentHP = 0;//HPが0未満にならないよう
    if (m_currentHP > m_MAXHP) m_currentHP = m_MAXHP;//HPが最大HPを超えないよう
}

//受けたダメージを計算する関数
void HP::ApplyDamage(float damage)
{
    m_currentHP -= damage; //ダメージを現在のHPから引く
}

//HPを回復する関数
void HP::Recover(float healAmount)
{
    m_currentHP += healAmount; //回復量を現在のHPに加える
}
