#pragma once

#include "zeus/CTransform.hpp"
#include "zeus/CVector3f.hpp"
#include "zeus/CAABox.hpp"
#include "zeus/CMRay.hpp"

namespace zeus {
class COBBox {
public:
#if ZE_ATHENA_TYPES

  void readBig(athena::io::IStreamReader& in) {
    transform.read34RowMajor(in);
    extents.readBig(in);
  }

  static COBBox ReadBig(athena::io::IStreamReader& in) {
    COBBox out;
    out.readBig(in);
    return out;
  }

#endif

  CTransform transform;
  CVector3f extents;

  constexpr COBBox() = default;

  COBBox(const CAABox& aabb) : extents(aabb.extents()) { transform.origin = aabb.center(); }

  constexpr COBBox(const CTransform& xf, const CVector3f& extents) : transform(xf), extents(extents) {}

  CAABox calculateAABox(const CTransform& worldXf = CTransform()) const;

  static COBBox FromAABox(const CAABox& box, const CTransform& xf) {
    const CVector3f extents = box.max - box.center();
    const CTransform newXf = CTransform::Translate(box.center()) * xf;
    return COBBox(newXf, extents);
  }

  bool OBBIntersectsBox(const COBBox& other) const;

  bool AABoxIntersectsBox(const CAABox& other) { return OBBIntersectsBox(FromAABox(other, CTransform())); }
};
} // namespace zeus
