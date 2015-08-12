#ifndef CVECTOR3F_HPP
#define CVECTOR3F_HPP

#include "Global.hpp"
#include "CVector2f.hpp"
#include "TVectorUnion.hpp"
#include <Athena/IStreamReader.hpp>
#include <math.h>
#include <assert.h>

class ZE_ALIGN(16) CVector3f
{
public:
    ZE_DECLARE_ALIGNED_ALLOCATOR();
    
    inline CVector3f() {zeroOut();}
#if __SSE__
    CVector3f(const __m128& mVec128) : mVec128(mVec128) {v[3] = 0.0f;}
#endif
#if ZE_ATHENA_TYPES
    CVector3f(const atVec3f& vec)
#if __SSE__
        : mVec128(vec.mVec128){}
#else
    {
        x = vec.vec[0], y = vec.vec[1], z = vec.vec[2], v[3] = 0.0f;
    }
#endif
#endif
    CVector3f(float xyz) {splat(xyz);}
    CVector3f(float x, float y, float z) {v[0] = x; v[1] = y; v[2] = z; v[3] = 0.0;}
    CVector3f(Athena::io::IStreamReader& input)
    {
        x = input.readFloat();
        y = input.readFloat();
        z = input.readFloat();
        v[3] = 0.0f;
    }
    CVector3f(const CVector2f& other)
    {
        x = other.x;
        y = other.y;
        z = 0.0;
        v[3] = 0.0f;
    }

    inline bool operator ==(const CVector3f& rhs) const
    {return (x == rhs.x && y == rhs.y && z == rhs.z);}
    inline bool operator !=(const CVector3f& rhs) const
    {return !(*this == rhs);}
    inline CVector3f operator+(const CVector3f& rhs) const
    {
#if __SSE__
        return CVector3f(_mm_add_ps(mVec128, rhs.mVec128));
#else
        return CVector3f(x + rhs.x, y + rhs.y, z + rhs.z);
#endif
    }
    inline CVector3f operator-(const CVector3f& rhs) const
    {
#if __SSE__
        return CVector3f(_mm_sub_ps(mVec128, rhs.mVec128));
#else
        return CVector3f(x - rhs.x, y - rhs.y, z - rhs.z);
#endif
    }
    inline CVector3f operator-() const
    {
#if __SSE__
        return CVector3f(_mm_sub_ps(_mm_xor_ps(mVec128, mVec128), mVec128));
#else
        return CVector3f(-x, -y, -z);
#endif
    }
    inline CVector3f operator*(const CVector3f& rhs) const
    {
#if __SSE__
        return CVector3f(_mm_mul_ps(mVec128, rhs.mVec128));
#else
        return CVector3f(x * rhs.x, y * rhs.y, z * rhs.z);
#endif
    }
    inline CVector3f operator/(const CVector3f& rhs) const
    {
#if __SSE__
        return CVector3f(_mm_div_ps(mVec128, rhs.mVec128));
#else
        return CVector3f(x / rhs.x, y / rhs.y, z / rhs.z);
#endif
    }
    inline CVector3f operator+(float val) const
    {
#if __SSE__
        TVectorUnion splat = {{val, val, val, 0.0}};
        return CVector3f(_mm_add_ps(mVec128, splat.mVec128));
#else
        return CVector3f(x + val, y + val, z + val);
#endif
    }
    inline CVector3f operator-(float val) const
    {
#if __SSE__
        TVectorUnion splat = {{val, val, val, 0.0}};
        return CVector3f(_mm_sub_ps(mVec128, splat.mVec128));
#else
        return CVector3f(x - val, y - val, z - val);
#endif
    }
    inline CVector3f operator*(float val) const
    {
#if __SSE__
        TVectorUnion splat = {{val, val, val, 0.0}};
        return CVector3f(_mm_mul_ps(mVec128, splat.mVec128));
#else
        return CVector3f(x * val, y * val, z * val);
#endif
    }
    inline CVector3f operator/(float val) const
    {
#if __SSE__
        TVectorUnion splat = {{val, val, val, 0.0}};
        return CVector3f(_mm_div_ps(mVec128, splat.mVec128));
#else
        return CVector3f(x / val, y / val, z / val);
#endif
    }
    inline const CVector3f& operator +=(const CVector3f& rhs)
    {
#if __SSE__
        mVec128 = _mm_add_ps(mVec128, rhs.mVec128);
#else
        x += rhs.x; y += rhs.y; z += rhs.z;
#endif
        return *this;
    }
    inline const CVector3f& operator -=(const CVector3f& rhs)
    {
#if __SSE__
        mVec128 = _mm_sub_ps(mVec128, rhs.mVec128);
#else
        x -= rhs.x; y -= rhs.y; z -= rhs.z;
#endif
        return *this;
    }
    inline const CVector3f& operator *=(const CVector3f& rhs)
    {
#if __SSE__
        mVec128 = _mm_mul_ps(mVec128, rhs.mVec128);
#else
        x *= rhs.x; y *= rhs.y; z *= rhs.z;
#endif
        return *this;
    }
    inline const CVector3f& operator /=(const CVector3f& rhs)
    {
#if __SSE__
        mVec128 = _mm_div_ps(mVec128, rhs.mVec128);
#else
        x /= rhs.x; y /= rhs.y; z /= rhs.z;
#endif
        return *this;
    }
    inline void normalize()
    {
        float mag = length();
        assert(mag != 0.0);
        mag = 1.0 / mag;
        *this *= mag;
    }
    inline CVector3f normalized() const
    {
        float mag = length();
        assert(mag != 0.0);
        mag = 1.0 / mag;
        return *this * mag;
    }
    inline CVector3f cross(const CVector3f& rhs) const
    {
        return CVector3f(y * rhs.z - z * rhs.y, z * rhs.x - x * rhs.z, x * rhs.y - y * rhs.x);
    }
    inline float     dot(const CVector3f& rhs) const
    {
#if __SSE4_1__
        TVectorUnion result;
        result.mVec128 = _mm_dp_ps(mVec128, rhs.mVec128, 0x71);
        return result.v[0];
#elif __SSE__
        TVectorUnion result;
        result.mVec128 = _mm_mul_ps(mVec128, rhs.mVec128);
        return result.v[0] + result.v[1] + result.v[2];
#else
        return (x * rhs.x) + (y * rhs.y) + (z * rhs.z);
#endif
    }
    inline float     lengthSquared() const
    {
#if __SSE4_1__
        TVectorUnion result;
        result.mVec128 = _mm_dp_ps(mVec128, mVec128, 0x71);
        return result.v[0];
#elif __SSE__
        TVectorUnion result;
        result.mVec128 = _mm_mul_ps(mVec128, mVec128);
        return result.v[0] + result.v[1] + result.v[2];
#else
        return x*x + y*y + z*z;
#endif
    }
    inline float     length() const
    {
        return sqrtf(lengthSquared());
    }
    
