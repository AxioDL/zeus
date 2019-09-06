#include "zeus/CColor.hpp"

#include <algorithm>

namespace zeus {
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
  const float min = std::min({r(), g(), b()});
  const float max = std::max({r(), g(), b()});
  v = max;

  const float delta = max - min;
  s = max == 0.f ? 0.f : delta / max;

  if (max == min) {
    h = 0.f;
  } else {
    if (max == r()) {
      h = (g() - b()) / delta + (g() < b() ? 6.f : 0.f);
    } else if (max == g()) {
      h = (b() - r()) / delta + 2.f;
    } else if (max == b()) {
      h = (r() - g()) / delta + 4.f;
    }

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
  const float min = std::min({r(), g(), b()});
  const float max = std::max({r(), g(), b()});

  l = (max + min) / 2.0f;

  if (max == min) {
    h = s = 0.f;
  } else {
    const float d = max - min;
    s = l > 0.5f ? d / (2.f - max - min) : d / (max + min);

    if (max == r()) {
      h = (g() - b()) / d + (g() < b() ? 6.f : 0.f);
    } else if (max == g()) {
      h = (b() - r()) / d + 2.f;
    } else if (max == b()) {
      h = (r() - g()) / d + 4.f;
    }

    h /= 6.f;
  }
}
} // namespace zeus
