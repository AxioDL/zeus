#pragma once

#include <cassert>

#include "zeus/CVector3f.hpp"
#include "zeus/Global.hpp"
#include "zeus/Math.hpp"

namespace zeus {
class CPlane {
public:
  constexpr CPlane() : mSimd(1.f, 0.f, 0.f, 0.f) {}

  constexpr CPlane(float a, float b, float c, float d) : mSimd(a, b, c, d) {}

  CPlane(const CVector3f& a, const CVector3f& b, const CVector3f& c) {
    mSimd = (b - a).cross(c - a).normalized().mSimd;
    mSimd[3] = a.dot(normal());
  }

  CPlane(const CVector3f& point, float displacement) {
    mSimd = point.mSimd;
    mSimd[3] = displacement;
  }

  [[nodiscard]] float clipLineSegment(const CVector3f& a, const CVector3f& b) {
    const float mag = (b - a).dot(normal());
    const float dis = (-(y() - d())) / mag;
    return clamp(0.0f, dis, 1.0f);
  }

  void normalize() {
    float nd = d();
    auto norm = normal();
    float mag = norm.magnitude();
    mag = 1.f / mag;
    mSimd = (norm * mag).mSimd;
    mSimd[3] = nd * mag;
  }

  [[nodiscard]] float pointToPlaneDist(const CVector3f& pos) const { return pos.dot(normal()) - d(); }

  [[nodiscard]] bool rayPlaneIntersection(const CVector3f& from, const CVector3f& to, CVector3f& point) const;

  [[nodiscard]] CVector3f normal() const { return mSimd; }

  [[nodiscard]] zeus::simd<float>::reference operator[](size_t idx) {
    assert(idx < 4);
    return mSimd[idx];
  }

  [[nodiscard]] float operator[](size_t idx) const {
    assert(idx < 4);
    return mSimd[idx];
  }

  [[nodiscard]] float x() const { return mSimd[0]; }
  [[nodiscard]] float y() const { return mSimd[1]; }
  [[nodiscard]] float z() const { return mSimd[2]; }
  [[nodiscard]] float d() const { return mSimd[3]; }

  [[nodiscard]] simd<float>::reference x() { return mSimd[0]; }
  [[nodiscard]] simd<float>::reference y() { return mSimd[1]; }
  [[nodiscard]] simd<float>::reference z() { return mSimd[2]; }
  [[nodiscard]] simd<float>::reference d() { return mSimd[3]; }

  zeus::simd<float> mSimd;
};
} // namespace zeus
