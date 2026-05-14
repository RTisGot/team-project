#include "scene/GameScene.h"
#include <math.h>

// 仮の移動速度
constexpr float MOVE_SPEED = 10.0f;

GameScene::GameScene(SceneManager* manager)
    : m_manager(manager)
    , m_position(VGet(0.0f, 0.0f, 0.0f))
    , m_angle(0.0f)
{
}

void GameScene::Initialize()
{
    // 今後ここで3Dモデルなどの初期化
    m_position = VGet(0.0f, 0.0f, 0.0f);
    m_angle = 0.0f;

    // カメラの視点、注視点を設定(y軸)
    SetCameraPositionAndTarget_UpVecY(
        VGet(0.0f, 300.0f, -500.0f),
        VGet(0.0f, 100.0f, 0.0f)
    );

    // カメラのクリップ距離を設定
    SetCameraNearFar(16.0f, 5000.0f);
}

void GameScene::Update()
{
    VECTOR direction = VGet(0.0f, 0.0f, 0.0f);

    if (CheckHitKey(KEY_INPUT_A)) direction.x = -1.0f;
    if (CheckHitKey(KEY_INPUT_D)) direction.x = 1.0f;
    if (CheckHitKey(KEY_INPUT_W)) direction.z = 1.0f;
    if (CheckHitKey(KEY_INPUT_S)) direction.z = -1.0f;

    if (direction.x != 0.0f || direction.z != 0.0f)
    {
        m_angle = atan2f(-direction.x, -direction.z);
        m_position.x -= sinf(m_angle) * MOVE_SPEED;
        m_position.z -= cosf(m_angle) * MOVE_SPEED;
    }
}

void GameScene::Draw()
{
    // 3D空間の仮プレイヤー描画
    DrawPlayerBox();

    // デバッグ用UI描画
    DrawString(10, 10, "Game Scene - WASDで移動 ", GetColor(255, 255, 255));
}

void GameScene::DrawPlayerBox()
{
    constexpr float size = 30.0f;
    VECTOR v[8] = {
        VGet(-size, -size, -size), VGet(size, -size, -size),
        VGet(size,  size, -size), VGet(-size,  size, -size),
        VGet(-size, -size,  size), VGet(size, -size,  size),
        VGet(size,  size,  size), VGet(-size,  size,  size),
    };

    float s = sinf(m_angle), c = cosf(m_angle);
    for (int i = 0; i < 8; i++)
    {
        VECTOR p = v[i];
        v[i].x = p.x * c - p.z * s;
        v[i].y = p.y;
        v[i].z = p.x * s + p.z * c;
        v[i] = VAdd(v[i], m_position);
    }

    int col = GetColor(255, 0, 0);
    DrawTriangle3D(v[0], v[1], v[2], col, TRUE); DrawTriangle3D(v[0], v[2], v[3], col, TRUE); // 前
    DrawTriangle3D(v[5], v[4], v[7], col, TRUE); DrawTriangle3D(v[5], v[7], v[6], col, TRUE); // 背
    DrawTriangle3D(v[4], v[0], v[3], col, TRUE); DrawTriangle3D(v[4], v[3], v[7], col, TRUE); // 左
    DrawTriangle3D(v[1], v[5], v[6], col, TRUE); DrawTriangle3D(v[1], v[6], v[2], col, TRUE); // 右
    DrawTriangle3D(v[3], v[2], v[6], col, TRUE); DrawTriangle3D(v[3], v[6], v[7], col, TRUE); // 上
    DrawTriangle3D(v[4], v[5], v[1], col, TRUE); DrawTriangle3D(v[4], v[1], v[0], col, TRUE); // 下
}