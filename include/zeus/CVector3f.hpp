#pragma once

#include <cassert>
#include <cfloat>
#include <cmath>

#include "zeus/CVector2f.hpp"
#include "zeus/Global.hpp"
#include "zeus/Math.hpp"

namespace zeus {
class CVector3d;
class CRelAngle;

class CVector3f {
public:
  zeus::simd<float> mSimd;
  constexpr CVector3f() : mSimd() {}

  template <typename T>
  constexpr CVector3f(const simd<T>& s) : mSimd(s) {}

  inline CVector3f(const CVector3d& vec);

  explicit constexpr CVector3f(float xyz) : mSimd(xyz) {}

  void assign(float x, float y, float z) { mSimd.set(x, y, z); }

  constexpr CVector3f(float x, float y, float z) : mSimd(x, y, z) {}

  constexpr CVector3f(const float* floats) : mSimd(floats[0], floats[1], floats[2]) {}

  CVector3f(const CVector2f& other, float z = 0.f) {
    mSimd = other.mSimd;
    mSimd[2] = z;
  }

  [[nodiscard]] CVector2f toVec2f() const { return CVector2f(mSimd); }

  [[nodiscard]] bool operator==(const CVector3f& rhs) const {
    const auto mask = mSimd == rhs.mSimd;
    return mask[0] && mask[1] && mask[2];
  }

  [[nodiscard]] bool operator!=(const CVector3f& rhs) const { return !(*this == rhs); }

  [[nodiscard]] simd<float>::mask_type operator>(const CVector3f& rhs) const { return mSimd > rhs.mSimd; }
  [[nodiscard]] simd<float>::mask_type operator>=(const CVector3f& rhs) const { return mSimd >= rhs.mSimd; }
  [[nodiscard]] simd<float>::mask_type operator<(const CVector3f& rhs) const { return mSimd < rhs.mSimd; }
  [[nodiscard]] simd<float>::mask_type operator<=(const CVector3f& rhs) const { return mSimd <= rhs.mSimd; }

  [[nodiscard]] constexpr CVector3f operator+(const CVector3f& rhs) const { return mSimd + rhs.mSimd; }

  [[nodiscard]] constexpr CVector3f operator-(const CVector3f& rhs) const { return mSimd - rhs.mSimd; }

  [[nodiscard]] constexpr CVector3f operator-() const { return -mSimd; }

  [[nodiscard]] constexpr CVector3f operator*(const CVector3f& rhs) const { return mSimd * rhs.mSimd; }

  [[nodiscard]] constexpr CVector3f operator/(const CVector3f& rhs) const { return mSimd / rhs.mSimd; }

  [[nodiscard]] constexpr CVector3f operator+(float val) const { return mSimd + val; }

  [[nodiscard]] constexpr CVector3f operator-(float val) const { return mSimd - val; }

  [[nodiscard]] constexpr CVector3f operator*(float val) const { return mSimd * val; }

  [[nodiscard]] constexpr CVector3f operator/(float val) const { return mSimd / val; }

  const CVector3f& operator+=(const CVector3f& rhs) {
    mSimd += rhs.mSimd;
    return *this;
  }

  const CVector3f& operator-=(const CVector3f& rhs) {
    mSimd -= rhs.mSimd;
    return *this;
  }

  const CVector3f& operator*=(const CVector3f& rhs) {
    mSimd *= rhs.mSimd;
    return *this;
  }

  const CVector3f& operator/=(const CVector3f& rhs) {
    mSimd /= rhs.mSimd;
    return *this;
  }

  void normalize() {
    float mag = 1.f / magnitude();
    *this *= CVector3f(mag);
  }

  [[nodiscard]] CVector3f normalized() const {
    float mag = 1.f / magnitude();
    return *this * mag;
  }

  [[nodiscard]] CVector3f cross(const CVector3f& rhs) const {
    return {y() * rhs.z() - z() * rhs.y(), z() * rhs.x() - x() * rhs.z(), x() * rhs.y() - y() * rhs.x()};
  }

  [[nodiscard]] float dot(const CVector3f& rhs) const { return mSimd.dot3(rhs.mSimd); }

  [[nodiscard]] float magSquared() const { return mSimd.dot3(mSimd); }

  [[nodiscard]] float magnitude() const { return std::sqrt(magSquared()); }

  [[nodiscard]] bool isNotInf() const { return !(std::isinf(x()) || std::isinf(y()) || std::isinf(z())); }

  [[nodiscard]] bool isMagnitudeSafe() const { return isNotInf() && magSquared() >= 9.9999994e-29; }
  [[nodiscard]] bool isNaN() const { return std::isnan(x()) || std::isnan(y()) || std::isnan(z()); }

