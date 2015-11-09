#include "CColor.hpp"

namespace Zeus
{
const CColor CColor::skRed   (0xFF0000FFul);
const CColor CColor::skBlack (0x000000FFul);
const CColor CColor::skBlue  (0x0000FFFFul);
const CColor CColor::skGreen (0x00FF00FFul);
const CColor CColor::skGrey  (0x808080FFul);
const CColor CColor::skOrange(0xFF7000FFul);
const CColor CColor::skPurple(0xA000FFFFul);
const CColor CColor::skYellow(0xFFFF00FFul);
const CColor CColor::skWhite (0xFFFFFFFFul);

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

    r = _r * 255;
    g = _g * 255;
    b = _b * 255;
    a = _a * 255;
}

void CColor::toHSV(float &h, float &s, float &v) const
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

void CColor::fromHSL(float h, float s, float l, float _a)
{
    float _r, _g, _b;

    if (s == 0.0f)
        r = g = b = l;
    else
    {
        const float q = l < 0.5f ? l * (1.f + s) : l + s - 1.f * s;
        const float p = 2 * l - q;
        _r = hueToRgb(p, q, h + 1.f/3);
        _g = hueToRgb(p, q, h);
        _b = hueToRgb(p, q, h - 1.f/3);
    }


    r = _r * 255.f;
    g = _g * 255.f;
    b = _b * 255.f;
    a = _a * 255.f;
}

void CColor::toHSL(float &h, float &s, float &l)
{
    const float rf = r / 255.f;
    const float gf = g / 255.f;
    const float bf = b / 255.f;
    const float min = Math::min(rf, Math::min(gf, bf));
    const float max = Math::max(rf, Math::max(gf, bf));
    const float d = max - min;

    if (max == min)
        h = s = 0;
    else
    {
        s = l > 0.5f ? d / (2.f - max - min) : d / (max + min);
        if (max == rf)
            h = (gf - bf) / d + (gf < bf ? 6.f : 0.f);
        else if (max == gf)
            h = (bf - rf) / d + 2.f;
        else if (max == bf)
            h = (rf - gf) / d + 4.f;

        h /= 6;
    }
}

}
