#pragma once

#include <cassert>
#include <cfloat>
#include <cmath>

#include "zeus/CVector3f.hpp"
#include "zeus/Global.hpp"

#if ZE_ATHENA_TYPES
#include <athena/IStreamReader.hpp>
#endif

namespace zeus {
class CColor;

class CVector4d {
public:
  zeus::simd<double> mSimd;

  constexpr CVector4d() : mSimd() {}

  template <typename T>
  constexpr CVector4d(const simd<T>& s) : mSimd(s) {}

#if ZE_ATHENA_TYPES

  constexpr CVector4d(const atVec4f& vec) : mSimd(vec.simd) {}

  operator atVec4f&() { return *reinterpret_cast<atVec4f*>(this); }

  operator const atVec4f&() const { return *reinterpret_cast<const atVec4f*>(this); }

  void readBig(athena::io::IStreamReader& input) {
    simd_doubles f;
    f[0] = input.readDoubleBig();
    f[1] = input.readDoubleBig();
    f[2] = input.readDoubleBig();
    f[3] = input.readDoubleBig();
    mSimd.copy_from(f);
  }

#endif

  explicit constexpr CVector4d(double xyzw) : mSimd(xyzw) {}

  void assign(double x, double y, double z, double w) { mSimd = simd<double>(x, y, z, w); }

  constexpr CVector4d(double x, double y, double z, double w) : mSimd(x, y, z, w) {}

  constexpr CVector4d(const CColor& other);

  CVector4d(const CVector3f& other, double wIn = 1.0) : mSimd(other.mSimd) { mSimd[3] = wIn; }

  [[nodiscard]] static CVector4d ToClip(const zeus::CVector3f& v, double w) { return CVector4d(v * w, w); }

  [[nodiscard]] CVector3f toVec3f() const { return CVector3f(mSimd); }

  constexpr CVector4d& operator=(const CColor& other);

  [[nodiscard]] bool operator==(const CVector4d& rhs) const {
    auto eq_mask = mSimd == rhs.mSimd;
    return eq_mask[0] && eq_mask[1] && eq_mask[2] && eq_mask[3];
  }

  [[nodiscard]] bool operator!=(const CVector4d& rhs) const {
    auto eq_mask = mSimd != rhs.mSimd;
    return eq_mask[0] || eq_mask[1] || eq_mask[2] || eq_mask[3];
  }

  [[nodiscard]] bool operator<(const CVector4d& rhs) const {
    auto eq_mask = mSimd < rhs.mSimd;
    return eq_mask[0] && eq_mask[1] && eq_mask[2] && eq_mask[3];
  }

  [[nodiscard]] bool operator<=(const CVector4d& rhs) const {
    auto eq_mask = mSimd <= rhs.mSimd;
    return eq_mask[0] && eq_mask[1] && eq_mask[2] && eq_mask[3];
  }

  [[nodiscard]] bool operator>(const CVector4d& rhs) const {
    auto eq_mask = mSimd > rhs.mSimd;
    return eq_mask[0] && eq_mask[1] && eq_mask[2] && eq_mask[3];
  }

  [[nodiscard]] bool operator>=(const CVector4d& rhs) const {
    auto eq_mask = mSimd >= rhs.mSimd;
    return eq_mask[0] && eq_mask[1] && eq_mask[2] && eq_mask[3];
  }

  [[nodiscard]] CVector4d operator+(const CVector4d& rhs) const { return mSimd + rhs.mSimd; }

  [[nodiscard]] CVector4d operator-(const CVector4d& rhs) const { return mSimd - rhs.mSimd; }

  [[nodiscard]] CVector4d operator-() const { return -mSimd; }

  [[nodiscard]] CVector4d operator*(const CVector4d& rhs) const { return mSimd * rhs.mSimd; }

  [[nodiscard]] CVector4d operator/(const CVector4d& rhs) const { return mSimd / rhs.mSimd; }

  [[nodiscard]] CVector4d operator+(double val) const { return mSimd + zeus::simd<double>(val); }

  [[nodiscard]] CVector4d operator-(double val) const { return mSimd - zeus::simd<double>(val); }

  [[nodiscard]] CVector4d operator*(double val) const { return mSimd * zeus::simd<double>(val); }

