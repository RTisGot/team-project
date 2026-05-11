#include "DxLib.h"

int WINAPI WinMain(
    HINSTANCE,
    HINSTANCE,
    LPSTR,
    int)
{
    ChangeWindowMode(TRUE);

    if (DxLib_Init() == -1)
    {
        return -1;
    }

    while (ProcessMessage() == 0)
    {
        ClearDrawScreen();

        DrawString(100, 100, "Hello DxLib!", GetColor(255, 255, 255));

        ScreenFlip();
    }

    DxLib_End();

    return 0;
}