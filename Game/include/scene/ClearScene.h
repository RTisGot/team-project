#ifndef DEF_CLEARSCENE_H
#define DEF_CLEARSCENE_H

#include "scene/BaseScene.h"

class SceneManager;

class ClearScene : public BaseScene
{
public:
    ClearScene(SceneManager* manager);

    void Init() override;
    void Update(float deltaTime) override;
    void Draw() override;

private:
    SceneManager* m_Manager;
};

#endif // DEF_CLEARSCENE_H
