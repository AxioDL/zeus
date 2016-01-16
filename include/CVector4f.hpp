#ifndef CVECTOR4F_HPP
#define CVECTOR4F_HPP

#include "Global.hpp"
#include "TVectorUnion.hpp"
#include "CVector3f.hpp"
#if ZE_ATHENA_TYPES
#include <Athena/IStreamReader.hpp>
#endif
#include <math.h>
#include <float.h>
#include <assert.h>

namespace Zeus
{
class CColor;
class alignas(16) CVector4f
{
#if __atdna__
    float clangVec __attribute__((__vector_size__(16)));
#endif
public:
    ZE_DECLARE_ALIGNED_ALLOCATOR();
    union
    {
        struct
        {
            float x, y, z, w;
        };
        float v[4];
#if __SSE__
        __m128 mVec128;
#endif
    };

    inline CVector4f() {zeroOut();}
#if __SSE__
    CVector4f(const __m128& mVec128) : mVec128(mVec128) {}
#endif
#if ZE_ATHENA_TYPES
    CVector4f(const atVec4f& vec)
#if __SSE__
        : mVec128(vec.mVec128){}
#else
    {
        x = vec.vec[0], y = vec.vec[1], z = vec.vec[2], w = vec.vec[3];
    }
#endif

    operator atVec4f()
    {
        atVec4f ret;
#if __SSE__
        ret.mVec128 = mVec128;
#else
        ret.vec = v;
#endif
        return ret;
    }
    operator atVec4f() const
    {
        atVec4f ret;
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
        w = input.readFloat();
    }

    CVector4f(Athena::io::IStreamReader& input)
    { read(input); }

#endif

    CVector4f(float xyzw) {splat(xyzw);}
    void assign(float x, float y, float z, float w) {v[0] = x; v[1] = y; v[2] = z; v[3] = w;}
    CVector4f(float x, float y, float z, float w) {assign(x, y, z, w);}
    CVector4f(const CColor& other);

    CVector4f(const CVector3f& other)
    {
        x = other.x;
        y = other.y;
        z = other.z;
        w = 1.0f;
    }

