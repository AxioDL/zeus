#include "zeus/CVector3f.hpp"

#include <algorithm>
#include <cassert>
#include <cfloat>
#include <cmath>

#include "zeus/CRelAngle.hpp"

namespace zeus {
float CVector3f::getAngleDiff(const CVector3f& a, const CVector3f& b) {
  float mag1 = a.magnitude();
  float mag2 = b.magnitude();

  if (mag1 <= FLT_EPSILON || mag2 <= FLT_EPSILON)
    return 0.f;

  float dot = a.dot(b);
  return std::acos(zeus::clamp(-1.f, dot / (mag1 * mag2), 1.f));
}

CVector3f CVector3f::slerp(const CVector3f& a, const CVector3f& b, CRelAngle clampAngle) {
  return a * std::cos(clampAngle) + a.cross(b).normalized().cross(a) * std::sin(clampAngle);
}
} // namespace zeus
