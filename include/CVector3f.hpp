#ifndef CVECTOR3F_HPP
#define CVECTOR3F_HPP

#include "Global.hpp"
#include "Math.hpp"
#include "CVector2f.hpp"
#include "TVectorUnion.hpp"
#if ZE_ATHENA_TYPES
#include <Athena/IStreamReader.hpp>
#endif

#include <math.h>
#include <assert.h>

namespace Zeus
{
class alignas(16) CVector3f
{
#if __atdna__
    float clangVec __attribute__((__vector_size__(12)));
#endif
public:
    ZE_DECLARE_ALIGNED_ALLOCATOR();

    union
    {
        struct { float x, y, z; };
        float v[4];
#if __SSE__
        __m128 mVec128;
#elif __GEKKO_PS__
        ps128_t mVec128;
#endif
    };

    inline CVector3f() {zeroOut();}
#if __SSE__ || __GEKKO_PS__
    CVector3f(const __m128& mVec128) : mVec128(mVec128) {v[3] = 0.0f;}
#endif

#if ZE_ATHENA_TYPES
    CVector3f(const atVec3f& vec)
#if __SSE__ || __GEKKO_PS__
        : mVec128(vec.mVec128){}
#else
    {
        x = vec.vec[0], y = vec.vec[1], z = vec.vec[2], v[3] = 0.0f;
    }
#endif

    operator atVec3f()
    {
        atVec3f ret;
#if __SSE__
        ret.mVec128 = mVec128;
#else
        ret.vec = v;
#endif
        return ret;
    }
    operator atVec3f() const
    {
        atVec3f ret;
#if __SSE__
        ret.mVec128 = mVec128;
#else
        ret.vec = v;
#endif
        return ret;
    }

    void read(Athena::io::IStreamReader& input)
    {
        x = input.readFloat();
        y = input.readFloat();
        z = input.readFloat();
        v[3] = 0.0f;
    }
    CVector3f(Athena::io::IStreamReader& input) {read(input);}
#endif

    CVector3f(float xyz) {splat(xyz);}
    void assign(float x, float y, float z) {v[0] = x; v[1] = y; v[2] = z; v[3] = 0.0;}
    CVector3f(float x, float y, float z) {assign(x, y, z);}

    CVector3f(const CVector2f& other)
    {
        x = other.x;
        y = other.y;
        z = 0.0;
        v[3] = 0.0f;
    }

    inline CVector2f toVec2f() const
    {
#if __SSE__
        return CVector2f(mVec128);
#else
        return CVector2f(x, y);
#endif
    }

    inline bool operator ==(const CVector3f& rhs) const
    {return (x == rhs.x && y == rhs.y && z == rhs.z);}
    inline bool operator !=(const CVector3f& rhs) const
    {return !(*this == rhs);}
    inline CVector3f operator+(const CVector3f& rhs) const
    {
#if __SSE__
        return CVector3f(_mm_add_ps(mVec128, rhs.mVec128));
#elif __GEKKO_PS__
        return CVector3f(__mm_gekko_add_ps(mVec128, rhs.mVec128));
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
#elif __GEKKO_PS__
        return CVector3f(_mm_gekko_neg_ps(mVec128));
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
#if __SSE__ || __GEKKO_PS__
        TVectorUnion splat = {{val, val, val, 0.0}};
#endif
#if __SSE__
        return CVector3f(_mm_sub_ps(mVec128, splat.mVec128));
#elif __GEKKO_PS__
        return CVector3f(_mm_gekko_sub_ps(mVec128, splat.mVec128));
#else
        return CVector3f(x - val, y - val, z - val);
#endif
    }
    inline CVector3f operator*(float val) const
    {
#if __SSE__ || __GEKKO_PS__
        TVectorUnion splat = {{val, val, val, 0.0}};
#endif
#if __SSE__
        return CVector3f(_mm_mul_ps(mVec128, splat.mVec128));
#elif __GEKKO_PS__
        return CVector3f(_mm_gekko_mul_ps(mVec128, splat.mVec128));
#else
        return CVector3f(x * val, y * val, z * val);
#endif
    }
    inline CVector3f operator/(float val) const
    {
#if __SSE__ || __GEKKO_PS__
        TVectorUnion splat = {{val, val, val, 0.0}};
#endif
#if __SSE__
        return CVector3f(_mm_div_ps(mVec128, splat.mVec128));
#elif __GEKKO_PS__
        return CVector3f(_mm_gekko_div_ps(mVec128, splat.mVec128));
#else
        return CVector3f(x / val, y / val, z / val);
#endif
    }
    inline const CVector3f& operator +=(const CVector3f& rhs)
    {
#if __SSE__
        mVec128 = _mm_add_ps(mVec128, rhs.mVec128);
#elif __GEKKO_PS__
        mVec128 = _mm_gekko_add_ps(mVec128, rhs.mVec128);
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
        float mag = magnitude();
        mag = 1.0 / mag;
        *this *= mag;
    }
    inline CVector3f normalized() const
    {
        float mag = magnitude();
        mag = 1.0 / mag;
        return *this * mag;
    }
    inline CVector3f cross(const CVector3f& rhs) const
    { return CVector3f(y * rhs.z - z * rhs.y, z * rhs.x - x * rhs.z, x * rhs.y - y * rhs.x); }

