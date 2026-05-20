#include "scene/SceneManager.h"

SceneManager::SceneManager()
    : m_currentScene(nullptr)
{
}

void SceneManager::Init(std::shared_ptr<BaseScene> initialScene)
{
    m_currentScene = initialScene;
    if (m_currentScene)
    {
        m_currentScene->Init();
    }
}

void SceneManager::Update()
{
    if (m_currentScene)
    {
        m_currentScene->Update();
    }
}

void SceneManager::Draw()
{
    if (m_currentScene)
    {
        m_currentScene->Draw();
    }
}

void SceneManager::ChangeScene(std::shared_ptr<BaseScene> nextScene)
{
    // スマートポインタを使用しているため代入時に古いシーンは自動的に破棄されます
    m_currentScene = nextScene;
    if (m_currentScene)
    {
        m_currentScene->Init();
    }
}