#ifndef DEF_BOXCOLLIDER_H
#define DEF_BOXCOLLIDER_H

#include <DxLib.h>

/**
 * @brief AABBコリジョン
 */
struct BoxCollider
{
    VECTOR m_Min;
    VECTOR m_Max;
};

#endif // DEF_BOXCOLLIDER_H
