#pragma once

#include "Global.hpp"
#include "zeus/CMatrix3f.hpp"
#include "zeus/CMatrix4f.hpp"
#include "zeus/CVector3f.hpp"
#include "zeus/CUnitVector.hpp"
#include <cstdint>
#include <cstdio>

namespace zeus {
class CTransform {
public:
  constexpr CTransform() : basis(false) {}

  constexpr CTransform(const CMatrix3f& basis, const CVector3f& offset = {})
  : basis(basis), origin(offset) {}

#if ZE_ATHENA_TYPES

  CTransform(const atVec4f* mtx)
  : basis(mtx[0], mtx[1], mtx[2]), origin(mtx[0].simd[3], mtx[1].simd[3], mtx[2].simd[3]) {}

  void read34RowMajor(athena::io::IStreamReader& r) {
    atVec4f r0 = r.readVec4fBig();
    atVec4f r1 = r.readVec4fBig();
    atVec4f r2 = r.readVec4fBig();
    basis = CMatrix3f(r0, r1, r2);
    basis.transpose();
    origin = CVector3f(r0.simd[3], r1.simd[3], r2.simd[3]);
  }

#endif

  /* Column constructor */
  constexpr CTransform(const CVector3f& c0, const CVector3f& c1, const CVector3f& c2, const CVector3f& c3)
  : basis(c0, c1, c2), origin(c3) {}

  bool operator==(const CTransform& other) const { return origin == other.origin && basis == other.basis; }

  CTransform operator*(const CTransform& rhs) const {
    return CTransform(basis * rhs.basis, origin + (basis * rhs.origin));
  }

  CTransform inverse() const {
    CMatrix3f inv = basis.inverted();
    return CTransform(inv, inv * -origin);
  }

  static CTransform Translate(const CVector3f& position) { return {CMatrix3f(), position}; }

  static CTransform Translate(float x, float y, float z) { return Translate({x, y, z}); }

  CTransform operator+(const CVector3f& other) { return CTransform(basis, origin + other); }

  CTransform& operator+=(const CVector3f& other) {
    origin += other;
    return *this;
  }

  CTransform operator-(const CVector3f& other) { return CTransform(basis, origin - other); }

  CTransform& operator-=(const CVector3f& other) {
    origin -= other;
    return *this;
  }

  zeus::CVector3f rotate(const CVector3f& vec) const { return basis * vec; }

  static CTransform RotateX(float theta) {
    float sinT = std::sin(theta);
    float cosT = std::cos(theta);
    return CTransform(CMatrix3f(simd<float>{1.f, 0.f, 0.f, 0.f}, simd<float>{0.f, cosT, sinT, 0.f},
                                simd<float>{0.f, -sinT, cosT, 0.f}));
  }

  static CTransform RotateY(float theta) {
    float sinT = std::sin(theta);
    float cosT = std::cos(theta);
    return CTransform(CMatrix3f(simd<float>{cosT, 0.f, -sinT, 0.f}, simd<float>{0.f, 1.f, 0.f, 0.f},
                                simd<float>{sinT, 0.f, cosT, 0.f}));
  }

  static CTransform RotateZ(float theta) {
    float sinT = std::sin(theta);
    float cosT = std::cos(theta);
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

  CVector3f transposeRotate(const CVector3f& in) const {
    return CVector3f(basis[0].dot(in), basis[1].dot(in), basis[2].dot(in));
  }

  void scaleBy(float factor) {
    CTransform xfrm(CMatrix3f(CVector3f(factor, factor, factor)));
    *this = *this * xfrm;
  }

  static CTransform Scale(const CVector3f& factor) {
    return CTransform(CMatrix3f(simd<float>{factor.x(), 0.f, 0.f, 0.f}, simd<float>{0.f, factor.y(), 0.f, 0.f},
                                simd<float>{0.f, 0.f, factor.z(), 0.f}));
  }

  static CTransform Scale(float x, float y, float z) {
    return CTransform(
        CMatrix3f(simd<float>{x, 0.f, 0.f, 0.f}, simd<float>{0.f, y, 0.f, 0.f}, simd<float>{0.f, 0.f, z, 0.f}));
  }

  static CTransform Scale(float factor) {
    return CTransform(CMatrix3f(simd<float>{factor, 0.f, 0.f, 0.f}, simd<float>{0.f, factor, 0.f, 0.f},
                                simd<float>{0.f, 0.f, factor, 0.f}));
  }

  CTransform multiplyIgnoreTranslation(const CTransform& rhs) const {
    return CTransform(basis * rhs.basis, origin + rhs.origin);
  }

  CTransform getRotation() const {
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
  const CMatrix3f& buildMatrix3f() const { return basis; }

  CVector3f operator*(const CVector3f& other) const { return origin + basis * other; }

  CMatrix4f toMatrix4f() const {
    CMatrix4f ret(basis[0], basis[1], basis[2], origin);
    ret[0][3] = 0.0f;
    ret[1][3] = 0.0f;
    ret[2][3] = 0.0f;
    ret[3][3] = 1.0f;
    return ret;
  }

  CVector3f upVector() const { return basis.m[2]; }

  CVector3f frontVector() const { return basis.m[1]; }

  CVector3f rightVector() const { return basis.m[0]; }

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

  static zeus::CTransform MakeRotationsBasedOnY(const CUnitVector3f& uVec) {
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

inline CTransform CTransformFromScaleVector(const CVector3f& scale) { return CTransform(CMatrix3f(scale)); }

CTransform CTransformFromEditorEuler(const CVector3f& eulerVec);

CTransform CTransformFromEditorEulers(const CVector3f& eulerVec, const CVector3f& origin);

CTransform CTransformFromAxisAngle(const CVector3f& axis, float angle);

CTransform lookAt(const CVector3f& pos, const CVector3f& lookPos, const CVector3f& up = skUp);
} // namespace zeus
