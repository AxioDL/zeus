#pragma once

#include "Global.hpp"
#include "CAxisAngle.hpp"
#include "zeus/CVector3f.hpp"
#include "zeus/CVector4f.hpp"
#include "zeus/CMatrix3f.hpp"
#include "zeus/Math.hpp"
#include "zeus/CRelAngle.hpp"
#include "zeus/CTransform.hpp"

#if ZE_ATHENA_TYPES

#include <athena/IStreamReader.hpp>

#endif

namespace zeus {

class CNUQuaternion;

/** Unit quaternion, used for all quaternion arithmetic */
class CQuaternion {
public:
  constexpr CQuaternion() : mSimd(1.f, 0.f, 0.f, 0.f) {}

  constexpr CQuaternion(float wi, float xi, float yi, float zi) : mSimd(wi, xi, yi, zi) {}

  CQuaternion(float xi, float yi, float zi) { fromVector3f(CVector3f(xi, yi, zi)); }

  CQuaternion(float wi, const CVector3f& vec) : mSimd(vec.mSimd.shuffle<0, 0, 1, 2>()) { mSimd[0] = wi; }

  template <typename T>
  constexpr CQuaternion(const simd<T>& s) : mSimd(s) {}

#if ZE_ATHENA_TYPES

  void readBig(athena::io::IStreamReader& input) {
    simd_floats f;
    f[0] = input.readFloatBig();
    f[1] = input.readFloatBig();
    f[2] = input.readFloatBig();
    f[3] = input.readFloatBig();
    mSimd.copy_from(f);
  }

  constexpr CQuaternion(const atVec4f& vec) : mSimd(vec.simd) {}

  operator atVec4f&() { return *reinterpret_cast<atVec4f*>(this); }

  operator const atVec4f&() const { return *reinterpret_cast<const atVec4f*>(this); }

#endif

  CQuaternion(const CMatrix3f& mat);

  CQuaternion(const CVector3f& vec) { fromVector3f(vec); }

  constexpr CQuaternion(const CVector4f& vec) : mSimd(vec.mSimd) {}

  CQuaternion(const CVector3f& vecA, const CVector3f& vecB) {
    CVector3f vecAN = vecA.normalized();
    CVector3f vecBN = vecB.normalized();
    CVector3f w = vecAN.cross(vecBN);
    *this = CQuaternion(1.f + vecAN.dot(vecBN), w).normalized();
  }

  void fromVector3f(const CVector3f& vec);

  CQuaternion& operator=(const CQuaternion& q);

  CQuaternion operator+(const CQuaternion& q) const;

  CQuaternion operator-(const CQuaternion& q) const;

  CQuaternion operator*(const CQuaternion& q) const;

  CQuaternion operator/(const CQuaternion& q) const;

  CQuaternion operator*(float scale) const;

  CQuaternion operator/(float scale) const;

  CQuaternion operator-() const;

  const CQuaternion& operator+=(const CQuaternion& q);

  const CQuaternion& operator-=(const CQuaternion& q);

  const CQuaternion& operator*=(const CQuaternion& q);

  const CQuaternion& operator*=(float scale);

  const CQuaternion& operator/=(float scale);

  float magnitude() const { return std::sqrt(magSquared()); }

  float magSquared() const { return mSimd.dot4(mSimd); }

  void normalize() { *this /= magnitude(); }

  CQuaternion normalized() const { return *this / magnitude(); }

  void invert();

  CQuaternion inverse() const;

  /**
   * @brief Set the rotation using axis angle notation
   * @param axis The axis to rotate around
   * @param angle The magnitude of the rotation in radians
   * @return
   */
  static CQuaternion fromAxisAngle(const CUnitVector3f& axis, const CRelAngle& angle) {
    return CQuaternion(std::cos(angle / 2.f), axis * std::sin(angle / 2.f));
  }

  void rotateX(const CRelAngle& angle) { *this *= fromAxisAngle({1.0f, 0.0f, 0.0f}, angle); }

  void rotateY(const CRelAngle& angle) { *this *= fromAxisAngle({0.0f, 1.0f, 0.0f}, angle); }

  void rotateZ(const CRelAngle& angle) { *this *= fromAxisAngle({0.0f, 0.0f, 1.0f}, angle); }

  static CVector3f rotate(const CQuaternion& rotation, const CAxisAngle& v) {
    CQuaternion q = rotation * v;
    q *= rotation.inverse();

    return {q.mSimd.shuffle<1, 2, 3, 3>()};
  }

  static CQuaternion lookAt(const CUnitVector3f& source, const CUnitVector3f& dest, const CRelAngle& maxAng);

  CVector3f transform(const CVector3f& v) const {
    CQuaternion r(0.f, v);
    return (*this * r * inverse()).getImaginary();
  }

  CQuaternion log() const;

  CQuaternion exp() const;

  CTransform toTransform() const { return CTransform(CMatrix3f(*this)); }

  CTransform toTransform(const zeus::CVector3f& origin) const { return CTransform(CMatrix3f(*this), origin); }

  float dot(const CQuaternion& rhs) const { return mSimd.dot4(rhs.mSimd); }

