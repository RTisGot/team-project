#ifndef DEF_MAPDATALOADER_H
#define DEF_MAPDATALOADER_H

#include <string>

/**
 * @file MapDataLoader.h
 * @brief マップデータローダーの定義
 */

struct MapData;

/**
 * @class MapDataLoader
 * @brief マップデータをファイルから読み込むクラス
 */
class MapDataLoader
{
public:
    static bool Load(const std::string& filePath, MapData& mapData);
};

#endif  // DEF_MAPDATALOADER_H
