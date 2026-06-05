#ifndef ORB_DATA_H
#define ORB_DATA_H

#include <cstdint>

// オーブの種類を表す列挙型
enum class OrbType
{
    Normal,
    Rare,
    Legendary,
};

// オーブの状態を表す列挙型
enum class OrbState
{
    World,      // 地面
    Player,     // プレイヤー所持
    Companion,  // お供所持
    Deposited   // 持ち帰り済み
};

// オーブのデータを管理するための構造体
struct OrbData
{
    uint32_t m_Id = 0;  // オーブの識別子 
    // TODO: 将来オーブの種類に応じたモデルやエフェクトを追加するために使用する
    OrbType m_Type = OrbType::Normal;   // オーブの種類

    OrbState m_State = OrbState::World; // オーブの状態
    // TODO: 将来オーブの価値に応じたスコアやアイテムを追加するために使用する
    int m_Value = 1;    // オーブの価値

    bool m_IsCollected = false; // オーブが収集されたかどうか
};

#endif // ORB_DATA_H
