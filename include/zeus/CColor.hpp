#ifndef CCOLOR_HPP
#define CCOLOR_HPP

#include "Global.hpp"
#include "zeus/Math.hpp"
#include "TVectorUnion.hpp"
#if ZE_ATHENA_TYPES
#include <athena/FileReader.hpp>
#include <athena/FileWriter.hpp>
#endif
#include <iostream>
#include <assert.h>

#undef min
#undef max

#if BYTE_ORDER == __ORDER_LITTLE_ENDIAN__
#define COLOR(rgba)                                                                                                            \
    (unsigned)(((rgba)&0x000000FF) << 24 | ((rgba)&0x0000FF00) << 8 | ((rgba)&0x00FF0000) >> 8 | ((rgba)&0xFF000000) >> 24)
#else
#define COLOR(rgba) rgba
#endif

namespace zeus
{
typedef uint8_t Comp8;
typedef uint32_t Comp32;
constexpr float OneOver255 = 1.f / 255.f;

typedef union {
    struct
    {
        Comp8 r, g, b, a;
    };
    Comp32 rgba;
} RGBA32;

class CVector4f;

class alignas(16) CColor
{
public:
    ZE_DECLARE_ALIGNED_ALLOCATOR();

    static const CColor skRed;
    static const CColor skBlack;
    static const CColor skBlue;
    static const CColor skGreen;
    static const CColor skGrey;
    static const CColor skOrange;
    static const CColor skPurple;
    static const CColor skYellow;
    static const CColor skWhite;
    static const CColor skClear;

#if __SSE__
    CColor(const __m128& mVec128) : mVec128(mVec128) {}
#endif

    CColor() : r(1.0f), g(1.0f), b(1.0f), a(1.0f) {}
    CColor(float rgb, float a = 1.0) { splat(rgb, a); }
    CColor(float r, float g, float b, float a = 1.0f)
    {
        v[0] = r;
        v[1] = g;
        v[2] = b;
        v[3] = a;
    }
#if ZE_ATHENA_TYPES
    CColor(const atVec4f& vec)
#if __SSE__ || __GEKKO_PS__
    : mVec128(vec.mVec128)
    {
    }
#else
    {
        r = vec.vec[0], g = vec.vec[1], b = vec.vec[2], a = vec.vec[3];
    }
#endif
#endif

    CColor(Comp32 rgba) { fromRGBA32(rgba); }
    CColor(const Comp8* rgba) { fromRGBA8(rgba[0], rgba[1], rgba[2], rgba[3]); }

    CColor(const CVector4f& other);
    CColor& operator=(const CVector4f& other);

#if ZE_ATHENA_TYPES

    static inline CColor ReadRGBABig(athena::io::IStreamReader& reader)
    {
        CColor ret;
        ret.readRGBABig(reader);
        return ret;
    }

    inline void readRGBABig(athena::io::IStreamReader& reader)
    {
        r = reader.readFloatBig();
        g = reader.readFloatBig();
        b = reader.readFloatBig();
        a = reader.readFloatBig();
    }
    inline void readBGRABig(athena::io::IStreamReader& reader)
    {
        b = reader.readFloatBig();
        g = reader.readFloatBig();
        r = reader.readFloatBig();
        a = reader.readFloatBig();
    }
    inline void writeRGBABig(athena::io::IStreamWriter& writer) const
    {
        writer.writeFloatBig(r);
        writer.writeFloatBig(g);
        writer.writeFloatBig(b);
        writer.writeFloatBig(a);
    }
    inline void writeBGRABig(athena::io::IStreamWriter& writer) const
    {
        writer.writeFloatBig(b);
        writer.writeFloatBig(g);
        writer.writeFloatBig(r);
        writer.writeFloatBig(a);
    }
#endif

    inline bool operator==(const CColor& rhs) const { return (r == rhs.r && g == rhs.g && b == rhs.b && a == rhs.a); }
    inline bool operator!=(const CColor& rhs) const { return !(*this == rhs); }
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
        r += rhs.r;
        g += rhs.g;
        b += rhs.b;
        a += rhs.a;
#endif
        return *this;
    }
    inline const CColor& operator-=(const CColor& rhs)
    {
#if __SSE__
        mVec128 = _mm_sub_ps(mVec128, rhs.mVec128);
#else
        r -= rhs.r;
        g -= rhs.g;
        b -= rhs.b;
        a -= rhs.a;
#endif
        return *this;
    }
    inline const CColor& operator*=(const CColor& rhs)
    {
#if __SSE__
        mVec128 = _mm_mul_ps(mVec128, rhs.mVec128);
#else
        r *= rhs.r;
        g *= rhs.g;
        b *= rhs.b;
        a *= rhs.a;
#endif
        return *this;
    }
    inline const CColor& operator/=(const CColor& rhs)
    {
#if __SSE__
        mVec128 = _mm_div_ps(mVec128, rhs.mVec128);
#else
        r /= rhs.r;
        g /= rhs.g;
        b /= rhs.b;
        a /= rhs.a;
#endif
        return *this;
    }
    inline void normalize()
    {
        float mag = magnitude();
        mag = 1.f / mag;
        *this *= mag;
    }
    inline CColor normalized() const
    {
        float mag = magnitude();
        mag = 1.f / mag;
        return *this * mag;
    }

