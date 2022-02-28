#pragma once

#include <algorithm>
#include <cassert>
#include <functional>

#include "zeus/CVector4f.hpp"
#include "zeus/Global.hpp"
#include "zeus/Math.hpp"

#undef min
#undef max

#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
#define COLOR(rgba)                                                                                                    \
  (zeus::Comp32)(((rgba)&0x000000FF) << 24 | ((rgba)&0x0000FF00) << 8 | ((rgba)&0x00FF0000) >> 8 |                     \
                 ((rgba)&0xFF000000) >> 24)
#else
#define COLOR(rgba) (zeus::Comp32)(rgba)
#endif

namespace zeus {
using Comp8 = uint8_t;
using Comp32 = uint32_t;
constexpr float OneOver255 = 1.f / 255.f;

union RGBA32 {
  struct {
    Comp8 r, g, b, a;
  };
  Comp32 rgba;
};

class CVector4f;

class CColor {
public:
  simd<float> mSimd;

  constexpr CColor() : mSimd(1.f) {}

  constexpr CColor(float rgb, float a = 1.0) : mSimd(rgb, rgb, rgb, a) {}

  constexpr CColor(float r, float g, float b, float a = 1.0f) : mSimd(r, g, b, a) {}

  constexpr CColor(Comp32 rgba) : mSimd(((COLOR(rgba) >> 0) & 0xff) * OneOver255,
                                        ((COLOR(rgba) >> 8) & 0xff) * OneOver255,
                                        ((COLOR(rgba) >> 16) & 0xff) * OneOver255,
                                        ((COLOR(rgba) >> 24) & 0xff) * OneOver255) {}

  constexpr CColor(const Comp8* rgba) : mSimd(rgba[0] * OneOver255,
                                              rgba[1] * OneOver255,
                                              rgba[2] * OneOver255,
                                              rgba[3] * OneOver255) {}

  constexpr CColor(const CVector4f& other) : mSimd(other.mSimd) {}

  template <typename T>
  constexpr CColor(const simd<T>& s) : mSimd(s) {}

  CColor& operator=(const CVector4f& other) {
    mSimd = other.mSimd;
    return *this;
  }

#if ZE_ATHENA_TYPES

  [[nodiscard]] static CColor ReadRGBABig(athena::io::IStreamReader& reader) {
    CColor ret;
    ret.readRGBABig(reader);
    return ret;
  }

  void readRGBABig(athena::io::IStreamReader& reader) {
    simd_floats f;
    f[0] = reader.readFloatBig();
    f[1] = reader.readFloatBig();
    f[2] = reader.readFloatBig();
    f[3] = reader.readFloatBig();
    mSimd.copy_from(f);
  }

  void readBGRABig(athena::io::IStreamReader& reader) {
    simd_floats f;
    f[2] = reader.readFloatBig();
    f[1] = reader.readFloatBig();
    f[0] = reader.readFloatBig();
    f[3] = reader.readFloatBig();
    mSimd.copy_from(f);
  }

  void writeRGBABig(athena::io::IStreamWriter& writer) const {
    simd_floats f(mSimd);
    writer.writeFloatBig(f[0]);
    writer.writeFloatBig(f[1]);
    writer.writeFloatBig(f[2]);
    writer.writeFloatBig(f[3]);
  }

  void writeBGRABig(athena::io::IStreamWriter& writer) const {
    simd_floats f(mSimd);
    writer.writeFloatBig(f[2]);
    writer.writeFloatBig(f[1]);
    writer.writeFloatBig(f[0]);
    writer.writeFloatBig(f[3]);
  }

  void writeRGBA8(athena::io::IStreamWriter& writer) const {
    simd_floats f(mSimd);
    writer.writeUByte(atUint8(f[0] * 255));
    writer.writeUByte(atUint8(f[1] * 255));
    writer.writeUByte(atUint8(f[2] * 255));
    writer.writeUByte(atUint8(f[3] * 255));
  }

#endif

  [[nodiscard]] bool operator==(const CColor& rhs) const {
    return (r() == rhs.r() && g() == rhs.g() && b() == rhs.b() && a() == rhs.a());
  }

  [[nodiscard]] bool operator!=(const CColor& rhs) const { return !(*this == rhs); }