  [[nodiscard]] CVector4d operator/(double val) const {
    double ooval = 1.0 / val;
    return mSimd * zeus::simd<double>(ooval);
  }

  const CVector4d& operator+=(const CVector4d& rhs) {
    mSimd += rhs.mSimd;
    return *this;
  }

  const CVector4d& operator-=(const CVector4d& rhs) {
    mSimd -= rhs.mSimd;
    return *this;
  }

  const CVector4d& operator*=(const CVector4d& rhs) {
    mSimd *= rhs.mSimd;
    return *this;
  }

  const CVector4d& operator/=(const CVector4d& rhs) {
    mSimd /= rhs.mSimd;
    return *this;
  }

  void normalize() {
    double mag = magnitude();
    mag = 1.0 / mag;
    *this *= CVector4d(mag);
  }

  [[nodiscard]] CVector4d normalized() const {
    double mag = magnitude();
    mag = 1.0 / mag;
    return *this * mag;
  }

  [[nodiscard]] double dot(const CVector4d& rhs) const { return mSimd.dot4(rhs.mSimd); }

  [[nodiscard]] double magSquared() const { return mSimd.dot4(mSimd); }

  [[nodiscard]] double magnitude() const { return std::sqrt(magSquared()); }

  void zeroOut() { mSimd = zeus::simd<double>(0.0); }

  void splat(double xyzw) { mSimd = zeus::simd<double>(xyzw); }

  [[nodiscard]] static CVector4d lerp(const CVector4d& a, const CVector4d& b, double t) {
    return zeus::simd<double>(1.0 - t) * a.mSimd + b.mSimd * zeus::simd<double>(t);
  }

  [[nodiscard]] static CVector4d nlerp(const CVector4d& a, const CVector4d& b, double t) {
    return lerp(a, b, t).normalized();
  }

  [[nodiscard]] bool isNormalized() const { return std::fabs(1.0 - magSquared()) < 0.01f; }

  [[nodiscard]] bool canBeNormalized() const {
    if (std::isinf(x()) || std::isinf(y()) || std::isinf(z()) || std::isinf(w()))
      return false;
    return std::fabs(x()) >= FLT_EPSILON || std::fabs(y()) >= FLT_EPSILON || std::fabs(z()) >= FLT_EPSILON ||
           std::fabs(w()) >= FLT_EPSILON;
  }

  [[nodiscard]] bool isEqu(const CVector4d& other, double epsilon = FLT_EPSILON) const {
    const CVector4d diffVec = other - *this;
    return (diffVec.x() <= epsilon && diffVec.y() <= epsilon && diffVec.z() <= epsilon && diffVec.w() <= epsilon);
  }

  [[nodiscard]] simd<double>::reference operator[](size_t idx) {
    assert(idx < 4);
    return mSimd[idx];
  }

  [[nodiscard]] double operator[](size_t idx) const {
    assert(idx < 4);
    return mSimd[idx];
  }

  [[nodiscard]] double x() const { return mSimd[0]; }
  [[nodiscard]] double y() const { return mSimd[1]; }
  [[nodiscard]] double z() const { return mSimd[2]; }
  [[nodiscard]] double w() const { return mSimd[3]; }

  [[nodiscard]] simd<double>::reference x() { return mSimd[0]; }
  [[nodiscard]] simd<double>::reference y() { return mSimd[1]; }
  [[nodiscard]] simd<double>::reference z() { return mSimd[2]; }
  [[nodiscard]] simd<double>::reference w() { return mSimd[3]; }
};
constexpr CVector4d skOne4d(1.0);
constexpr CVector4d skNegOne4d(-1.0);
constexpr CVector4d skZero4d(0.0);

[[nodiscard]] inline CVector4d operator+(double lhs, const CVector4d& rhs) { return zeus::simd<double>(lhs) + rhs.mSimd; }

[[nodiscard]] inline CVector4d operator-(double lhs, const CVector4d& rhs) { return zeus::simd<double>(lhs) - rhs.mSimd; }

[[nodiscard]] inline CVector4d operator*(double lhs, const CVector4d& rhs) { return zeus::simd<double>(lhs) * rhs.mSimd; }

[[nodiscard]] inline CVector4d operator/(double lhs, const CVector4d& rhs) { return zeus::simd<double>(lhs) / rhs.mSimd; }

} // namespace zeus
