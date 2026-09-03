#pragma once

#include <cstdint>
#include <cstdio>

#include "zeus/CMatrix3f.hpp"
#include "zeus/CMatrix4f.hpp"
#include "zeus/CUnitVector.hpp"
#include "zeus/CVector3f.hpp"
#include "zeus/Global.hpp"

namespace zeus {
class CTransform4f {
public:
  constexpr CTransform4f() : basis(false) {}

  constexpr CTransform4f(const CMatrix3f& basis, const CVector3f& offset = {}) : basis(basis), origin(offset) {}

  /* Column constructor */
  constexpr CTransform4f(const CVector3f& c0, const CVector3f& c1, const CVector3f& c2, const CVector3f& c3)
  : basis(c0, c1, c2), origin(c3) {}

  [[nodiscard]] bool operator==(const CTransform4f& other) const {
    return origin == other.origin && basis == other.basis;
  }

  [[nodiscard]] bool operator!=(const CTransform4f& other) const { return !operator==(other); }

  [[nodiscard]] CTransform4f operator*(const CTransform4f& rhs) const {
    return CTransform4f(basis * rhs.basis, origin + (basis * rhs.origin));
  }

  [[nodiscard]] CTransform4f Inverse() const {
    CMatrix3f inv = basis.inverted();
    return CTransform4f(inv, inv * -origin);
  }

  [[nodiscard]] CTransform4f QuickInverse() const {
    return CTransform4f{basis.transposed(),
                      CVector3f{
                          basis[0][0] * -origin.x() - basis[0][1] * origin.y() - basis[0][2] * origin.z(),
                          basis[1][0] * -origin.x() - basis[1][1] * origin.y() - basis[1][2] * origin.z(),
                          basis[2][0] * -origin.x() - basis[2][1] * origin.y() - basis[2][2] * origin.z(),
                      }};
  }

  [[nodiscard]] static CTransform4f Translate(const CVector3f& position) { return {CMatrix3f(), position}; }

  [[nodiscard]] static CTransform4f Translate(float x, float y, float z) { return Translate({x, y, z}); }

  [[nodiscard]] CTransform4f operator+(const CVector3f& other) const { return CTransform4f(basis, origin + other); }

  CTransform4f& operator+=(const CVector3f& other) {
    origin += other;
    return *this;
  }

  CTransform4f operator-(const CVector3f& other) const { return CTransform4f(basis, origin - other); }

  CTransform4f& operator-=(const CVector3f& other) {
    origin -= other;
    return *this;
  }

  [[nodiscard]] CVector3f Rotate(const CVector3f& vec) const { return basis * vec; }

  [[nodiscard]] static CTransform4f RotateX(float theta) {
    const float sinT = std::sin(theta);
    const float cosT = std::cos(theta);
    return CTransform4f(CMatrix3f(simd<float>{1.f, 0.f, 0.f, 0.f}, simd<float>{0.f, cosT, sinT, 0.f},
                                simd<float>{0.f, -sinT, cosT, 0.f}));
  }

  [[nodiscard]] static CTransform4f RotateY(float theta) {
    const float sinT = std::sin(theta);
    const float cosT = std::cos(theta);
    return CTransform4f(CMatrix3f(simd<float>{cosT, 0.f, -sinT, 0.f}, simd<float>{0.f, 1.f, 0.f, 0.f},
                                simd<float>{sinT, 0.f, cosT, 0.f}));
  }

  [[nodiscard]] static CTransform4f RotateZ(float theta) {
    const float sinT = std::sin(theta);
    const float cosT = std::cos(theta);
    return CTransform4f(CMatrix3f(simd<float>{cosT, sinT, 0.f, 0.f}, simd<float>{-sinT, cosT, 0.f, 0.f},
                                simd<float>{0.f, 0.f, 1.f, 0.f}));
  }

  void RotateLocalX(float theta) {
    float sinT = std::sin(theta);
    float cosT = std::cos(theta);

    zeus::CVector3f b2 = basis[2] * sinT;
    zeus::CVector3f b1 = basis[1] * sinT;
    zeus::CVector3f cosV(cosT);

    basis[1] *= cosV;
    basis[2] *= cosV;

    basis[1] += b2;
    basis[2] -= b1;
  }

  void RotateLocalY(float theta) {
    float sinT = std::sin(theta);
    float cosT = std::cos(theta);

    zeus::CVector3f b0 = basis[0] * sinT;
    zeus::CVector3f b2 = basis[2] * sinT;
    zeus::CVector3f cosV(cosT);

    basis[0] *= cosV;
    basis[2] *= cosV;

    basis[2] += b0;
    basis[0] -= b2;
  }

  void RotateLocalZ(float theta) {
    float sinT = std::sin(theta);
    float cosT = std::cos(theta);

    zeus::CVector3f b0 = basis[0] * sinT;
    zeus::CVector3f b1 = basis[1] * sinT;
    zeus::CVector3f cosV(cosT);

    basis[0] *= cosV;
    basis[1] *= cosV;

    basis[0] += b1;
    basis[1] -= b0;
  }

  [[nodiscard]] CVector3f TransposeRotate(const CVector3f& in) const {
    return CVector3f(basis[0].dot(in), basis[1].dot(in), basis[2].dot(in));
  }

