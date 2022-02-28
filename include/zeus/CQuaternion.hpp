#pragma once

#include <cmath>

#include "zeus/CAxisAngle.hpp"
#include "zeus/CMatrix3f.hpp"
#include "zeus/CRelAngle.hpp"
#include "zeus/CTransform.hpp"
#include "zeus/CVector3f.hpp"
#include "zeus/CVector4f.hpp"
#include "zeus/Global.hpp"

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

  CQuaternion& operator=(const CQuaternion& q) {
    mSimd = q.mSimd;
    return *this;
  }

  [[nodiscard]] CQuaternion operator+(const CQuaternion& q) const { return mSimd + q.mSimd; }

  [[nodiscard]] CQuaternion operator-(const CQuaternion& q) const { return mSimd - q.mSimd; }

  [[nodiscard]] CQuaternion operator*(const CQuaternion& q) const;

  [[nodiscard]] CQuaternion operator/(const CQuaternion& q) const;

  [[nodiscard]] CQuaternion operator*(float scale) const { return mSimd * simd<float>(scale); }

  [[nodiscard]] CQuaternion operator/(float scale) const { return mSimd / simd<float>(scale); }

  [[nodiscard]] CQuaternion operator-() const { return -mSimd; }

  const CQuaternion& operator+=(const CQuaternion& q) {
    mSimd += q.mSimd;
    return *this;
  }

  const CQuaternion& operator-=(const CQuaternion& q) {
    mSimd -= q.mSimd;
    return *this;
  }

  const CQuaternion& operator*=(const CQuaternion& q);

  const CQuaternion& operator*=(float scale) {
    mSimd *= simd<float>(scale);
    return *this;
  }

  const CQuaternion& operator/=(float scale) {
    mSimd /= simd<float>(scale);
    return *this;
  }

  [[nodiscard]] float magnitude() const { return std::sqrt(magSquared()); }

  [[nodiscard]] float magSquared() const { return mSimd.dot4(mSimd); }

  void normalize() { *this /= magnitude(); }

  [[nodiscard]] CQuaternion normalized() const { return *this / magnitude(); }

  static constexpr simd<float> InvertQuat = {1.f, -1.f, -1.f, -1.f};

  void invert() { mSimd *= InvertQuat; }

  [[nodiscard]] CQuaternion inverse() const { return mSimd * InvertQuat; }

  /**
   * @brief Set the rotation using axis angle notation
   * @param axis The axis to rotate around
   * @param angle The magnitude of the rotation in radians
   * @return
   */
  [[nodiscard]] static CQuaternion fromAxisAngle(const CUnitVector3f& axis, const CRelAngle& angle) {
    return CQuaternion(std::cos(angle / 2.f), axis * std::sin(angle / 2.f));
  }

  void rotateX(const CRelAngle& angle) { *this *= fromAxisAngle({1.0f, 0.0f, 0.0f}, angle); }

  void rotateY(const CRelAngle& angle) { *this *= fromAxisAngle({0.0f, 1.0f, 0.0f}, angle); }

  void rotateZ(const CRelAngle& angle) { *this *= fromAxisAngle({0.0f, 0.0f, 1.0f}, angle); }

  [[nodiscard]] static CVector3f rotate(const CQuaternion& rotation, const CAxisAngle& v) {
    CQuaternion q = rotation * v;
    q *= rotation.inverse();

    return {q.mSimd.shuffle<1, 2, 3, 3>()};
  }

  [[nodiscard]] static CQuaternion lookAt(const CUnitVector3f& source, const CUnitVector3f& dest,
                                          const CRelAngle& maxAng);

  [[nodiscard]] CVector3f transform(const CVector3f& v) const {
    const CQuaternion r(0.f, v);
    return (*this * r * inverse()).getImaginary();
  }

  [[nodiscard]] CQuaternion log() const;

  [[nodiscard]] CQuaternion exp() const;

  [[nodiscard]] CTransform toTransform() const { return CTransform(CMatrix3f(*this)); }

  [[nodiscard]] CTransform toTransform(const zeus::CVector3f& origin) const {
    return CTransform(CMatrix3f(*this), origin);
  }

  [[nodiscard]] float dot(const CQuaternion& rhs) const { return mSimd.dot4(rhs.mSimd); }

  [[nodiscard]] static CQuaternion lerp(const CQuaternion& a, const CQuaternion& b, double t);

  [[nodiscard]] static CQuaternion slerp(const CQuaternion& a, const CQuaternion& b, double t);

  [[nodiscard]] static CQuaternion slerpShort(const CQuaternion& a, const CQuaternion& b, double t);

  [[nodiscard]] static CQuaternion nlerp(const CQuaternion& a, const CQuaternion& b, double t);

  [[nodiscard]] static CQuaternion shortestRotationArc(const zeus::CVector3f& v0, const zeus::CVector3f& v1);

  [[nodiscard]] static CQuaternion clampedRotateTo(const zeus::CUnitVector3f& v0, const zeus::CUnitVector3f& v1,
                                                   const zeus::CRelAngle& angle);

  [[nodiscard]] float roll() const {
    simd_floats f(mSimd);
    return std::asin(-2.f * (f[1] * f[3] - f[0] * f[2]));
  }

  [[nodiscard]] float pitch() const {
    simd_floats f(mSimd);
    return std::atan2(2.f * (f[2] * f[3] + f[0] * f[1]), f[0] * f[0] - f[1] * f[1] - f[2] * f[2] + f[3] * f[3]);
  }

  [[nodiscard]] float yaw() const {
    simd_floats f(mSimd);
    return std::atan2(2.f * (f[1] * f[2] + f[0] * f[3]), f[0] * f[0] + f[1] * f[1] - f[2] * f[2] - f[3] * f[3]);
  }

  [[nodiscard]] CQuaternion buildEquivalent() const;

  [[nodiscard]] CVector3f getImaginary() const { return mSimd.shuffle<1, 2, 3, 3>(); }

  void setImaginary(const CVector3f& i) {
    x() = i.x();
    y() = i.y();
    z() = i.z();
  }

  [[nodiscard]] CRelAngle angleFrom(const CQuaternion& other) const;

  [[nodiscard]] simd<float>::reference operator[](size_t idx) {
    assert(idx < 4);
    return mSimd[idx];
  }

  [[nodiscard]] float operator[](size_t idx) const {
    assert(idx < 4);
    return mSimd[idx];
  }

  [[nodiscard]] float w() const { return mSimd[0]; }
  [[nodiscard]] float x() const { return mSimd[1]; }
  [[nodiscard]] float y() const { return mSimd[2]; }
  [[nodiscard]] float z() const { return mSimd[3]; }

  [[nodiscard]] simd<float>::reference w() { return mSimd[0]; }
  [[nodiscard]] simd<float>::reference x() { return mSimd[1]; }
  [[nodiscard]] simd<float>::reference y() { return mSimd[2]; }
  [[nodiscard]] simd<float>::reference z() { return mSimd[3]; }

  simd<float> mSimd;

  [[nodiscard]] static CQuaternion fromNUQuaternion(const CNUQuaternion& q);
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

  [[nodiscard]] static CNUQuaternion fromAxisAngle(const CUnitVector3f& axis, const CRelAngle& angle) {
    return CNUQuaternion(CQuaternion::fromAxisAngle(axis, angle));
  }

  [[nodiscard]] float magnitude() const { return std::sqrt(magSquared()); }

  [[nodiscard]] float magSquared() const { return mSimd.dot4(mSimd); }

  void normalize() {
    float magDiv = 1.f / magnitude();
    mSimd *= magDiv;
  }

  [[nodiscard]] CNUQuaternion normalized() const {
    float magDiv = 1.f / magnitude();
    return mSimd * simd<float>(magDiv);
  }

  [[nodiscard]] CNUQuaternion operator*(const CNUQuaternion& q) const;

  [[nodiscard]] CNUQuaternion operator*(float f) const { return mSimd * simd<float>(f); }

  const CNUQuaternion& operator+=(const CNUQuaternion& q) {
    mSimd += q.mSimd;
    return *this;
  }

  [[nodiscard]] simd<float>::reference operator[](size_t idx) {
    assert(idx < 4);
    return mSimd[idx];
  }

  [[nodiscard]] float operator[](size_t idx) const {
    assert(idx < 4);
    return mSimd[idx];
  }

  [[nodiscard]] float w() const { return mSimd[0]; }
  [[nodiscard]] float x() const { return mSimd[1]; }
  [[nodiscard]] float y() const { return mSimd[2]; }
  [[nodiscard]] float z() const { return mSimd[3]; }

  [[nodiscard]] simd<float>::reference w() { return mSimd[0]; }
  [[nodiscard]] simd<float>::reference x() { return mSimd[1]; }
  [[nodiscard]] simd<float>::reference y() { return mSimd[2]; }
  [[nodiscard]] simd<float>::reference z() { return mSimd[3]; }

  simd<float> mSimd;
};

inline CQuaternion CQuaternion::fromNUQuaternion(const CNUQuaternion& q) {
  auto norm = q.normalized();
  return norm.mSimd;
}

[[nodiscard]] CQuaternion operator+(float lhs, const CQuaternion& rhs);

[[nodiscard]] CQuaternion operator-(float lhs, const CQuaternion& rhs);

[[nodiscard]] CQuaternion operator*(float lhs, const CQuaternion& rhs);

[[nodiscard]] CNUQuaternion operator*(float lhs, const CNUQuaternion& rhs);
} // namespace zeus
