#ifndef CCOLOR_HPP
#define CCOLOR_HPP

#include "Global.hpp"
#include "Math.hpp"
#if ZE_ATHENA_TYPES
#include <Athena/IStreamReader.hpp>
#endif

#if BYTE_ORDER == __ORDER_LITTLE_ENDIAN__
#define COLOR(rgba) (unsigned long)( ( (rgba) & 0x000000FF ) << 24 | ( (rgba) & 0x0000FF00 ) <<  8 \
                    | ( (rgba) & 0x00FF0000 ) >>  8 | ( (rgba) & 0xFF000000 ) >> 24 )
#else
#define COLOR(rgba) (unsigned long)rgba
#endif

namespace Zeus
{
typedef unsigned char Comp8;
typedef unsigned long Comp32;

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


    CColor() : rgba(~0u) {}
    CColor(float rgb, float a = 1.0) { splat(rgb, a); }
    CColor(float r, float g, float b, float a = 1.0f)
        : r(Comp8(255*r)),
          g(Comp8(255*g)),
          b(Comp8(255*b)),
          a(Comp8(255*a))
    {}

#if ZE_ATHENA_TYPES
    CColor(Athena::io::IStreamReader& reader) {readRGBA(reader);}
#endif
    CColor(Comp32 rgba) { fromRGBA32(rgba); }
    CColor(const Comp8 rgba[4]) : r(rgba[0]), g(rgba[1]), b(rgba[2]), a(rgba[3]) {}
    CColor(Comp8 r, Comp8 g, Comp8 b, Comp8 a = 255)
        : r(r),  g(g),  b(b), a(a) {}

#if ZE_ATHENA_TYPES
    inline void readRGBA(Athena::io::IStreamReader& reader)
    {
        r = Comp8(255 * reader.readFloat());
        g = Comp8(255 * reader.readFloat());
        b = Comp8(255 * reader.readFloat());
        a = Comp8(255 * reader.readFloat());
    }
    inline void readBGRA(Athena::io::IStreamReader& reader)
    {
        b = Comp8(255 * reader.readFloat());
        g = Comp8(255 * reader.readFloat());
        r = Comp8(255 * reader.readFloat());
        a = Comp8(255 * reader.readFloat());
    }
#endif

    inline bool operator==(const CColor& rhs) const
    { return (rgba == rhs.rgba); }
    inline bool operator!=(const CColor& rhs) const
    { return !(*this == rhs); }
    inline CColor operator+(const CColor& rhs) const
    {
        CColor ret; ret.r = r + rhs.r; ret.g = g + rhs.g; ret.b = b + rhs.b; ret.a = a + rhs.a;
        return ret;
    }
    inline CColor operator-(const CColor& rhs) const
    {
        CColor ret; ret.r = r - rhs.r; ret.g = g - rhs.g; ret.b = b - rhs.b; ret.a = a - rhs.a;
        return ret;
    }

    inline CColor operator*(const CColor& rhs) const
    {
        CColor ret; ret.r = r * rhs.r; ret.g = g * rhs.g; ret.b = b * rhs.b; ret.a = a * rhs.a;
        return ret;
    }
    inline CColor operator/(const CColor& rhs) const
    {
        CColor ret; ret.r = r / rhs.r; ret.g = g / rhs.g; ret.b = b / rhs.b; ret.a = a / rhs.a;
        return ret;
    }
    inline CColor operator+(float val) const
    {
        CColor ret;
        ret.r = r + Comp8(255 * val); ret.g = g + Comp8(255 * val); ret.b = b + Comp8(255 * val); ret.a = a + Comp8(255 * val);
        return ret;
    }
    inline CColor operator-(float val) const
    {
        CColor ret;
        ret.r = r - Comp8(255 * val); ret.g = g - Comp8(255 * val); ret.b = b - Comp8(255 * val); ret.a = a - Comp8(255 * val);
        return ret;
    }
    inline CColor operator*(float val) const
    {
        CColor ret;
        ret.r = r * Comp8(255 * val); ret.g = g * Comp8(255 * val); ret.b = b * Comp8(255 * val); ret.a = a * Comp8(255 * val);
        return ret;
    }
    inline CColor operator/(float val) const
    {
        CColor ret;
        ret.r = r / Comp8(255 * val); ret.g = g / Comp8(255 * val); ret.b = b / Comp8(255 * val); ret.a = a / Comp8(255 * val);
        return ret;
    }
    inline const CColor& operator+=(const CColor& rhs)
    {
        r += rhs.r; g += rhs.g; b += rhs.b;
        a += rhs.a; return *this;
    }
    inline const CColor& operator-=(const CColor& rhs)
    {
        r -= rhs.r; g -= rhs.g; b -= rhs.b; a -= rhs.a;
        return *this;
    }
    inline const CColor& operator *=(const CColor& rhs)
    {
        r *= rhs.r; g *= rhs.g; b *= rhs.b; a *= rhs.a;
        return *this;
    }
    inline const CColor& operator /=(const CColor& rhs)
    {
        r /= rhs.r; g /= rhs.g; b /= rhs.b; a /= rhs.a;
        return *this;
    }
    inline void normalize()
    {
        float mag = magnitude();
        *this *= mag;
    }
    inline CColor normalized()
    {
        float mag = magnitude();
        return *this * mag;
    }
    inline float magSquared() const
    { return ((r * r + g * g + b * b + a * a) / 255.f); }

