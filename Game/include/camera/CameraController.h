#pragma once

#include <DxLib.h>

/**
 * @class CameraController
 * @brief プレイヤーを追従するカメラの制御クラス
 */
class CameraController
{
public:
    CameraController();

    void Update(
        float deltaTime,
        const VECTOR& playerPos,
        bool isDashing);

    void Apply();

    float GetYaw() const;

private:

    float m_Yaw;
    float m_Pitch;

    float m_Distance;
    float m_TargetDistance;
    float m_BaseDistance;

    float m_CameraHeight;

    VECTOR m_TargetActual;

    float m_MouseSensitivity;
};
