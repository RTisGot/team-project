#ifndef DEF_AUDIOMANAGER_H
#define DEF_AUDIOMANAGER_H

#include "DxLib.h"

enum class BGMType
{
    Title,
    Game,
    Result
};

enum class SEType
{
    Jump,
    Slide,
    Walk,
    Damage,
    Elevator
};

class AudioManager
{
public:

    AudioManager();
    ~AudioManager();

    bool Init();

    /**
     * @brief BGM再生
     */
    void PlayBGM(BGMType type);

    /**
     * @brief BGM停止
     */
    void StopBGM();

    /**
     * @brief SE再生
     */
    void PlaySE(SEType type);

private:

    // BGM
    int m_TitleBGMHandle;
    int m_GameBGMHandle;
    int m_ResultBGMHandle;

    // SE
    int m_JumpSEHandle;
    int m_SlideSEHandle;
    int m_WalkSEHandle;
    int m_DamageSEHandle;
    int m_ElevatorSEHandle;
};

#endif // DEF_AUDIOMANAGER_H
