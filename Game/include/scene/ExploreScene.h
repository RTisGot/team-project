#pragma once
#include "scene/BaseScene.h"
#include "scene/SceneManager.h"
#include <memory>
#include <map/KindergartenMap.h>
#include "player/Player.h"
#include "enemy/Enemy.h"
#include "map/RoofTop.h"
#include "system/LightManager.h"
#include "Collision/CollisionManager.h"
#include <DxLib.h>
/**
 * @file ExploreScene.h
 * @brief 
 */

class Player;

/**
 * @class ExploreScene
 * @brief 探索シーン
 * @details プレイヤーがマップを探索するシーン。今後正式に仕様が決まったら追加予定。
 */

class ExploreScene : public BaseScene
{
public:
    ExploreScene(SceneManager* manager);
    ~ExploreScene() override; // 定義を cpp に移動して Player を不完全型として保持できるようにする

    void Init() override;
    void Update() override;
    void Draw() override;

private:
    SceneManager* m_manager; ///< シーンマネージャーへのポインタ
    //std::unique_ptr<Player> m_player; ///< プレイヤーインスタンス
    std::unique_ptr<KindergartenMap> m_kindergartenMap;
    std::unique_ptr<Player> m_player; ///< プレイヤーインスタンス
    std::unique_ptr<CollisionManager> m_collisionManager; ///< 当たり判定マネージャー
    std::unique_ptr<LightManager> m_lightManager; ///< ライトマネージャー
    std::unique_ptr<Enemy> m_enemy; ///< 敵インスタンス
};
