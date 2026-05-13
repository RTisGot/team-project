#include "DxLib.h"
#include <math.h>

// 移動速度
#define MOVESPEED 10.0f

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{

	// モデルハンドル
	// int modelHandle;
	// 座標
	VECTOR position;
	// 向き
	float angle;

	// ウインドウモードで起動
	ChangeWindowMode(true);

	// DXライブラリの初期化
	if (DxLib_Init() < 0)
	{
		return -1;
	}

	// 向きを初期化
	angle = 0.0f;

	// モデルの座標を初期化
	position = VGet(0.0f, 0.0f, 0.0f);

	// 3Dモデルの読み込み
	//modelHandle = MV1LoadModel("");


	// 描画先を裏画面にする
	SetDrawScreen(DX_SCREEN_BACK);

	// 	カメラの視点、注視点を設定(y軸)
	SetCameraPositionAndTarget_UpVecY(
		VGet(0.0f, 300.0f, -500.0f),
		VGet(0.0f, 100.0f, 0.0f)
	);

	// カメラの 手前クリップ距離と 奥クリップ距離を設定
	SetCameraNearFar(16.0f, 5000.0f);

	// 画面の背景色を設定
	SetBackgroundColor(0, 0, 0);

	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
	{
		// 画面のクリア
		ClearDrawScreen();

		// 入力
		VECTOR direction = VGet(0.0f, 0.0f, 0.0f);

		// 移動操作
		if (CheckHitKey(KEY_INPUT_A)) direction.x = -1.0f;
		if (CheckHitKey(KEY_INPUT_D)) direction.x = 1.0f;
		if (CheckHitKey(KEY_INPUT_W)) direction.z = 1.0f;
		if (CheckHitKey(KEY_INPUT_S)) direction.z = -1.0f;

		// 移動と向き
		if (direction.x != 0.0f || direction.z != 0.0f)
		{
			angle = atan2f(-direction.x, -direction.z);

			position.x -= sinf(angle) * MOVESPEED;
			position.z -= cosf(angle) * MOVESPEED;
		}

		// 大きさ
		float size = 30.0f;

		VECTOR v[8] =
		{
			VGet(-size, -size, -size),	// 左下奥
			VGet(size, -size, -size),   // 右下奥
			VGet(size,  size, -size),	// 右上奥
			VGet(-size,  size, -size),	// 左上奥
			VGet(-size, -size,  size),	// 左下手前
			VGet(size, -size,  size),	// 右下手前
			VGet(size,  size,  size),	// 右上手前
			VGet(-size,  size,  size),	// 左上手前
		};

		// 回転（Y軸）
		float s = sinf(angle);
		float c = cosf(angle);

		auto RotateY = [&](VECTOR p)
			{
				VECTOR r;
				r.x = p.x * c - p.z * s;
				r.y = p.y;
				r.z = p.x * s + p.z * c;
				return r;
			};

		for (int i = 0; i < 8; i++)
		{
			v[i] = RotateY(v[i]);
			v[i] = VAdd(v[i], position);
		}

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

	// 3Dモデル削除
	//MV1DeleteModel(modelHandle);

	DxLib_End();

	// ソフトの終了
	return 0;
}