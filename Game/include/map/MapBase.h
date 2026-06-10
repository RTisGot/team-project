#ifndef DEF_MAPBASE_H
#define DEF_MAPBASE_H

#include "map/MapData.h"

class MapBase
{
public:
    virtual ~MapBase() = default;

    virtual bool Init() = 0;
    virtual void Update() = 0;
    virtual void Draw() = 0;

    virtual int GetModelHandle() const = 0;
    virtual const MapData& GetMapData() const = 0;

protected:
    MapData m_MapData;
};

#endif // DEF_MAPBASE_H
