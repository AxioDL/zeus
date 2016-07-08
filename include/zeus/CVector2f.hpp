#ifndef CVECTOR2f_HPP
#define CVECTOR2f_HPP

#include "Global.hpp"
#include "zeus/Math.hpp"
#include "TVectorUnion.hpp"

#if ZE_ATHENA_TYPES
#include <athena/IStreamReader.hpp>
#endif

#include "zeus/Math.hpp"
#include <cassert>

namespace zeus
{
class alignas(16) CVector2f
{
#if __atdna__
    float clangVec __attribute__((__vector_size__(8)));
#endif
public:
    // ZE_DECLARE_ALIGNED_ALLOCATOR();
    union {
        struct
        {
            float x, y;
        };
        float v[4];
#if __SSE__
        __m128 mVec128;
#endif
    };

    inline CVector2f() { zeroOut(); }
#if __SSE__
    CVector2f(const __m128& mVec128) : mVec128(mVec128)
    {
        v[2] = 0.0f;
        v[3] = 0.0f;
    }
#endif
#if ZE_ATHENA_TYPES
    CVector2f(const atVec2f& vec)
#if __SSE__
    : mVec128(vec.mVec128)
    {
    }
#else
    {
        x = vec.vec[0], y = vec.vec[1], v[2] = 0.0f, v[3] = 0.0f;
    }
#endif

    operator atVec2f()
    {
        atVec2f ret;
#if __SSE__
        ret.mVec128 = mVec128;
#else
        ret.vec = v;
#endif
        return ret;
    }
    operator atVec2f() const
    {
        atVec2f ret;
#if __SSE__
        ret.mVec128 = mVec128;
#else
        ret.vec = v;
#endif
        return ret;
    }

    void readBig(athena::io::IStreamReader& input)
    {
        x = input.readFloatBig();
        y = input.readFloatBig();
        v[2] = 0.0f;
        v[3] = 0.0f;
    }
#endif

    CVector2f(float xy) { splat(xy); }
    void assign(float x, float y)
    {
        v[0] = x;
        v[1] = y;
        v[2] = 0;
        v[3] = 0.0f;
    }
    CVector2f(float x, float y) { assign(x, y); }

    inline bool operator==(const CVector2f& rhs) const { return (x == rhs.x && y == rhs.y); }
    inline bool operator!=(const CVector2f& rhs) const { return !(*this == rhs); }
    inline bool operator<(const CVector2f& rhs) const
    {
#if __SSE__
        TVectorUnion vec;
        vec.mVec128 = _mm_cmplt_ps(mVec128, rhs.mVec128);
        return (vec.v[0] != 0 || vec.v[1] != 0);
#else
        return (x < rhs.x || y < rhs.y);
#endif
    }
    inline bool operator<=(const CVector2f& rhs) const
    {
#if __SSE__
        TVectorUnion vec;
        vec.mVec128 = _mm_cmple_ps(mVec128, rhs.mVec128);
        return (vec.v[0] != 0 || vec.v[1] != 0);
#else
        return (x <= rhs.x || y <= rhs.y);
#endif
    }
    inline bool operator>(const CVector2f& rhs) const
    {
#if __SSE__
        TVectorUnion vec;
        vec.mVec128 = _mm_cmpgt_ps(mVec128, rhs.mVec128);
        return (vec.v[0] != 0 || vec.v[1] != 0);
#else
        return (x > rhs.x || y > rhs.y);
#endif
    }
    inline bool operator>=(const CVector2f& rhs) const
    {
#if __SSE__
        TVectorUnion vec;
        vec.mVec128 = _mm_cmpge_ps(mVec128, rhs.mVec128);
        return (vec.v[0] != 0 || vec.v[1] != 0);
#else
        return (x >= rhs.x || y >= rhs.y);
#endif
    }

