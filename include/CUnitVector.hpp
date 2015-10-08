#ifndef CUNITVECTOR_HPP
#define CUNITVECTOR_HPP

#include "CVector3f.hpp"

namespace Zeus
{
class ZE_ALIGN(16) CUnitVector3f : public CVector3f
{
public:
    ZE_DECLARE_ALIGNED_ALLOCATOR();

    CUnitVector3f(const CVector3f& vec)
        : CVector3f(vec)
    {
        if (canBeNormalized())
            normalize();
    }
};
}
#endif
