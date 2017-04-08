#ifndef CUNITVECTOR_HPP
#define CUNITVECTOR_HPP

#include "zeus/CVector3f.hpp"

namespace zeus
{
class alignas(16) CUnitVector3f : public CVector3f
{
public:
    ZE_DECLARE_ALIGNED_ALLOCATOR();

    CUnitVector3f() : CVector3f(0, 1, 0) {}
    CUnitVector3f(float x, float y, float z, bool doNormalize = true) : CVector3f(x, y, z)
    {
        if (doNormalize && canBeNormalized())
            normalize();
    }
    CUnitVector3f(const CVector3f& vec, bool doNormalize = true) : CVector3f(vec)
    {
        if (doNormalize && canBeNormalized())
            normalize();
    }
};
}
#endif
