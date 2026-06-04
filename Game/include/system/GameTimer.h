#ifndef DEF_GAMETIMER_H
#define DEF_GAMETIMER_H

/**
 * @file GameTimer.h
 * @brief
 */

class GameTimer
{
public:

    void Init();

    void Update();

    float GetDeltaTime() const;

private:

    int m_PreviousTime = 0;

    float m_DeltaTime = 0.016f;

    float m_FPS = 60.0f;
};

#endif // DEF_GAMETIMER_H
