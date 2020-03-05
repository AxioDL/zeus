#pragma once

#include <array>
#include "zeus/CPlane.hpp"

namespace zeus {
class CAABox;
class CMatrix4f;
class CProjection;
class CSphere;

class CFrustum {
  std::array<CPlane, 6> planes;
  bool valid = false;

public:
  void updatePlanes(const CMatrix4f& viewMtx, const CMatrix4f& projection);
  void updatePlanes(const CTransform& viewPointMtx, const CProjection& projection);
  [[nodiscard]] bool aabbFrustumTest(const CAABox& aabb) const;
  [[nodiscard]] bool sphereFrustumTest(const CSphere& sphere) const;
  [[nodiscard]] bool pointFrustumTest(const CVector3f& point) const;
};
} // namespace zeus
