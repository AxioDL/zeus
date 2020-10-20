#pragma once

#include <cassert>

#include "zeus/CLineSeg.hpp"
#include "zeus/CPlane.hpp"
#include "zeus/CSphere.hpp"
#include "zeus/CTransform.hpp"
#include "zeus/CVector3f.hpp"
#include "zeus/Math.hpp"

#if ZE_ATHENA_TYPES
#include <athena/IStreamReader.hpp>
#endif

namespace zeus {
class CAABox {
public:
  enum class EBoxEdgeId { Z0, X0, Z1, X1, Z2, X2, Z3, X3, Y0, Y1, Y2, Y3 };

  enum class EBoxFaceId {
    YMin,
    YMax,
    XMin,
    XMax,
    ZMax,
    ZMin
  };

  CVector3f min;
  CVector3f max;

  // set default AABox to insane inverse min/max to allow for accumulation
  constexpr CAABox() : CAABox(1e16f, -1e16f) {}

  constexpr CAABox(const CVector3f& min, const CVector3f& max) : min(min), max(max) {}

  constexpr CAABox(float min, float max) : min(CVector3f(min)), max(CVector3f(max)) {}

  constexpr CAABox(float minX, float minY, float minZ, float maxX, float maxY, float maxZ)
  : min(minX, minY, minZ), max(maxX, maxY, maxZ) {}

#if ZE_ATHENA_TYPES

  void readBoundingBoxBig(athena::io::IStreamReader& in) {
    min.readBig(in);
    max.readBig(in);
  }

  [[nodiscard]] static CAABox ReadBoundingBoxBig(athena::io::IStreamReader& in) {
    CAABox ret;
    ret.readBoundingBoxBig(in);
    return ret;
  }

#endif

  [[nodiscard]] bool intersects(const CAABox& other) const {
    const auto mmax = max >= other.min;
    const auto mmin = min <= other.max;
    return mmax[0] && mmax[1] && mmax[2] && mmin[0] && mmin[1] && mmin[2];
  }

  [[nodiscard]] bool intersects(const CSphere& other) const;
  [[nodiscard]] float intersectionRadius(const CSphere& other) const;
  [[nodiscard]] CAABox booleanIntersection(const CAABox& other) const;

  [[nodiscard]] bool inside(const CAABox& other) const {
    const auto mmax = max <= other.max;
    const auto mmin = min >= other.min;
    return mmax[0] && mmax[1] && mmax[2] && mmin[0] && mmin[1] && mmin[2];
  }

  [[nodiscard]] bool insidePlane(const CPlane& plane) const {
    CVector3f vmax;
    /* X axis */
    if (plane.x() >= 0.f)
      vmax[0] = max[0];
    else
      vmax[0] = min[0];
    /* Y axis */
    if (plane.y() >= 0.f)
      vmax[1] = max[1];
    else
      vmax[1] = min[1];
    /* Z axis */
    if (plane.z() >= 0.f)
      vmax[2] = max[2];
    else
      vmax[2] = min[2];
    return plane.normal().dot(vmax) + plane.d() >= 0.f;
  }

  [[nodiscard]] CVector3f center() const { return (min + max) * 0.5f; }

  [[nodiscard]] CVector3f extents() const { return (max - min) * 0.5f; }

  [[nodiscard]] float volume() const {
    const auto delta = max - min;
    return delta.x() * delta.y() * delta.z();
  }

