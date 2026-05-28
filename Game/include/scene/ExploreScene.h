#pragma once
#include "scene/BaseScene.h"
#include "scene/SceneManager.h"
#include <memory>
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
};
