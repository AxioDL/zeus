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
    CAxisAngle(float x, float y, float z) : CVector3f(x, y, z) {}
    CAxisAngle(const CUnitVector3f& axis, float angle) : CVector3f(angle * axis) {}

    CAxisAngle(const CVector3f& axisAngle) : CVector3f(axisAngle) {}

    float angle() const { return magnitude(); }
    const CVector3f& getVector() const { return *this; }

    static const CAxisAngle sIdentity;
};
}

#endif // CAXISANGLE_H
