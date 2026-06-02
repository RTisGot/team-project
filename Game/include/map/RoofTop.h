#pragma once
#include <DxLib.h>
#include <memory>
#include <follower/follower.h>
#include "object/Door.h"

/**
 * @file RoofTop.h
 * @brief 屋上クラスの定義
 */

 /**
  * @class RoofTop
  * @brief 屋上クラス
  */
class RoofTop
{
public:
    RoofTop();
    ~RoofTop();
    bool Init();

    /**
     * @brief 更新処理
     * @param playerPos プレイヤーの座標
     */
    void Update(const VECTOR& playerPos, float playerAngle);

    void Draw();

    int GetModelHandle() const { return m_modelHandle; }

private:
    int m_modelHandle;              ///< 屋上のモデルハンドル
    int m_itemModelHandle;          ///< アイテムモデル

    std::unique_ptr<Door> m_door;   ///< ドア

    // お供キャラクター
    std::unique_ptr<Follower> m_follower;
};
