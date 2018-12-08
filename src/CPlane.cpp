#include "zeus/CPlane.hpp"

namespace zeus {

bool CPlane::rayPlaneIntersection(const CVector3f& from, const CVector3f& to, CVector3f& point) const {
  zeus::CVector3f delta = to - from;
  if (std::fabs(delta.normalized().dot(normal())) < 0.01f)
    return false;
  float tmp = -pointToPlaneDist(from) / delta.dot(normal());
  if (tmp < -0.f || tmp > 1.0001f)
    return false;
  point = delta * tmp + from;
  return true;
}

}
