#pragma once

#include "scene/BaseScene.h"
#include <memory>

class SceneManager;

class LoadingScene : public BaseScene
{
public:
    LoadingScene(
        SceneManager* manager,
        std::shared_ptr<BaseScene> nextScene);

    void Init() override;
    void Update() override;
    void Draw() override;

private:
    SceneManager* m_Manager;

    std::shared_ptr<BaseScene> m_NextScene;

    int m_LogoHandle;
    int m_Frame;
};
