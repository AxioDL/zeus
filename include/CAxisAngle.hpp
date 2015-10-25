#ifndef CAXISANGLE_H
#define CAXISANGLE_H

#include "Global.hpp"
#include "CVector3f.hpp"
#include "CUnitVector.hpp"

namespace Zeus
{
struct alignas(16) CAxisAngle : CVector3f
{
    ZE_DECLARE_ALIGNED_ALLOCATOR();
    
    CAxisAngle() = default;
    CAxisAngle(const CUnitVector3f& axis, float angle)
        : CVector3f(axis * angle)
    {}

    CAxisAngle(const CVector3f& axisAngle)
        : CVector3f(axisAngle)
    {}

    float angle() { return magnitude(); }
    const CVector3f& getVector() { return *this; }
};
}

#endif // CAXISANGLE_H