  static CQuaternion lerp(const CQuaternion& a, const CQuaternion& b, double t);

  static CQuaternion slerp(const CQuaternion& a, const CQuaternion& b, double t);

  static CQuaternion slerpShort(const CQuaternion& a, const CQuaternion& b, double t);

  static CQuaternion nlerp(const CQuaternion& a, const CQuaternion& b, double t);

  static CQuaternion shortestRotationArc(const zeus::CVector3f& v0, const zeus::CVector3f& v1);

  static CQuaternion clampedRotateTo(const zeus::CUnitVector3f& v0, const zeus::CUnitVector3f& v1,
                                     const zeus::CRelAngle& angle);

  float roll() const {
    simd_floats f(mSimd);
    return std::asin(-2.f * (f[1] * f[3] - f[0] * f[2]));
  }

  float pitch() const {
    simd_floats f(mSimd);
    return std::atan2(2.f * (f[2] * f[3] + f[0] * f[1]), f[0] * f[0] - f[1] * f[1] - f[2] * f[2] + f[3] * f[3]);
  }

  float yaw() const {
    simd_floats f(mSimd);
    return std::atan2(2.f * (f[1] * f[2] + f[0] * f[3]), f[0] * f[0] + f[1] * f[1] - f[2] * f[2] - f[3] * f[3]);
  }

  CQuaternion buildEquivalent() const;

  zeus::CVector3f getImaginary() const { return mSimd.shuffle<1, 2, 3, 3>(); }

  void setImaginary(const zeus::CVector3f& i) {
    x() = i.x();
    y() = i.y();
    z() = i.z();
  }

  CRelAngle angleFrom(const zeus::CQuaternion& other);

  simd<float>::reference operator[](size_t idx) {
    assert(idx < 4);
    return mSimd[idx];
  }

  float operator[](size_t idx) const {
    assert(idx < 4);
    return mSimd[idx];
  }

  float w() const { return mSimd[0]; }
  float x() const { return mSimd[1]; }
  float y() const { return mSimd[2]; }
  float z() const { return mSimd[3]; }

  simd<float>::reference w() { return mSimd[0]; }
  simd<float>::reference x() { return mSimd[1]; }
  simd<float>::reference y() { return mSimd[2]; }
  simd<float>::reference z() { return mSimd[3]; }

  simd<float> mSimd;

  static CQuaternion fromNUQuaternion(const CNUQuaternion& q);
};

/** Non-unit quaternion, no guarantee that it's normalized.
 *  Converting to CQuaternion will perform normalize operation.
 */
class CNUQuaternion {
public:
  CNUQuaternion() : mSimd(1.f, 0.f, 0.f, 0.f) {}

  CNUQuaternion(float wi, float xi, float yi, float zi) : mSimd(wi, xi, yi, zi) {}

  CNUQuaternion(float win, const zeus::CVector3f& vec) : mSimd(vec.mSimd.shuffle<0, 0, 1, 2>()) { w() = win; }

  CNUQuaternion(const CQuaternion& other) : mSimd(other.mSimd) {}

  CNUQuaternion(const CMatrix3f& mtx) : CNUQuaternion(CQuaternion(mtx)) {}

  CNUQuaternion(const simd<float>& s) : mSimd(s) {}

  static CNUQuaternion fromAxisAngle(const CUnitVector3f& axis, const CRelAngle& angle) {
    return CNUQuaternion(CQuaternion::fromAxisAngle(axis, angle));
  }

  float magnitude() const { return std::sqrt(magSquared()); }

  float magSquared() const { return mSimd.dot4(mSimd); }

  void normalize() {
    float magDiv = 1.f / magnitude();
    mSimd *= magDiv;
  }

  CNUQuaternion normalized() const {
    float magDiv = 1.f / magnitude();
    return mSimd * simd<float>(magDiv);
  }

  CNUQuaternion operator*(const CNUQuaternion& q) const;

  CNUQuaternion operator*(float f) const;

  const CNUQuaternion& operator+=(const CNUQuaternion& q);

  zeus::simd<float>::reference operator[](size_t idx) {
    assert(idx < 4);
    return mSimd[idx];
  }

  float operator[](size_t idx) const {
    assert(idx < 4);
    return mSimd[idx];
  }

  float w() const { return mSimd[0]; }
  float x() const { return mSimd[1]; }
  float y() const { return mSimd[2]; }
  float z() const { return mSimd[3]; }

  simd<float>::reference w() { return mSimd[0]; }
  simd<float>::reference x() { return mSimd[1]; }
  simd<float>::reference y() { return mSimd[2]; }
  simd<float>::reference z() { return mSimd[3]; }

  simd<float> mSimd;
};

inline CQuaternion CQuaternion::fromNUQuaternion(const CNUQuaternion& q) {
  auto norm = q.normalized();
  return norm.mSimd;
}

CQuaternion operator+(float lhs, const CQuaternion& rhs);

CQuaternion operator-(float lhs, const CQuaternion& rhs);

CQuaternion operator*(float lhs, const CQuaternion& rhs);

CNUQuaternion operator*(float lhs, const CNUQuaternion& rhs);
} // namespace zeus
