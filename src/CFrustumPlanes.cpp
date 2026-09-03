#include "zeus/CFrustumPlanes.hpp"

#include <algorithm>
#include <cmath>

#include "zeus/CAABox.hpp"
#include "zeus/CProjection.hpp"
#include "zeus/CTransform4f.hpp"

namespace zeus {

void CFrustumPlanes::updatePlanes(const CMatrix4f& viewMtx, const CMatrix4f& projection) {
  const CMatrix4f mvp = projection * viewMtx;
  const CMatrix4f mvp_rm = mvp.transposed();

  /* Left */
  planes[0].mSimd = mvp_rm.m[3].mSimd + mvp_rm.m[0].mSimd;

  /* Right */
  planes[1].mSimd = mvp_rm.m[3].mSimd - mvp_rm.m[0].mSimd;

  /* Bottom */
  planes[2].mSimd = mvp_rm.m[3].mSimd + mvp_rm.m[1].mSimd;

  /* Top */
  planes[3].mSimd = mvp_rm.m[3].mSimd - mvp_rm.m[1].mSimd;

  /* Near */
  planes[4].mSimd = mvp_rm.m[3].mSimd + mvp_rm.m[2].mSimd;

  /* Far */
  planes[5].mSimd = mvp_rm.m[3].mSimd - mvp_rm.m[2].mSimd;

  planes[0].normalize();
  planes[1].normalize();
  planes[2].normalize();
  planes[3].normalize();
  planes[4].normalize();
  planes[5].normalize();

  valid = true;
}

void CFrustumPlanes::updatePlanes(const CTransform4f& viewPointMtx, const CProjection& projection) {
  const CMatrix3f tmp(viewPointMtx.basis[0], viewPointMtx.basis[2], -viewPointMtx.basis[1]);
  const CTransform4f viewBasis = CTransform4f(tmp.transposed());
  const CTransform4f viewMtx = viewBasis * CTransform4f::Translate(-viewPointMtx.origin);

  updatePlanes(viewMtx.ToMatrix4f(), projection.getCachedMatrix());
}

bool CFrustumPlanes::aabbFrustumTest(const CAABox& aabb) const {
  if (!valid) {
    return true;
  }

  const CVector3f center = aabb.center();
  const CVector3f extents = aabb.extents();

  return std::none_of(planes.cbegin(), planes.cend(), [&center, &extents](const CPlane& plane) {
    const float m = plane.normal().dot(center) + plane.d();
    const float n = extents.dot({std::fabs(plane.x()), std::fabs(plane.y()), std::fabs(plane.z())});
    return m + n < 0.f;
  });
}

bool CFrustumPlanes::sphereFrustumTest(const CSphere& sphere) const {
  if (!valid) {
    return true;
  }

  return std::none_of(planes.cbegin(), planes.cend(), [&sphere](const CPlane& plane) {
    const float dadot = plane.normal().dot(sphere.position);
    return (dadot + plane.d() + sphere.radius) < 0.f;
  });
}

bool CFrustumPlanes::pointFrustumTest(const CVector3f& point) const {
  if (!valid) {
    return true;
  }

  return std::none_of(planes.cbegin(), planes.cend(), [&point](const CPlane& plane) {
    const float dadot = plane.normal().dot(point);
    return (dadot + plane.d()) < 0.f;
  });
}

} // namespace zeus