    inline CVector2f operator+(const CVector2f& rhs) const
    {
#if __SSE__
        return CVector2f(_mm_add_ps(mVec128, rhs.mVec128));
#else
        return CVector2f(x + rhs.x, y + rhs.y);
#endif
    }
    inline CVector2f operator-(const CVector2f& rhs) const
    {
#if __SSE__
        return CVector2f(_mm_sub_ps(mVec128, rhs.mVec128));
#else
        return CVector2f(x - rhs.x, y - rhs.y);
#endif
    }
    inline CVector2f operator-() const
    {
#if __SSE__
        return CVector2f(_mm_sub_ps(_mm_xor_ps(mVec128, mVec128), mVec128));
#else
        return CVector2f(-x, -y);
#endif
    }
    inline CVector2f operator*(const CVector2f& rhs) const
    {
#if __SSE__
        return CVector2f(_mm_mul_ps(mVec128, rhs.mVec128));
#else
        return CVector2f(x * rhs.x, y * rhs.y);
#endif
    }
    inline CVector2f operator/(const CVector2f& rhs) const
    {
#if __SSE__
        return CVector2f(_mm_div_ps(mVec128, rhs.mVec128));
#else
        return CVector2f(x / rhs.x, y / rhs.y);
#endif
    }
    inline CVector2f operator+(float val) const
    {
#if __SSE__
        TVectorUnion splat = {{val, val, 0.0f, 0.0f}};
        return CVector2f(_mm_add_ps(mVec128, splat.mVec128));
#else
        return CVector2f(x + val, y + val);
#endif
    }
    inline CVector2f operator-(float val) const
    {
#if __SSE__
        TVectorUnion splat = {{val, val, 0.0f, 0.0f}};
        return CVector2f(_mm_sub_ps(mVec128, splat.mVec128));
#else
        return CVector2f(x - val, y - val);
#endif
    }
    inline CVector2f operator*(float val) const
    {
#if __SSE__
        TVectorUnion splat = {{val, val, 0.0f, 0.0f}};
        return CVector2f(_mm_mul_ps(mVec128, splat.mVec128));
#else
        return CVector2f(x * val, y * val);
#endif
    }
    inline CVector2f operator/(float val) const
    {
#if __SSE__
        TVectorUnion splat = {{val, val, val, 0.0f}};
        return CVector2f(_mm_div_ps(mVec128, splat.mVec128));
#else
        return CVector2f(x / val, y / val);
#endif
    }
    inline const CVector2f& operator+=(const CVector2f& rhs)
    {
#if __SSE__
        mVec128 = _mm_add_ps(mVec128, rhs.mVec128);
#else
        x += rhs.x;
        y += rhs.y;
#endif
        return *this;
    }
    inline const CVector2f& operator-=(const CVector2f& rhs)
    {
#if __SSE__
        mVec128 = _mm_sub_ps(mVec128, rhs.mVec128);
#else
        x -= rhs.x;
        y -= rhs.y;
#endif
        return *this;
    }
    inline const CVector2f& operator*=(const CVector2f& rhs)
    {
#if __SSE__
        mVec128 = _mm_mul_ps(mVec128, rhs.mVec128);
#else
        x *= rhs.x;
        y *= rhs.y;
#endif
        return *this;
    }
    inline const CVector2f& operator/=(const CVector2f& rhs)
    {
#if __SSE__
        mVec128 = _mm_div_ps(mVec128, rhs.mVec128);
#else
        x /= rhs.x;
        y /= rhs.y;
#endif
        return *this;
    }
    inline void normalize()
    {
        float mag = magnitude();
        mag = 1.0 / mag;
        *this *= mag;
    }

    inline CVector2f normalized() const
    {
        float mag = magnitude();
        mag = 1.0f / mag;
        return *this * mag;
    }

    inline CVector2f perpendicularVector() const { return {-y, x}; }

