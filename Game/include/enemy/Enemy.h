#pragma once
#include "DxLib.h"

struct Enemy
{
    VECTOR pos;
    float angle;
    int modelHandle;

    void Init(VECTOR startPos);
    void Update();   // ç°ÇÕãÛÇ≈OK
    void Draw();
};