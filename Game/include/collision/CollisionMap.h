#ifndef DEF_COLLISIONMAP_H
#define DEF_COLLISIONMAP_H

#include "collision/BoxCollider.h"
#include <vector>
#include <DxLib.h>

/**
 * @file CollisionMap.h
 * @brief コリジョンマップの定義
 */

/**
 * @class CollisionMap
 * @brief コリジョンマップクラス
 */
class CollisionMap
{
public:

    /**
     * @brief AABBコリジョンマップにボックスを追加
     * @param min ボックスの最小座標
     * @param max ボックスの最大座標
     */
    void AddBox(const VECTOR& min, const VECTOR& max);

    /**
     * @brief 球とコリジョンマップの当たり判定
     * @param center 球の中心座標
     * @param radius 球の半径
     * @param pushOut 当たった場合の押し出しベクトルの出力
     */
    bool CheckSphere(const VECTOR& center, float radius, VECTOR& pushOut) const;

#ifdef _DEBUG
    void DrawDebug() const;
#endif

private:

    std::vector<BoxCollider> m_Boxes;   ///< コリジョンマップに登録されたボックスのリスト
};

#endif // DEF_COLLISIONMAP_H
