#ifndef DEF_MAPBASE_H
#define DEF_MAPBASE_H

#include "map/SpawnPoint.h"

class MapBase
{
public:
    virtual ~MapBase() = default;

    virtual bool Load() = 0;
    virtual void Update() = 0;
    virtual void Draw() = 0;

    const SpawnPoint& GetPlayerSpawnPoint() const
    {
        return m_PlayerSpawnPoint;
    }

protected:
    SpawnPoint m_PlayerSpawnPoint;
};

#endif // DEF_MAPBASE_H
