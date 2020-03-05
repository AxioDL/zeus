#pragma once

#include "zeus/CVector3f.hpp"

namespace zeus {
class CSphere {
public:
  constexpr CSphere(const CVector3f& position, float radius) : position(position), radius(radius) {}

  [[nodiscard]] CVector3f getSurfaceNormal(const CVector3f& coord) const { return (coord - position).normalized(); }

  [[nodiscard]] bool intersects(const CSphere& other) const {
    const float dist = (position - other.position).magnitude();
    return dist < (radius + other.radius);
  }

  CVector3f position;
  float radius;
};
} // namespace zeus