    CVector4f& operator=(const CColor& other);
    inline bool operator ==(const CVector4f& rhs) const
    {
#if __SSE__
        TVectorUnion vec;
        vec.mVec128 = _mm_cmpeq_ps(mVec128, rhs.mVec128);
        return (vec.v[0] != 0 && vec.v[1] != 0 && vec.v[2] != 0 && vec.v[3] != 0);
#else
        return (x == rhs.x && y == rhs.y && z == rhs.z && w == rhs.w);
#endif
    }
    inline bool operator !=(const CVector4f& rhs) const
    {
#if __SSE__
        TVectorUnion vec;
        vec.mVec128 = _mm_cmpneq_ps(mVec128, rhs.mVec128);
        return (vec.v[0] != 0 && vec.v[1] != 0 && vec.v[2] != 0 && vec.v[3] != 0);
#else
        return !(*this == rhs);
#endif
    }
    inline bool operator <(const CVector4f& rhs) const
    {
#if __SSE__
        TVectorUnion vec;
        vec.mVec128 = _mm_cmplt_ps(mVec128, rhs.mVec128);
        return (vec.v[0] != 0 || vec.v[1] != 0 || vec.v[2] != 0 || vec.v[3] != 0);
#else
        return (x < rhs.x || y < rhs.y || z < rhs.z || w < rhs.w);
#endif
    }
    inline bool operator <=(const CVector4f& rhs) const
    {
#if __SSE__
        TVectorUnion vec;
        vec.mVec128 = _mm_cmple_ps(mVec128, rhs.mVec128);
        return (vec.v[0] != 0 || vec.v[1] != 0 || vec.v[2] != 0 || vec.v[3] != 0);
#else
        return (x <= rhs.x || y <= rhs.y || z <= rhs.z || w <= rhs.w);
#endif
    }
    inline bool operator >(const CVector4f& rhs) const
    {
#if __SSE__
        TVectorUnion vec;
        vec.mVec128 = _mm_cmpgt_ps(mVec128, rhs.mVec128);
        return (vec.v[0] != 0 || vec.v[1] != 0 || vec.v[2] != 0 || vec.v[3] != 0);
#else
        return (x > rhs.x || y > rhs.y || z > rhs.z || w > rhs.w);
#endif
    }
    inline bool operator >=(const CVector4f& rhs) const
    {
#if __SSE__
        TVectorUnion vec;
        vec.mVec128 = _mm_cmpge_ps(mVec128, rhs.mVec128);
        return (vec.v[0] != 0 || vec.v[1] != 0 || vec.v[2] != 0 || vec.v[3] != 0);
#else
        return (x >= rhs.x || y >= rhs.y || z >= rhs.z || w >= rhs.w);
#endif
    }
    inline CVector4f operator+(const CVector4f& rhs) const
    {
#if __SSE__
        return CVector4f(_mm_add_ps(mVec128, rhs.mVec128));
#else
        return CVector4f(x + rhs.x, y + rhs.y, z + rhs.z, w + rhs.w);
#endif
    }
    inline CVector4f operator-(const CVector4f& rhs) const
    {
#if __SSE__
        return CVector4f(_mm_sub_ps(mVec128, rhs.mVec128));
#else
        return CVector4f(x - rhs.x, y - rhs.y, z - rhs.z, w - rhs.w);
#endif
    }
    inline CVector4f operator-() const
    {
#if __SSE__
        return CVector4f(_mm_sub_ps(_mm_xor_ps(mVec128, mVec128), mVec128));
#else
        return CVector4f(-x, -y, -z, -w);
#endif
    }
    inline CVector4f operator*(const CVector4f& rhs) const
    {
#if __SSE__
        return CVector4f(_mm_mul_ps(mVec128, rhs.mVec128));
#else
        return CVector4f(x * rhs.x, y * rhs.y, z * rhs.z, w * rhs.w);
#endif
    }
    inline CVector4f operator/(const CVector4f& rhs) const
    {
#if __SSE__
        return CVector4f(_mm_div_ps(mVec128, rhs.mVec128));
#else
        return CVector4f(x / rhs.x, y / rhs.y, z / rhs.z, w / rhs.w);
#endif
    }
    inline CVector4f operator+(float val) const
    {
#if __SSE__
        TVectorUnion splat = {{val, val, val, val}};
        return CVector4f(_mm_add_ps(mVec128, splat.mVec128));
#else
        return CVector4f(x + val, y + val, z + val, w + val);
#endif
    }
    inline CVector4f operator-(float val) const
    {
#if __SSE__
        TVectorUnion splat = {{val, val, val, val}};
        return CVector4f(_mm_sub_ps(mVec128, splat.mVec128));
#else
        return CVector4f(x - val, y - val, z - val, w - val);
#endif
    }
    inline CVector4f operator*(float val) const
    {
#if __SSE__
        TVectorUnion splat = {{val, val, val, val}};
        return CVector4f(_mm_mul_ps(mVec128, splat.mVec128));
#else
        return CVector4f(x * val, y * val, z * val, w * val);
#endif
    }
    inline CVector4f operator/(float val) const
    {
#if __SSE__
        TVectorUnion splat = {{val, val, val, val}};
        return CVector4f(_mm_div_ps(mVec128, splat.mVec128));
#else
        return CVector4f(x / val, y / val, z / val, w / val);
#endif
    }
    inline const CVector4f& operator +=(const CVector4f& rhs)
    {
#if __SSE__
        mVec128 = _mm_add_ps(mVec128, rhs.mVec128);
#else
        x += rhs.x; y += rhs.y; z += rhs.z; w += rhs.w;
#endif
        return *this;
    }
    inline const CVector4f& operator -=(const CVector4f& rhs)
    {
#if __SSE__
        mVec128 = _mm_sub_ps(mVec128, rhs.mVec128);
#else
        x -= rhs.x; y -= rhs.y; z -= rhs.z; w -= rhs.w;
#endif
        return *this;
    }
    inline const CVector4f& operator *=(const CVector4f& rhs)
    {
#if __SSE__
        mVec128 = _mm_mul_ps(mVec128, rhs.mVec128);
#else
        x *= rhs.x; y *= rhs.y; z *= rhs.z; w *= rhs.w;
#endif
        return *this;
    }
    inline const CVector4f& operator /=(const CVector4f& rhs)
    {
#if __SSE__
        mVec128 = _mm_div_ps(mVec128, rhs.mVec128);
#else
        x /= rhs.x; y /= rhs.y; z /= rhs.z; w /= rhs.w;
#endif
        return *this;
    }
    inline void normalize()
    {
        float mag = magnitude();
        mag = 1.0 / mag;
        *this *= mag;
    }
    inline CVector4f normalized() const
    {
        float mag = magnitude();
        mag = 1.0 / mag;
        return *this * mag;
    }