    inline void zeroOut()
    {
#if __SSE__
        mVec128 = _mm_xor_ps(mVec128, mVec128);
#else
        v[0] = 0.0; v[1] = 0.0; v[2] = 0.0; v[3] = 0.0;
#endif
    }
    
    inline void splat(float xyz)
    {
#if __SSE__
        TVectorUnion splat = {{xyz, xyz, xyz, 0.0f}};
        mVec128 = splat.mVec128;
#else
        v[0] = xyz; v[1] = xyz; v[2] = xyz; v[3] = 0.0f;
#endif
    }
    
    static float getAngleDiff(const CVector3f& a, const CVector3f& b);

    static inline CVector3f lerp(const CVector3f& a, const CVector3f& b, float t)
    {
        return (a + (b - a) * t);
    }
    static inline CVector3f nlerp(const CVector3f& a, const CVector3f& b, float t)
    {
        return lerp(a, b, t).normalized();
    }
    static CVector3f slerp(const CVector3f& a, const CVector3f& b, float t);

    inline bool isNormalized(float thresh = 0.0001f) const
    {
        return (length() > thresh);
    }

    inline float& operator[](size_t idx) {return (&x)[idx];}
    inline const float& operator[](size_t idx) const {return (&x)[idx];}


    union
    {
        struct
        {
            float x, y, z;
        };
        float v[4];
#if __SSE__
        __m128 mVec128;
#endif
    };

    static const CVector3f skOne;
    static const CVector3f skNegOne;
    static const CVector3f skZero;
};


static inline CVector3f operator+(float lhs, const CVector3f& rhs)
{
#if __SSE__
    TVectorUnion splat = {{lhs, lhs, lhs, 0.0}};
    return CVector3f(_mm_add_ps(splat.mVec128, rhs.mVec128));
#else
    return CVector3f(lhs + rhs.x, lhs + rhs.y, lhs + rhs.z);
#endif
}

static inline CVector3f operator-(float lhs, const CVector3f& rhs)
{
#if __SSE__
    TVectorUnion splat = {{lhs, lhs, lhs, 0.0}};
    return CVector3f(_mm_sub_ps(splat.mVec128, rhs.mVec128));
#else
    return CVector3f(lhs - rhs.x, lhs - rhs.y, lhs - rhs.z);
#endif
}

static inline CVector3f operator*(float lhs, const CVector3f& rhs)
{
#if __SSE__
    TVectorUnion splat = {{lhs, lhs, lhs, 0.0}};
    return CVector3f(_mm_mul_ps(splat.mVec128, rhs.mVec128));
#else
    return CVector3f(lhs * rhs.x, lhs * rhs.y, lhs * rhs.z);
#endif
}

static inline CVector3f operator/(float lhs, const CVector3f& rhs)
{
#if __SSE__
    TVectorUnion splat = {{lhs, lhs, lhs, 0.0}};
    return CVector3f(_mm_div_ps(splat.mVec128, rhs.mVec128));
#else
    return CVector3f(lhs / rhs.x, lhs / rhs.y, lhs / rhs.z);
#endif
}

#endif // CVECTOR3F_HPP
