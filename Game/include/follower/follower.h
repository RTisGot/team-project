#pragma once

#include <DxLib.h>

class Follower
{
public:
    Follower();
    ~Follower();

    void LoadModel();

    void Update();
    void Draw();

    void SetTargetPosition(const VECTOR& pos);
    void SetTargetAngle(float angle);

private:
    int m_ModelHandle;

    VECTOR m_Position;
    VECTOR m_TargetPosition;

    float m_Angle;
    float m_TargetAngle;
};
