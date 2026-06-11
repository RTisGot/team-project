#ifndef DEF_LOBBYSCENE_H
#define DEF_LOBBYSCENE_H

#include "scene/BaseScene.h"
#include "scene/SceneManager.h"
#include "player/Player.h"
#include "map/RoofTop.h"
#include "follower/follower.h"
#include "system/LightManager.h"
#include "audio/AudioManager.h"
#include "Collision/CollisionManager.h"
#include "ui/InteractionUI.h"
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
    bool m_isPlayerInRoom; ///< プレイヤーが部屋にいるかどうかのフラグ
    bool m_IsLoading;
    float m_LoadTimer;
    VECTOR m_roomMin;
    VECTOR m_roomMax;
    AudioManager m_AudioManager;
    SceneManager* m_manager; ///< シーンマネージャー
    std::unique_ptr<InteractionUI> m_InteractionUI;
    std::unique_ptr<Player> m_player; ///< プレイヤーインスタンス
    std::unique_ptr<CollisionManager> m_collisionManager; ///< 当たり判定マネージャー
    std::unique_ptr<RoofTop> m_roofTop; ///< 屋上インスタンス
    std::unique_ptr<LightManager> m_lightManager; ///< ライトマネージャー
};

#endif // DEF_LOBBYSCENE_H
