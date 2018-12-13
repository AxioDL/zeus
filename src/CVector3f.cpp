#include "zeus/CVector3f.hpp"
#include "zeus/CVector3d.hpp"
#include <memory.h>
#include <cmath>
#include <cassert>
#include "zeus/Math.hpp"
#include "zeus/CRelAngle.hpp"

namespace zeus {
const CVector3f CVector3f::skOne(1.f);
const CVector3f CVector3f::skNegOne(-1.f);
const CVector3f CVector3f::skZero;
const CVector3f CVector3f::skForward(0.f, 1.f, 0.f);
const CVector3f CVector3f::skBack(0.f, -1.f, 0.f);
const CVector3f CVector3f::skLeft(-1.f, 0.f, 0.f);
const CVector3f CVector3f::skRight(1.f, 0.f, 0.f);
const CVector3f CVector3f::skUp(0.f, 0.f, 1.f);
const CVector3f CVector3f::skDown(0.f, 0.f, -1.f);
const CVector3f CVector3f::skRadToDegVec(180.0f / M_PIF);
const CVector3f CVector3f::skDegToRadVec(M_PIF / 180.0f);
const CVector3d CVector3d::skZero(0.0, 0.0, 0.0);

CVector3f::CVector3f(const CVector3d& vec) : mSimd(vec.mSimd) {}

float CVector3f::getAngleDiff(const CVector3f& a, const CVector3f& b) {
  float mag1 = a.magnitude();
  float mag2 = b.magnitude();

  if (!mag1 || !mag2)
    return 0.f;

  float dot = a.dot(b);
  float theta = std::acos(dot / (mag1 * mag2));
  return theta;
}

CVector3f CVector3f::slerp(const CVector3f& a, const CVector3f& b, CRelAngle clampAngle) {
  return a * std::cos(clampAngle) + a.cross(b).normalized().cross(a) * std::sin(clampAngle);
}
} // namespace zeus
