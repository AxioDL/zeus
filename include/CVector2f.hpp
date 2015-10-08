#ifndef CVECTOR2f_HPP
#define CVECTOR2f_HPP

#include "Global.hpp"
#include "TVectorUnion.hpp"

#include <Athena/IStreamReader.hpp>
#include <math.h>
#include <assert.h>

namespace Zeus
{
class ZE_ALIGN(16) CVector2f
{
    public:
    ZE_DECLARE_ALIGNED_ALLOCATOR();

    inline CVector2f() {zeroOut();}
#if __SSE__
    CVector2f(const __m128& mVec128) : mVec128(mVec128) {v[2] = 0.0f; v[3] = 0.0f;}
#endif
#if ZE_ATHENA_TYPES
    CVector2f(const atVec2f& vec)
#if __SSE__
        : mVec128(vec.mVec128){}
#else
    {
        x = vec.vec[0], y = vec.vec[1], v[2] = 0.0f, v[3] = 0.0f;
    }
#endif
#endif
    CVector2f(float xy) {splat(xy);}
    CVector2f(float x, float y) {v[0] = x; v[1] = y; v[2] = 0; v[3] = 0.0;}
    CVector2f(Athena::io::IStreamReader& input)
    {
        x = input.readFloat();
        y = input.readFloat();
        v[2] = 0.0f;
        v[3] = 0.0f;
    }

    inline bool operator ==(const CVector2f& rhs) const
    {return (x == rhs.x && y == rhs.y);}
    inline bool operator !=(const CVector2f& rhs) const
    {return !(*this == rhs);}
    inline bool operator <(const CVector2f& rhs) const
    {
#if __SSE__
        TVectorUnion vec;
        vec.mVec128 = _mm_cmplt_ps(mVec128, rhs.mVec128);
        return (vec.v[0] != 0 || vec.v[1] != 0);
#else
        return (x < rhs.x || y < rhs.y);
#endif
    }
    inline bool operator <=(const CVector2f& rhs) const
    {
#if __SSE__
        TVectorUnion vec;
        vec.mVec128 = _mm_cmple_ps(mVec128, rhs.mVec128);
        return (vec.v[0] != 0 || vec.v[1] != 0);
#else
        return (x <= rhs.x || y <= rhs.y);
#endif
    }
    inline bool operator >(const CVector2f& rhs) const
    {
#if __SSE__
        TVectorUnion vec;
        vec.mVec128 = _mm_cmpgt_ps(mVec128, rhs.mVec128);
        return (vec.v[0] != 0 || vec.v[1] != 0);
#else
        return (x > rhs.x || y > rhs.y);
#endif
    }
    inline bool operator >=(const CVector2f& rhs) const
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
        TVectorUnion splat = {{val, val, 0.0, 0.0}};
        return CVector2f(_mm_add_ps(mVec128, splat.mVec128));
#else
        return CVector2f(x + val, y + val);
#endif
    }
    inline CVector2f operator-(float val) const
    {
#if __SSE__
        TVectorUnion splat = {{val, val, 0.0, 0.0}};
        return CVector2f(_mm_sub_ps(mVec128, splat.mVec128));
#else
        return CVector2f(x - val, y - val);
#endif
    }
    inline CVector2f operator*(float val) const
    {
#if __SSE__
        TVectorUnion splat = {{val, val, 0.0, 0.0}};
        return CVector2f(_mm_mul_ps(mVec128, splat.mVec128));
#else
        return CVector2f(x * val, y * val);
#endif
    }
    inline CVector2f operator/(float val) const
    {
#if __SSE__
        TVectorUnion splat = {{val, val, val, 0.0}};
        return CVector2f(_mm_div_ps(mVec128, splat.mVec128));
#else
        return CVector2f(x / val, y / val);
#endif
    }
    inline const CVector2f& operator +=(const CVector2f& rhs)
    {
#if __SSE__
        mVec128 = _mm_add_ps(mVec128, rhs.mVec128);
#else
        x += rhs.x; y += rhs.y;
#endif
        return *this;
    }
    inline const CVector2f& operator -=(const CVector2f& rhs)
    {
#if __SSE__
        mVec128 = _mm_sub_ps(mVec128, rhs.mVec128);
#else
        x -= rhs.x; y -= rhs.y;
#endif
        return *this;
    }
    inline const CVector2f& operator *=(const CVector2f& rhs)
    {
#if __SSE__
        mVec128 = _mm_mul_ps(mVec128, rhs.mVec128);
#else
        x *= rhs.x; y *= rhs.y;
#endif
        return *this;
    }
    inline const CVector2f& operator /=(const CVector2f& rhs)
    {
#if __SSE__
        mVec128 = _mm_div_ps(mVec128, rhs.mVec128);
#else
        x /= rhs.x; y /= rhs.y;
#endif
        return *this;
    }
    inline void normalize()
    {
        float mag = magnitude();
        if (mag > 1e-6f)
        {
            mag = 1.0 / mag;
            *this *= mag;
        }
        else
            zeroOut();
    }

