#ifndef CAXISANGLE_H
#define CAXISANGLE_H

#include "Global.hpp"
#include "zeus/CVector3f.hpp"
#include "CUnitVector.hpp"

namespace zeus
{
struct alignas(16) CAxisAngle : CVector3f
{
    ZE_DECLARE_ALIGNED_ALLOCATOR();

    CAxisAngle() = default;
    CAxisAngle(const CUnitVector3f& axis, float distance) : CVector3f(distance * axis) {}

    CAxisAngle(const CVector3f& axisAngle) : CVector3f(axisAngle) {}

    float angle() { return magnitude(); }
    const CVector3f& getVector() { return *this; }
};
}

#endif // CAXISANGLE_H
