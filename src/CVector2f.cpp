#include "zeus/CVector2f.hpp"

#include <algorithm>
#include <cassert>
#include <cfloat>
#include <cmath>

namespace zeus {
float CVector2f::getAngleDiff(const CVector2f& a, const CVector2f& b) {
  float mag1 = a.magnitude();
  float mag2 = b.magnitude();

  if (mag1 <= FLT_EPSILON || mag2 <= FLT_EPSILON)
    return 0.f;

  float dot = a.dot(b);
  return std::acos(std::clamp(-1.f, dot / (mag1 * mag2), 1.f));
}

CVector2f CVector2f::slerp(const CVector2f& a, const CVector2f& b, float t) {
  if (t <= 0.0f)
    return a;
  if (t >= 1.0f)
    return b;

  CVector2f ret;

  float mag = std::sqrt(a.dot(a) * b.dot(b));

  float prod = a.dot(b) / mag;

  if (std::fabs(prod) < 1.0f) {
    const double sign = (prod < 0.0f) ? -1.0f : 1.0f;

    const double theta = std::acos(sign * prod);
    const double s1 = std::sin(sign * t * theta);
    const double d = 1.0 / std::sin(theta);
    const double s0 = std::sin((1.0f - t) * theta);

    ret = (a * s0 + b * s1) * d;
    return ret;
  }
  return a;
}
} // namespace zeus
