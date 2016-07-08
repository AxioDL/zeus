#ifndef CSPHERE_HPP
#define CSPHERE_HPP

#include "zeus/CVector3f.hpp"

namespace zeus
{
class alignas(16) CSphere
{
public:
    ZE_DECLARE_ALIGNED_ALLOCATOR();

    CSphere(const CVector3f& position, float radius) : position(position), radius(radius) {}

    inline CVector3f getSurfaceNormal(const CVector3f& coord) { return (position - coord).normalized(); }

    inline bool intersects(const CSphere& other)
    {
        float dist = (position - other.position).magnitude();
        return dist < (radius + other.radius);
    }

    CVector3f position;
    float radius;
};
}

#endif
