#include "DxLib.h"
<<<<<<< HEAD
#include "scene/SceneManager.h"
#include "scene/TitleScene.h"
#include <memory>

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    // 繧ｦ繧､繝ｳ繝峨え繝｢繝ｼ繝峨〒襍ｷ蜍・
    ChangeWindowMode(true);

    // 譁・ｭ励さ繝ｼ繝峨ｒUTF-8縺ｨ縺励※謇ｱ縺・ｼ医た繝ｼ繧ｹ繧ｳ繝ｼ繝峨′UTF-8縺ｧ菫晏ｭ倥＆繧後※縺・ｋ蝣ｴ蜷医・譁・ｭ怜喧縺大ｯｾ遲厄ｼ・
    SetUseCharCodeFormat(DX_CHARCODEFORMAT_UTF8);

    // DX繝ｩ繧､繝悶Λ繝ｪ縺ｮ蛻晄悄蛹・
    if (DxLib_Init() < 0)
    {
        return -1;
    }
=======
#include "../../include/player/player.h"
#include <math.h>

// 移動速度
#define MOVESPEED 10.0f

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	// ウインドウモードで起動
	ChangeWindowMode(true);
>>>>>>> 2bb8d47587798f262701e18bc5ea4ecd799a7e7a

    // 謠冗判蜈医ｒ陬冗判髱｢縺ｫ縺吶ｋ
    SetDrawScreen(DX_SCREEN_BACK);

<<<<<<< HEAD
    // 逕ｻ髱｢縺ｮ閭梧勹濶ｲ繧定ｨｭ螳・
    SetBackgroundColor(0, 0, 0);

    // 繧ｷ繝ｼ繝ｳ繝槭ロ繝ｼ繧ｸ繝｣繝ｼ縺ｮ蛻晄悄蛹・
    SceneManager sceneManager;
    sceneManager.Initialize(std::make_shared<TitleScene>(&sceneManager));

    // 繝｡繧､繝ｳ繝ｫ繝ｼ繝・
    while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
    {
        ClearDrawScreen();

        // 迴ｾ蝨ｨ縺ｮ繧ｷ繝ｼ繝ｳ縺ｮ譖ｴ譁ｰ縺ｨ謠冗判
        sceneManager.Update();
        sceneManager.Draw();

        ScreenFlip();
    }

    // 繧ｽ繝輔ヨ縺ｮ邨ゆｺ・・逅・
    DxLib_End();
    return 0;
=======
	// 描画先を裏画面にする
	SetDrawScreen(DX_SCREEN_BACK);

	// マウスカーソルを非表示
	// TPS/FPS視点用
	SetMouseDispFlag(FALSE);

	// カメラの描画範囲設定
	SetCameraNearFar(16.0f, 5000.0f);

	// 画面の背景色を設定
	SetBackgroundColor(0, 0, 0);

	// プレイヤー設定
	Player player;

	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
	{
		// 画面のクリア
		ClearDrawScreen();

		// プレイヤー更新
		// 入力・移動・カメラ更新
		player.Update();

		// プレイヤー描画
		player.Draw();

		// =========================================
		// 3Dデバッグ用床グリッド
		// =========================================

		// グリッドサイズ
		const int gridSize = 1000;

		// グリッド間隔
		const int gridStep = 100;

		// グリッド色
		int gridColor = GetColor(80, 80, 80);

		// X方向
		for (int z = -gridSize; z <= gridSize; z += gridStep)
		{
			DrawLine3D(
				VGet((float)-gridSize, 0.0f, (float)z),
				VGet((float)gridSize, 0.0f, (float)z),
				gridColor
			);
		}

		// Z方向
		for (int x = -gridSize; x <= gridSize; x += gridStep)
		{
			DrawLine3D(
				VGet((float)x, 0.0f, (float)-gridSize),
				VGet((float)x, 0.0f, (float)gridSize),
				gridColor
			);
		}

		// 画面更新
		ScreenFlip();
	}

	DxLib_End();

	// ソフトの終了
	return 0;
>>>>>>> 2bb8d47587798f262701e18bc5ea4ecd799a7e7a
}