    inline float cross(const CVector2f& rhs) const { return (x * rhs.y) - (y * rhs.x); }
    inline float dot(const CVector2f& rhs) const
    {
#if __SSE__
        TVectorUnion result;
#if __SSE4_1__
        if (cpuFeatures().SSE41 || cpuFeatures().SSE42)
        {
            result.mVec128 = _mm_dp_ps(mVec128, rhs.mVec128, 0x31);
            return result.v[0];
        }
#endif
        result.mVec128 = _mm_mul_ps(mVec128, rhs.mVec128);
        return result.v[0] + result.v[1];
#else
        return (x * rhs.x) + (y * rhs.y);
#endif
    }
    inline float magSquared() const
    {
#if __SSE__
        TVectorUnion result;
#if __SSE4_1__ || __SSE4_2__
        if (cpuFeatures().SSE41 || cpuFeatures().SSE42)
        {
            result.mVec128 = _mm_dp_ps(mVec128, mVec128, 0x31);
            return result.v[0];
        }
#endif
        result.mVec128 = _mm_mul_ps(mVec128, mVec128);
        return result.v[0] + result.v[1];
#else
        return x * x + y * y;
#endif
    }
    inline float magnitude() const { return sqrtF(magSquared()); }

    inline void zeroOut()
    {
#if __SSE__
        mVec128 = _mm_xor_ps(mVec128, mVec128);
#else
        v[0] = 0.0;
        v[1] = 0.0;
        v[2] = 0.0;
        v[3] = 0.0;
#endif
    }

    inline void splat(float xy)
    {
#if __SSE__
        TVectorUnion splat = {{xy, xy, 0.0f, 0.0f}};
        mVec128 = splat.mVec128;
#else
        v[0] = xy;
        v[1] = xy;
        v[2] = 0.0f;
        v[3] = 0.0f;
#endif
    }

    static float getAngleDiff(const CVector2f& a, const CVector2f& b);

    static inline CVector2f lerp(const CVector2f& a, const CVector2f& b, float t) { return (a + (b - a) * t); }
    static inline CVector2f nlerp(const CVector2f& a, const CVector2f& b, float t) { return lerp(a, b, t).normalized(); }
    static CVector2f slerp(const CVector2f& a, const CVector2f& b, float t);

    inline bool isNormalized() const { return std::fabs(1.f - magSquared()) < 0.01f; }

    inline bool canBeNormalized() const { return !isNormalized(); }

    inline bool isZero() const { return magSquared() <= 1.1920929e-7f; }

    inline bool isEqu(const CVector2f& other, float epsilon = 1.1920929e-7f)
    {
        const CVector2f diffVec = other - *this;
        return (diffVec.x <= epsilon && diffVec.y <= epsilon);
    }

    inline float& operator[](size_t idx) { return (&x)[idx]; }
    inline const float& operator[](size_t idx) const { return (&x)[idx]; }

    static const CVector2f skOne;
    static const CVector2f skNegOne;
    static const CVector2f skZero;
};

static inline CVector2f operator+(float lhs, const CVector2f& rhs)
{
#if __SSE__
    TVectorUnion splat = {{lhs, lhs, 0.0f, 0.0f}};
    return CVector2f(_mm_add_ps(splat.mVec128, rhs.mVec128));
#else
    return CVector2f(lhs + rhs.x, lhs + rhs.y);
#endif
}

static inline CVector2f operator-(float lhs, const CVector2f& rhs)
{
#if __SSE__
    TVectorUnion splat = {{lhs, lhs, 0.0f, 0.0f}};
    return CVector2f(_mm_sub_ps(splat.mVec128, rhs.mVec128));
#else
    return CVector2f(lhs - rhs.x, lhs - rhs.y);
#endif
}

static inline CVector2f operator*(float lhs, const CVector2f& rhs)
{
#if __SSE__
    TVectorUnion splat = {{lhs, lhs, 0.0f, 0.0f}};
    return CVector2f(_mm_mul_ps(splat.mVec128, rhs.mVec128));
#else
    return CVector2f(lhs * rhs.x, lhs * rhs.y);
#endif
}

static inline CVector2f operator/(float lhs, const CVector2f& rhs)
{
#if __SSE__
    TVectorUnion splat = {{lhs, lhs, 0.0f, 0.0f}};
    return CVector2f(_mm_div_ps(splat.mVec128, rhs.mVec128));
#else
    return CVector2f(lhs / rhs.x, lhs / rhs.y);
#endif
}
}

#endif // CVECTOR2F_HPP
