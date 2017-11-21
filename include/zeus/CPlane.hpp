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

    inline CPlane() : a(1.f), b(0.f), c(0.f), d(0.f) {}
    CPlane(float a, float b, float c, float d) : a(a), b(b), c(c), d(d) {}
    CPlane(const CVector3f& a, const CVector3f& b, const CVector3f& c)
    {
        vec = (b - a).cross(c - a).normalized();
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
        float mag = (b-a).dot(vec);
        float dis = (-(vec.y - d)) / mag;
        return clamp(0.0f, dis, 1.0f);
    }

    inline void normalize()
    {
        float nd = d;
        float mag = vec.magnitude();
        mag = 1.f / mag;
        vec = vec * mag;
        d = nd * mag;
    }

    float pointToPlaneDist(const CVector3f& pos) const
    {
        return pos.dot(vec) - d;
    }

    bool rayPlaneIntersection(const CVector3f& from, const CVector3f& to, CVector3f& point) const;

    const CVector3f& normal() const { return vec; }

    inline float& operator[](size_t idx) { return p[idx]; }
    inline const float& operator[](size_t idx) const { return p[idx]; }

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
