#include "../../include/player/player.h"
#include <math.h>

#define MOVE_SPEED 10.0f


// コンストラクタ
// プレイヤーとカメラ情報を初期化
Player::Player()
{
	// プレイヤー座標
	m_Position = VGet(200.0f, 400.0f, 0.0f);

	// プレイヤー向き
	m_PlayerAngle = 0.0f;

	// カメラ横回転
	m_CameraYaw = 0.0f;

	// カメラ縦回転
	m_CameraPitch = 0.3f;

	// カメラ距離
	m_CameraDistance = 50.0f;

	// マウス感度
	m_MouseSensitivity = 0.005f;

	// ジャンプ・物理
	m_VelocityY = 0.0f;		// Y速度
	m_Gravity = -0.2f;		// 重力
	m_JumpPower = 3.0f;		// ジャンプ力
	m_IsGround = true;		// 接地フラグ

	// ダッシュ関連
	m_MoveSpeed = 1.0f;
	m_DashMultiplier = 2.0f;  // 2倍速
	m_IsDashing = false;
}

// 更新処理
// 入力・移動・カメラ更新
void Player::Update(int roofTopModelHandle)
{
	// 画面サイズ
	int screenX;
	int screenY;

	GetDrawScreenSize(&screenX, &screenY);

	// マウス座標取得
	int mouseX;
	int mouseY;

	GetMousePoint(&mouseX, &mouseY);

	// 画面中央
	int centerX = screenX / 2;
	int centerY = screenY / 2;

	// マウス移動量
	int moveX = mouseX - centerX;
	int moveY = mouseY - centerY;

	// マウスを中央へ戻す
	SetMousePoint(centerX, centerY);

	// カメラ回転

	// 横回転
	m_CameraYaw += moveX * m_MouseSensitivity;

	// 縦回転
	m_CameraPitch += moveY * m_MouseSensitivity;

	// カメラ縦回転制限
	// 真上・真下防止
	if (m_CameraPitch > 1.0f)
	{
		m_CameraPitch = 1.0f;
	}

	if (m_CameraPitch < -1.0f)
	{
		m_CameraPitch = -1.0f;
	}

	// カメラ前方向
	VECTOR forward =
	{
		cosf(m_CameraPitch) * sinf(m_CameraYaw),
		-sinf(m_CameraPitch),
		cosf(m_CameraPitch) * cosf(m_CameraYaw)
	};

	// カメラ右方向
	VECTOR right =
	{
		cosf(m_CameraYaw),
		0.0f,
		-sinf(m_CameraYaw)
	};

	// 入力方向
	VECTOR move = VGet(0.0f, 0.0f, 0.0f);

	// 前進
	if (CheckHitKey(KEY_INPUT_W))
	{
		move = VAdd(move, forward);
	}

	// 後退
	if (CheckHitKey(KEY_INPUT_S))
	{
		move = VSub(move, forward);
	}

	// 右移動
	if (CheckHitKey(KEY_INPUT_D))
	{
		move = VAdd(move, right);
	}

	// 左移動
	if (CheckHitKey(KEY_INPUT_A))
	{
		move = VSub(move, right);
	}

	// 上昇
	if (CheckHitKey(KEY_INPUT_E))
	{
		move.y += 1.0f;
	}

	// 下降
	if (CheckHitKey(KEY_INPUT_Q))
	{
		move.y -= 1.0f;
	}

	// プレイヤー移動
	if (VSize(move) > 0.0f)
	{
		// 正規化
		move = VNorm(move);

		// 移動
		float speed = m_MoveSpeed;

		if (m_IsDashing)
		{
			speed *= m_DashMultiplier;
		}

		m_Position = VAdd(m_Position, VScale(move, speed));
	}

	// ジャンプ入力
	if (CheckHitKey(KEY_INPUT_SPACE))
	{
		if (m_IsGround)
		{
			m_VelocityY = m_JumpPower;
			m_IsGround = false;
		}
	}

	//重力
	/*m_VelocityY += m_Gravity;
	m_Position.y += m_VelocityY;*/

	//TODO:屋上との当たり判定の仮実装
	if (roofTopModelHandle != -1)
	{
		float playerHeight = 2.0f;
		float playerRadius = 2.0f;

		VECTOR lineStart = VAdd(m_Position, VGet(0.0f, playerHeight, 0.0f));
		VECTOR lineEnd = VAdd(m_Position, VGet(0.0f, -playerHeight, 0.0f));

		MV1_COLL_RESULT_POLY hitLine = MV1CollCheck_Line(roofTopModelHandle, -1, lineStart, lineEnd);

		if(hitLine.HitFlag==TRUE)
		{
			if (m_VelocityY < 0.0f)
			{
				m_Position.y = hitLine.HitPosition.y;
				m_VelocityY = 0.0f;
				m_IsGround = true;
			}
		}
		else
		{
			m_IsGround = false;
		}

		VECTOR center = VAdd(m_Position, VGet(0.0f, playerHeight, 0.0f));
		MV1_COLL_RESULT_POLY_DIM hitSphere = MV1CollCheck_Sphere(roofTopModelHandle, -1, center, playerRadius);

		for(int i=0;i<hitSphere.HitNum; i++)
		{
			if(hitSphere.Dim[i].Normal.y<0.5f)
			{
				m_Position = VAdd(m_Position, VScale(hitSphere.Dim[i].Normal, 1.0f));
			}
		}
		MV1CollResultPolyDimTerminate(hitSphere);
	}
	if (m_Position.y <= 0.0f)
	{
		m_Position.y = 400.0f;
		m_Position.x = 200.0f;
		m_Position.z = 0.0f;
		m_VelocityY = 0.0f;
	}

	// ダッシュ入力
	if (CheckHitKey(KEY_INPUT_LSHIFT))
	{
		m_IsDashing = true;
	}
	else
	{
		m_IsDashing = false;
	}

	// プレイヤーの向き
	// マウス方向へ向ける
	m_PlayerAngle = m_CameraYaw + DX_PI_F;

	// カメラ位置
	VECTOR cameraPos =
	{
		m_Position.x -
		cosf(m_CameraPitch) *
		sinf(m_CameraYaw) *
		m_CameraDistance,

		m_Position.y +
		sinf(m_CameraPitch) *
		m_CameraDistance,

		m_Position.z -
		cosf(m_CameraPitch) *
		cosf(m_CameraYaw) *
		m_CameraDistance
	};

	// 注視点
	VECTOR targetPos =
	{
		m_Position.x,
		m_Position.y + 5.0f,
		m_Position.z
	};

	// カメラ設定
	SetCameraPositionAndTarget_UpVecY(cameraPos,targetPos);
}

// 描画処理
// キューブ描画
void Player::Draw()
{
	// キューブサイズ
	float size = 2.0f;

	// 頂点
	VECTOR v[8] =
	{
		VGet(-size, -size, -size),	// 左下奥
		VGet(size, -size, -size),	// 右下奥
		VGet(size,  size, -size),	// 右上奥
		VGet(-size,  size, -size),	// 左上奥

		VGet(-size, -size,  size),	// 左下手前
		VGet(size, -size,  size),	// 右下手前
		VGet(size,  size,  size),	// 右上手前
		VGet(-size,  size,  size),	// 左上手前
	};

	// Y軸回転
	float s = sinf(m_PlayerAngle);
	float c = cosf(m_PlayerAngle);

	auto RotateY = [&](VECTOR p)
		{
			VECTOR r;
			r.x = p.x * c + p.z * s;
			r.y = p.y;
			r.z = -p.x * s + p.z * c;
			return r;
		};

	// 回転＋移動
	for (int i = 0; i < 8; i++)
	{
		v[i] = RotateY(v[i]);

		v[i] = VAdd(v[i], m_Position);
	}

	// 色
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
}