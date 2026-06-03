#include "System/GameTimer.h"
#include <DxLib.h>

void GameTimer::Init()
{
    m_PreviousTime = GetNowCount();
}

void GameTimer::Update()
{
    const int currentTime = GetNowCount();

    m_DeltaTime =static_cast<float>(currentTime - m_PreviousTime)/ 1000.0f;

    m_PreviousTime = currentTime;
}

float GameTimer::GetDeltaTime() const
{
    return m_DeltaTime;
}
