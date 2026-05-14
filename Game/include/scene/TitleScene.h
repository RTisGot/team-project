#pragma once
#include "scene/BaseScene.h"
#include "scene/SceneManager.h"

/**
 * @brief タイトルシーン
 */
class TitleScene : public BaseScene
{
public:
    TitleScene(SceneManager* manager);
    ~TitleScene() override = default;

    void Initialize() override;
    void Update() override;
    void Draw() override;

private:
    SceneManager* m_manager; ///< シーンマネージャーへのポインタ
};
