#pragma once

#include "zeus/CVector3f.hpp"

namespace zeus {
class CSphere {
public:
  CSphere(const CVector3f& position, float radius) : position(position), radius(radius) {}

  CVector3f getSurfaceNormal(const CVector3f& coord) const { return (coord - position).normalized(); }

  bool intersects(const CSphere& other) {
    float dist = (position - other.position).magnitude();
    return dist < (radius + other.radius);
  }

  CVector3f position;
  float radius;
};
}

