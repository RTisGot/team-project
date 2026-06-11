#ifndef DEF_GAMETIMER_H
#define DEF_GAMETIMER_H

/**
 * @file GameTimer.h
 * @brief ゲームのフレームタイムを管理するクラスの定義
 */

/**
 * @brief ゲームタイマークラス
 * @brief ゲームのフレームタイムを管理するクラス
 */
class GameTimer
{
public:

    void Init();

    void Update();

    float GetDeltaTime() const;

private:

    int m_PreviousTime = 0;      ///< 前フレームの時間（ミリ秒）

    float m_DeltaTime = 0.016f;  ///< 前フレームからの経過時間（秒）

    float m_FPS = 60.0f;         ///< 現在のフレームレート
};

#endif // DEF_GAMETIMER_H
