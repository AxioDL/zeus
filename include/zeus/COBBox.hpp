#pragma once

#include "zeus/CAABox.hpp"
#include "zeus/CTransform.hpp"
#include "zeus/CVector3f.hpp"

#if ZE_ATHENA_TYPES
#include <athena/IStreamReader.hpp>
#endif

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
    CVector3f center = box.center();
    const CVector3f extents = box.max - center;
    const CTransform newXf = xf * CTransform::Translate(center);
    return COBBox(newXf, extents);
  }

  bool OBBIntersectsBox(const COBBox& other) const;

  bool AABoxIntersectsBox(const CAABox& other) { return OBBIntersectsBox(FromAABox(other, CTransform())); }
};
} // namespace zeus
