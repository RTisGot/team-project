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
    const MapData& GetMapData() const { return m_MapData; }

protected:
    MapData m_MapData;
};

#endif // DEF_MAPBASE_H
