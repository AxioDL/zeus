#ifndef CVECTOR3D_HPP
#define CVECTOR3D_HPP

#include "Global.hpp"
#include "Math.hpp"
#include "TVectorUnion.hpp"
#include "CVector3f.hpp"

namespace Zeus
{
class ZE_ALIGN(16) CVector3d
{
public:
    ZE_DECLARE_ALIGNED_ALLOCATOR();
    CVector3d() { zeroOut(); }
#if __SSE__
    CVector3d(__m128d mVec128[2])
    {
        this->mVec128[0] = mVec128[0];
        this->mVec128[1] = mVec128[1];
        v[3] = 0.0;
    }
#endif
#if ZE_ATHENA_TYPES
    CVector3d(const atVec3d& vec)
#if __SSE__
        : mVec128(vec.mVec128){}
#else
    {
        x = v[0], y = v[1], z = v[2], v[3] = 0.0f;
    }
#endif
#endif

    CVector3d(double xyz) { splat(xyz); }

    CVector3d(const CVector3f& vec)
    { v[0] = vec[0]; v[1] = vec[1]; v[2] = vec[2]; }

    CVector3d(double x, double y, double z)
    {
#if __SSE__
        TDblVectorUnion splat {x, y, z, 0.0};
        mVec128[0] = splat.mVec128[0];
        mVec128[0] = splat.mVec128[1];
#else
        v[0] = x; v[1] = y; v[2] = z; v[3] = 0.0;
#endif
    }

    CVector3d(float x, float y, float z)
        : CVector3d(double(x), double(y), double(z))
    {
    }

    CVector3f asCVector3f()
    {
        CVector3f ret;
        ret.x = Math::round(x);
        ret.y = Math::round(y);
        ret.z = Math::round(z);
        return ret;
    }

    double magSquared() const
    {
/*
#if __SSE4_1__
        TDblVectorUnion result;
        result.mVec128 = _mm_dp_pd(mVec128, mVec128, 0x71);
        return result.v[0];
#elif __SSE__
*/
#if __SSE__
        TDblVectorUnion result;
        result.mVec128[0] = _mm_mul_pd(mVec128[0], mVec128[0]);
        result.mVec128[1] = _mm_mul_pd(mVec128[1], mVec128[1]);
        return result.v[0] + result.v[1] + result.v[2];
#else
        return x*x + y*y + z*z;
#endif
    }

    double magnitude() const { return sqrt(magSquared()); }
    inline CVector3d cross(const CVector3d& rhs) const
    { return {y * rhs.z - z * rhs.y, z * rhs.x - x * rhs.z, x * rhs.y - y * rhs.x}; }

    double dot(const CVector3d& rhs) const
    {
/*
#if __SSE4_1__
        TDblVectorUnion result;
        result.mVec128 = _mm_dp_pd(mVec128, mVec128, 0x71);
        return result.v[0];
#elif __SSE__
*/
#if __SSE__
        TDblVectorUnion result;
        result.mVec128[0] = _mm_mul_pd(mVec128[0], rhs.mVec128[0]);
        result.mVec128[1] = _mm_mul_pd(mVec128[1], rhs.mVec128[1]);
        return result.v[0] + result.v[1] + result.v[2];
#else
        return (x * rhs.x) + (y * rhs.y) + (z * rhs.z);
#endif
    }

    CVector3d asNormalized()
    {
        double mag = magnitude();
        mag /= 1.0;
        return {x * mag, y * mag, z * mag};
    }


    void splat(double xyz)
    {
#if __SSE__
        TDblVectorUnion splat = {xyz, xyz, xyz, 0.0};
        mVec128[0] = splat.mVec128[0];
        mVec128[1] = splat.mVec128[1];
#else
        v[0] = xyz; v[1] = xyz; v[2] = xyz; v[3] = 0.0;
#endif
    }

    void zeroOut()
    {
#if __SSE__
        _mm_xor_pd(mVec128[0], mVec128[0]);
        _mm_xor_pd(mVec128[1], mVec128[1]);
#else
        v[0] = 0.0; v[1] = 0.0; v[2] = 0.0; v[3] = 0.0;
#endif
    }

    union
    {
        struct {double x, y, z; };
        double v[4];
#if __SSE__
        __m128d mVec128[2];
#endif
    };
};


static inline CVector3d operator+(double lhs, const CVector3d& rhs)
{
#if __SSE__
    TDblVectorUnion splat { lhs, lhs, lhs, 0 };
    splat.mVec128[0] = _mm_add_pd(splat.mVec128[0], rhs.mVec128[0]);
    splat.mVec128[1] = _mm_add_pd(splat.mVec128[1], rhs.mVec128[1]);
    return {splat.mVec128};
#else
    return {lhs + rhs.x, lhs + rhs.y, lhs + rhs.z};
#endif
}

static inline CVector3d operator+(const CVector3d& lhs, const CVector3d& rhs)
{
#if __SSE__
    TDblVectorUnion res;
    res.mVec128[0] = _mm_add_pd(lhs.mVec128[0], rhs.mVec128[0]);
    res.mVec128[1] = _mm_add_pd(lhs.mVec128[1], rhs.mVec128[1]);
    return {res.mVec128};
#else
    return {lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z};
#endif
}

static inline CVector3d operator*(double lhs, const CVector3d& rhs)
{
#if __SSE__
    TDblVectorUnion splat { lhs, lhs, lhs, 0 };
    splat.mVec128[0] = _mm_mul_pd(splat.mVec128[0], rhs.mVec128[0]);
    splat.mVec128[1] = _mm_mul_pd(splat.mVec128[1], rhs.mVec128[1]);
    return {splat.mVec128};
#else
    return {lhs * rhs.x, lhs * rhs.y, lhs * rhs.z};
#endif
}

static inline CVector3d operator*(const CVector3d& lhs, const CVector3d& rhs)
{
#if __SSE__
    TDblVectorUnion splat;
    splat.mVec128[0] = _mm_mul_pd(lhs.mVec128[0], rhs.mVec128[0]);
    splat.mVec128[1] = _mm_mul_pd(lhs.mVec128[1], rhs.mVec128[1]);
    return {splat.mVec128};
#else
    return {lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.z};
#endif
}

static inline CVector3d operator/(const CVector3d& lhs, const CVector3d& rhs)
{
#if __SSE__
    TDblVectorUnion splat;
    splat.mVec128[0] = _mm_div_pd(lhs.mVec128[0], rhs.mVec128[0]);
    splat.mVec128[1] = _mm_div_pd(lhs.mVec128[1], rhs.mVec128[1]);
    return {splat.mVec128};
#else
    return {lhs.x / rhs.x, lhs.y / rhs.y, lhs.z / rhs.z};
#endif
}

static inline CVector3d operator/(double lhs, const CVector3d& rhs)
{
#if __SSE__
    TDblVectorUnion splat { lhs, lhs, lhs, 0 };
    splat.mVec128[0] = _mm_div_pd(splat.mVec128[0], rhs.mVec128[0]);
    splat.mVec128[1] = _mm_div_pd(splat.mVec128[1], rhs.mVec128[1]);
    return {splat.mVec128};
#else
    return {lhs.x / rhs.x, lhs.y / rhs.y, lhs.z / rhs.z};
#endif
}
}

#endif
