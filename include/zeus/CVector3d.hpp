#pragma once

#include <cassert>

#include "zeus/CVector3f.hpp"
#include "zeus/Global.hpp"

namespace zeus {

class CVector3d {
public:
  zeus::simd<double> mSimd;
  constexpr CVector3d() : mSimd() {}

  template <typename T>
  constexpr CVector3d(const simd<T>& s) : mSimd(s) {}

  explicit constexpr CVector3d(double xyz) : mSimd(xyz) {}

  CVector3d(const CVector3f& vec) : mSimd(vec.mSimd) {}

  constexpr CVector3d(double x, double y, double z) : mSimd(x, y, z) {}

  [[nodiscard]] CVector3f asCVector3f() const { return mSimd; }

  [[nodiscard]] double magSquared() const { return mSimd.dot3(mSimd); }

  [[nodiscard]] double magnitude() const { return sqrt(magSquared()); }

  [[nodiscard]] CVector3d cross(const CVector3d& rhs) const {
    return {y() * rhs.z() - z() * rhs.y(), z() * rhs.x() - x() * rhs.z(), x() * rhs.y() - y() * rhs.x()};
  }

  [[nodiscard]] double dot(const CVector3d& rhs) const { return mSimd.dot3(rhs.mSimd); }

  [[nodiscard]] CVector3d asNormalized() const {
    double mag = magnitude();
    mag = 1.0 / mag;
    return mSimd * zeus::simd<double>(mag);
  }

  void splat(double xyz) { mSimd = zeus::simd<double>(xyz); }

  void zeroOut() { mSimd = zeus::simd<double>(0.0); }

  [[nodiscard]] CVector3d operator+(const CVector3d& rhs) const { return mSimd + rhs.mSimd; }

  [[nodiscard]] CVector3d operator-(const CVector3d& rhs) const { return mSimd - rhs.mSimd; }

  [[nodiscard]] CVector3d operator*(const CVector3d& rhs) const { return mSimd * rhs.mSimd; }

  [[nodiscard]] CVector3d operator/(const CVector3d& rhs) const { return mSimd / rhs.mSimd; }

  [[nodiscard]] simd<double>::reference operator[](size_t idx) {
    assert(idx < 3);
    return mSimd[idx];
  }

  [[nodiscard]] double operator[](size_t idx) const {
    assert(idx < 3);
    return mSimd[idx];
  }

  [[nodiscard]] double x() const { return mSimd[0]; }
  [[nodiscard]] double y() const { return mSimd[1]; }
  [[nodiscard]] double z() const { return mSimd[2]; }

  [[nodiscard]] simd<double>::reference x() { return mSimd[0]; }
  [[nodiscard]] simd<double>::reference y() { return mSimd[1]; }
  [[nodiscard]] simd<double>::reference z() { return mSimd[2]; }
};
inline CVector3f::CVector3f(const CVector3d& vec) : mSimd(vec.mSimd) {}

constexpr inline CVector3d skZero3d(0.0);

[[nodiscard]] inline CVector3d operator+(double lhs, const CVector3d& rhs) {
  return zeus::simd<double>(lhs) + rhs.mSimd;
}

[[nodiscard]] inline CVector3d operator-(double lhs, const CVector3d& rhs) {
  return zeus::simd<double>(lhs) - rhs.mSimd;
}

[[nodiscard]] inline CVector3d operator*(double lhs, const CVector3d& rhs) {
  return zeus::simd<double>(lhs) * rhs.mSimd;
}

[[nodiscard]] inline CVector3d operator/(double lhs, const CVector3d& rhs) {
  return zeus::simd<double>(lhs) / rhs.mSimd;
}

} // namespace zeus
