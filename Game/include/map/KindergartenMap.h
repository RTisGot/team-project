#pragma once

#include <DxLib.h>

/**
 * @class KindergartenMap
 * @brief 保育園マップクラス
 */
class KindergartenMap
{
public:
    KindergartenMap();
    ~KindergartenMap();

    bool Init();

    void Update();

    void Draw();

    int GetModelHandle() const { return m_modelHandle; }

private:
    int m_modelHandle;    ///< 保育園マップモデル
};
