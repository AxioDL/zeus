#ifndef CVECTOR3D_HPP
#define CVECTOR3D_HPP

#include "Global.hpp"
#include "zeus/Math.hpp"
#include "TVectorUnion.hpp"
#include "zeus/CVector3f.hpp"

namespace zeus
{
class alignas(16) CVector3d
{
public:
    ZE_DECLARE_ALIGNED_ALLOCATOR();
    CVector3d() { zeroOut(); }
#if __SSE__
    CVector3d(const __m128d mVec128[2])
    {
        this->mVec128[0] = mVec128[0];
        this->mVec128[1] = mVec128[1];
        v[3] = 0.0;
    }
#endif
#if ZE_ATHENA_TYPES
    CVector3d(const atVec3d& vec)
    {
#if __SSE__
        mVec128[0] = vec.mVec128[0];
        mVec128[1] = vec.mVec128[1];
#else
        x = v[0], y = v[1], z = v[2], v[3] = 0.0f;
#endif
    }
#endif

    explicit CVector3d(double xyz) { splat(xyz); }

    CVector3d(const CVector3f& vec)
    {
#if __SSE__
        mVec128[0] = _mm_cvtps_pd(vec.mVec128);
        v[2] = vec[2];
        v[3] = 0.0;
#else
        v[0] = vec[0];
        v[1] = vec[1];
        v[2] = vec[2];
#endif
    }

    CVector3d(double x, double y, double z)
    {
#if __SSE__
        TDblVectorUnion splat{x, y, z, 0.0};
        mVec128[0] = splat.mVec128[0];
        mVec128[1] = splat.mVec128[1];
#else
        v[0] = x;
        v[1] = y;
        v[2] = z;
#endif
    }

    CVector3f asCVector3f()
    {
        return CVector3f(float(x), float(y), float(z));
    }

    double magSquared() const
    {
#if __SSE__
        TDblVectorUnion result;
#if __SSE4_1__
        result.mVec128[0] = _mm_dp_pd(mVec128[0], mVec128[0], 0x31);
        return result.v[0] + (v[2] * v[2]);
#else
        result.mVec128[0] = _mm_mul_pd(mVec128[0], mVec128[0]);
        result.mVec128[1] = _mm_mul_pd(mVec128[1], mVec128[1]);
        return result.v[0] + result.v[1] + result.v[2];
#endif
#else
        return x * x + y * y + z * z;
#endif
    }

    double magnitude() const { return sqrt(magSquared()); }
    inline CVector3d cross(const CVector3d& rhs) const
    {
        return {y * rhs.z - z * rhs.y, z * rhs.x - x * rhs.z, x * rhs.y - y * rhs.x};
    }

    double dot(const CVector3d& rhs) const
    {
#if __SSE__
        TDblVectorUnion result;
#if __SSE4_1__
        result.mVec128[0] = _mm_dp_pd(mVec128[0], rhs.mVec128[0], 0x31);
        return result.v[0] + (v[2] * rhs.v[2]);
#else
        result.mVec128[0] = _mm_mul_pd(mVec128[0], rhs.mVec128[0]);
        result.mVec128[1] = _mm_mul_pd(mVec128[1], rhs.mVec128[1]);
        return result.v[0] + result.v[1] + result.v[2];
#endif
#else
        return (x * rhs.x) + (y * rhs.y) + (z * rhs.z);
#endif
    }

    CVector3d asNormalized()
    {
        double mag = magnitude();
        mag = 1.0 / mag;
        return {x * mag, y * mag, z * mag};
    }

    void splat(double xyz)
    {
#if __SSE__
        TDblVectorUnion splat = {xyz, xyz, xyz, 0.0};
        mVec128[0] = splat.mVec128[0];
        mVec128[1] = splat.mVec128[1];
#else
        v[0] = xyz;
        v[1] = xyz;
        v[2] = xyz;
        v[3] = 0.0;
#endif
    }

    void zeroOut()
    {
        *this = skZero;
    }

