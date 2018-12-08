#include "zeus/CColor.hpp"
#include "zeus/CVector4f.hpp"

namespace zeus {
const CColor CColor::skRed(Comp32(0xFF0000FFul));
const CColor CColor::skBlack(Comp32(0x000000FFul));
const CColor CColor::skBlue(Comp32(0x0000FFFFul));
const CColor CColor::skGreen(Comp32(0x00FF00FFul));
const CColor CColor::skGrey(Comp32(0x808080FFul));
const CColor CColor::skOrange(Comp32(0xFF7000FFul));
const CColor CColor::skPurple(Comp32(0xA000FFFFul));
const CColor CColor::skYellow(Comp32(0xFFFF00FFul));
const CColor CColor::skWhite(Comp32(0xFFFFFFFFul));
const CColor CColor::skClear(Comp32(0x00000000ul));

float hueToRgb(float p, float q, float t) {
  if (t < 0.0f)
    t += 1.0f;
  if (t > 1.0f)
    t -= 1.0f;
  if (t < 1.f / 6.f)
    return p + (q - p) * 6.f * t;
  if (t < 1.f / 2.f)
    return q;
  if (t < 2.f / 3.f)
    return p + (q - p) * (2.f / 3.f - t) * 6.f;
  return p;
}

void CColor::fromHSV(float h, float s, float v, float _a) {
  int i = int(h * 6.f);
  float f = h * 6.f - i;
  float p = v * (1.f - s);
  float q = v * (1.f - f * s);
  float t = v * (1.f - (1.f - f) * s);
  simd_floats fo;

  switch (i % 6) {
  case 0:
    fo[0] = v, fo[1] = t, fo[2] = p;
    break;
  case 1:
    fo[0] = q, fo[1] = v, fo[2] = p;
    break;
  case 2:
    fo[0] = p, fo[1] = v, fo[2] = t;
    break;
  case 3:
    fo[0] = p, fo[1] = q, fo[2] = v;
    break;
  case 4:
    fo[0] = t, fo[1] = p, fo[2] = v;
    break;
  case 5:
    fo[0] = v, fo[1] = p, fo[2] = q;
    break;
  default:
    break;
  }

  fo[3] = _a;
  mSimd.copy_from(fo);
}

void CColor::toHSV(float& h, float& s, float& v) const {
  float min = std::min(r(), std::min(g(), b()));
  float max = std::max(r(), std::max(g(), b()));
  v = max;

  float delta = max - min;
  s = max == 0.f ? 0.f : delta / max;

  if (max == min)
    h = 0.f;
  else {
    if (max == r())
      h = (g() - b()) / delta + (g() < b() ? 6.f : 0.f);
    else if (max == g())
      h = (b() - r()) / delta + 2.f;
    else if (max == b())
      h = (r() - g()) / delta + 4.f;
    h /= 6.f;
  }
}

void CColor::fromHSL(float h, float s, float l, float _a) {
  if (s == 0.0f) {
    mSimd = simd<float>(l);
  } else {
    const float q = l < 0.5f ? l * (1.f + s) : l + s - 1.f * s;
    const float p = 2.f * l - q;
    r() = hueToRgb(p, q, h + 1.f / 3.f);
    g() = hueToRgb(p, q, h);
    b() = hueToRgb(p, q, h - 1.f / 3.f);
  }
  a() = _a;
}

void CColor::toHSL(float& h, float& s, float& l) const {
  const float min = std::min(r(), std::min(g(), b()));
  const float max = std::max(r(), std::max(g(), b()));
  const float d = max - min;

  if (max == min)
    h = s = 0.f;
  else {
    s = l > 0.5f ? d / (2.f - max - min) : d / (max + min);
    if (max == r())
      h = (g() - b()) / d + (g() < b() ? 6.f : 0.f);
    else if (max == g())
      h = (b() - r()) / d + 2.f;
    else if (max == b())
      h = (r() - g()) / d + 4.f;

    h /= 6.f;
  }
}
} // namespace zeus
