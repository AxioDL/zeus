#ifndef CCOLOR_HPP
#define CCOLOR_HPP

#include "MathLib.hpp"
#include <iostream>

typedef union _RGBA32
{
    struct
    {
        unsigned char r, g, b, a;
    };
    unsigned int rgba;
} RGBA32;

class ZE_ALIGN(16) CColor
{
public:
    ZE_DECLARE_ALIGNED_ALLOCATOR();

#if __SSE__
    CColor(const __m128& mVec128) : mVec128(mVec128) {}
#endif

    CColor() : r(1.0f), g(1.0f), b(1.0f), a(1.0f) {}
    CColor(float rgb, float a = 1.0) { splat(rgb, a); }
    CColor(float r, float g, float b, float a = 1.0f) {v[0] = r; v[1] = g; v[2] = b; v[3] = a; }
    CColor(Athena::io::IStreamReader& reader) {readRGBA(reader);}

    inline void readRGBA(Athena::io::IStreamReader& reader)
    {
        r = reader.readFloat();
        g = reader.readFloat();
        b = reader.readFloat();
        a = reader.readFloat();
    }
    inline void readBGRA(Athena::io::IStreamReader& reader)
    {
        b = reader.readFloat();
        g = reader.readFloat();
        r = reader.readFloat();
        a = reader.readFloat();
    }
    inline bool operator==(const CColor& rhs) const
    { return (r == rhs.r && g == rhs.g && b == rhs.b && a == rhs.a); }
    inline bool operator!=(const CColor& rhs) const
    { return !(*this == rhs); }
    inline CColor operator+(const CColor& rhs) const
    {
#if __SSE__
        return CColor(_mm_add_ps(mVec128, rhs.mVec128));
#else
        return CColor(r + rhs.r, g + rhs.g, b + rhs.b, a + rhs.a);
#endif
    }
    inline CColor operator-(const CColor& rhs) const
    {
#if __SSE__
        return CColor(_mm_sub_ps(mVec128, rhs.mVec128));
#else
        return CColor(r - rhs.r, g - rhs.g, b - rhs.b, a - rhs.a);
#endif
    }
    inline CColor operator*(const CColor& rhs) const
    {
#if __SSE__
        return CColor(_mm_mul_ps(mVec128, rhs.mVec128));
#else
        return CColor(r * rhs.r, g * rhs.g, b * rhs.b, a * rhs.a);
#endif
    }
    inline CColor operator/(const CColor& rhs) const
    {
#if __SSE__
        return CColor(_mm_div_ps(mVec128, rhs.mVec128));
#else
        return CColor(r / rhs.r, g / rhs.g, b / rhs.b, a / rhs.a);
#endif
    }
    inline CColor operator+(float val) const
    {
#if __SSE__
        TVectorUnion splat = {{val, val, val, val}};
        return CColor(_mm_add_ps(mVec128, splat.mVec128));
#else
        return CColor(r + val, g + val, b + val, a + val);
#endif
    }
    inline CColor operator-(float val) const
    {
#if __SSE__
        TVectorUnion splat = {{val, val, val, val}};
        return CColor(_mm_sub_ps(mVec128, splat.mVec128));
#else
        return CColor(r - val, g - val, b - val, a - val);
#endif
    }
    inline CColor operator*(float val) const
    {
#if __SSE__
        TVectorUnion splat = {{val, val, val, val}};
        return CColor(_mm_mul_ps(mVec128, splat.mVec128));
#else
        return CColor(r * val, g * val, b * val, a * val);
#endif
    }
    inline CColor operator/(float val) const
    {
#if __SSE__
        TVectorUnion splat = {{val, val, val, val}};
        return CColor(_mm_div_ps(mVec128, splat.mVec128));
#else
        return CColor(r / val, g / val, b / val, a / val);
#endif
    }
    inline const CColor& operator+=(const CColor& rhs)
    {
#if __SSE__
        mVec128 = _mm_add_ps(mVec128, rhs.mVec128);
#else
        r += rhs.r; g += rhs.g; b += rhs.b; a += rhs.a;
#endif
        return *this;
    }
    inline const CColor& operator-=(const CColor& rhs)
    {
#if __SSE__
        mVec128 = _mm_sub_ps(mVec128, rhs.mVec128);
#else
        r -= rhs.r; g -= rhs.g; b -= rhs.b; a -= rhs.a;
#endif
        return *this;
    }
    inline const CColor& operator *=(const CColor& rhs)
    {
#if __SSE__
        mVec128 = _mm_mul_ps(mVec128, rhs.mVec128);
#else
        r *= rhs.r; g *= rhs.g; b *= rhs.b; a *= rhs.a;
#endif
        return *this;
    }
    inline const CColor& operator /=(const CColor& rhs)
    {
#if __SSE__
        mVec128 = _mm_div_ps(mVec128, rhs.mVec128);
#else
        r /= rhs.r; g /= rhs.g; b /= rhs.b; a /= rhs.a;
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
    inline CColor normalized()
    {
        float mag = length();
        assert(mag != 0.0);
        mag = 1.0 / mag;
        return *this * mag;
    }
    inline float lengthSquared() const
    {
#if __SSE4_1__
        TVectorUnion result;
        result.mVec128 = _mm_dp_ps(mVec128, mVec128, 0xF1);
        return result.v[0];
#elif __SSE__
        TVectorUnion result;
        result.mVec128 = _mm_mul_ps(mVec128, mVec128);
        return result.v[0] + result.v[1] + result.v[2] + result.v[3];
#else
        return r * r + g * g + b * b + a * a;
#endif
    }
    inline float length() const
    {
        return sqrtf(lengthSquared());
    }
    static inline CColor lerp(const CColor& a, const CColor& b, float t)
    {
        return (a + (b - a) * t);
    }
    static inline CColor nlerp(const CColor& a, const CColor& b, float t)
    {
        return lerp(a, b, t).normalized();
    }
    inline float& operator[](const size_t& idx) {return (&r)[idx];}
    inline const float& operator[](const size_t& idx) const { return (&r)[idx]; }
    inline void splat(float rgb, float a)
    {
#if __SSE__
        TVectorUnion splat = {{rgb, rgb, rgb, a}};
        mVec128 = splat.mVec128;
#else
        v[0] = rgb; v[1] = rgb; v[2] = rgb; v[3] = a;
#endif
    }

    union
    {
        struct
        {
            float r, g, b, a;
        };
        float v[4];
#if __SSE__
        __m128 mVec128;
#endif
    };

    void fromRGBA8(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
    {
        this->r = r / 255.f;
        this->g = g / 255.f;
        this->b = b / 255.f;
        this->a = a / 255.f;
    }

    void fromRGBA32(unsigned int rgba)
    {
        static RGBA32 tmp;
        tmp.rgba = rgba;
        fromRGBA8(tmp.r, tmp.g, tmp.b, tmp.a);
    }
};

static inline CColor operator+(float lhs, const CColor& rhs)
{
#if __SSE__
    TVectorUnion splat = {{lhs, lhs, lhs, lhs}};
    return CColor(_mm_add_ps(splat.mVec128, rhs.mVec128));
#else
    return CColor(lhs + rhs.r, lhs + rhs.g, lhs + rhs.b, lhs + rhs.a);
#endif
}

static inline CColor operator-(float lhs, const CColor& rhs)
{
#if __SSE__
    TVectorUnion splat = {{lhs, lhs, lhs, lhs}};
    return CColor(_mm_sub_ps(splat.mVec128, rhs.mVec128));
#else
    return CColor(lhs - rhs.r, lhs - rhs.g, lhs - rhs.b, lhs - rhs.a);
#endif
}

static inline CColor operator*(float lhs, const CColor& rhs)
{
#if __SSE__
    TVectorUnion splat = {{lhs, lhs, lhs, lhs}};
    return CColor(_mm_mul_ps(splat.mVec128, rhs.mVec128));
#else
    return CColor(lhs * rhs.r, lhs * rhs.g, lhs * rhs.b, lhs * rhs.a);
#endif
}

static inline CColor operator/(float lhs, const CColor& rhs)
{
#if __SSE__
    TVectorUnion splat = {{lhs, lhs, lhs, lhs}};
    return CColor(_mm_div_ps(splat.mVec128, rhs.mVec128));
#else
    return CColor(lhs / rhs.r, lhs / rhs.g, lhs / rhs.b, lhs / rhs.a);
#endif
}

#endif // CCOLOR_HPP
