#pragma once

#include "zeus/CUnitVector.hpp"
#include "zeus/CVector3f.hpp"

namespace zeus {
struct CAxisAngle : CVector3f {
  constexpr CAxisAngle() = default;
  constexpr CAxisAngle(float x, float y, float z) : CVector3f(x, y, z) {}
  CAxisAngle(const CUnitVector3f& axis, float angle) : CVector3f(angle * axis) {}
  constexpr CAxisAngle(const CVector3f& axisAngle) : CVector3f(axisAngle) {}

  [[nodiscard]] float angle() const { return magnitude(); }
  [[nodiscard]] const CVector3f& getVector() const { return *this; }
};
} // namespace zeus
