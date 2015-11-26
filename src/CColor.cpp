#include "CColor.hpp"
#include "CVector4f.hpp"

namespace Zeus
{
const CColor CColor::skRed   (Comp32(0xFF0000FFul));
const CColor CColor::skBlack (Comp32(0x000000FFul));
const CColor CColor::skBlue  (Comp32(0x0000FFFFul));
const CColor CColor::skGreen (Comp32(0x00FF00FFul));
const CColor CColor::skGrey  (Comp32(0x808080FFul));
const CColor CColor::skOrange(Comp32(0xFF7000FFul));
const CColor CColor::skPurple(Comp32(0xA000FFFFul));
const CColor CColor::skYellow(Comp32(0xFFFF00FFul));
const CColor CColor::skWhite (Comp32(0xFFFFFFFFul));

float hueToRgb(float p, float q, float t)
{
    if (t < 0.0f)
        t += 1.0f;
    if (t > 1.0f)
        t -= 1.0f;
    if (t < 1.f/6.f)
        return p + (q - p) * 6.f * t;
    if (t < 1.f/2.f)
        return q;
    if (t < 2.f/3.f)
        return p + (q - p) * (2.f/3.f - t) * 6.f;
    return p;
}

CColor::CColor(const CVector4f& other)
{ r = other.x; g = other.y; b = other.z; a = other.w; }

CColor& CColor::operator=(const CVector4f& other)
{
    r = other.x;
    g = other.y;
    b = other.z;
    a = other.w;

    return *this;
}
void CColor::fromHSV(float h, float s, float v, float _a)
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

    r = _r;
    g = _g;
    b = _b;
    a = _a;
}

void CColor::toHSV(float &h, float &s, float &v) const
{
    float min = Math::min(r, Math::min(g, b));
    float max = Math::max(r, Math::max(g, b));
    v = max;

    float delta = max - min;
    s = max == 0 ? 0 : delta / max;

    if (max == min)
        h = 0;
    else
    {
        if (max == r)
            h = (g - b) / delta + (g < b ? 6 : 0);
        else if (max == g)
            h = (b - r) / delta + 2;
        else if (max == b)
            h = (r - g) / delta + 4;
        h /= 6;
    }
}

void CColor::fromHSL(float h, float s, float l, float _a)
{
    if (s == 0.0f)
        r = g = b = l;
    else
    {
        const float q = l < 0.5f ? l * (1.f + s) : l + s - 1.f * s;
        const float p = 2 * l - q;
        r = hueToRgb(p, q, h + 1.f/3);
        g = hueToRgb(p, q, h);
        b = hueToRgb(p, q, h - 1.f/3);
    }
    a = _a;
}

void CColor::toHSL(float &h, float &s, float &l)
{
    const float min = Math::min(r, Math::min(g, b));
    const float max = Math::max(r, Math::max(g, b));
    const float d = max - min;

    if (max == min)
        h = s = 0;
    else
    {
        s = l > 0.5f ? d / (2.f - max - min) : d / (max + min);
        if (max == r)
            h = (g - b) / d + (g < b ? 6.f : 0.f);
        else if (max == g)
            h = (b - r) / d + 2.f;
        else if (max == b)
            h = (r - g) / d + 4.f;

        h /= 6;
    }
}

}
