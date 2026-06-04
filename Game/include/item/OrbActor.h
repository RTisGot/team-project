#ifndef ORB_ACTOR_H
#define ORB_ACTOR_H

#include "item/OrbData.h"
#include <memory>
#include <DxLib.h>

class Player;

class OrbActor
{
public:
    OrbActor();
    ~OrbActor();

    /**
     * @brief 初期化
     * @param id オーブの識別子
     * @param position オーブの初期位置
     */
    bool Init(uint32_t id, const VECTOR& position, int modelHandle);

    void Update();
    void Draw() const;

    /**
     * @brief プレイヤーがオーブを拾えるかどうかを判定する
     * @param playerPos プレイヤーの座標
     * @param range 判定範囲
     * @return プレイヤーがオーブを拾える場合はtrue、それ以外はfalse
     */
    bool CanPickup(const VECTOR& playerPos, float range) const;

    /**
     * @brief オーブのデータを取得する
     * @return オーブのデータ
     */
    const OrbData& GetData() const;

    /**
     * @brief オーブのデータを取得する（編集可能）
     * @return オーブのデータ
     */
    OrbData& GetData();

    // オーブが収集されたかどうかを設定する
    void SetCollected(bool collected);

    // オーブが収集されたかどうかを取得する
    bool IsCollected() const;

    // オーブの位置を取得する
    const VECTOR& GetPosition() const;

private:
    OrbData m_Data; // オーブのデータ
    VECTOR m_Position{};    // オーブの位置
    VECTOR m_Rotation{};    // オーブの回転
    int m_ModelHandle = -1; // オーブのモデルハンドル
};

#endif // ORB_ACTOR_H
