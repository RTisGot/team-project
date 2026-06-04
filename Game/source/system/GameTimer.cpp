#include "System/GameTimer.h"
#include <DxLib.h>

void GameTimer::Init()
{
    m_PreviousTime = GetNowCount();
}

void GameTimer::Update()
{
    // 現在の時間をミリ秒単位で取得
    const int currentTime = GetNowCount();

    // 前回からの経過時間を秒単位で計算
    m_DeltaTime =static_cast<float>(currentTime - m_PreviousTime)/ 1000.0f;

    // フレームレートを安定させるために、デルタタイムが極端に大きくならないように制限
    m_PreviousTime = currentTime;
}

float GameTimer::GetDeltaTime() const
{
    return m_DeltaTime;
}
