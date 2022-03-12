#pragma once

#include <cassert>

#include "zeus/Global.hpp"
#include "zeus/Math.hpp"

namespace zeus {
class CVector2d {
public:
  simd<double> mSimd;
  constexpr CVector2d() : mSimd() {}

  template <typename T>
  constexpr CVector2d(const simd<T>& s) : mSimd(s) {}

  explicit constexpr CVector2d(double xy) : mSimd(xy) {}

  constexpr void assign(double x, double y) {
    mSimd.set(x, y);
  }

  constexpr CVector2d(double x, double y) : mSimd(x, y, 0.f, 0.f) {}

  [[nodiscard]] bool operator==(const CVector2d& rhs) const {
    return mSimd[0] == rhs.mSimd[0] && mSimd[1] == rhs.mSimd[1];
  }

  [[nodiscard]] bool operator!=(const CVector2d& rhs) const {
    return mSimd[0] != rhs.mSimd[0] || mSimd[1] != rhs.mSimd[1];
  }

  [[nodiscard]] bool operator<(const CVector2d& rhs) const {
    return mSimd[0] < rhs.mSimd[0] && mSimd[1] < rhs.mSimd[1];
  }

  [[nodiscard]] bool operator<=(const CVector2d& rhs) const {
    return mSimd[0] <= rhs.mSimd[0] && mSimd[1] <= rhs.mSimd[1];
  }

  [[nodiscard]] bool operator>(const CVector2d& rhs) const {
    return mSimd[0] > rhs.mSimd[0] && mSimd[1] > rhs.mSimd[1];
  }

  [[nodiscard]] bool operator>=(const CVector2d& rhs) const {
    return mSimd[0] >= rhs.mSimd[0] && mSimd[1] >= rhs.mSimd[1];
  }

  [[nodiscard]] constexpr CVector2d operator+(const CVector2d& rhs) const { return mSimd + rhs.mSimd; }

  [[nodiscard]] constexpr CVector2d operator-(const CVector2d& rhs) const { return mSimd - rhs.mSimd; }

  [[nodiscard]] constexpr CVector2d operator-() const { return -mSimd; }

  [[nodiscard]] constexpr CVector2d operator*(const CVector2d& rhs) const { return mSimd * rhs.mSimd; }

  [[nodiscard]] constexpr CVector2d operator/(const CVector2d& rhs) const { return mSimd / rhs.mSimd; }

  [[nodiscard]] constexpr CVector2d operator+(double val) const { return mSimd + simd<double>(val); }

  [[nodiscard]] constexpr CVector2d operator-(double val) const { return mSimd - simd<double>(val); }

  [[nodiscard]] constexpr CVector2d operator*(double val) const { return mSimd * simd<double>(val); }

  [[nodiscard]] constexpr CVector2d operator/(double val) const {
    double ooval = 1.0 / val;
    return mSimd * simd<double>(ooval);
  }

  const CVector2d& operator+=(const CVector2d& rhs) {
    mSimd += rhs.mSimd;
    return *this;
  }

  const CVector2d& operator-=(const CVector2d& rhs) {
    mSimd -= rhs.mSimd;
    return *this;
  }

  const CVector2d& operator*=(const CVector2d& rhs) {
    mSimd *= rhs.mSimd;
    return *this;
  }

  const CVector2d& operator/=(const CVector2d& rhs) {
    mSimd /= rhs.mSimd;
    return *this;
  }

  const CVector2d& operator+=(double rhs) {
    mSimd += simd<double>(rhs);
    return *this;
  }

  const CVector2d& operator-=(double rhs) {
    mSimd -= simd<double>(rhs);
    return *this;
  }

  const CVector2d& operator*=(double rhs) {
    mSimd *= simd<double>(rhs);
    return *this;
  }

  const CVector2d& operator/=(double rhs) {
    double oorhs = 1.0 / rhs;
    mSimd *= simd<double>(oorhs);
    return *this;
  }

  void normalize() {
    double mag = magnitude();
    mag = 1.f / mag;
    *this *= CVector2d(mag);
  }

  [[nodiscard]] CVector2d normalized() const {
    double mag = magnitude();
    mag = 1.f / mag;
    return *this * mag;
  }

  [[nodiscard]] constexpr CVector2d perpendicularVector() const { return {-y(), x()}; }

  [[nodiscard]] constexpr double cross(const CVector2d& rhs) const { return (x() * rhs.y()) - (y() * rhs.x()); }

  [[nodiscard]] constexpr double dot(const CVector2d& rhs) const { return mSimd.dot2(rhs.mSimd); }

  [[nodiscard]] constexpr double magSquared() const { return mSimd.dot2(mSimd); }

  [[nodiscard]] double magnitude() const { return std::sqrt(magSquared()); }

  constexpr void zeroOut() { mSimd = 0.f; }

  constexpr void splat(double xy) { mSimd = xy; }

  [[nodiscard]] static double getAngleDiff(const CVector2d& a, const CVector2d& b);

  [[nodiscard]] static CVector2d lerp(const CVector2d& a, const CVector2d& b, double t) {
    return zeus::simd<double>(1.f - t) * a.mSimd + b.mSimd * zeus::simd<double>(t);
  }

  [[nodiscard]] static CVector2d nlerp(const CVector2d& a, const CVector2d& b, double t) {
    return lerp(a, b, t).normalized();
  }

  [[nodiscard]] static CVector2d slerp(const CVector2d& a, const CVector2d& b, double t);

  [[nodiscard]] bool isNormalized() const { return std::fabs(1.0 - magSquared()) < 0.01; }

  [[nodiscard]] bool canBeNormalized() const {
    if (std::isinf(x()) || std::isinf(y()))
      return false;
    return std::fabs(x()) >= DBL_EPSILON || std::fabs(y()) >= DBL_EPSILON;
  }

  [[nodiscard]] bool isZero() const { return mSimd[0] == 0.f && mSimd[1] == 0.f; }

  [[nodiscard]] bool isEqu(const CVector2d& other, double epsilon = FLT_EPSILON) const {
    const CVector2d diffVec = other - *this;
    return (diffVec.x() <= epsilon && diffVec.y() <= epsilon);
  }

  [[nodiscard]] simd<double>::reference operator[](size_t idx) {
    assert(idx < 2);
    return mSimd[idx];
  }

  [[nodiscard]] constexpr double operator[](size_t idx) const {
    assert(idx < 2);
    return mSimd[idx];
  }

  [[nodiscard]] constexpr double x() const { return mSimd[0]; }
  [[nodiscard]] constexpr double y() const { return mSimd[1]; }

  [[nodiscard]] simd<double>::reference x() { return mSimd[0]; }
  [[nodiscard]] simd<double>::reference y() { return mSimd[1]; }
};
constexpr inline CVector2d skOne2d(1.0);
constexpr inline CVector2d skNegOne2d(-1.0);
constexpr inline CVector2d skZero2d(0.0);

[[nodiscard]] inline CVector2d operator+(double lhs, const CVector2d& rhs) { return zeus::simd<double>(lhs) + rhs.mSimd; }

[[nodiscard]] inline CVector2d operator-(double lhs, const CVector2d& rhs) { return zeus::simd<double>(lhs) - rhs.mSimd; }

[[nodiscard]] inline CVector2d operator*(double lhs, const CVector2d& rhs) { return zeus::simd<double>(lhs) * rhs.mSimd; }

[[nodiscard]] inline CVector2d operator/(double lhs, const CVector2d& rhs) { return zeus::simd<double>(lhs) / rhs.mSimd; }
} // namespace zeus
