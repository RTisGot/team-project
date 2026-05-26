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

private:
    SceneManager* m_manager; ///< シーンマネージャーへのポインタ
};
