#pragma once

#include "athena/Types.hpp"
#include "Global.hpp"
#include "zeus/Math.hpp"
#include "zeus/CVector3f.hpp"

namespace zeus {

class CVector3d {
public:
  zeus::simd<double> mSimd;
  CVector3d() : mSimd(0.0) {}

  template <typename T>
  CVector3d(const simd<T>& s) : mSimd(s) {}

#if ZE_ATHENA_TYPES
  CVector3d(const atVec3d& vec) : mSimd(vec.simd) {}
#endif

  explicit CVector3d(double xyz) : mSimd(xyz) {}

  CVector3d(const CVector3f& vec) : mSimd(vec.mSimd) {}

  CVector3d(double x, double y, double z) : mSimd(x, y, z) {}

  CVector3f asCVector3f() { return mSimd; }

  double magSquared() const { return mSimd.dot3(mSimd); }

  double magnitude() const { return sqrt(magSquared()); }

  CVector3d cross(const CVector3d& rhs) const {
    return {y() * rhs.z() - z() * rhs.y(), z() * rhs.x() - x() * rhs.z(), x() * rhs.y() - y() * rhs.x()};
  }

  double dot(const CVector3d& rhs) const { return mSimd.dot3(rhs.mSimd); }

  CVector3d asNormalized() {
    double mag = magnitude();
    mag = 1.0 / mag;
    return mSimd * zeus::simd<double>(mag);
  }

  void splat(double xyz) { mSimd = zeus::simd<double>(xyz); }

  void zeroOut() { *this = skZero; }

  CVector3d operator+(const CVector3d& rhs) const { return mSimd + rhs.mSimd; }

  CVector3d operator-(const CVector3d& rhs) const { return mSimd - rhs.mSimd; }

  CVector3d operator*(const CVector3d& rhs) const { return mSimd * rhs.mSimd; }

  CVector3d operator/(const CVector3d& rhs) const { return mSimd / rhs.mSimd; }

  zeus::simd<double>::reference operator[](size_t idx) {
    assert(idx < 3);
    return mSimd[idx];
  }

  double operator[](size_t idx) const {
    assert(idx < 3);
    return mSimd[idx];
  }

  double x() const { return mSimd[0]; }
  double y() const { return mSimd[1]; }
  double z() const { return mSimd[2]; }

  simd<double>::reference x() { return mSimd[0]; }
  simd<double>::reference y() { return mSimd[1]; }
  simd<double>::reference z() { return mSimd[2]; }

  static const CVector3d skZero;
};

static inline CVector3d operator+(double lhs, const CVector3d& rhs) { return zeus::simd<double>(lhs) + rhs.mSimd; }

static inline CVector3d operator-(double lhs, const CVector3d& rhs) { return zeus::simd<double>(lhs) - rhs.mSimd; }

static inline CVector3d operator*(double lhs, const CVector3d& rhs) { return zeus::simd<double>(lhs) * rhs.mSimd; }

static inline CVector3d operator/(double lhs, const CVector3d& rhs) { return zeus::simd<double>(lhs) / rhs.mSimd; }

} // namespace zeus
