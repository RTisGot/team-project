#include "DxLib.h"
#include "scene/SceneManager.h"
#include "scene/TitleScene.h"
#include <memory>

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    // ウインドウモードで起動
    ChangeWindowMode(true);
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
				return r;
	DxLib_End();

	// ソフトの終了
	return 0;

		// 赤色
		int col = GetColor(255, 0, 0);

		// 前面
		DrawTriangle3D(v[0], v[1], v[2], col, TRUE);
		DrawTriangle3D(v[0], v[2], v[3], col, TRUE);

		// 背面
		DrawTriangle3D(v[5], v[4], v[7], col, TRUE);
		DrawTriangle3D(v[5], v[7], v[6], col, TRUE);

		// 左面
		DrawTriangle3D(v[4], v[0], v[3], col, TRUE);
		DrawTriangle3D(v[4], v[3], v[7], col, TRUE);

		// 右面
		DrawTriangle3D(v[1], v[5], v[6], col, TRUE);
		DrawTriangle3D(v[1], v[6], v[2], col, TRUE);

		// 上面
		DrawTriangle3D(v[3], v[2], v[6], col, TRUE);
		DrawTriangle3D(v[3], v[6], v[7], col, TRUE);

		// 下面
		DrawTriangle3D(v[4], v[5], v[1], col, TRUE);
		DrawTriangle3D(v[4], v[1], v[0], col, TRUE);

		// 新しい向きをセット
		//1SetRotationXYZ(modelHandle, VGet(0.0f, angle, 0.0f));

		// 3Dモデルに新しい座標をセット
		//1SetPosition(modelHandle, position);

		// 3Dモデルの描画
		//1DrawModel(modelHandle);


		ScreenFlip();
	}

	// 3D���f���폜
	//MV1DeleteModel(modelHandle);

	DxLib_End();

	// �\�t�g�̏I��
	return 0;
}