  void ScaleBy(float factor) {
    CTransform4f xfrm(CMatrix3f(CVector3f(factor, factor, factor)));
    *this = *this * xfrm;
  }

  [[nodiscard]] static CTransform4f Scale(const CVector3f& factor) {
    return CTransform4f(CMatrix3f(simd<float>{factor.x(), 0.f, 0.f, 0.f}, simd<float>{0.f, factor.y(), 0.f, 0.f},
                                simd<float>{0.f, 0.f, factor.z(), 0.f}));
  }

  [[nodiscard]] static CTransform4f Scale(float x, float y, float z) {
    return CTransform4f(
        CMatrix3f(simd<float>{x, 0.f, 0.f, 0.f}, simd<float>{0.f, y, 0.f, 0.f}, simd<float>{0.f, 0.f, z, 0.f}));
  }

  [[nodiscard]] static CTransform4f Scale(float factor) {
    return CTransform4f(CMatrix3f(simd<float>{factor, 0.f, 0.f, 0.f}, simd<float>{0.f, factor, 0.f, 0.f},
                                simd<float>{0.f, 0.f, factor, 0.f}));
  }

  [[nodiscard]] CTransform4f MultiplyIgnoreTranslation(const CTransform4f& rhs) const {
    return CTransform4f(basis * rhs.basis, origin + rhs.origin);
  }

  [[nodiscard]] CTransform4f GetRotation() const {
    CTransform4f ret = *this;
    ret.origin.zeroOut();
    return ret;
  }

  void SetRotation(const CMatrix3f& mat) { basis = mat; }

  void SetRotation(const CTransform4f& xfrm) { SetRotation(xfrm.basis); }

  /**
   * @brief BuildMatrix3f Returns the stored matrix
   * BuildMatrix3f is here for compliance with Retro's Math API
   * @return The Matrix (Neo, you are the one)
   */
  [[nodiscard]] const CMatrix3f& BuildMatrix3f() const { return basis; }

  [[nodiscard]] CVector3f operator*(const CVector3f& other) const { return origin + basis * other; }

  [[nodiscard]] CMatrix4f ToMatrix4f() const {
    CMatrix4f ret(basis[0], basis[1], basis[2], origin);
    ret[0][3] = 0.0f;
    ret[1][3] = 0.0f;
    ret[2][3] = 0.0f;
    ret[3][3] = 1.0f;
    return ret;
  }

  /**
   * Outputs the matrix to a C-style array (column-major, GX style)
   */
  void GetCStyleMatrix(float mtx[3][4]) const {
    mtx[0][0] = basis[0][0];
    mtx[0][1] = basis[1][0];
    mtx[0][2] = basis[2][0];
    mtx[0][3] = origin.x();
    mtx[1][0] = basis[0][1];
    mtx[1][1] = basis[1][1];
    mtx[1][2] = basis[2][1];
    mtx[1][3] = origin.y();
    mtx[2][0] = basis[0][2];
    mtx[2][1] = basis[1][2];
    mtx[2][2] = basis[2][2];
    mtx[2][3] = origin.z();
  }

  [[nodiscard]] CVector3f GetUp() const { return basis.m[2]; }

  [[nodiscard]] CVector3f GetForward() const { return basis.m[1]; }

  [[nodiscard]] CVector3f GetRight() const { return basis.m[0]; }

  void Orthonormalize() {
    basis[0].normalize();
    basis[2] = basis[0].cross(basis[1]);
    basis[2].normalize();
    basis[1] = basis[2].cross(basis[0]);
  }

  void PrintMatrix() const {
    printf(
        "%f %f %f %f\n"
        "%f %f %f %f\n"
        "%f %f %f %f\n"
        "%f %f %f %f\n",
        basis[0][0], basis[1][0], basis[2][0], origin[0], basis[0][1], basis[1][1], basis[2][1], origin[1], basis[0][2],
        basis[1][2], basis[2][2], origin[2], 0.f, 0.f, 0.f, 1.f);
  }

  [[nodiscard]] static CTransform4f MakeRotationsBasedOnY(const CUnitVector3f& uVec) {
    uint32_t i;
    if (uVec.y() < uVec.x() || uVec.z() < uVec.y() || uVec.z() < uVec.x())
      i = 2;
    else
      i = 1;

    CVector3f v;
    v[i] = 1.f;
    CUnitVector3f newUVec(uVec.cross(v));
    return {newUVec, uVec, uVec.cross(newUVec), CVector3f()};
  }

  [[nodiscard]] static CTransform4f LookAt(const CVector3f& pos, const CVector3f& lookPos, const CVector3f& up = skUp);
  CMatrix3f basis;
  CVector3f origin;
};

[[nodiscard]] inline CTransform4f CTransformFromScaleVector(const CVector3f& scale) {
  return CTransform4f(CMatrix3f(scale));
}

[[nodiscard]] CTransform4f CTransformFromEditorEuler(const CVector3f& eulerVec);

[[nodiscard]] CTransform4f CTransformFromEditorEulers(const CVector3f& eulerVec, const CVector3f& origin);

[[nodiscard]] CTransform4f CTransformFromAxisAngle(const CVector3f& axis, float angle);

} // namespace zeus
