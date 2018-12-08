#pragma once

#include "Global.hpp"
#include "zeus/Math.hpp"
#include "zeus/Math.hpp"
#include <cassert>

namespace zeus {
class CVector2f {
public:
  simd<float> mSimd;
  CVector2f() : mSimd(0.f) {}

  template <typename T>
  CVector2f(const simd<T>& s) : mSimd(s) {}

#if ZE_ATHENA_TYPES

  CVector2f(const atVec2f& vec) : mSimd(vec.simd) {}

  operator atVec2f&() { return *reinterpret_cast<atVec2f*>(this); }

  operator const atVec2f&() const { return *reinterpret_cast<const atVec2f*>(this); }

  void readBig(athena::io::IStreamReader& input) {
    mSimd[0] = input.readFloatBig();
    mSimd[1] = input.readFloatBig();
    mSimd[2] = 0.0f;
    mSimd[3] = 0.0f;
  }

  static CVector2f ReadBig(athena::io::IStreamReader& input) {
    CVector2f ret;
    ret.readBig(input);
    return ret;
  }

#endif

  explicit CVector2f(float xy) { splat(xy); }

  void assign(float x, float y) {
    mSimd[0] = x;
    mSimd[1] = y;
    mSimd[2] = 0.0f;
    mSimd[3] = 0.0f;
  }

  CVector2f(float x, float y) { assign(x, y); }

  bool operator==(const CVector2f& rhs) const { return mSimd[0] == rhs.mSimd[0] && mSimd[1] == rhs.mSimd[1]; }

  bool operator!=(const CVector2f& rhs) const { return mSimd[0] != rhs.mSimd[0] || mSimd[1] != rhs.mSimd[1]; }

  bool operator<(const CVector2f& rhs) const { return mSimd[0] < rhs.mSimd[0] && mSimd[1] < rhs.mSimd[1]; }

  bool operator<=(const CVector2f& rhs) const { return mSimd[0] <= rhs.mSimd[0] && mSimd[1] <= rhs.mSimd[1]; }

  bool operator>(const CVector2f& rhs) const { return mSimd[0] > rhs.mSimd[0] && mSimd[1] > rhs.mSimd[1]; }

  bool operator>=(const CVector2f& rhs) const { return mSimd[0] >= rhs.mSimd[0] && mSimd[1] >= rhs.mSimd[1]; }

  CVector2f operator+(const CVector2f& rhs) const { return mSimd + rhs.mSimd; }

  CVector2f operator-(const CVector2f& rhs) const { return mSimd - rhs.mSimd; }

  CVector2f operator-() const { return -mSimd; }

  CVector2f operator*(const CVector2f& rhs) const { return mSimd * rhs.mSimd; }

  CVector2f operator/(const CVector2f& rhs) const { return mSimd / rhs.mSimd; }

  CVector2f operator+(float val) const { return mSimd + simd<float>(val); }

  CVector2f operator-(float val) const { return mSimd - simd<float>(val); }

  CVector2f operator*(float val) const { return mSimd * simd<float>(val); }

  CVector2f operator/(float val) const {
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

  CVector2f normalized() const {
    float mag = magnitude();
    mag = 1.f / mag;
    return *this * mag;
  }

  CVector2f perpendicularVector() const { return {-y(), x()}; }

  float cross(const CVector2f& rhs) const { return (x() * rhs.y()) - (y() * rhs.x()); }

  float dot(const CVector2f& rhs) const { return mSimd.dot2(rhs.mSimd); }

  float magSquared() const { return mSimd.dot2(mSimd); }

  float magnitude() const { return std::sqrt(magSquared()); }

  void zeroOut() { *this = CVector2f::skZero; }

  void splat(float xy) { mSimd = zeus::simd<float>(xy); }

  static float getAngleDiff(const CVector2f& a, const CVector2f& b);

  static CVector2f lerp(const CVector2f& a, const CVector2f& b, float t) {
    return zeus::simd<float>(1.f - t) * a.mSimd + b.mSimd * zeus::simd<float>(t);
  }

  static CVector2f nlerp(const CVector2f& a, const CVector2f& b, float t) { return lerp(a, b, t).normalized(); }

  static CVector2f slerp(const CVector2f& a, const CVector2f& b, float t);

  bool isNormalized() const { return std::fabs(1.f - magSquared()) < 0.01f; }

  bool canBeNormalized() const {
    if (std::isinf(x()) || std::isinf(y()))
      return false;
    return std::fabs(x()) >= FLT_EPSILON || std::fabs(y()) >= FLT_EPSILON;
  }

  bool isZero() const { return magSquared() <= FLT_EPSILON; }

  bool isEqu(const CVector2f& other, float epsilon = FLT_EPSILON) {
    const CVector2f diffVec = other - *this;
    return (diffVec.x() <= epsilon && diffVec.y() <= epsilon);
  }

  zeus::simd<float>::reference operator[](size_t idx) {
    assert(idx < 2);
    return mSimd[idx];
  }

  float operator[](size_t idx) const {
    assert(idx < 2);
    return mSimd[idx];
  }

  float x() const { return mSimd[0]; }
  float y() const { return mSimd[1]; }

  simd<float>::reference x() { return mSimd[0]; }
  simd<float>::reference y() { return mSimd[1]; }

  static const CVector2f skOne;
  static const CVector2f skNegOne;
  static const CVector2f skZero;
};

static inline CVector2f operator+(float lhs, const CVector2f& rhs) { return zeus::simd<float>(lhs) + rhs.mSimd; }

static inline CVector2f operator-(float lhs, const CVector2f& rhs) { return zeus::simd<float>(lhs) - rhs.mSimd; }

static inline CVector2f operator*(float lhs, const CVector2f& rhs) { return zeus::simd<float>(lhs) * rhs.mSimd; }

static inline CVector2f operator/(float lhs, const CVector2f& rhs) { return zeus::simd<float>(lhs) / rhs.mSimd; }
} // namespace zeus
