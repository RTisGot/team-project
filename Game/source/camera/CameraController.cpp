#include "camera/CameraController.h"
#include <cmath>
#include <DxLib.h>

CameraController::CameraController()
{
    m_Yaw = 0.0f;
    m_Pitch = 0.3f;

    m_Distance = 30.0f;
    m_TargetDistance = 30.0f;
    m_BaseDistance = 30.0f;

    m_CameraHeight = 0.0f;

    m_TargetActual = VGet(0.0f, 10.0f, 0.0f);

    m_CameraPosition = VGet(0.0f, 0.0f, 0.0f);

    m_MouseSensitivity = 0.005f;

    m_LastWheelRot = 0;

    m_IsFirstUpdate = true;
}

void CameraController::InitMouse()
{
    int screenX, screenY;

    GetDrawScreenSize(&screenX, &screenY);

    SetMousePoint(screenX / 2, screenY / 2);

    m_IsFirstUpdate = false;
}

void CameraController::Update(float deltaTime, const VECTOR& playerPos, bool isDashing, bool isMoving)
{
    int screenX;
    int screenY;

    // 画面サイズ取得
    GetDrawScreenSize(&screenX, &screenY);

    int mouseX;
    int mouseY;

    // マウス座標取得
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
    m_Yaw += moveX * m_MouseSensitivity;
    m_Pitch += moveY * m_MouseSensitivity;

    // カメラ縦回転制限
    if (m_Pitch > 1.0f) m_Pitch = 1.0f;
    if (m_Pitch < -1.0f) m_Pitch = -1.0f;

    // マウスホイールの回転量を取得
    m_LastWheelRot = GetMouseWheelRotVol();

    // ホイール回転でカメラ距離を変更
    if (m_LastWheelRot != 0)
    {
        m_BaseDistance -= m_LastWheelRot * 1.5f;    // ホイールの回転量に応じて基準距離を変更
    }

    // ダッシュ中はカメラ距離を少し遠くする
    if (m_BaseDistance < 15.0f) m_BaseDistance = 15.0f;

    // カメラ距離の制限
    if (m_BaseDistance > 60.0f) m_BaseDistance = 60.0f;

    // ダッシュ中の速度管理
    if (isDashing && isMoving)
    {
        m_TargetDistance = m_BaseDistance + 10.0f;  // ダッシュ中は基準距離より少し遠くする
    }
    else
    {
        m_TargetDistance = m_BaseDistance;  // 通常時の距離
    }

    constexpr float cameraFollowSpeed = 6.0f;

    // カメラ距離を目標距離に近づける
    m_Distance += (m_TargetDistance - m_Distance) * cameraFollowSpeed * deltaTime;

    float targetY = playerPos.y;

    // 着地硬直中はカメラの高さを少し上げる
    m_CameraHeight += (targetY - m_CameraHeight) * 8.0f * deltaTime;

    // キャラの頭上に注視点を設定
    VECTOR idealTargetPos =
    {
        playerPos.x,
        playerPos.y + 10.0f,
        playerPos.z
    };

    // キャラに少し遅れてついてくる
    constexpr float targetFollowSpeed = 15.0f;

    // カメラの注視点をキャラの頭上に近づける
    m_TargetActual = VAdd(m_TargetActual, VScale(VSub(idealTargetPos, m_TargetActual), targetFollowSpeed * deltaTime));

    m_CameraPosition =
    {
        playerPos.x -
        cosf(m_Pitch) *
        sinf(m_Yaw) *
        m_Distance,

        m_CameraHeight +
        sinf(m_Pitch) *
        m_Distance,

        playerPos.z -
        cosf(m_Pitch) *
        cosf(m_Yaw) *
        m_Distance
    };
}

void CameraController::Apply()
{
    SetCameraPositionAndTarget_UpVecY(m_CameraPosition, m_TargetActual);
}

float CameraController::GetYaw() const
{
    return m_Yaw;
}

void CameraController::SetCameraParameter(float yaw, float pitch, float distance)
{
    m_Yaw = yaw;
    m_Pitch = pitch;

    m_Distance = distance;
    m_BaseDistance = distance;
    m_TargetDistance = distance;
}

void CameraController::Warp(const VECTOR& playerPos)
{
    m_CameraHeight = playerPos.y;

    m_TargetActual =
    {
        playerPos.x,
        playerPos.y + 10.0f,
        playerPos.z
    };

    m_CameraPosition =
    {
        playerPos.x -
        cosf(m_Pitch) *
        sinf(m_Yaw) *
        m_Distance,

        m_CameraHeight +
        sinf(m_Pitch) *
        m_Distance,

        playerPos.z -
        cosf(m_Pitch) *
        cosf(m_Yaw) *
        m_Distance
    };
}