  void zeroOut() { mSimd.broadcast(0.f); }

  void splat(float xyz) { mSimd.broadcast(xyz); }

  [[nodiscard]] static float getAngleDiff(const CVector3f& a, const CVector3f& b);

  [[nodiscard]] static CVector3f lerp(const CVector3f& a, const CVector3f& b, float t) {
    return zeus::simd<float>(1.f - t) * a.mSimd + b.mSimd * zeus::simd<float>(t);
  }

  [[nodiscard]] static CVector3f nlerp(const CVector3f& a, const CVector3f& b, float t) {
    return lerp(a, b, t).normalized();
  }

  [[nodiscard]] static CVector3f slerp(const CVector3f& a, const CVector3f& b, CRelAngle clampAngle);

  [[nodiscard]] bool isNormalized() const { return std::fabs(1.f - magSquared()) <= FLT_EPSILON; }

  [[nodiscard]] bool canBeNormalized() const {
    if (std::isinf(x()) || std::isinf(y()) || std::isinf(z()))
      return false;
    return !(std::fabs(x()) < FLT_EPSILON && std::fabs(y()) < FLT_EPSILON && std::fabs(z()) < FLT_EPSILON);
  }

  [[nodiscard]] bool isZero() const { return mSimd[0] == 0.f && mSimd[1] == 0.f && mSimd[2] == 0.f; }

  void scaleToLength(float newLength) {
    float length = magSquared();
    if (length < FLT_EPSILON) {
      mSimd[0] = newLength, mSimd[1] = 0.f, mSimd[2] = 0.f;
      return;
    }

    length = std::sqrt(length);
    float scalar = newLength / length;
    *this *= CVector3f(scalar);
  }

  [[nodiscard]] CVector3f scaledToLength(float newLength) const {
    CVector3f v = *this;
    v.scaleToLength(newLength);
    return v;
  }

  [[nodiscard]] bool isEqu(const CVector3f& other, float epsilon = FLT_EPSILON) const {
    const CVector3f diffVec = other - *this;
    return (diffVec.x() <= epsilon && diffVec.y() <= epsilon && diffVec.z() <= epsilon);
  }

  [[nodiscard]] simd<float>::reference operator[](size_t idx) {
    assert(idx < 3);
    return mSimd[idx];
  }

  [[nodiscard]] float operator[](size_t idx) const {
    assert(idx < 3);
    return mSimd[idx];
  }

  [[nodiscard]] float x() const { return mSimd[0]; }
  [[nodiscard]] float y() const { return mSimd[1]; }
  [[nodiscard]] float z() const { return mSimd[2]; }

  [[nodiscard]] simd<float>::reference x() { return mSimd[0]; }
  [[nodiscard]] simd<float>::reference y() { return mSimd[1]; }
  [[nodiscard]] simd<float>::reference z() { return mSimd[2]; }

  [[nodiscard]] static inline CVector3f radToDeg(const CVector3f& rad);

  [[nodiscard]] static inline CVector3f degToRad(const CVector3f& deg);
};
constexpr inline CVector3f skOne3f(1.f);
constexpr inline CVector3f skNegOne3f(-1.f);
constexpr inline CVector3f skZero3f(0.f);
constexpr inline CVector3f skForward(0.f, 1.f, 0.f);
constexpr inline CVector3f skBack(0.f, -1.f, 0.f);
constexpr inline CVector3f skLeft(-1.f, 0.f, 0.f);
constexpr inline CVector3f skRight(1.f, 0.f, 0.f);
constexpr inline CVector3f skUp(0.f, 0.f, 1.f);
constexpr inline CVector3f skDown(0.f, 0.f, -1.f);
constexpr inline CVector3f skRadToDegVec(180.f / M_PIF);
constexpr inline CVector3f skDegToRadVec(M_PIF / 180.f);

[[nodiscard]] inline CVector3f operator+(float lhs, const CVector3f& rhs) { return zeus::simd<float>(lhs) + rhs.mSimd; }

[[nodiscard]] inline CVector3f operator-(float lhs, const CVector3f& rhs) { return zeus::simd<float>(lhs) - rhs.mSimd; }

[[nodiscard]] inline CVector3f operator*(float lhs, const CVector3f& rhs) { return zeus::simd<float>(lhs) * rhs.mSimd; }

[[nodiscard]] inline CVector3f operator/(float lhs, const CVector3f& rhs) { return zeus::simd<float>(lhs) / rhs.mSimd; }

inline CVector3f CVector3f::radToDeg(const CVector3f& rad) { return rad * skRadToDegVec; }

inline CVector3f CVector3f::degToRad(const CVector3f& deg) { return deg * skDegToRadVec; }

} // namespace zeus
