#include "../../include/object/Skybox.h"

Skybox::Skybox() : m_ModelHandle(-1) {}

Skybox::~Skybox() {
    if (m_ModelHandle != -1) MV1DeleteModel(m_ModelHandle);
}

bool Skybox::Load(const char* filepath) {
    m_ModelHandle = MV1LoadModel(filepath);
    return (m_ModelHandle != -1);
}

void Skybox::Draw(const VECTOR& centerPos) {
    if (m_ModelHandle == -1) return;

    // 描画設定の保存と変更
    // 1. プレイヤー(カメラ)の位置に合わせる
    MV1SetPosition(m_ModelHandle, centerPos);

    // 2. 背景として描画するための設定
    SetUseLighting(FALSE);           // 光源の影響を受けない
    SetUseBackCulling(FALSE);        // 内側から見るため裏面も描画
    SetWriteZBufferFlag(FALSE);   // Zバッファに書き込まない（常に奥に描画）

    MV1DrawModel(m_ModelHandle);

    // 設定を戻す
    SetWriteZBufferFlag(TRUE);
    SetUseBackCulling(TRUE);
    SetUseLighting(TRUE);
}
