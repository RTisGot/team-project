#include "DxLib.h"
#include "../../include/player/player.h"
#include <math.h>

// 移動速度
#define MOVESPEED 10.0f

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	// ウインドウモードで起動
	ChangeWindowMode(true);

	// DXライブラリの初期化
	if (DxLib_Init() < 0)
	{
		return -1;
	}

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
}