    inline CVector3d operator+(const CVector3d& rhs) const
    {
#if __SSE__
        const __m128d tmpVec128[2] = {_mm_add_pd(mVec128[0], rhs.mVec128[0]),
                                      _mm_add_pd(mVec128[1], rhs.mVec128[1])};
        return CVector3d(tmpVec128);
#else
        return CVector3d(x + rhs.x, y + rhs.y, z + rhs.z);
#endif
    }
    inline CVector3d operator-(const CVector3d& rhs) const
    {
#if __SSE__
        const __m128d tmpVec128[2] = {_mm_sub_pd(mVec128[0], rhs.mVec128[0]),
                                      _mm_sub_pd(mVec128[1], rhs.mVec128[1])};
        return CVector3d(tmpVec128);
#else
        return CVector3d(x - rhs.x, y - rhs.y, z - rhs.z);
#endif
    }
    inline CVector3d operator*(const CVector3d& rhs) const
    {
#if __SSE__
        const __m128d tmpVec128[2] = {_mm_mul_pd(mVec128[0], rhs.mVec128[0]),
                                      _mm_mul_pd(mVec128[1], rhs.mVec128[1])};
        return CVector3d(tmpVec128);
#else
        return CVector3d(x * rhs.x, y * rhs.y, z * rhs.z);
#endif
    }
    inline CVector3d operator/(const CVector3d& rhs) const
    {
#if __SSE__
        const __m128d tmpVec128[2] = {_mm_div_pd(mVec128[0], rhs.mVec128[0]),
                                      _mm_div_pd(mVec128[1], rhs.mVec128[1])};
        return CVector3d(tmpVec128);
#else
        return CVector3d(x / rhs.x, y / rhs.y, z / rhs.z);
#endif
    }

    inline double& operator[](size_t idx) { assert(idx < 3); return v[idx]; }
    inline const double& operator[](size_t idx) const { assert(idx < 3); return v[idx]; }

    union {
        struct
        {
            double x, y, z;
        };
        double v[4];
#if __SSE__
        __m128d mVec128[2];
#endif
    };

    static const CVector3d skZero;
};

static inline CVector3d operator+(double lhs, const CVector3d& rhs)
{
#if __SSE__
    TDblVectorUnion splat{lhs, lhs, lhs, 0};
    splat.mVec128[0] = _mm_add_pd(splat.mVec128[0], rhs.mVec128[0]);
    splat.mVec128[1] = _mm_add_pd(splat.mVec128[1], rhs.mVec128[1]);
    return {splat.mVec128};
#else
    return {lhs + rhs.x, lhs + rhs.y, lhs + rhs.z};
#endif
}

static inline CVector3d operator-(double lhs, const CVector3d& rhs)
{
#if __SSE__
    TDblVectorUnion splat{lhs, lhs, lhs, 0};
    splat.mVec128[0] = _mm_sub_pd(splat.mVec128[0], rhs.mVec128[0]);
    splat.mVec128[1] = _mm_sub_pd(splat.mVec128[1], rhs.mVec128[1]);
    return {splat.mVec128};
#else
    return {lhs - rhs.x, lhs - rhs.y, lhs - rhs.z};
#endif
}

static inline CVector3d operator*(double lhs, const CVector3d& rhs)
{
#if __SSE__
    TDblVectorUnion splat{lhs, lhs, lhs, 0};
    splat.mVec128[0] = _mm_mul_pd(splat.mVec128[0], rhs.mVec128[0]);
    splat.mVec128[1] = _mm_mul_pd(splat.mVec128[1], rhs.mVec128[1]);
    return {splat.mVec128};
#else
    return {lhs * rhs.x, lhs * rhs.y, lhs * rhs.z};
#endif
}

static inline CVector3d operator/(double lhs, const CVector3d& rhs)
{
#if __SSE__
    TDblVectorUnion splat{lhs, lhs, lhs, 0};
    splat.mVec128[0] = _mm_div_pd(splat.mVec128[0], rhs.mVec128[0]);
    splat.mVec128[1] = _mm_div_pd(splat.mVec128[1], rhs.mVec128[1]);
    return {splat.mVec128};
#else
    return {lhs.x / rhs.x, lhs.y / rhs.y, lhs.z / rhs.z};
#endif
}
}

#endif
