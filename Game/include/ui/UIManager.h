#ifndef DEF_UIMANAGER_H
#define DEF_UIMANAGER_H

//UIを管理するクラス
class Player;  //前方宣言

class UIManager
{
private:
    int m_UIBGHandle;//UIの背景画像のハンドル
    int m_UIBarHandle;//UIのバー画像のハンドル
    int m_SkillUIHandle; //スキルUIのハンドル
    int m_CookieHandle[4];

    int m_SkillCount;
public:
    UIManager();
    void LoadResources(); //UIのリソースを読み込む関数
    void Draw(Player* player); //UIの描画処理
};

#endif // DEF_UIMANAGER_H
