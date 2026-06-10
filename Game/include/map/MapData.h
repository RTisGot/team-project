#pragma once

#include <vector>
#include <DxLib.h>

/**
 * @brief カメラ初期設定
 */
struct CameraData
{
    float Yaw = 0.0f;
    float Pitch = 0.3f;
    float Distance = 30.0f;
};

/**
 * @brief スポーン地点情報
 */
struct SpawnPoint
{
    VECTOR Position{};
    VECTOR Rotation{};

    CameraData Camera;
};

/**
 * @brief 敵配置情報
 */
struct EnemySpawnData
{
    VECTOR Position{};
    int EnemyType = 0;
};

/**
 * @brief アイテム配置情報
 */
struct ItemSpawnData
{
    VECTOR Position{};
    int ItemId = 0;
};

/**
 * @brief 収集アイテム配置情報
 */
struct CollectibleSpawnData
{
    VECTOR Position{};
    int CollectibleId = 0;
};

/**
 * @brief ボックスコライダー情報
 */
struct BoxColliderData
{
    VECTOR m_Min;
    VECTOR m_Max;
};

/**
 * @brief マップ全体データ
 */
struct MapData
{
    SpawnPoint PlayerSpawn;
    std::vector<EnemySpawnData> EnemySpawns;
    std::vector<ItemSpawnData> ItemSpawns;
    std::vector<CollectibleSpawnData> CollectibleSpawns;
    std::vector<BoxColliderData> BoxColliders;
};
