#pragma once
//キャラクターの体力管理

//前方宣言
class Player;

class HP
{
private:
    float m_MAXHP;    //最大HP
    float m_currentHP;//現在のHP
    float m_DisplayHP;//表示用のHP

public:
    HP();
    void Update();

    void ApplyDamage(float damage); //受けたダメージを適用する関数
    void Recover(float healAmount); //回復する関数

    float GetHP() const { return m_currentHP; }
    float GetMaxHP() const { return m_MAXHP; }
    float GetDisplayHP() const { return m_DisplayHP; }
};
