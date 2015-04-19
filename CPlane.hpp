#ifndef CPLANE_HPP
#define CPLANE_HPP

#include "Global.hpp"
#include "CVector3f.hpp"

class ZE_ALIGN(16) CPlane
{
public:
    ZE_DECLARE_ALIGNED_ALLOCATOR();

    inline CPlane() {}
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
    
    inline void normalize()
    {
        float nd = d;
        float mag = vec.length();
        assert(mag != 0.0f);
        mag = 1.0 / mag;
        vec *= mag;
        d = nd * mag;
    }
    
    union
    {
        struct
        {
            float a, b, c, d;
        };
        float p[4];
        CVector3f vec;
#ifdef __SSE__
        __m128 mVec128;
#endif
    };
};

#endif // CPLANE_HPP