    inline float dot(const CVector3f& rhs) const
    {
#if __SSE__
        TVectorUnion result;
#if __SSE4_1__ || __SSE4_2__
        if (cpuFeatures().SSE41 || cpuFeatures().SSE42)
        {
            result.mVec128 = _mm_dp_ps(mVec128, rhs.mVec128, 0xF1);
            return result.v[0];
        }
#endif
        result.mVec128 = _mm_mul_ps(mVec128, rhs.mVec128);
        return result.v[0] + result.v[1] + result.v[2];
#else
        return (x * rhs.x) + (y * rhs.y) + (z * rhs.z);
#endif
    }
    inline float magSquared() const
    {
#if __SSE__
        TVectorUnion result;
#if __SSE4_1__ || __SSE4_2__
        if (cpuFeatures().SSE41 || cpuFeatures().SSE42)
        {
            result.mVec128 = _mm_dp_ps(mVec128, mVec128, 0x71);
            return result.v[0];
        }
#endif

        result.mVec128 = _mm_mul_ps(mVec128, mVec128);
        return result.v[0] + result.v[1] + result.v[2];
#else
        return x*x + y*y + z*z;
#endif
    }
    inline float magnitude() const
    { return Math::sqrtF(magSquared()); }
    
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
    { return (a + (b - a) * t); }
    static inline CVector3f nlerp(const CVector3f& a, const CVector3f& b, float t)
    { return lerp(a, b, t).normalized(); }
    static CVector3f slerp(const CVector3f& a, const CVector3f& b, float t);
    //static CVector3f slerp(const CVector3f& a, const CVector3f& b, const CRelAngle& angle);

    inline bool canBeNormalized() const
    {
        const float epsilon = 1.1920929e-7f;
        if (fabs(x) >= epsilon || fabs(y) >= epsilon || fabs(z) >= epsilon)
            return true;
        return false;
    }

    inline bool isNormalized() const
    { return !canBeNormalized(); }

    inline bool isZero() const
    { return magSquared() <= 1.1920929e-7f; }

    inline void scaleToLength(float newLength)
    {
        float length = magSquared();
        if (length < 1.1920929e-7f)
        {
            x = newLength, y = 0.f, z = 0.f;
            return;
        }

        length = sqrt(length);
        float scalar = newLength / length;
        *this *= scalar;
    }

    inline CVector3f scaledToLength(float newLength) const
    {
        CVector3f v = *this;
        v.scaleToLength(newLength);
        return v;
    }

    inline bool isEqu(const CVector3f& other, float epsilon=1.1920929e-7f)
    {
        CVector3f diffVec = other - *this;
        return (diffVec.x <= epsilon && diffVec.y <= epsilon && diffVec.z <= epsilon);
    }

    inline float& operator[](size_t idx) {return (&x)[idx];}
    inline const float& operator[](size_t idx) const {return (&x)[idx];}

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
}

#endif // CVECTOR3F_HPP
