#include "collision/PlayerStageCollider.h"
#include "collision/CollisionMap.h"
#include <cmath>

namespace
{
    constexpr float STEP_HEIGHT = 4.0f;

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
    TryStepUp(position, previousPosition, radius, velocityY);
    ResolveCapsule(position, velocityY, radius, height);
    ResolveFloor(position, velocityY, isGround, height);    // 最後にもう一度床の当たり判定を行うことで段差上昇後の床との当たり判定を確実に処理する
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

    // 前回の位置と当たった位置のY座標の差を計算
    float groundDistance = position.y - hit.HitPosition.y;
    if (groundDistance > 1.0f)
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

    // 複数回繰り返して壁や天井の押し出しベクトルを累積
    for (int i = 0; i < 4; i++)
    {
        // カプセルの中心を計算
        VECTOR center = VAdd(position, VGet(0.0f, radius, 0.0f));

        // ステージとの球の当たり判定
        VECTOR pushOut;

        // 当たったポリゴンの法線から壁や天井の押し出しベクトルを累積
        if (!m_CollisionMap->CheckSphere(center, radius, pushOut))
        {
            break;
        }

        // 押し出しベクトルが0なら当たり判定なし
        position = VAdd(position, pushOut);
    }
}

void PlayerStageCollider::TryStepUp(VECTOR& position, const VECTOR& previousPosition, float radius , float velocityY)
{
    if (velocityY > 0.0f)
    {
        return;
    }
    // 移動方向を計算
    VECTOR moveDir = VSub(position, previousPosition);
    moveDir.y = 0.0f;

    // 移動方向の大きさが小さい場合は段差上昇処理を行わない
    if (VSize(moveDir) < 0.01f)
    {
        return;
    }

    // 移動方向を正規化
    moveDir = VNorm(moveDir);

    // 移動方向に半径分だけオフセットした位置から足元に向かって線分を伸ばす
    VECTOR checkPos = VAdd(position, VScale(moveDir, radius + 3.0f));
    VECTOR start = VAdd(checkPos, VGet(0.0f, STEP_HEIGHT, 0.0f));
    VECTOR end = VAdd(checkPos, VGet(0.0f, -5.0f, 0.0f));

    // 段差上昇用の線分とステージの当たり判定
    auto hit = MV1CollCheck_Line(m_StageModelHandle, -1, start, end);

    if (!hit.HitFlag)
    {
        return;
    }

    // 前回の位置と当たった位置のY座標の差を計算
    float diff = hit.HitPosition.y - previousPosition.y;

    if (diff > 0.0f && diff <= STEP_HEIGHT)
    {
        // キャラのY座標を当たった位置に修正
        position.y = hit.HitPosition.y;
    }
}

void PlayerStageCollider::SetCollisionMap(CollisionMap* collisionMap)
{
    m_CollisionMap = collisionMap;
}
