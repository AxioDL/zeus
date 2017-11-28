#include "zeus/CFrustum.hpp"

namespace zeus
{

void CFrustum::updatePlanes(const CMatrix4f& viewMtx, const CMatrix4f& projection)
{
    CMatrix4f mvp = projection * viewMtx;
    CMatrix4f mvp_rm = mvp.transposed();

#if __SSE__

    /* Left */
    planes[0].mVec128 = _mm_add_ps(mvp_rm.vec[3].mVec128, mvp_rm.vec[0].mVec128);

    /* Right */
    planes[1].mVec128 = _mm_sub_ps(mvp_rm.vec[3].mVec128, mvp_rm.vec[0].mVec128);

    /* Bottom */
    planes[2].mVec128 = _mm_add_ps(mvp_rm.vec[3].mVec128, mvp_rm.vec[1].mVec128);

    /* Top */
    planes[3].mVec128 = _mm_sub_ps(mvp_rm.vec[3].mVec128, mvp_rm.vec[1].mVec128);

    /* Near */
    planes[4].mVec128 = _mm_add_ps(mvp_rm.vec[3].mVec128, mvp_rm.vec[2].mVec128);

    /* Far */
    planes[5].mVec128 = _mm_sub_ps(mvp_rm.vec[3].mVec128, mvp_rm.vec[2].mVec128);

#else
    /* Left */
        planes[0].a = mvp.m[0][0] + mvp.m[3][0];
        planes[0].b = mvp.m[0][1] + mvp.m[3][1];
        planes[0].c = mvp.m[0][2] + mvp.m[3][2];
        planes[0].d = mvp.m[0][3] + mvp.m[3][3];

        /* Right */
        planes[1].a = -mvp.m[0][0] + mvp.m[3][0];
        planes[1].b = -mvp.m[0][1] + mvp.m[3][1];
        planes[1].c = -mvp.m[0][2] + mvp.m[3][2];
        planes[1].d = -mvp.m[0][3] + mvp.m[3][3];

        /* Bottom */
        planes[2].a = mvp.m[1][0] + mvp.m[3][0];
        planes[2].b = mvp.m[1][1] + mvp.m[3][1];
        planes[2].c = mvp.m[1][2] + mvp.m[3][2];
        planes[2].d = mvp.m[1][3] + mvp.m[3][3];

        /* Top */
        planes[3].a = -mvp.m[1][0] + mvp.m[3][0];
        planes[3].b = -mvp.m[1][1] + mvp.m[3][1];
        planes[3].c = -mvp.m[1][2] + mvp.m[3][2];
        planes[3].d = -mvp.m[1][3] + mvp.m[3][3];

        /* Near */
        planes[4].a = mvp.m[2][0] + mvp.m[3][0];
        planes[4].b = mvp.m[2][1] + mvp.m[3][1];
        planes[4].c = mvp.m[2][2] + mvp.m[3][2];
        planes[4].d = mvp.m[2][3] + mvp.m[3][3];

        /* Far */
        planes[5].a = -mvp.m[2][0] + mvp.m[3][0];
        planes[5].b = -mvp.m[2][1] + mvp.m[3][1];
        planes[5].c = -mvp.m[2][2] + mvp.m[3][2];
        planes[5].d = -mvp.m[2][3] + mvp.m[3][3];

#endif

    planes[0].normalize();
    planes[1].normalize();
    planes[2].normalize();
    planes[3].normalize();
    planes[4].normalize();
    planes[5].normalize();

    valid = true;
}

void CFrustum::updatePlanes(const CTransform& viewPointMtx, const CProjection& projection)
{
    zeus::CMatrix3f tmp(viewPointMtx.basis[0], viewPointMtx.basis[2], -viewPointMtx.basis[1]);
    zeus::CTransform viewBasis = zeus::CTransform(tmp.transposed());
    zeus::CTransform viewMtx = viewBasis * zeus::CTransform::Translate(-viewPointMtx.origin);

    updatePlanes(viewMtx.toMatrix4f(), projection.getCachedMatrix());
}

bool CFrustum::aabbFrustumTest(const CAABox& aabb) const
{
    if (!valid)
        return true;

    CVector3f center = aabb.center();
    CVector3f extents = aabb.extents();

    for (uint32_t i = 0; i < 6; ++i)
    {
        const CPlane& plane = planes[i];

        float m = plane.vec.dot(center) + plane.d;
        float n = extents.dot({std::fabs(plane.a), std::fabs(plane.b), std::fabs(plane.c)});

        if (m + n < 0)
            return false;
    }
    return true;
}

bool CFrustum::sphereFrustumTest(const CSphere& sphere) const
{
    if (!valid)
        return true;

    for (uint32_t i = 0 ; i<6 ; ++i)
    {
        float dadot = planes[i].vec.dot(sphere.position);
        if ((dadot + planes[i].d + sphere.radius) < 0)
            return false;
    }
    return true;
}

bool CFrustum::pointFrustumTest(const CVector3f& point) const
{
    if (!valid)
        return true;

    for (uint32_t i = 0 ; i<6 ; ++i)
    {
        float dadot = planes[i].vec.dot(point);
        if ((dadot + planes[i].d) < 0)
            return false;
    }
    return true;
}

}