  [[nodiscard]] CLineSeg getEdge(EBoxEdgeId id) const {
    switch (id) {
    case EBoxEdgeId::Z0:
    default:
      return CLineSeg({min.x(), min.y(), max.z()}, {min.x(), min.y(), min.z()});
    case EBoxEdgeId::X0:
      return CLineSeg({min.x(), min.y(), min.z()}, {max.x(), min.y(), min.z()});
    case EBoxEdgeId::Z1:
      return CLineSeg({max.x(), min.y(), min.z()}, {max.x(), min.y(), max.z()});
    case EBoxEdgeId::X1:
      return CLineSeg({max.x(), min.y(), max.z()}, {min.x(), min.y(), max.z()});
    case EBoxEdgeId::Z2:
      return CLineSeg({max.x(), max.y(), max.z()}, {max.x(), max.y(), min.z()});
    case EBoxEdgeId::X2:
      return CLineSeg({max.x(), max.y(), min.z()}, {min.x(), max.y(), min.z()});
    case EBoxEdgeId::Z3:
      return CLineSeg({min.x(), max.y(), min.z()}, {min.x(), max.y(), max.z()});
    case EBoxEdgeId::X3:
      return CLineSeg({min.x(), max.y(), max.z()}, {max.x(), max.y(), max.z()});
    case EBoxEdgeId::Y0:
      return CLineSeg({min.x(), min.y(), max.z()}, {min.x(), max.y(), max.z()});
    case EBoxEdgeId::Y1:
      return CLineSeg({min.x(), min.y(), min.z()}, {min.x(), max.y(), min.z()});
    case EBoxEdgeId::Y2:
      return CLineSeg({max.x(), min.y(), min.z()}, {max.x(), max.y(), min.z()});
    case EBoxEdgeId::Y3:
      return CLineSeg({max.x(), min.y(), max.z()}, {max.x(), max.y(), max.z()});
    }
  }

  struct Tri {
    zeus::CPlane x0_plane;
    zeus::CVector3f x10_verts[3];
  };
  [[nodiscard]] Tri getTri(EBoxFaceId face, int windOffset) const;

  [[nodiscard]] CAABox getTransformedAABox(const CTransform& xfrm) const {
    CAABox box;
    CVector3f point = xfrm * getPoint(0);
    box.accumulateBounds(point);
    point = xfrm * getPoint(1);
    box.accumulateBounds(point);
    point = xfrm * getPoint(2);
    box.accumulateBounds(point);
    point = xfrm * getPoint(3);
    box.accumulateBounds(point);
    point = xfrm * getPoint(4);
    box.accumulateBounds(point);
    point = xfrm * getPoint(5);
    box.accumulateBounds(point);
    point = xfrm * getPoint(6);
    box.accumulateBounds(point);
    point = xfrm * getPoint(7);
    box.accumulateBounds(point);
    return box;
  }

  void accumulateBounds(const CVector3f& point) {
    if (min.x() > point.x())
      min.x() = point.x();
    if (min.y() > point.y())
      min.y() = point.y();
    if (min.z() > point.z())
      min.z() = point.z();
    if (max.x() < point.x())
      max.x() = point.x();
    if (max.y() < point.y())
      max.y() = point.y();
    if (max.z() < point.z())
      max.z() = point.z();
  }

  void accumulateBounds(const CAABox& other) {
    accumulateBounds(other.min);
    accumulateBounds(other.max);
  }

  [[nodiscard]] bool pointInside(const CVector3f& other) const {
    return (min.x() <= other.x() && other.x() <= max.x() && min.y() <= other.y() && other.y() <= max.y() &&
            min.z() <= other.z() && other.z() <= max.z());
  }

  [[nodiscard]] CVector3f closestPointAlongVector(const CVector3f& other) const {
    return {(other.x() >= 0.f ? min.x() : max.x()), (other.y() >= 0.f ? min.y() : max.y()),
            (other.z() >= 0.f ? min.z() : max.z())};
  }

  [[nodiscard]] CVector3f furthestPointAlongVector(const CVector3f& other) const {
    return {(other.x() >= 0.f ? max.x() : min.x()), (other.y() >= 0.f ? max.y() : min.y()),
            (other.z() >= 0.f ? max.z() : min.z())};
  }

