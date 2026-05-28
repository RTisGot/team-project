#include "Collision/CollisionManager.h"

CollisionManager::CollisionManager()
    : m_StageModelHandle(-1)
{
}

void CollisionManager::SetStageModel(int modelHandle)
{
    m_StageModelHandle = modelHandle;
}


// ステージとの当たり判定処理
void CollisionManager::ResolveStageCollision(VECTOR& position, float& velocityY, bool& isGround, float height, float radius)
{
    if (m_StageModelHandle == -1)
    {
        return;
    }

    // 足元の判定
    VECTOR lineStart = VAdd(position, VGet(0.0f, height * 0.5f, 0.0f));
    VECTOR lineEnd = VAdd(position, VGet(0.0f, -2.0, 0.0f));
    MV1_COLL_RESULT_POLY hitLineFloor = MV1CollCheck_Line(m_StageModelHandle, -1, lineStart, lineEnd);

    if (hitLineFloor.HitFlag == TRUE)
    {
        if (velocityY <= 0.0f)
        {
            // キャラのY座標を当たった位置に修正
            position.y = hitLineFloor.HitPosition.y;
            velocityY = 0.0f;
            isGround = true;
        }
    }
    else
    {
        isGround = false;
    }

    // 足元球（段差や低いオブジェクト用）
    VECTOR centerBottom = VAdd(position, VGet(0.0f, radius, 0.0f));
    MV1_COLL_RESULT_POLY_DIM hitBottom = MV1CollCheck_Sphere(m_StageModelHandle, -1, centerBottom, radius);

    for (int i = 0; i < hitBottom.HitNum; i++)
    {
        // Y成分が一定以下なら壁とみなす（床からの押し出しを防ぐ）
        if (hitBottom.Dim[i].Normal.y < 0.5f && hitBottom.Dim[i].Normal.y > -0.5f)
        {
            position = VAdd(position, VScale(hitBottom.Dim[i].Normal, 1.0f));
        }
    }
    MV1CollResultPolyDimTerminate(hitBottom);

    // 腰～頭球（高めの壁や柱用）
    VECTOR centerTop = VAdd(position, VGet(0.0f, height - radius, 0.0f));
    MV1_COLL_RESULT_POLY_DIM hitTop = MV1CollCheck_Sphere(m_StageModelHandle, -1, centerTop, radius);

    for (int i = 0; i < hitTop.HitNum; i++)
    {
        if (hitTop.Dim[i].Normal.y < 0.5f && hitTop.Dim[i].Normal.y > -0.5f)
        {
            position = VAdd(position, VScale(hitTop.Dim[i].Normal, 1.0f));
        }
    }
    MV1CollResultPolyDimTerminate(hitTop);
}