    inline float dot(const CVector4f& rhs) const
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
        return result.v[0] + result.v[1] + result.v[2] + result.v[3];
#else
        return (x * rhs.x) + (y * rhs.y) + (z * rhs.z) + (w * rhs.w);
#endif
    }
    inline float magSquared() const
    {
#if __SSE__
        TVectorUnion result;
#if __SSE4_1__ || __SSE4_2__
        if (cpuFeatures().SSE41 || cpuFeatures().SSE42)
        {
            result.mVec128 = _mm_dp_ps(mVec128, mVec128, 0xF1);
            return result.v[0];
        }
#endif
        result.mVec128 = _mm_mul_ps(mVec128, mVec128);
        return result.v[0] + result.v[1] + result.v[2];
#else
        return x*x + y*y + z*z + w*w;
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

    inline void splat(float xyzw)
    {
#if __SSE__
        TVectorUnion splat = {{xyzw, xyzw, xyzw, xyzw}};
        mVec128 = splat.mVec128;
#else
        v[0] = xyz; v[1] = xyz; v[2] = xyz; v[3] = xyzw;
#endif
    }

    static inline CVector4f lerp(const CVector4f& a, const CVector4f& b, float t)
    {
        return (a + (b - a) * t);
    }
    static inline CVector4f nlerp(const CVector4f& a, const CVector4f& b, float t)
    {
        return lerp(a, b, t).normalized();
    }

    inline bool canBeNormalized() const
    {
        const float epsilon = 1.1920929e-7f;
        if (fabs(x) >= epsilon || fabs(y) >= epsilon || fabs(z) >= epsilon || fabs(w) >= epsilon)
            return true;
        return false;
    }

    inline bool isNormalized() const
    { return !canBeNormalized(); }

    inline float& operator[](size_t idx) {return (&x)[idx];}
    inline const float& operator[](size_t idx) const {return (&x)[idx];}


    static const CVector4f skOne;
    static const CVector4f skNegOne;
    static const CVector4f skZero;
};


static inline CVector4f operator+(float lhs, const CVector4f& rhs)
{
#if __SSE__
    TVectorUnion splat = {{lhs, lhs, lhs, lhs}};
    return CVector4f(_mm_add_ps(splat.mVec128, rhs.mVec128));
#else
    return CVector4f(lhs + rhs.x, lhs + rhs.y, lhs + rhs.z, lhs + rhs.w);
#endif
}

static inline CVector4f operator-(float lhs, const CVector4f& rhs)
{
#if __SSE__
    TVectorUnion splat = {{lhs, lhs, lhs, lhs}};
    return CVector4f(_mm_sub_ps(splat.mVec128, rhs.mVec128));
#else
    return CVector4f(lhs - rhs.x, lhs - rhs.y, lhs - rhs.z, lhs - rhs.w);
#endif
}

static inline CVector4f operator*(float lhs, const CVector4f& rhs)
{
#if __SSE__
    TVectorUnion splat = {{lhs, lhs, lhs, lhs}};
    return CVector4f(_mm_mul_ps(splat.mVec128, rhs.mVec128));
#else
    return CVector4f(lhs * rhs.x, lhs * rhs.y, lhs * rhs.z, lhs * rhs.w);
#endif
}

static inline CVector4f operator/(float lhs, const CVector4f& rhs)
{
#if __SSE__
    TVectorUnion splat = {{lhs, lhs, lhs, lhs}};
    return CVector4f(_mm_div_ps(splat.mVec128, rhs.mVec128));
#else
    return CVector4f(lhs / rhs.x, lhs / rhs.y, lhs / rhs.z, lhs / rhs.w);
#endif
}
}

#endif // CVECTOR4F_HPP
