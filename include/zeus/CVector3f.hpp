#pragma once

#include <cassert>
#include <cfloat>
#include <cmath>

#include "zeus/CVector2f.hpp"
#include "zeus/Global.hpp"
#include "zeus/Math.hpp"

#if ZE_ATHENA_TYPES
#include <athena/IStreamReader.hpp>
#endif

namespace zeus {
class CVector3d;
class CRelAngle;

class CVector3f {
public:
  zeus::simd<float> mSimd;
  constexpr CVector3f() : mSimd(0.f) {}

  template <typename T>
  constexpr CVector3f(const simd<T>& s) : mSimd(s) {}

#if ZE_ATHENA_TYPES

  constexpr CVector3f(const atVec3f& vec) : mSimd(vec.simd) {}

  operator atVec3f&() { return *reinterpret_cast<atVec3f*>(this); }

  operator const atVec3f&() const { return *reinterpret_cast<const atVec3f*>(this); }

  void readBig(athena::io::IStreamReader& input) {
    simd_floats f;
    f[0] = input.readFloatBig();
    f[1] = input.readFloatBig();
    f[2] = input.readFloatBig();
    f[3] = 0.0f;
    mSimd.copy_from(f);
  }

  static CVector3f ReadBig(athena::io::IStreamReader& input) {
    CVector3f ret;
    ret.readBig(input);
    return ret;
  }

#endif

  inline CVector3f(const CVector3d& vec);

  explicit constexpr CVector3f(float xyz) : mSimd(xyz) {}

  void assign(float x, float y, float z) { mSimd = zeus::simd<float>(x, y, z); }

  constexpr CVector3f(float x, float y, float z) : mSimd(x, y, z) {}

  constexpr CVector3f(const float* floats) : mSimd(floats[0], floats[1], floats[2]) {}

  CVector3f(const CVector2f& other, float z = 0.f) {
    mSimd = other.mSimd;
    mSimd[2] = z;
    mSimd[3] = 0.f;
  }

  CVector2f toVec2f() const { return CVector2f(mSimd); }

  bool operator==(const CVector3f& rhs) const {
    return mSimd[0] == rhs.mSimd[0] && mSimd[1] == rhs.mSimd[1] && mSimd[2] == rhs.mSimd[2];
  }

  bool operator!=(const CVector3f& rhs) const { return !(*this == rhs); }

  CVector3f operator+(const CVector3f& rhs) const { return mSimd + rhs.mSimd; }

  CVector3f operator-(const CVector3f& rhs) const { return mSimd - rhs.mSimd; }

  CVector3f operator-() const { return -mSimd; }

  CVector3f operator*(const CVector3f& rhs) const { return mSimd * rhs.mSimd; }

  CVector3f operator/(const CVector3f& rhs) const { return mSimd / rhs.mSimd; }

  CVector3f operator+(float val) const { return mSimd + zeus::simd<float>(val); }

  CVector3f operator-(float val) const { return mSimd - zeus::simd<float>(val); }

  CVector3f operator*(float val) const { return mSimd * zeus::simd<float>(val); }

  CVector3f operator/(float val) const {
    float ooval = 1.f / val;
    return mSimd * zeus::simd<float>(ooval);
  }

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

  CVector3f normalized() const {
    float mag = 1.f / magnitude();
    return *this * mag;
  }

  CVector3f cross(const CVector3f& rhs) const {
    return CVector3f(y() * rhs.z() - z() * rhs.y(), z() * rhs.x() - x() * rhs.z(), x() * rhs.y() - y() * rhs.x());
  }

  float dot(const CVector3f& rhs) const { return mSimd.dot3(rhs.mSimd); }

  float magSquared() const { return mSimd.dot3(mSimd); }

  float magnitude() const { return std::sqrt(magSquared()); }

  bool isNotInf() const { return !(std::isinf(x()) || std::isinf(y()) || std::isinf(z())); }

