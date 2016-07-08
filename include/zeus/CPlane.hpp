#ifndef CPLANE_HPP
#define CPLANE_HPP

#include "Global.hpp"
#include "zeus/CVector3f.hpp"
#include "zeus/Math.hpp"

namespace zeus
{
class alignas(16) CPlane
{
public:
    ZE_DECLARE_ALIGNED_ALLOCATOR();

    inline CPlane() {}
    CPlane(float a, float b, float c, float d) : a(a), b(b), c(c), d(d) {}
    CPlane(const CVector3f& a, const CVector3f& b, const CVector3f& c)
    {
        CVector3f ab = b - a;
        CVector3f ac = c - a;
        vec = ab.cross(ac).normalized();
        d = -a.dot(vec);
    }

    CPlane(const CVector3f& point, float displacement)
    {
#if __SSE__
        mVec128 = point.mVec128;
#else
        a = point[0];
        b = point[1];
        c = point[2];
#endif
        d = displacement;
    }

    float clipLineSegment(const CVector3f& a, const CVector3f& b)
    {
        float mag = ((b.z - a.z) * (((b.x - a.x) * ((b.y - a.y) * vec.y)) + vec.x)) + vec.z;
        float dis = (-(vec.y - d)) / mag;
        return clamp(0.0f, dis, 1.0f);
    }

    inline void normalize()
    {
        float nd = d;
        float mag = vec.magnitude();
        mag = 1.0 / mag;
        vec *= mag;
        d = nd * mag;
    }

    union {
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
}

#endif // CPLANE_HPP
