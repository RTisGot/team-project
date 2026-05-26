#include "object/Door.h"
#include <DxLib.h>

Door::Door()
    :m_modelHandle(-1)
    , m_animIndex(-1)
    , m_animTime(0.0f)
    , m_animTotalTime(0.0f)
    , m_state(DoorState::Closed)
    , m_interactionPos(VGet(0.0f, 0.0f, 0.0f))
    , m_interactionRadius(50.0f)
{
}

Door::~Door()
{
    // モデルの解放
    if (m_modelHandle != -1)
    {
        MV1DeleteModel(m_modelHandle);
        m_modelHandle = -1;
    }
}

bool Door::Init()
{
    // モデルの読み込み
    m_modelHandle = MV1LoadModel("Game/assets/models/map/door.mv1");
    if (m_modelHandle == -1)
    {
        return false; // モデルの読み込みに失敗
    }

    m_animIndex = MV1AttachAnim(m_modelHandle, 0, -1, FALSE);
    if(m_animIndex!=-1)
    {
        m_animTotalTime = MV1GetAnimTotalTime(m_modelHandle, m_animIndex);
        MV1SetAttachAnimTime(m_modelHandle, m_animIndex, 0.0f);
    }
    return true;
}

void Door::Update(const VECTOR& playerPos)
{
    if(m_state == DoorState::Closed)
    {
        // 距離の2乗を直接計算する
        VECTOR diff = VSub(playerPos, m_interactionPos);
        float distSq = VSquareSize(diff);
        float radiusSq = m_interactionRadius * m_interactionRadius;

        if (distSq <= radiusSq && CheckHitKey(KEY_INPUT_F) == 1)
        {
            m_state = DoorState::Opening;
        }
    }
    else if(m_state == DoorState::Opening)
    {
        m_animTime += 1.0f; // アニメーションの再生速度

        if(m_animTime >= m_animTotalTime)
        {
            m_animTime = m_animTotalTime;
            m_state = DoorState::Opened;
        }
        MV1SetAttachAnimTime(m_modelHandle, m_animIndex, m_animTime);
    }
}

void Door::Draw()
{
    if (m_modelHandle != -1)
    {
        MV1DrawModel(m_modelHandle);
    }
    else
    {
        DrawString(100, 100, "ドアモデルが読み込まれていません", GetColor(255, 0, 0));
    }
}