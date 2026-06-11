#ifndef DEF_BOXCOLLIDER_H
#define DEF_BOXCOLLIDER_H

#include <DxLib.h>

/**
 * @file BoxCollider.h
 * @brief AABBコリジョンの定義
 */

/**
 * @brief AABBコリジョンを表す構造体
 */
struct BoxCollider
{
    VECTOR m_Min;
    VECTOR m_Max;
};

#endif // DEF_BOXCOLLIDER_H
