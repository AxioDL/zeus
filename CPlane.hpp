#ifndef CPLANE_HPP
#define CPLANE_HPP

#include "Global.hpp"
#include "CVector3f.hpp"

class ZE_ALIGN(16) CPlane
{
public:
    ZE_DECLARE_ALIGNED_ALLOCATOR();

    CPlane()
    {
#if __SSE__
        mVec128 = _mm_xor_ps(mVec128, mVec128);
#else
        a = 0.0f; b = 0.0f; c = 0.0f; d = 0.0f;
#endif
    }
    CPlane(float a, float b, float c, float d) : a(a), b(b), c(c), d(d) {}
    CPlane(const CVector3f& point, float displacement)
    {
#if __SSE__
        mVec128 = point.mVec128;
#else
        a = point[0]; b = point[1]; c = point[2];
#endif
        d = displacement;
    }
    
protected:
    union
    {
        struct
        {
            float a, b, c, d;
        };
        float p[4];
#ifdef __SSE__
        __m128 mVec128;
#endif
    };
};

#endif // CPLANE_HPP
