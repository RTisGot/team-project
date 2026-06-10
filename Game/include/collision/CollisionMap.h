#ifndef DEF_COLLISIONMAP_H
#define DEF_COLLISIONMAP_H

#include "collision/BoxCollider.h"
#include <vector>

class CollisionMap
{
public:

    void AddBox(const VECTOR& min, const VECTOR& max);

    bool CheckSphere(const VECTOR& center, float radius, VECTOR& pushOut) const;

#ifdef _DEBUG
    void DrawDebug() const;
#endif

private:

    std::vector<BoxCollider> m_Boxes;
};

#endif // DEF_COLLISIONMAP_H
