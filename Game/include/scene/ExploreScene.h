#ifndef DEF_EXPLORESCENE_H
#define DEF_EXPLORESCENE_H

#include "scene/BaseScene.h"
#include "scene/SceneManager.h"
#include "player/Player.h"
#include "enemy/Enemy.h"
#include "map/MapBase.h"
#include "map/KindergartenMap.h"
#include "system/LightManager.h"
#include "Collision/CollisionManager.h"
#include "ui/UIManager.h"
#include "object/PlayObject.h"
#include "item/OrbManager.h"
#include <memory>
#include <DxLib.h>

/**
 * @file ExploreScene.h
 * @brief 探索シーンの定義
 */

class Player;
class CollisionManager;
class LightManager;

/**
 * @class ExploreScene
 * @brief 探索シーン
 * @details プレイヤーがマップを探索するシーン。今後正式に仕様が決まったら追加予定。
 */

class ExploreScene : public BaseScene
{
public:
    ExploreScene(SceneManager* manager);
    ~ExploreScene() override;

    void Init() override;
    void Update(float deltaTime) override;
    void Draw() override;

private:
    SceneManager* m_manager;                                ///< シーンマネージャーへのポインタ
    std::unique_ptr<Player> m_player;                       ///< プレイヤーインスタンス
    std::unique_ptr<CollisionManager> m_collisionManager;   ///< 当たり判定マネージャー
    std::unique_ptr<MapBase> m_CurrentMap;                  ///< 現在のマップインスタンス
    std::unique_ptr<LightManager> m_lightManager;           ///< ライトマネージャー
    std::unique_ptr<Enemy> m_enemy;                         ///< 敵インスタンス
    std::unique_ptr<PlayObject> m_playObject;               ///< 遊具インスタンス
    std::unique_ptr<UIManager> m_UIManager;                 ///< UIマネージャー
    std::unique_ptr<OrbManager> m_OrbManager;               ///< オーブマネージャー
};

#endif // DEF_EXPLORESCENE_H
