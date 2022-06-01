#pragma once

#include <array>
#include <cassert>

#include "zeus/CVector3f.hpp"
#include "zeus/Global.hpp"

/* Column-major matrix class */
namespace zeus {
class CQuaternion;

class CMatrix3f {
public:
  explicit constexpr CMatrix3f(bool zero = false) {
    if (!zero) {
      m[0][0] = 1.0;
      m[1][1] = 1.0;
      m[2][2] = 1.0;
    }
  }

  constexpr CMatrix3f(float m00, float m01, float m02, float m10, float m11, float m12, float m20, float m21, float m22)
  : m{{{m00, m10, m20}, {m01, m11, m21}, {m02, m12, m22}}} {}

  CMatrix3f(const CVector3f& scaleVec) {
    m[0][0] = scaleVec[0];
    m[1][1] = scaleVec[1];
    m[2][2] = scaleVec[2];
  }

  CMatrix3f(float scale) : CMatrix3f(CVector3f(scale)) {}

  constexpr CMatrix3f(const CVector3f& r0, const CVector3f& r1, const CVector3f& r2) : m{{r0, r1, r2}} {}

  constexpr CMatrix3f(const CMatrix3f& other) = default;

  constexpr CMatrix3f(const simd<float>& r0, const simd<float>& r1, const simd<float>& r2) {
    m[0].mSimd = r0;
    m[1].mSimd = r1;
    m[2].mSimd = r2;
  }

#if ZE_ATHENA_TYPES

  constexpr CMatrix3f(const atVec4f& r0, const atVec4f& r1, const atVec4f& r2) {
    m[0].mSimd = r0.simd;
    m[1].mSimd = r1.simd;
    m[2].mSimd = r2.simd;
  }

  void readBig(athena::io::IStreamReader& input) {
    m[0][0] = input.readFloatBig();
    m[1][0] = input.readFloatBig();
    m[2][0] = input.readFloatBig();
    m[0][1] = input.readFloatBig();
    m[1][1] = input.readFloatBig();
    m[2][1] = input.readFloatBig();
    m[0][2] = input.readFloatBig();
    m[1][2] = input.readFloatBig();
    m[2][2] = input.readFloatBig();
  }

  [[nodiscard]] static CMatrix3f ReadBig(athena::io::IStreamReader& input) {
    CMatrix3f ret;
    ret.readBig(input);
    return ret;
  }

#endif

  CMatrix3f(const CQuaternion& quat);

  constexpr CMatrix3f& operator=(const CMatrix3f& other) = default;

  [[nodiscard]] CVector3f operator*(const CVector3f& other) const {
    return m[0].mSimd * other.mSimd.shuffle<0, 0, 0, 0>() + m[1].mSimd * other.mSimd.shuffle<1, 1, 1, 1>() +
           m[2].mSimd * other.mSimd.shuffle<2, 2, 2, 2>();
  }

  [[nodiscard]] CVector3f& operator[](size_t i) {
    assert(i < m.size());
    return m[i];
  }

  [[nodiscard]] const CVector3f& operator[](size_t i) const {
    assert(i < m.size());
    return m[i];
  }

  [[nodiscard]] CMatrix3f orthonormalized() const {
    CMatrix3f ret;
    ret[0] = m[0].normalized();
    ret[2] = ret[0].cross(m[1]);
    ret[2].normalize();
    ret[1] = ret[2].cross(ret[0]);
    return ret;
  }

  [[nodiscard]] bool operator==(const CMatrix3f& other) const {
    return m[0] == other.m[0] && m[1] == other.m[1] && m[2] == other.m[2];
  }

  [[nodiscard]] bool operator!=(const CMatrix3f& other) const { return !operator==(other); }

  void transpose();

  [[nodiscard]] CMatrix3f transposed() const;

  void invert() { *this = inverted(); }

  [[nodiscard]] CMatrix3f inverted() const;

  void addScaledMatrix(const CMatrix3f& other, float scale) {
    CVector3f scaleVec(scale);
    m[0] += other.m[0] * scaleVec;
    m[1] += other.m[1] * scaleVec;
    m[2] += other.m[2] * scaleVec;
  }

  [[nodiscard]] static CMatrix3f RotateX(float theta) {
    float sinT = std::sin(theta);
    float cosT = std::cos(theta);
    return CMatrix3f(simd<float>{1.f, 0.f, 0.f, 0.f}, simd<float>{0.f, cosT, sinT, 0.f},
                     simd<float>{0.f, -sinT, cosT, 0.f});
  }

  [[nodiscard]] static CMatrix3f RotateY(float theta) {
    float sinT = std::sin(theta);
    float cosT = std::cos(theta);
    return CMatrix3f(simd<float>{cosT, 0.f, -sinT, 0.f}, simd<float>{0.f, 1.f, 0.f, 0.f},
                     simd<float>{sinT, 0.f, cosT, 0.f});
  }

  [[nodiscard]] static CMatrix3f RotateZ(float theta) {
    float sinT = std::sin(theta);
    float cosT = std::cos(theta);
    return CMatrix3f(simd<float>{cosT, sinT, 0.f, 0.f}, simd<float>{-sinT, cosT, 0.f, 0.f},
                     simd<float>{0.f, 0.f, 1.f, 0.f});
  }

  [[nodiscard]] float determinant() const {
    return m[1][0] * (m[2][1] * m[0][2] - m[0][1] * m[2][2]) + m[0][0] * (m[1][1] * m[2][2] - m[2][1] * m[1][2]) +
           m[2][0] * (m[0][1] * m[1][2] - m[1][1] * m[0][2]);
  }

  std::array<CVector3f, 3> m;
};

[[nodiscard]] inline CMatrix3f operator*(const CMatrix3f& lhs, const CMatrix3f& rhs) {
  std::array<simd<float>, 3> v;
  for (size_t i = 0; i < v.size(); ++i) {
    v[i] = lhs.m[0].mSimd * rhs[i].mSimd.shuffle<0, 0, 0, 0>() + lhs.m[1].mSimd * rhs[i].mSimd.shuffle<1, 1, 1, 1>() +
           lhs.m[2].mSimd * rhs[i].mSimd.shuffle<2, 2, 2, 2>();
  }
  return CMatrix3f(v[0], v[1], v[2]);
}
} // namespace zeus
