#pragma once

#include <cassert>

#include "zeus/Global.hpp"
#include "zeus/Math.hpp"

#if ZE_ATHENA_TYPES
#include <athena/IStreamReader.hpp>
#endif

namespace zeus {
class CVector2f {
public:
  simd<float> mSimd;
  constexpr CVector2f() : mSimd() {}

  template <typename T>
  constexpr CVector2f(const simd<T>& s) : mSimd(s) {}

#if ZE_ATHENA_TYPES

  constexpr CVector2f(const atVec2f& vec) : mSimd(vec.simd) {}

  operator atVec2f&() { return *reinterpret_cast<atVec2f*>(this); }

  operator const atVec2f&() const { return *reinterpret_cast<const atVec2f*>(this); }

  void readBig(athena::io::IStreamReader& input) {
    mSimd[0] = input.readFloatBig();
    mSimd[1] = input.readFloatBig();
    mSimd[2] = 0.0f;
    mSimd[3] = 0.0f;
  }

  [[nodiscard]] static CVector2f ReadBig(athena::io::IStreamReader& input) {
    CVector2f ret;
    ret.readBig(input);
    return ret;
  }

#endif

  explicit constexpr CVector2f(float xy) : mSimd(xy) {}

  constexpr void assign(float x, float y) {
    mSimd.set(x, y);
  }

  constexpr CVector2f(float x, float y) : mSimd(x, y, 0.f, 0.f) {}

  [[nodiscard]] bool operator==(const CVector2f& rhs) const {
    return mSimd[0] == rhs.mSimd[0] && mSimd[1] == rhs.mSimd[1];
  }

  [[nodiscard]] bool operator!=(const CVector2f& rhs) const {
    return mSimd[0] != rhs.mSimd[0] || mSimd[1] != rhs.mSimd[1];
  }

  [[nodiscard]] bool operator<(const CVector2f& rhs) const {
    return mSimd[0] < rhs.mSimd[0] && mSimd[1] < rhs.mSimd[1];
  }

  [[nodiscard]] bool operator<=(const CVector2f& rhs) const {
    return mSimd[0] <= rhs.mSimd[0] && mSimd[1] <= rhs.mSimd[1];
  }

  [[nodiscard]] bool operator>(const CVector2f& rhs) const {
    return mSimd[0] > rhs.mSimd[0] && mSimd[1] > rhs.mSimd[1];
  }

  [[nodiscard]] bool operator>=(const CVector2f& rhs) const {
    return mSimd[0] >= rhs.mSimd[0] && mSimd[1] >= rhs.mSimd[1];
  }

  [[nodiscard]] CVector2f operator+(const CVector2f& rhs) const { return mSimd + rhs.mSimd; }

  [[nodiscard]] CVector2f operator-(const CVector2f& rhs) const { return mSimd - rhs.mSimd; }

  [[nodiscard]] CVector2f operator-() const { return -mSimd; }

  [[nodiscard]] CVector2f operator*(const CVector2f& rhs) const { return mSimd * rhs.mSimd; }

  [[nodiscard]] CVector2f operator/(const CVector2f& rhs) const { return mSimd / rhs.mSimd; }

  [[nodiscard]] CVector2f operator+(float val) const { return mSimd + simd<float>(val); }

  [[nodiscard]] CVector2f operator-(float val) const { return mSimd - simd<float>(val); }

  [[nodiscard]] CVector2f operator*(float val) const { return mSimd * simd<float>(val); }

  [[nodiscard]] CVector2f operator/(float val) const {
    float ooval = 1.f / val;
    return mSimd * simd<float>(ooval);
  }

  const CVector2f& operator+=(const CVector2f& rhs) {
    mSimd += rhs.mSimd;
    return *this;
  }

  const CVector2f& operator-=(const CVector2f& rhs) {
    mSimd -= rhs.mSimd;
    return *this;
  }

  const CVector2f& operator*=(const CVector2f& rhs) {
    mSimd *= rhs.mSimd;
    return *this;
  }

  const CVector2f& operator/=(const CVector2f& rhs) {
    mSimd /= rhs.mSimd;
    return *this;
  }

  const CVector2f& operator+=(float rhs) {
    mSimd += simd<float>(rhs);
    return *this;
  }

