#pragma once

#include "Global.hpp"
#include "zeus/CVector3f.hpp"
#include "zeus/Math.hpp"

namespace zeus {
class CPlane {
public:
  CPlane() : mSimd(1.0, 0.f, 0.f, 0.f) {}

  CPlane(float a, float b, float c, float d) : mSimd(a, b, c, d) {}

  CPlane(const CVector3f& a, const CVector3f& b, const CVector3f& c) {
    mSimd = (b - a).cross(c - a).normalized().mSimd;
    mSimd[3] = a.dot(normal());
  }

  CPlane(const CVector3f& point, float displacement) {
    mSimd = point.mSimd;
    mSimd[3] = displacement;
  }

  float clipLineSegment(const CVector3f& a, const CVector3f& b) {
    float mag = (b - a).dot(normal());
    float dis = (-(y() - d())) / mag;
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

  float pointToPlaneDist(const CVector3f& pos) const {
    return pos.dot(normal()) - d();
  }

  bool rayPlaneIntersection(const CVector3f& from, const CVector3f& to, CVector3f& point) const;

  CVector3f normal() const { return mSimd; }

  zeus::simd<float>::reference operator[](size_t idx) {
    assert(idx < 4);
    return mSimd[idx];
  }

  float operator[](size_t idx) const {
    assert(idx < 4);
    return mSimd[idx];
  }

  float x() const { return mSimd[0]; }
  float y() const { return mSimd[1]; }
  float z() const { return mSimd[2]; }
  float d() const { return mSimd[3]; }

  simd<float>::reference x() { return mSimd[0]; }
  simd<float>::reference y() { return mSimd[1]; }
  simd<float>::reference z() { return mSimd[2]; }
  simd<float>::reference d() { return mSimd[3]; }

  zeus::simd<float> mSimd;
};
}

