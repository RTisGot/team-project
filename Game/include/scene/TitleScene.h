#pragma once
#include "scene/BaseScene.h"
#include "scene/SceneManager.h"

/**
 * @file TitleScene.h
 * @brief タイトルシーンの定義
 */

/**
 * @class TitleScene
 * @brief タイトルシーン
 * @details ゲーム開始前の画面でスペースキーを押すとゲームシーンに遷移する。
 */
class TitleScene : public BaseScene
{
public:
    TitleScene(SceneManager* manager);
    ~TitleScene() override = default;

    void Init() override;
    void Update() override;
    void Draw() override;
    void Loadgraph();

    
private:
    SceneManager* m_manager; ///< シーンマネージャーへのポインタ
    // 演出の列挙型
    enum class Phase
    {
        LogoFadeIn,   // ロゴのフェードイン
        LogoStay,     // ロゴが静止中
        LogoFadeOut,  // ロゴのフェードアウト
        TitleMain     // 本体のタイトル画面
    };

    Phase m_CurrentPhase;  // 現在のフェーズ
    int m_LogoHandle;      // チームロゴの画像ハンドル
    int m_TitleMainHandle; // 本体のタイトル背景画像ハンドル（もしあれば）

    int m_Timer;           // ロゴ用のタイマー
    float m_Alpha;         // ロゴの不透明度
};
