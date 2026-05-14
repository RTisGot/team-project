#pragma once
#include "scene/BaseScene.h"
#include "scene/SceneManager.h"
#include <DxLib.h>

/**
 * @brief ゲームシーン
 */
class GameScene : public BaseScene
{
public:
    GameScene(SceneManager* manager);
    ~GameScene() override = default;

    void Initialize() override;
    void Update() override;
    void Draw() override;

private:
    /**
     * @brief プレイヤーの描画処理
     */
    void DrawPlayerBox();

    SceneManager* m_manager; ///< シーンマネージャー
    VECTOR m_position;       ///< プレイヤーの座標
    float m_angle;           ///< プレイヤーの向き
};
