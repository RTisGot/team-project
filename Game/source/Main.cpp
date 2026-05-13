#include "DxLib.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
    LPSTR lpCmdLine, int nCmdShow)
{
    ChangeWindowMode(TRUE);

    if (DxLib_Init() == -1)
    {
        return -1;
    }

    while (ProcessMessage() == 0)
    {
        ClearDrawScreen();



        ScreenFlip();
    }

    DxLib_End();

    return 0;
}