  [[nodiscard]] float distanceBetween(const CAABox& other) const {
    int intersects = 0;
    if (max.x() >= other.min.x() && min.x() <= other.max.x())
      intersects |= 0x1;
    if (max.y() >= other.min.y() && min.y() <= other.max.y())
      intersects |= 0x2;
    if (max.z() >= other.min.z() && min.z() <= other.max.z())
      intersects |= 0x4;

    float minX, maxX;
    if (max.x() < other.min.x()) {
      minX = max.x();
      maxX = other.min.x();
    } else {
      minX = min.x();
      maxX = other.max.x();
    }

    float minY, maxY;
    if (max.y() < other.min.y()) {
      minY = max.y();
      maxY = other.min.y();
    } else {
      minY = min.y();
      maxY = other.max.y();
    }

    float minZ, maxZ;
    if (max.z() < other.min.z()) {
      minZ = max.z();
      maxZ = other.min.z();
    } else {
      minZ = min.z();
      maxZ = other.max.z();
    }

    switch (intersects) {
    case 0:
      return zeus::CVector3f(maxX - minX, maxY - minY, maxZ - minZ).magnitude();
    case 1:
      return zeus::CVector2f(maxY - minY, maxZ - minZ).magnitude();
    case 2:
      return zeus::CVector2f(maxX - minX, maxZ - minZ).magnitude();
    case 3:
      return std::fabs(maxZ - minZ);
    case 4:
      return zeus::CVector2f(maxX - minX, maxY - minY).magnitude();
    case 5:
      return std::fabs(maxY - minY);
    case 6:
      return std::fabs(maxX - minX);
    case 7:
    default:
      return 0.f;
    }
  }

  [[nodiscard]] CVector3f getPoint(const int point) const {
    const CVector3f* vecs = &min;
    return CVector3f(vecs[(point & 1) != 0].x(), vecs[(point & 2) != 0].y(), vecs[(point & 4) != 0].z());
  }

  [[nodiscard]] CVector3f clampToBox(const CVector3f& vec) const {
    CVector3f ret = vec;
    ret.x() = clamp(min.x(), float(ret.x()), max.x());
    ret.y() = clamp(min.y(), float(ret.y()), max.y());
    ret.z() = clamp(min.z(), float(ret.z()), max.z());
    return ret;
  }

  [[nodiscard]] bool projectedPointTest(const CMatrix4f& mvp, const CVector2f& point) const;

  void splitX(CAABox& negX, CAABox& posX) const {
    float midX = (max.x() - min.x()) * .5f + min.x();
    posX.max = max;
    posX.min = min;
    posX.min.x() = midX;
    negX.max = max;
    negX.max.x() = midX;
    negX.min = min;
  }

  void splitY(CAABox& negY, CAABox& posY) const {
    float midY = (max.y() - min.y()) * .5f + min.y();
    posY.max = max;
    posY.min = min;
    posY.min.y() = midY;
    negY.max = max;
    negY.max.y() = midY;
    negY.min = min;
  }

  void splitZ(CAABox& negZ, CAABox& posZ) const {
    float midZ = (max.z() - min.z()) * .5f + min.z();
    posZ.max = max;
    posZ.min = min;
    posZ.min.z() = midZ;
    negZ.max = max;
    negZ.max.z() = midZ;
    negZ.min = min;
  }

  [[nodiscard]] bool invalid() const { return max.x() < min.x() || max.y() < min.y() || max.z() < min.z(); }

  [[nodiscard]] float operator[](size_t idx) const {
    assert(idx < 6);
    if (idx < 3)
      return min[idx];
    else
      return max[idx - 3];
  }
};
constexpr inline CAABox skInvertedBox;
constexpr inline CAABox skNullBox(CVector3f{}, CVector3f{});

[[nodiscard]] inline bool operator==(const CAABox& left, const CAABox& right) {
  return (left.min == right.min && left.max == right.max);
}

[[nodiscard]] inline bool operator!=(const CAABox& left, const CAABox& right) {
  return (left.min != right.min || left.max != right.max);
}
} // namespace zeus
