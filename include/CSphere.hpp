#ifndef CSPHERE_HPP
#define CSPHERE_HPP

#include "CVector3f.hpp"

namespace Zeus
{
class ZE_ALIGN(16) CSphere
{
public:
    ZE_DECLARE_ALIGNED_ALLOCATOR();

    CSphere(const CVector3f& position, float radius) { vec = position; r = radius; }
    inline CVector3f getSurfaceNormal(const CVector3f& coord) { return (vec - coord).normalized(); }

    union
    {
        struct { float x, y, z, r; };
        float s[4];
        CVector3f vec;
#if __SSE__
        __m128 mVec128;
#endif
    };
};
}

#endif