  const CVector2f& operator-=(float rhs) {
    mSimd -= simd<float>(rhs);
    return *this;
  }

  const CVector2f& operator*=(float rhs) {
    mSimd *= simd<float>(rhs);
    return *this;
  }

  const CVector2f& operator/=(float rhs) {
    float oorhs = 1.f / rhs;
    mSimd /= simd<float>(oorhs);
    return *this;
  }

  void normalize() {
    float mag = magnitude();
    mag = 1.f / mag;
    *this *= CVector2f(mag);
  }

  [[nodiscard]] CVector2f normalized() const {
    float mag = magnitude();
    mag = 1.f / mag;
    return *this * mag;
  }

  [[nodiscard]] constexpr CVector2f perpendicularVector() const { return {-y(), x()}; }

  [[nodiscard]] constexpr float cross(const CVector2f& rhs) const { return (x() * rhs.y()) - (y() * rhs.x()); }

  [[nodiscard]] constexpr float dot(const CVector2f& rhs) const { return mSimd.dot2(rhs.mSimd); }

  [[nodiscard]] constexpr float magSquared() const { return mSimd.dot2(mSimd); }

  [[nodiscard]] float magnitude() const { return std::sqrt(magSquared()); }

  constexpr void zeroOut() { mSimd = 0.f; }

  constexpr void splat(float xy) { mSimd = xy; }

  [[nodiscard]] static float getAngleDiff(const CVector2f& a, const CVector2f& b);

  [[nodiscard]] static CVector2f lerp(const CVector2f& a, const CVector2f& b, float t) {
    return zeus::simd<float>(1.f - t) * a.mSimd + b.mSimd * zeus::simd<float>(t);
  }

  [[nodiscard]] static CVector2f nlerp(const CVector2f& a, const CVector2f& b, float t) {
    return lerp(a, b, t).normalized();
  }

  [[nodiscard]] static CVector2f slerp(const CVector2f& a, const CVector2f& b, float t);

  [[nodiscard]] bool isNormalized() const { return std::fabs(1.f - magSquared()) < 0.01f; }

  [[nodiscard]] bool canBeNormalized() const {
    if (std::isinf(x()) || std::isinf(y()))
      return false;
    return std::fabs(x()) >= FLT_EPSILON || std::fabs(y()) >= FLT_EPSILON;
  }

  [[nodiscard]] bool isZero() const { return mSimd[0] == 0.f && mSimd[1] == 0.f; }

  [[nodiscard]] bool isEqu(const CVector2f& other, float epsilon = FLT_EPSILON) const {
    const CVector2f diffVec = other - *this;
    return (diffVec.x() <= epsilon && diffVec.y() <= epsilon);
  }

  [[nodiscard]] constexpr simd<float>::reference operator[](size_t idx) {
    assert(idx < 2);
    return mSimd[idx];
  }

  [[nodiscard]] constexpr float operator[](size_t idx) const {
    assert(idx < 2);
    return mSimd[idx];
  }

  [[nodiscard]] constexpr float x() const { return mSimd[0]; }
  [[nodiscard]] constexpr float y() const { return mSimd[1]; }

  [[nodiscard]] constexpr simd<float>::reference x() { return mSimd[0]; }
  [[nodiscard]] constexpr simd<float>::reference y() { return mSimd[1]; }
};
constexpr inline CVector2f skOne2f(1.f);
constexpr inline CVector2f skNegOne2f(-1.f);
constexpr inline CVector2f skZero2f(0.f);

[[nodiscard]] inline CVector2f operator+(float lhs, const CVector2f& rhs) { return zeus::simd<float>(lhs) + rhs.mSimd; }

[[nodiscard]] inline CVector2f operator-(float lhs, const CVector2f& rhs) { return zeus::simd<float>(lhs) - rhs.mSimd; }

[[nodiscard]] inline CVector2f operator*(float lhs, const CVector2f& rhs) { return zeus::simd<float>(lhs) * rhs.mSimd; }

[[nodiscard]] inline CVector2f operator/(float lhs, const CVector2f& rhs) { return zeus::simd<float>(lhs) / rhs.mSimd; }
} // namespace zeus
