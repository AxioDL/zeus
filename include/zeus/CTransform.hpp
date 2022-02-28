#pragma once

#include <cstdint>
#include <cstdio>

#include "zeus/CMatrix3f.hpp"
#include "zeus/CMatrix4f.hpp"
#include "zeus/CUnitVector.hpp"
#include "zeus/CVector3f.hpp"
#include "zeus/Global.hpp"

namespace zeus {
class CTransform {
public:
  constexpr CTransform() : basis(false) {}

  constexpr CTransform(const CMatrix3f& basis, const CVector3f& offset = {})
  : basis(basis), origin(offset) {}

  /* Column constructor */
  constexpr CTransform(const CVector3f& c0, const CVector3f& c1, const CVector3f& c2, const CVector3f& c3)
  : basis(c0, c1, c2), origin(c3) {}

  [[nodiscard]] bool operator==(const CTransform& other) const {
    return origin == other.origin && basis == other.basis;
  }

  [[nodiscard]] bool operator!=(const CTransform& other) const { return !operator==(other); }

  [[nodiscard]] CTransform operator*(const CTransform& rhs) const {
    return CTransform(basis * rhs.basis, origin + (basis * rhs.origin));
  }

  [[nodiscard]] CTransform inverse() const {
    CMatrix3f inv = basis.inverted();
    return CTransform(inv, inv * -origin);
  }

  [[nodiscard]] static CTransform Translate(const CVector3f& position) { return {CMatrix3f(), position}; }

  [[nodiscard]] static CTransform Translate(float x, float y, float z) { return Translate({x, y, z}); }

  [[nodiscard]] CTransform operator+(const CVector3f& other) const { return CTransform(basis, origin + other); }

  CTransform& operator+=(const CVector3f& other) {
    origin += other;
    return *this;
  }

  CTransform operator-(const CVector3f& other) const { return CTransform(basis, origin - other); }

  CTransform& operator-=(const CVector3f& other) {
    origin -= other;
    return *this;
  }

  [[nodiscard]] CVector3f rotate(const CVector3f& vec) const { return basis * vec; }

  [[nodiscard]] static CTransform RotateX(float theta) {
    const float sinT = std::sin(theta);
    const float cosT = std::cos(theta);
    return CTransform(CMatrix3f(simd<float>{1.f, 0.f, 0.f, 0.f}, simd<float>{0.f, cosT, sinT, 0.f},
                                simd<float>{0.f, -sinT, cosT, 0.f}));
  }

  [[nodiscard]] static CTransform RotateY(float theta) {
    const float sinT = std::sin(theta);
    const float cosT = std::cos(theta);
    return CTransform(CMatrix3f(simd<float>{cosT, 0.f, -sinT, 0.f}, simd<float>{0.f, 1.f, 0.f, 0.f},
                                simd<float>{sinT, 0.f, cosT, 0.f}));
  }

  [[nodiscard]] static CTransform RotateZ(float theta) {
    const float sinT = std::sin(theta);
    const float cosT = std::cos(theta);
    return CTransform(CMatrix3f(simd<float>{cosT, sinT, 0.f, 0.f}, simd<float>{-sinT, cosT, 0.f, 0.f},
                                simd<float>{0.f, 0.f, 1.f, 0.f}));
  }

  void rotateLocalX(float theta) {
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

  void rotateLocalY(float theta) {
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

  void rotateLocalZ(float theta) {
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

  [[nodiscard]] CVector3f transposeRotate(const CVector3f& in) const {
    return CVector3f(basis[0].dot(in), basis[1].dot(in), basis[2].dot(in));
  }

  void scaleBy(float factor) {
    CTransform xfrm(CMatrix3f(CVector3f(factor, factor, factor)));
    *this = *this * xfrm;
  }

  [[nodiscard]] static CTransform Scale(const CVector3f& factor) {
    return CTransform(CMatrix3f(simd<float>{factor.x(), 0.f, 0.f, 0.f}, simd<float>{0.f, factor.y(), 0.f, 0.f},
                                simd<float>{0.f, 0.f, factor.z(), 0.f}));
  }

  [[nodiscard]] static CTransform Scale(float x, float y, float z) {
    return CTransform(
        CMatrix3f(simd<float>{x, 0.f, 0.f, 0.f}, simd<float>{0.f, y, 0.f, 0.f}, simd<float>{0.f, 0.f, z, 0.f}));
  }

  [[nodiscard]] static CTransform Scale(float factor) {
    return CTransform(CMatrix3f(simd<float>{factor, 0.f, 0.f, 0.f}, simd<float>{0.f, factor, 0.f, 0.f},
                                simd<float>{0.f, 0.f, factor, 0.f}));
  }

  [[nodiscard]] CTransform multiplyIgnoreTranslation(const CTransform& rhs) const {
    return CTransform(basis * rhs.basis, origin + rhs.origin);
  }

  [[nodiscard]] CTransform getRotation() const {
    CTransform ret = *this;
    ret.origin.zeroOut();
    return ret;
  }

  void setRotation(const CMatrix3f& mat) { basis = mat; }

  void setRotation(const CTransform& xfrm) { setRotation(xfrm.basis); }

  /**
   * @brief buildMatrix3f Returns the stored matrix
   * buildMatrix3f is here for compliance with Retro's Math API
   * @return The Matrix (Neo, you are the one)
   */
  [[nodiscard]] const CMatrix3f& buildMatrix3f() const { return basis; }

  [[nodiscard]] CVector3f operator*(const CVector3f& other) const { return origin + basis * other; }

  [[nodiscard]] CMatrix4f toMatrix4f() const {
    CMatrix4f ret(basis[0], basis[1], basis[2], origin);
    ret[0][3] = 0.0f;
    ret[1][3] = 0.0f;
    ret[2][3] = 0.0f;
    ret[3][3] = 1.0f;
    return ret;
  }

  [[nodiscard]] CVector3f upVector() const { return basis.m[2]; }

  [[nodiscard]] CVector3f frontVector() const { return basis.m[1]; }

  [[nodiscard]] CVector3f rightVector() const { return basis.m[0]; }

  void orthonormalize() {
    basis[0].normalize();
    basis[2] = basis[0].cross(basis[1]);
    basis[2].normalize();
    basis[1] = basis[2].cross(basis[0]);
  }

  void printMatrix() const {
    printf(
        "%f %f %f %f\n"
        "%f %f %f %f\n"
        "%f %f %f %f\n"
        "%f %f %f %f\n",
        basis[0][0], basis[1][0], basis[2][0], origin[0], basis[0][1], basis[1][1], basis[2][1], origin[1], basis[0][2],
        basis[1][2], basis[2][2], origin[2], 0.f, 0.f, 0.f, 1.f);
  }

  [[nodiscard]] static CTransform MakeRotationsBasedOnY(const CUnitVector3f& uVec) {
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

  CMatrix3f basis;
  CVector3f origin;
};

[[nodiscard]] inline CTransform CTransformFromScaleVector(const CVector3f& scale) {
  return CTransform(CMatrix3f(scale));
}

[[nodiscard]] CTransform CTransformFromEditorEuler(const CVector3f& eulerVec);

[[nodiscard]] CTransform CTransformFromEditorEulers(const CVector3f& eulerVec, const CVector3f& origin);

[[nodiscard]] CTransform CTransformFromAxisAngle(const CVector3f& axis, float angle);

[[nodiscard]] CTransform lookAt(const CVector3f& pos, const CVector3f& lookPos, const CVector3f& up = skUp);
} // namespace zeus
