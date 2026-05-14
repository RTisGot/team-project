#include "DxLib.h"
#include "scene/SceneManager.h"
#include "scene/TitleScene.h"
#include <memory>

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    // ウインドウモードで起動
    ChangeWindowMode(true);

    // 文字コードをUTF-8として扱う（ソースコードがUTF-8で保存されている場合の文字化け対策）
    SetUseCharCodeFormat(DX_CHARCODEFORMAT_UTF8);

    // DXライブラリの初期化
    if (DxLib_Init() < 0)
    {
        return -1;
    }

    // 描画先を裏画面にする
    SetDrawScreen(DX_SCREEN_BACK);

    // 画面の背景色を設定
    SetBackgroundColor(0, 0, 0);

    // シーンマネージャーの初期化
    SceneManager sceneManager;
    sceneManager.Initialize(std::make_shared<TitleScene>(&sceneManager));

    // メインループ
    while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
    {
        ClearDrawScreen();

        // 現在のシーンの更新と描画
        sceneManager.Update();
        sceneManager.Draw();

        ScreenFlip();
    }

    // ソフトの終了処理
    DxLib_End();
    return 0;
}