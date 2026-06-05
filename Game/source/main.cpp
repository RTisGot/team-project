#include "scene/SceneManager.h"
#include "scene/TitleScene.h"
#include "system/Define.h"
#include "system/GameTimer.h"
#include "DxLib.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    SetGraphMode(SCREEN_WIDTH, SCREEN_HEIGHT, 32);

    // ウインドウモードで起動
    ChangeWindowMode(true);

    // 文字コードをUTF-8として扱う
    SetUseCharCodeFormat(DX_CHARCODEFORMAT_UTF8);

    //ver.DirectX11
    DxLib::SetUseDirect3DVersion(DX_DIRECT3D_11);

    // DXライブラリの初期化
    if (DxLib_Init() < 0)
    {
        return -1;
    }

    GetUseDirect3DVersion();
    // 描画先を裏画面にする
    SetDrawScreen(DX_SCREEN_BACK);

    // 画面の背景色を設定
    SetBackgroundColor(0, 0, 0);

    // シーンマネージャーの初期化
    SceneManager sceneManager;
    sceneManager.Init(std::make_shared<TitleScene>(&sceneManager));

    // ゲームタイマーの初期化
    GameTimer gameTimer;
    gameTimer.Init();

    // メインループ
    while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
    {
        ClearDrawScreen();

        // ゲームタイマーの更新
        gameTimer.Update();

        const float deltaTime = gameTimer.GetDeltaTime();

        // 現在のシーンの更新と描画
        sceneManager.Update(deltaTime);
        sceneManager.Draw();

        ScreenFlip();
    }

    // ソフトの終了処理
    DxLib_End();
    return 0;
}
