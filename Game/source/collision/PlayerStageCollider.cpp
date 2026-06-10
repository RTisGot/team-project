#include "collision/PlayerStageCollider.h"
#include "collision/CollisionMap.h"
#include <cmath>

namespace
{
    // 法線のY成分が一定範囲内なら壁
    bool IsWall(const VECTOR& normal)
    {
        return fabsf(normal.y) < 0.5f;
    }

    // 法線のY成分が一定以上なら床
    bool IsFloor(const VECTOR& normal)
    {
        return normal.y > 0.7f;
    }

    // 法線のY成分が一定以下なら天井
    bool IsCeiling(const VECTOR& normal)
    {
        return normal.y < -0.7f;
    }

    // 当たったポリゴンの法線から壁や天井の押し出しベクトルを累積する
    static VECTOR AccumulatePushVector(const MV1_COLL_RESULT_POLY_DIM& hit, float& velocityY)
    {
        // 累積する押し出しベクトル
        VECTOR push = VGet(0, 0, 0);

        for (int i = 0; i < hit.HitNum; i++)
        {
            // 法線を取得
            const VECTOR& normal = hit.Dim[i].Normal;

            // 壁の判定と押し出しベクトルの累積
            if (IsWall(normal))
            {
                push = VAdd(push, normal);
            }

            // 天井の判定と押し出しベクトルの累積
            if (IsCeiling(normal))
            {
                if (velocityY > 0.0f)
                {
                    velocityY = 0.0f;
                }

                push.y -= 1.0f;
            }
        }
        return push;
    }
}

void PlayerStageCollider::ResolvePlayerCollision(
    VECTOR& position,
    const VECTOR& previousPosition,
    float& velocityY,
    bool& isGround,
    float height,
    float radius)
{
    if (m_StageModelHandle == -1)
    {
        return;
    }

    isGround = false;

    ResolveFloor(position, velocityY, isGround, height);
    ResolveCapsule(position, velocityY, radius, height);
}

void PlayerStageCollider::ResolveFloor(VECTOR& position, float& velocityY, bool& isGround, float height)
{
    // 足元の線分の開始点と終了点を計算
    VECTOR lineStart = VAdd(position, VGet(0.0f, 2.0f, 0.0f));
    VECTOR lineEnd = VAdd(position, VGet(0.0f, -5.0f, 0.0f));

    // 足元の線分とステージの当たり判定
    MV1_COLL_RESULT_POLY hit = MV1CollCheck_Line(m_StageModelHandle, -1, lineStart, lineEnd);;

    if (!hit.HitFlag)
    {
        return;
    }

    if (velocityY > 0.0f)
    {
        return;
    }

    position.y = hit.HitPosition.y;
    velocityY = 0.0f;
    isGround = true;
}
void PlayerStageCollider::ResolveCapsule(VECTOR& position, float& velocityY, float radius, float height)
{
    // 足元と頭上の球の中心位置を計算
    VECTOR bottomCenter = VAdd(position, VGet(0.0f, radius, 0.0f));
    VECTOR topCenter = VAdd(position, VGet(0.0f, height - radius, 0.0f));

    // 累積する押し出しベクトル
    VECTOR push = VGet(0, 0, 0);

    // 足元の球で当たり判定
    auto hitBottom = MV1CollCheck_Sphere(m_StageModelHandle, -1, bottomCenter, radius);

    // 当たったポリゴンの法線から押し出しベクトルを累積
    push = VAdd(push, AccumulatePushVector(hitBottom, velocityY));;

    // コリジョン結果の後始末
    MV1CollResultPolyDimTerminate(hitBottom);

    // 頭上の球で当たり判定
    auto hitTop = MV1CollCheck_Sphere(m_StageModelHandle, -1, topCenter, radius);

    // 当たったポリゴンの法線から押し出しベクトルを累積
    push = VAdd(push, AccumulatePushVector(hitTop, velocityY));
    MV1CollResultPolyDimTerminate(hitTop);

    // 押し出しベクトルがある程度の大きさなら正規化してから位置に加算
    if (VSize(push) > 0.001f)
    {
        push = VNorm(push);

        position = VAdd(position, VScale(push, 1.0f));
    }

    for (int i = 0; i < 4; i++)
    {
        VECTOR center = VAdd(position, VGet(0.0f, radius, 0.0f));

        VECTOR pushOut;

        if (!m_CollisionMap->CheckSphere(center, radius, pushOut))
        {
            break;
        }

        position = VAdd(position, pushOut);
    }
}

void PlayerStageCollider::SetCollisionMap(CollisionMap* collisionMap)
{
    m_CollisionMap = collisionMap;
}