    inline CVector2f normalized() const
    {
        float mag = magnitude();
        if (mag > 1e-6f)
        {
            mag = 1.0 / mag;
            return *this * mag;
        }
        return {0, 0};
    }

    inline float cross(const CVector2f& rhs) const
    {
        return (x * rhs.y) - (y * rhs.x);
    }
    inline float dot(const CVector2f& rhs) const
    {
#if __SSE4_1__
        TVectorUnion result;
        result.mVec128 = _mm_dp_ps(mVec128, rhs.mVec128, 0x31);
        return result.v[0];
#elif __SSE__
        TVectorUnion result;
        result.mVec128 = _mm_mul_ps(mVec128, rhs.mVec128);
        return result.v[0] + result.v[1];
#else
        return (x * rhs.x) + (y * rhs.y);
#endif
    }
    inline float magSquared() const
    {
#if __SSE4_1__
        TVectorUnion result;
        result.mVec128 = _mm_dp_ps(mVec128, mVec128, 0x31);
        return result.v[0];
#elif __SSE__
        TVectorUnion result;
        result.mVec128 = _mm_mul_ps(mVec128, mVec128);
        return result.v[0] + result.v[1];
#else
        return x*x + y*y;
#endif
    }
    inline float magnitude() const
    {
        return sqrtf(magSquared());
    }

    inline void zeroOut()
    {
#if __SSE__
        mVec128 = _mm_xor_ps(mVec128, mVec128);
#else
        v[0] = 0.0; v[1] = 0.0; v[2] = 0.0; v[3] = 0.0;
#endif
    }

    inline void splat(float xy)
    {
#if __SSE__
        TVectorUnion splat = {{xy, xy, 0.0f, 0.0f}};
        mVec128 = splat.mVec128;
#else
        v[0] = xy; v[1] = xy; v[2] = 0.0f; v[3] = 0.0f;
#endif
    }

    static float getAngleDiff(const CVector2f& a, const CVector2f& b);

    static inline CVector2f lerp(const CVector2f& a, const CVector2f& b, float t)
    {
        return (a + (b - a) * t);
    }
    static inline CVector2f nlerp(const CVector2f& a, const CVector2f& b, float t)
    {
        return lerp(a, b, t).normalized();
    }
    static CVector2f slerp(const CVector2f& a, const CVector2f& b, float t);

    inline bool isNormalized(float thresh = 1e-5f) const
    {
        return (fabs(1.0f - magSquared()) <= thresh);
    }

    inline bool canBeNormalized()
    {
        return !isNormalized();
    }

    inline float& operator[](size_t idx) {return (&x)[idx];}
    inline const float& operator[](size_t idx) const {return (&x)[idx];}


    union
    {
        struct
        {
            float x, y;
        };
        float v[4];
#if __SSE__
        __m128 mVec128;
#endif
    };

    static const CVector2f skOne;
    static const CVector2f skNegOne;
    static const CVector2f skZero;
};


static inline CVector2f operator+(float lhs, const CVector2f& rhs)
{
#if __SSE__
    TVectorUnion splat = {{lhs, lhs, 0.0, 0.0}};
    return CVector2f(_mm_add_ps(splat.mVec128, rhs.mVec128));
#else
    return CVector2f(lhs + rhs.x, lhs + rhs.y);
#endif
}

static inline CVector2f operator-(float lhs, const CVector2f& rhs)
{
#if __SSE__
    TVectorUnion splat = {{lhs, lhs, 0.0, 0.0}};
    return CVector2f(_mm_sub_ps(splat.mVec128, rhs.mVec128));
#else
    return CVector2f(lhs - rhs.x, lhs - rhs.y);
#endif
}

static inline CVector2f operator*(float lhs, const CVector2f& rhs)
{
#if __SSE__
    TVectorUnion splat = {{lhs, lhs, 0.0, 0.0}};
    return CVector2f(_mm_mul_ps(splat.mVec128, rhs.mVec128));
#else
    return CVector2f(lhs * rhs.x, lhs * rhs.y);
#endif
}

static inline CVector2f operator/(float lhs, const CVector2f& rhs)
{
#if __SSE__
    TVectorUnion splat = {{lhs, lhs, 0.0, 0.0}};
    return CVector2f(_mm_div_ps(splat.mVec128, rhs.mVec128));
#else
    return CVector2f(lhs / rhs.x, lhs / rhs.y);
#endif
}
}

#endif // CVECTOR2F_HPP
