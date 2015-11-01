#ifndef CUNITVECTOR_HPP
#define CUNITVECTOR_HPP

#include "CVector3f.hpp"

namespace Zeus
{
class alignas(16) CUnitVector3f : public CVector3f
{
public:
    ZE_DECLARE_ALIGNED_ALLOCATOR();

    CUnitVector3f()
        : CVector3f(0, 1, 0)
    {
    }

    CUnitVector3f(const CVector3f& vec, bool doNormalize = false)
        : CVector3f(vec)
    {
        if (doNormalize && canBeNormalized())
            normalize();
    }
};
}
#endif