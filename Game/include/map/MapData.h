#pragma once

#include <vector>
#include <DxLib.h>

/**
 * @brief スポーン地点情報
 */
struct SpawnPoint
{
    VECTOR Position{};
    VECTOR Rotation{};
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
 * @brief マップ全体データ
 */
struct MapData
{
    SpawnPoint PlayerSpawn;
    std::vector<EnemySpawnData> EnemySpawns;
    std::vector<ItemSpawnData> ItemSpawns;
    std::vector<CollectibleSpawnData> CollectibleSpawns;
};
