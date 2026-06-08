#ifndef DEF_MAPDATALOADER_H
#define DEF_MAPDATALOADER_H

#include <string>

struct MapData;

class MapDataLoader
{
public:
    static bool Load(const std::string& filePath, MapData& mapData);
};

#endif  // DEF_MAPDATALOADER_H