    inline float magnitude() const
    {
        return Math::sqrtF(magSquared());
    }
    static inline CColor lerp(const CColor& a, const CColor& b, float t)
    {
        return (a + (b - a) * t);
    }
    static inline CColor nlerp(const CColor& a, const CColor& b, float t)
    {
        return lerp(a, b, t).normalized();
    }

    inline Comp8& operator[](size_t idx) {return (&r)[idx];}
    inline const Comp8& operator[](size_t idx) const { return (&r)[idx]; }
    inline void splat(float rgb, float a)
    {
        r = Comp8(255 * rgb);
        g = Comp8(255 * rgb);
        b = Comp8(255 * rgb);
        this->a = Comp8(255 * a);
    }

    union
    {
        struct { Comp8 r, g, b, a; };
        Comp32 rgba;
    };

    void fromRGBA8(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
    {
        this->r = r;
        this->g = g;
        this->b = b;
        this->a = a;
    }

    /**
     * @brief Assigns rgba from hsv
     * @param h[0-1] The hue percentagee of the color.
     * @param s[0-1] The saturation percentage of the color.
     * @param v[0-1] The value percentage of the color.
     * @param a[0-1] The alpha percentage of the color.
     */
    void fromHSV(float h, float s, float v, float _a = 1.0)
    {
        int i = int(h * 6);
        float f = h * 6 - i;
        float p = v * (1 - s);
        float q = v * (1 - f * s);
        float t = v * (1 - (1 - f) * s);
        float _r, _g, _b;

        switch(i % 6)
        {
            case 0: _r = v, _g = t, _b = p; break;
            case 1: _r = q, _g = v, _b = p; break;
            case 2: _r = p, _g = v, _b = t; break;
            case 3: _r = p, _g = q, _b = v; break;
            case 4: _r = t, _g = p, _b = v; break;
            case 5: _r = v, _g = p, _b = q; break;
        }

        r = _r * 255;
        g = _g * 255;
        b = _b * 255;
        a = _a * 255;
    }

    /**
     * @brief Converts rgba to hsv
     * @param h[0-1] The hue percentagee of the color.
     * @param s[0-1] The saturation percentage of the color.
     * @param v[0-1] The value percentage of the color.
     * @param a[0-1] The alpha percentage of the color.
     */
    void toHSV(float& h, float& s, float& v) const
    {
        float rf = r/255.f;
        float gf = g/255.f;
        float bf = b/255.f;

        float min = Math::min(rf, Math::min(gf, bf));
        float max = Math::max(rf, Math::max(gf, bf));
        v = max;

        float delta = max - min;
        s = max == 0 ? 0 : delta / max;

        if (max == min)
            h = 0;
        else
        {
            if (max == rf)
                h = (gf - bf) / delta + (gf < bf ? 6 : 0);
            else if (max == gf)
                h = (bf - rf) / delta + 2;
            else if (max == bf)
                h = (rf - gf) / delta + 4;
            h /= 6;
        }
    }

    void fromRGBA32(unsigned int rgba)
    { this->rgba = COLOR(rgba); }
};

static inline CColor operator+(float lhs, const CColor& rhs)
{
    CColor ret;
    ret.r = Comp8(255 * lhs) + rhs.r;
    ret.g = Comp8(255 * lhs) + rhs.g;
    ret.b = Comp8(255 * lhs) + rhs.b;
    ret.a = Comp8(255 * lhs) + rhs.a;
    return ret;
}

static inline CColor operator-(float lhs, const CColor& rhs)
{
    CColor ret;
    ret.r = Comp8(255 * lhs) - rhs.r;
    ret.g = Comp8(255 * lhs) - rhs.g;
    ret.b = Comp8(255 * lhs) - rhs.b;
    ret.a = Comp8(255 * lhs) - rhs.a;
    return ret;
}

static inline CColor operator*(float lhs, const CColor& rhs)
{
    CColor ret;
    ret.r = Comp8(255 * lhs) * rhs.r;
    ret.g = Comp8(255 * lhs) * rhs.g;
    ret.b = Comp8(255 * lhs) * rhs.b;
    ret.a = Comp8(255 * lhs) * rhs.a;
    return ret;
}

static inline CColor operator/(float lhs, const CColor& rhs)
{
    CColor ret;
    ret.r = Comp8(255 * lhs) / rhs.r;
    ret.g = Comp8(255 * lhs) / rhs.g;
    ret.b = Comp8(255 * lhs) / rhs.b;
    ret.a = Comp8(255 * lhs) / rhs.a;
    return ret;
}

}

#endif // CCOLOR_HPP
