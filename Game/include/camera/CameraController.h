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

    // マウスを画面の中央に固定する
    void InitMouse();

    /**
     * @brief カメラの更新
     * @param deltaTime 経過時間
     * @param playerPos プレイヤーの位置
     * @param isDashing ダッシュ中かどうか
     * @param isMoving 移動中かどうか
     */
    void Update(float deltaTime, const VECTOR& playerPos, bool isDashing, bool isMoving);

    // カメラの適用
    void Apply();

    // カメラのパラメータの取得
    float GetYaw() const;

    /**
     * @brief カメラのパラメータを設定する
     * @param yaw 横回転角度
     * @param pitch 縦回転角度
     * @param distance カメラ距離
     */
    void SetCameraParameter(float yaw, float pitch, float distance);

    // ステージモデルのハンドルを設定する
    void SetStageModelHandle(int modelHandle);

    // プレイヤーの位置に瞬間移動する
    void Warp(const VECTOR& playerPos);

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

    bool m_IsFirstUpdate;   // 最初のUpdateかどうか

    int m_StageModelHandle; // ステージモデルのハンドル
};