  bool isMagnitudeSafe() const { return isNotInf() && magSquared() >= 9.9999994e-29; }

  void zeroOut() { mSimd = zeus::simd<float>(0.f); }

  void splat(float xyz) { mSimd = zeus::simd<float>(xyz); }

  static float getAngleDiff(const CVector3f& a, const CVector3f& b);

  static CVector3f lerp(const CVector3f& a, const CVector3f& b, float t) {
    return zeus::simd<float>(1.f - t) * a.mSimd + b.mSimd * zeus::simd<float>(t);
  }

  static CVector3f nlerp(const CVector3f& a, const CVector3f& b, float t) { return lerp(a, b, t).normalized(); }

  static CVector3f slerp(const CVector3f& a, const CVector3f& b, CRelAngle clampAngle);

  bool isNormalized() const { return std::fabs(1.f - magSquared()) < 0.01f; }

  bool canBeNormalized() const {
    if (std::isinf(x()) || std::isinf(y()) || std::isinf(z()))
      return false;
    return std::fabs(x()) >= FLT_EPSILON || std::fabs(y()) >= FLT_EPSILON || std::fabs(z()) >= FLT_EPSILON;
  }

  bool isZero() const { return magSquared() <= FLT_EPSILON; }

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

  CVector3f scaledToLength(float newLength) const {
    CVector3f v = *this;
    v.scaleToLength(newLength);
    return v;
  }

  bool isEqu(const CVector3f& other, float epsilon = FLT_EPSILON) const {
    const CVector3f diffVec = other - *this;
    return (diffVec.x() <= epsilon && diffVec.y() <= epsilon && diffVec.z() <= epsilon);
  }

  zeus::simd<float>::reference operator[](size_t idx) {
    assert(idx < 3);
    return mSimd[idx];
  }

  float operator[](size_t idx) const {
    assert(idx < 3);
    return mSimd[idx];
  }

  float x() const { return mSimd[0]; }
  float y() const { return mSimd[1]; }
  float z() const { return mSimd[2]; }

  simd<float>::reference x() { return mSimd[0]; }
  simd<float>::reference y() { return mSimd[1]; }
  simd<float>::reference z() { return mSimd[2]; }

  static inline CVector3f radToDeg(const CVector3f& rad);

  static inline CVector3f degToRad(const CVector3f& deg);
};
constexpr CVector3f skOne3f(1.f);
constexpr CVector3f skNegOne3f(-1.f);
constexpr CVector3f skZero3f(0.f);
constexpr CVector3f skForward(0.f, 1.f, 0.f);
constexpr CVector3f skBack(0.f, -1.f, 0.f);
constexpr CVector3f skLeft(-1.f, 0.f, 0.f);
constexpr CVector3f skRight(1.f, 0.f, 0.f);
constexpr CVector3f skUp(0.f, 0.f, 1.f);
constexpr CVector3f skDown(0.f, 0.f, -1.f);
constexpr CVector3f skRadToDegVec(180.f / M_PIF);
constexpr CVector3f skDegToRadVec(M_PIF / 180.f);

inline CVector3f operator+(float lhs, const CVector3f& rhs) { return zeus::simd<float>(lhs) + rhs.mSimd; }

inline CVector3f operator-(float lhs, const CVector3f& rhs) { return zeus::simd<float>(lhs) - rhs.mSimd; }

inline CVector3f operator*(float lhs, const CVector3f& rhs) { return zeus::simd<float>(lhs) * rhs.mSimd; }

inline CVector3f operator/(float lhs, const CVector3f& rhs) { return zeus::simd<float>(lhs) / rhs.mSimd; }

inline CVector3f CVector3f::radToDeg(const CVector3f& rad) { return rad * skRadToDegVec; }

inline CVector3f CVector3f::degToRad(const CVector3f& deg) { return deg * skDegToRadVec; }

} // namespace zeus
