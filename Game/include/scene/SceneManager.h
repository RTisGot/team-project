#pragma once
#include <memory>
#include "BaseScene.h"

/**
 * @file SceneManager.h
 * @brief シーン管理クラス
 */

/**
 * @class SceneManager
 * @brief シーン管理クラス
 * @details シーンの初期化、更新、描画、シーン遷移を管理するクラス
 */
class SceneManager
{
public:
    SceneManager();
    ~SceneManager() = default;

    /**
     * @brief 初期シーンを設定する
     * @param initialScene 最初に再生するシーン
     */
    void Init(std::shared_ptr<BaseScene> initialScene);
    void Update();
    void Draw();

    /**
     * @brief 次のシーンへ遷移する
     * @param nextScene 遷移先のシーン
     */
    void ChangeScene(std::shared_ptr<BaseScene> nextScene);

private:
    std::shared_ptr<BaseScene> m_currentScene; ///< 現在のシーン
};

