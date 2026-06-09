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

    void Update(float deltaTime, const VECTOR& playerPos, bool isDashing, bool isMoving);

    void Apply();

    float GetYaw() const;

private:
    float m_Yaw;		// カメラ横回転
    float m_Pitch;	    // カメラ縦回転

    float m_Distance;   	// カメラ距離
    float m_TargetDistance; // カメラ距離
    float m_BaseDistance;   // ホイールで変更する基準のカメラ距離

    float m_CameraHeight; // 実際のカメラの高さ

    VECTOR m_TargetActual; // 実際のカメラ注視点

    float m_MouseSensitivity;	// マウス感度

    VECTOR m_CameraPosition;    // カメラの位置

    int m_LastWheelRot; // マウスホイールの回転量
};
