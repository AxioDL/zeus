#include "zeus/CFrustum.hpp"

namespace zeus {

void CFrustum::updatePlanes(const CMatrix4f& viewMtx, const CMatrix4f& projection) {
  CMatrix4f mvp = projection * viewMtx;
  CMatrix4f mvp_rm = mvp.transposed();

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

void CFrustum::updatePlanes(const CTransform& viewPointMtx, const CProjection& projection) {
  zeus::CMatrix3f tmp(viewPointMtx.basis[0], viewPointMtx.basis[2], -viewPointMtx.basis[1]);
  zeus::CTransform viewBasis = zeus::CTransform(tmp.transposed());
  zeus::CTransform viewMtx = viewBasis * zeus::CTransform::Translate(-viewPointMtx.origin);

  updatePlanes(viewMtx.toMatrix4f(), projection.getCachedMatrix());
}

bool CFrustum::aabbFrustumTest(const CAABox& aabb) const {
  if (!valid)
    return true;

  CVector3f center = aabb.center();
  CVector3f extents = aabb.extents();

  for (uint32_t i = 0; i < 6; ++i) {
    const CPlane& plane = planes[i];

    float m = plane.normal().dot(center) + plane.d();
    float n = extents.dot({std::fabs(plane.x()), std::fabs(plane.y()), std::fabs(plane.z())});

    if (m + n < 0.f)
      return false;
  }
  return true;
}

bool CFrustum::sphereFrustumTest(const CSphere& sphere) const {
  if (!valid)
    return true;

  for (uint32_t i = 0; i < 6; ++i) {
    float dadot = planes[i].normal().dot(sphere.position);
    if ((dadot + planes[i].d() + sphere.radius) < 0.f)
      return false;
  }
  return true;
}

bool CFrustum::pointFrustumTest(const CVector3f& point) const {
  if (!valid)
    return true;

  for (uint32_t i = 0; i < 6; ++i) {
    float dadot = planes[i].normal().dot(point);
    if ((dadot + planes[i].d()) < 0.f)
      return false;
  }
  return true;
}

} // namespace zeus
