#include "collision/CollisionMap.h"
#include <DxLib.h>

void CollisionMap::AddBox(const VECTOR& min, const VECTOR& max)
{
    BoxCollider box;

    box.m_Min = min;
    box.m_Max = max;

    m_Boxes.push_back(box);
}

bool CollisionMap::CheckSphere(const VECTOR& center, float radius, VECTOR& pushOut) const
{
    for (const auto& box : m_Boxes)
    {
        float closestX = max(box.m_Min.x, min(center.x, box.m_Max.x));
        float closestY = max(box.m_Min.y, min(center.y, box.m_Max.y));
        float closestZ = max(box.m_Min.z, min(center.z, box.m_Max.z));

        VECTOR closest =
        {
            closestX,
            closestY,
            closestZ
        };

        VECTOR diff = VSub(center, closest);

        float distance = VSize(diff);

        if (distance < radius)
        {
            if (distance < 0.001f)
            {
                diff = VGet(1, 0, 0);
                distance = 1.0f;
            }

            float penetration = radius - distance;

            pushOut = VScale(VNorm(diff), penetration);

            return true;
        }
    }
    return false;
}

#ifdef _DEBUG

void CollisionMap::DrawDebug() const
{
    for (const auto& box : m_Boxes)
    {
        VECTOR p[8];

        p[0] = VGet(box.m_Min.x, box.m_Min.y, box.m_Min.z);
        p[1] = VGet(box.m_Max.x, box.m_Min.y, box.m_Min.z);
        p[2] = VGet(box.m_Max.x, box.m_Max.y, box.m_Min.z);
        p[3] = VGet(box.m_Min.x, box.m_Max.y, box.m_Min.z);

        p[4] = VGet(box.m_Min.x, box.m_Min.y, box.m_Max.z);
        p[5] = VGet(box.m_Max.x, box.m_Min.y, box.m_Max.z);
        p[6] = VGet(box.m_Max.x, box.m_Max.y, box.m_Max.z);
        p[7] = VGet(box.m_Min.x, box.m_Max.y, box.m_Max.z);

        unsigned int color = GetColor(255, 0, 0);

        DrawLine3D(p[0], p[1], color);
        DrawLine3D(p[1], p[2], color);
        DrawLine3D(p[2], p[3], color);
        DrawLine3D(p[3], p[0], color);

        DrawLine3D(p[4], p[5], color);
        DrawLine3D(p[5], p[6], color);
        DrawLine3D(p[6], p[7], color);
        DrawLine3D(p[7], p[4], color);

        DrawLine3D(p[0], p[4], color);
        DrawLine3D(p[1], p[5], color);
        DrawLine3D(p[2], p[6], color);
        DrawLine3D(p[3], p[7], color);
    }
}

#endif