    inline float magSquared() const
    {
#if __SSE__
        TVectorUnion result;
#if __SSE4_1__
        result.mVec128 = _mm_dp_ps(mVec128, mVec128, 0xF1);
        return result.v[0];
#else
        result.mVec128 = _mm_mul_ps(mVec128, mVec128);
        return result.v[0] + result.v[1] + result.v[2] + result.v[3];
#endif
#else
        return r * r + g * g + b * b + a * a;
#endif
    }
    inline float magnitude() const { return std::sqrt(magSquared()); }
    static inline CColor lerp(const CColor& a, const CColor& b, float t) { return (a + (b - a) * t); }
    static inline CColor nlerp(const CColor& a, const CColor& b, float t) { return lerp(a, b, t).normalized(); }
    inline float& operator[](const size_t& idx) { assert(idx < 4); return (&r)[idx]; }
    inline const float& operator[](const size_t& idx) const { assert(idx < 4); return (&r)[idx]; }
    inline void splat(float rgb, float a)
    {
#if __SSE__
        TVectorUnion splat = {{rgb, rgb, rgb, a}};
        mVec128 = splat.mVec128;
#else
        v[0] = rgb;
        v[1] = rgb;
        v[2] = rgb;
        v[3] = a;
#endif
    }

    inline float rgbDot(const CColor& rhs) const
    {
#if __SSE__
        TVectorUnion result;
#if __SSE4_1__
        result.mVec128 = _mm_dp_ps(mVec128, rhs.mVec128, 0x71);
        return result.v[0];
#else
        result.mVec128 = _mm_mul_ps(mVec128, rhs.mVec128);
        return result.v[0] + result.v[1] + result.v[2];
#endif
#else
        return (r * rhs.r) + (g * rhs.g) + (b * rhs.b);
#endif
    }

    union {
        struct
        {
            float r, g, b, a;
        };
        float v[4];
#if __SSE__
        __m128 mVec128;
#endif
    };

    void fromRGBA8(Comp8 r, Comp8 g, Comp8 b, Comp8 a)
    {
        this->r = r * OneOver255;
        this->g = g * OneOver255;
        this->b = b * OneOver255;
        this->a = a * OneOver255;
    }

    void fromRGBA32(Comp32 rgba)
    {
        static RGBA32 tmp;
        tmp.rgba = COLOR(rgba);
        fromRGBA8(tmp.r, tmp.g, tmp.b, tmp.a);
    }

    /*!
     * \brief Converts a CColor to RGBA8
     * \param r
     * \param g
     * \param b
     * \param a
     */
    void toRGBA8(Comp8& r, Comp8& g, Comp8& b, Comp8& a)
    {
        r = this->r * 255;
        g = this->g * 255;
        b = this->b * 255;
        a = this->a * 255;
    }

    /**
     * @brief Assigns rgba from hsv
     * @param h[0-1] The hue percentagee of the color.
     * @param s[0-1] The saturation percentage of the color.
     * @param v[0-1] The value percentage of the color.
     * @param a[0-1] The alpha percentage of the color.
     */
    void fromHSV(float h, float s, float v, float _a = 1.0);

    /**
     * @brief Converts rgba to hsv
     * @param h[0-1] The hue percentagee of the color.
     * @param s[0-1] The saturation percentage of the color.
     * @param v[0-1] The value percentage of the color.
     * @param a[0-1] The alpha percentage of the color.
     */
    void toHSV(float& h, float& s, float& v) const;

    void fromHSL(float h, float s, float l, float _a = 1.0);

    void toHSL(float& h, float& s, float& l);

    CColor toGrayscale() { return {sqrtF((r * r + g * g + b * b) / 3), a}; }

    /**
     * @brief Clamps to GPU-safe RGBA values [0,1]
     */
    void Clamp()
    {
        this->r = std::min(1.f, std::max(0.f, this->r));
        this->g = std::min(1.f, std::max(0.f, this->g));
        this->b = std::min(1.f, std::max(0.f, this->b));
        this->a = std::min(1.f, std::max(0.f, this->a));
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
}
#endif // CCOLOR_HPP
