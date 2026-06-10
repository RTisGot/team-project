#pragma once

#include "scene/BaseScene.h"

class SceneManager;

class GameOverScene : public BaseScene
{
public:
    GameOverScene(SceneManager* manager);

    void Init() override;
    void Update(float deltaTime) override;
    void Draw() override;

private:
    SceneManager* m_manager;
};
