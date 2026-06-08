#include "audio/AudioManager.h"

AudioManager::AudioManager()
    : m_TitleBGMHandle(-1),
      m_GameBGMHandle(-1),
      m_ResultBGMHandle(-1),
      m_JumpSEHandle(-1),
      m_SlideSEHandle(-1),
      m_CollisionSEHandle(-1)
{
}

AudioManager::~AudioManager()
{
    // BGMのハンドルが有効な場合は音声を解放
    if (m_TitleBGMHandle != -1)
    {
        StopBGM();
        m_TitleBGMHandle = -1;
    }
    if (m_GameBGMHandle != -1)
    {
        StopBGM();
        m_GameBGMHandle = -1;
    }
    if (m_ResultBGMHandle != -1)
    {
        StopBGM();
        m_ResultBGMHandle = -1;
    }
    // SEのハンドルが有効な場合は音声を解放
    if (m_JumpSEHandle != -1)
    {
        m_JumpSEHandle = -1;
    }
    if (m_SlideSEHandle != -1)
    {
        m_SlideSEHandle = -1;
    }
    if (m_CollisionSEHandle != -1)
    {
        m_CollisionSEHandle = -1;
    }
}

bool AudioManager::Init()
{
    // BGMの読み込み
    m_TitleBGMHandle = LoadSoundMem("Game/assets/audio/bgm/title_bgm.wav");
    m_GameBGMHandle = LoadSoundMem("Game/assets/audio/bgm/game_bgm.wav");
    m_ResultBGMHandle = LoadSoundMem("Game/assets/audio/bgm/result_bgm.wav");

    // SEの読み込み
    m_JumpSEHandle = LoadSoundMem("Game/assets/audio/se/jump_se.wav");
    m_SlideSEHandle = LoadSoundMem("Game/assets/audio/se/slide_se.wav");
    m_CollisionSEHandle = LoadSoundMem("Game/assets/audio/se/collision_se.wav");
    
    // 音声の読み込みに失敗した場合はエラーメッセージを出力
    if (m_TitleBGMHandle == -1 ||
        m_GameBGMHandle == -1 ||
        m_ResultBGMHandle == -1 ||
        m_JumpSEHandle == -1 ||
        m_SlideSEHandle == -1 ||
        m_CollisionSEHandle == -1)
    {
        return false;
    }
    return true;
}

void AudioManager::PlayBGM(BGMType type)
{
    int handle = -1;
    switch (type)
    {
    case BGMType::Title:
        handle = m_TitleBGMHandle;
        break;
    case BGMType::Game:
        handle = m_GameBGMHandle;
        break;
    case BGMType::Result:
        handle = m_ResultBGMHandle;
        break;
    }
    if (handle != -1)
    {
        PlaySoundMem(handle, DX_PLAYTYPE_LOOP);
    }
}

void AudioManager::StopBGM()
{
    if (m_TitleBGMHandle != -1)
    {
        StopSoundMem(m_TitleBGMHandle);
    }
    if (m_GameBGMHandle != -1)
    {
        StopSoundMem(m_GameBGMHandle);
    }
    if (m_ResultBGMHandle != -1)
    {
        StopSoundMem(m_ResultBGMHandle);
    }
}
