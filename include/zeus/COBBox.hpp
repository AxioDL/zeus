#pragma once

#include "zeus/CAABox.hpp"
#include "zeus/CTransform.hpp"
#include "zeus/CVector3f.hpp"

namespace zeus {
class COBBox {
public:
  CTransform transform;
  CVector3f extents;

  constexpr COBBox() = default;

  COBBox(const CAABox& aabb) : extents(aabb.extents()) { transform.origin = aabb.center(); }

  constexpr COBBox(const CTransform& xf, const CVector3f& extents) : transform(xf), extents(extents) {}

  [[nodiscard]] CAABox calculateAABox(const CTransform& worldXf = CTransform()) const;

  [[nodiscard]] static COBBox FromAABox(const CAABox& box, const CTransform& xf) {
    const CVector3f center = box.center();
    const CVector3f extents = box.max - center;
    const CTransform newXf = xf * CTransform::Translate(center);
    return COBBox(newXf, extents);
  }

  [[nodiscard]] bool OBBIntersectsBox(const COBBox& other) const;

  [[nodiscard]] bool AABoxIntersectsBox(const CAABox& other) const {
    return OBBIntersectsBox(FromAABox(other, CTransform()));
  }
};
} // namespace zeus
