#ifndef CAXISANGLE_H
#define CAXISANGLE_H

#include "Global.hpp"
#include "CVector3f.hpp"

namespace Zeus
{
struct ZE_ALIGN(16) CAxisAngle
{
    ZE_DECLARE_ALIGNED_ALLOCATOR();
    
    CAxisAngle()
        : axis(CVector3f::skOne),
          angle(0)
    {}
    CAxisAngle(const CVector3f& axis, float angle)
        : axis(axis),
          angle(angle)
    {}

    CVector3f axis;
    float     angle;
};
}

#endif // CAXISANGLE_H