  [[nodiscard]] CColor operator+(const CColor& rhs) const { return CColor(mSimd + rhs.mSimd).Clamp(); }

  [[nodiscard]] CColor operator-(const CColor& rhs) const { return CColor(mSimd - rhs.mSimd).Clamp(); }

  [[nodiscard]] CColor operator*(const CColor& rhs) const { return CColor(mSimd * rhs.mSimd).Clamp(); }

  [[nodiscard]] CColor operator/(const CColor& rhs) const { return CColor(mSimd / rhs.mSimd).Clamp(); }

  [[nodiscard]] CColor operator+(float val) const { return CColor(mSimd + simd<float>(val)).Clamp(); }

  [[nodiscard]] CColor operator-(float val) const { return CColor(mSimd - simd<float>(val)).Clamp(); }

  [[nodiscard]] CColor operator*(float val) const { return CColor(mSimd * simd<float>(val)).Clamp(); }

  [[nodiscard]] CColor operator/(float val) const { return CColor(mSimd / simd<float>(val)).Clamp(); }

  const CColor& operator+=(const CColor& rhs) {
    mSimd += rhs.mSimd;
    Clamp();
    return *this;
  }

  const CColor& operator-=(const CColor& rhs) {
    mSimd -= rhs.mSimd;
    Clamp();
    return *this;
  }

  const CColor& operator*=(const CColor& rhs) {
    mSimd *= rhs.mSimd;
    Clamp();
    return *this;
  }

  const CColor& operator/=(const CColor& rhs) {
    mSimd /= rhs.mSimd;
    Clamp();
    return *this;
  }

  const CColor& operator+=(float rhs) {
    mSimd += simd<float>(rhs);
    Clamp();
    return *this;
  }

  const CColor& operator-=(float rhs) {
    mSimd -= simd<float>(rhs);
    Clamp();
    return *this;
  }

  const CColor& operator*=(float rhs) {
    mSimd *= simd<float>(rhs);
    Clamp();
    return *this;
  }

  const CColor& operator/=(float rhs) {
    mSimd /= simd<float>(rhs);
    Clamp();
    return *this;
  }

  void normalize() {
    float mag = magnitude();
    mag = 1.f / mag;
    *this *= mag;
  }

  [[nodiscard]] CColor normalized() const {
    float mag = magnitude();
    mag = 1.f / mag;
    return *this * mag;
  }

  [[nodiscard]] float magSquared() const { return mSimd.dot4(mSimd); }

  [[nodiscard]] float magnitude() const { return std::sqrt(magSquared()); }

  [[nodiscard]] static CColor lerp(const CColor& a, const CColor& b, float t) {
    return zeus::simd<float>(1.f - t) * a.mSimd + b.mSimd * zeus::simd<float>(t);
  }

  [[nodiscard]] static CColor nlerp(const CColor& a, const CColor& b, float t) { return lerp(a, b, t).normalized(); }

  [[nodiscard]] simd<float>::reference operator[](const size_t& idx) {
    assert(idx < 4);
    return mSimd[idx];
  }

  [[nodiscard]] float operator[](const size_t& idx) const {
    assert(idx < 4);
    return mSimd[idx];
  }

  void splat(float rgb, float a) {
    mSimd = simd<float>(rgb);
    mSimd[3] = a;
  }

  [[nodiscard]] float rgbDot(const CColor& rhs) const { return mSimd.dot3(rhs.mSimd); }

  void fromRGBA8(const Comp8 ri, const Comp8 gi, const Comp8 bi, const Comp8 ai) {
    mSimd = simd<float>(ri * OneOver255, gi * OneOver255, bi * OneOver255, ai * OneOver255);
  }

  void fromRGBA32(Comp32 rgba) {
    RGBA32 tmp;
    tmp.rgba = COLOR(rgba);
    fromRGBA8(tmp.r, tmp.g, tmp.b, tmp.a);
  }

  /**
   * @brief Converts a CColor to RGBA8
   * @param ro Red component
   * @param go Green component
   * @param bo Blue component
   * @param ao Alpha component
   */
  void toRGBA8(Comp8& ro, Comp8& go, Comp8& bo, Comp8& ao) const {
    ro = Comp8(r() * 255);
    go = Comp8(g() * 255);
    bo = Comp8(b() * 255);
    ao = Comp8(a() * 255);
  }

