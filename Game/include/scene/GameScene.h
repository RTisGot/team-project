#pragma once
#include "scene/BaseScene.h"
#include "scene/SceneManager.h"
#include "player/Player.h"
#include "enemy/Enemy.h"
#include "map/RoofTop.h"
#include "system/LightManager.h"
#include <memory>
#include <DxLib.h>

/**
 * @brief ゲームシーン
 */
class GameScene : public BaseScene
{
public:
    GameScene(SceneManager* manager);
    ~GameScene() override = default;

    void Init() override;
    void Update() override;
    void Draw() override;

private:
    /**
     * @brief デバッグ用床グリッドの描画
     */
    void DrawDebugGrid();

    SceneManager* m_manager; ///< シーンマネージャー
    std::unique_ptr<Player> m_player; ///< プレイヤーインスタンス
	std::unique_ptr<RoofTop> m_roofTop; ///< 屋上インスタンス
	std::unique_ptr<LightManager> m_lightManager; ///< ライトマネージャー
    std::unique_ptr<Enemy> m_enemy; ///< 敵インスタンス
};
