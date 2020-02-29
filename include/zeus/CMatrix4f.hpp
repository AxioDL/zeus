#pragma once

#include <array>
#include <cassert>

#include "zeus/CMatrix3f.hpp"
#include "zeus/CVector3f.hpp"
#include "zeus/CVector4f.hpp"

namespace zeus {
class CMatrix4f {
public:
  explicit constexpr CMatrix4f(bool zero = false) {
    if (!zero) {
      m[0][0] = 1.0;
      m[1][1] = 1.0;
      m[2][2] = 1.0;
      m[3][3] = 1.0;
    }
  }

  constexpr CMatrix4f(float m00, float m01, float m02, float m03, float m10, float m11, float m12, float m13, float m20,
                      float m21, float m22, float m23, float m30, float m31, float m32, float m33)
  : m{{{m00, m10, m20, m30}, {m01, m11, m21, m31}, {m02, m12, m22, m32}, {m03, m13, m23, m33}}} {}

  CMatrix4f(const CVector3f& scaleVec) {
    m[0][0] = scaleVec[0];
    m[1][1] = scaleVec[1];
    m[2][2] = scaleVec[2];
    m[3][3] = 1.0f;
  }

  constexpr CMatrix4f(const CVector4f& r0, const CVector4f& r1, const CVector4f& r2, const CVector4f& r3)
  : m{{r0, r1, r2, r3}} {}

  constexpr CMatrix4f(const CMatrix4f& other) = default;

  constexpr CMatrix4f(const simd<float>& r0, const simd<float>& r1, const simd<float>& r2, const simd<float>& r3) {
    m[0].mSimd = r0;
    m[1].mSimd = r1;
    m[2].mSimd = r2;
    m[3].mSimd = r3;
  }

  constexpr CMatrix4f(const CMatrix3f& other) {
    m[0].mSimd = other.m[0].mSimd;
    m[1].mSimd = other.m[1].mSimd;
    m[2].mSimd = other.m[2].mSimd;
    m[3].mSimd = CVector4f(0.f, 0.f, 0.f, 1.0f).mSimd;
  }

  constexpr CMatrix4f& operator=(const CMatrix4f& other) = default;

  [[nodiscard]] CVector4f operator*(const CVector4f& other) const {
    return m[0].mSimd * other.mSimd.shuffle<0, 0, 0, 0>() + m[1].mSimd * other.mSimd.shuffle<1, 1, 1, 1>() +
           m[2].mSimd * other.mSimd.shuffle<2, 2, 2, 2>() + m[3].mSimd * other.mSimd.shuffle<3, 3, 3, 3>();
  }

  [[nodiscard]] CVector4f& operator[](size_t i) {
    assert(i < m.size());
    return m[i];
  }

  [[nodiscard]] const CVector4f& operator[](size_t i) const {
    assert(i < m.size());
    return m[i];
  }

  [[nodiscard]] CMatrix4f transposed() const;

  [[nodiscard]] CVector3f multiplyOneOverW(const CVector3f& point) const {
    CVector4f xfVec = *this * point;
    return xfVec.toVec3f() / xfVec.w();
  }

  [[nodiscard]] CVector3f multiplyOneOverW(const CVector3f& point, float& wOut) const {
    CVector4f xfVec = *this * point;
    wOut = xfVec.w();
    return xfVec.toVec3f() / xfVec.w();
  }

  std::array<CVector4f, 4> m;
};
extern const CMatrix4f skIdentityMatrix4f;

[[nodiscard]] inline CMatrix4f operator*(const CMatrix4f& lhs, const CMatrix4f& rhs) {
  std::array<simd<float>, 4> v;
  for (size_t i = 0; i < v.size(); ++i) {
    v[i] = lhs.m[0].mSimd * rhs[i].mSimd.shuffle<0, 0, 0, 0>() + lhs.m[1].mSimd * rhs[i].mSimd.shuffle<1, 1, 1, 1>() +
           lhs.m[2].mSimd * rhs[i].mSimd.shuffle<2, 2, 2, 2>() + lhs.m[3].mSimd * rhs[i].mSimd.shuffle<3, 3, 3, 3>();
  }
  return CMatrix4f(v[0], v[1], v[2], v[3]);
}
} // namespace zeus