  /**
   * @brief Assigns rgba from hsv
   * @param h[0-1] The hue percentage of the color.
   * @param s[0-1] The saturation percentage of the color.
   * @param v[0-1] The value percentage of the color.
   * @param _a[0-1] The alpha percentage of the color.
   */
  void fromHSV(float h, float s, float v, float _a = 1.0);

  /**
   * @brief Converts rgba to hsv
   * @param h[0-1] The hue percentage of the color.
   * @param s[0-1] The saturation percentage of the color.
   * @param v[0-1] The value percentage of the color.
   */
  void toHSV(float& h, float& s, float& v) const;

  void fromHSL(float h, float s, float l, float _a = 1.0);

  void toHSL(float& h, float& s, float& l) const;

  [[nodiscard]] CColor toGrayscale() const { return {std::sqrt((r() * r() + g() * g() + b() * b()) / 3), a()}; }

  /**
   * @brief Clamps to GPU-safe RGBA values [0,1]
   */
  CColor& Clamp() {
    r() = std::min(1.f, std::max(0.f, float(r())));
    g() = std::min(1.f, std::max(0.f, float(g())));
    b() = std::min(1.f, std::max(0.f, float(b())));
    a() = std::min(1.f, std::max(0.f, float(a())));
    return *this;
  }

  [[nodiscard]] float r() const { return mSimd[0]; }
  [[nodiscard]] float g() const { return mSimd[1]; }
  [[nodiscard]] float b() const { return mSimd[2]; }
  [[nodiscard]] float a() const { return mSimd[3]; }

  [[nodiscard]] simd<float>::reference r() { return mSimd[0]; }
  [[nodiscard]] simd<float>::reference g() { return mSimd[1]; }
  [[nodiscard]] simd<float>::reference b() { return mSimd[2]; }
  [[nodiscard]] simd<float>::reference a() { return mSimd[3]; }
};
constexpr CVector4f::CVector4f(const zeus::CColor& other) : mSimd(other.mSimd) {}

constexpr CVector4f& CVector4f::operator=(const CColor& other) {
  mSimd = other.mSimd;
  return *this;
}

constexpr inline CColor skRed(1.f, 0.f, 0.f, 1.f);
constexpr inline CColor skBlack(0.f, 0.f, 0.f, 1.f);
constexpr inline CColor skBlue(0.f, 0.f, 1.f, 1.f);
constexpr inline CColor skCyan(0.f, 1.f, 1.f, 1.f);
constexpr inline CColor skGreen(0.f, 1.f, 0.f, 1.f);
constexpr inline CColor skGrey(0.5f, 0.5f, 0.5f, 1.f);
constexpr inline CColor skMagenta(1.f, 0.f, 1.f, 1.f);
constexpr inline CColor skOrange(1.f, 0.43f, 0.f, 1.f);
constexpr inline CColor skPurple(0.63f, 0.f, 1.f, 1.f);
constexpr inline CColor skYellow(1.f, 1.f, 0.f, 1.f);
constexpr inline CColor skWhite(1.f, 1.f, 1.f, 1.f);
constexpr inline CColor skClear(0.f, 0.f, 0.f, 0.f);

[[nodiscard]] inline CColor operator+(float lhs, const CColor& rhs) {
  return CColor(simd<float>(lhs) + rhs.mSimd).Clamp();
}

[[nodiscard]] inline CColor operator-(float lhs, const CColor& rhs) {
  return CColor(simd<float>(lhs) - rhs.mSimd).Clamp();
}

[[nodiscard]] inline CColor operator*(float lhs, const CColor& rhs) {
  return CColor(simd<float>(lhs) * rhs.mSimd).Clamp();
}

[[nodiscard]] inline CColor operator/(float lhs, const CColor& rhs) {
  return CColor(simd<float>(lhs) / rhs.mSimd).Clamp();
}
} // namespace zeus

namespace std {
template <>
struct hash<zeus::CColor> {
  size_t operator()(const zeus::CColor& color) const noexcept {
    size_t ret = std::hash<float>()(color.r());
    zeus::hash_combine_impl(ret, std::hash<float>()(color.g()));
    zeus::hash_combine_impl(ret, std::hash<float>()(color.b()));
    zeus::hash_combine_impl(ret, std::hash<float>()(color.a()));
    return ret;
  }
};
}
