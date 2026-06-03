#pragma once
#pragma once
#include "scene/BaseScene.h"
#include "scene/SceneManager.h"
#include "player/Player.h"
#include "map/RoofTop.h"
#include "follower/follower.h"
#include "system/LightManager.h"
#include "Collision/CollisionManager.h"
#include <memory>
#include <DxLib.h>


class Player;

/**
 * @brief 屋上シーン
 */
class LobbyScene : public BaseScene
{
public:
    LobbyScene(SceneManager* manager);
    ~LobbyScene() override = default;

    void Init() override;
    void Update(float deltaTime) override;
    void Draw() override;

private:
    /**
     * @brief デバッグ用床グリッドの描画
     */
    void DrawDebugGrid();
    void DrawRoomDebug();

    bool m_isPlayerInRoom; ///< プレイヤーが部屋にいるかどうかのフラグ

    VECTOR m_roomMin;
    VECTOR m_roomMax;

    SceneManager* m_manager; ///< シーンマネージャー
    std::unique_ptr<Player> m_player; ///< プレイヤーインスタンス
    std::unique_ptr<CollisionManager> m_collisionManager; ///< 当たり判定マネージャー
    std::unique_ptr<RoofTop> m_roofTop; ///< 屋上インスタンス
    std::unique_ptr<LightManager> m_lightManager; ///< ライトマネージャー
};
