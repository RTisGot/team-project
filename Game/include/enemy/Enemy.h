#pragma once
#include <DxLib.h>

struct Enemy
{
    VECTOR pos;
    float angle;
    int modelHandle;

    void Init(VECTOR startPos);
    void Update();   // 今は空でOK
    void Draw();
};