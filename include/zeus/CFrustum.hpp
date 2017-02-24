#ifndef CFRUSTUM_HPP
#define CFRUSTUM_HPP

#include "zeus/CPlane.hpp"
#include "zeus/CAABox.hpp"
#include "zeus/CProjection.hpp"

namespace zeus
{
class CFrustum
{
    CPlane planes[6];

public:
    inline void updatePlanes(const CMatrix4f& modelview, const CMatrix4f& projection)
    {
        CMatrix4f mvp = projection * modelview;
        CMatrix4f mvp_rm = mvp.transposed();

#if __SSE__

        /* Left */
        planes[0].mVec128 = _mm_add_ps(mvp_rm.vec[0].mVec128, mvp_rm.vec[3].mVec128);

        /* Right */
        planes[1].mVec128 = _mm_add_ps(_mm_sub_ps(CVector3f::skZero.mVec128, mvp_rm.vec[0].mVec128), mvp_rm.vec[3].mVec128);

        /* Bottom */
        planes[2].mVec128 = _mm_add_ps(mvp_rm.vec[1].mVec128, mvp_rm.vec[3].mVec128);

        /* Top */
        planes[3].mVec128 = _mm_add_ps(_mm_sub_ps(CVector3f::skZero.mVec128, mvp_rm.vec[1].mVec128), mvp_rm.vec[3].mVec128);

        /* Near */
        planes[4].mVec128 = _mm_add_ps(mvp_rm.vec[2].mVec128, mvp_rm.vec[3].mVec128);

        /* Far */
        planes[5].mVec128 = _mm_add_ps(_mm_sub_ps(CVector3f::skZero.mVec128, mvp_rm.vec[2].mVec128), mvp_rm.vec[3].mVec128);

#else
        /* Left */
        m_planes[0].a = mvp_rm.m[0][0] + mvp_rm.m[3][0];
        m_planes[0].b = mvp_rm.m[0][1] + mvp_rm.m[3][1];
        m_planes[0].c = mvp_rm.m[0][2] + mvp_rm.m[3][2];
        m_planes[0].d = mvp_rm.m[0][3] + mvp_rm.m[3][3];

        /* Right */
        m_planes[1].a = -mvp_rm.m[0][0] + mvp_rm.m[3][0];
        m_planes[1].b = -mvp_rm.m[0][1] + mvp_rm.m[3][1];
        m_planes[1].c = -mvp_rm.m[0][2] + mvp_rm.m[3][2];
        m_planes[1].d = -mvp_rm.m[0][3] + mvp_rm.m[3][3];

        /* Bottom */
        m_planes[2].a = mvp_rm.m[1][0] + mvp_rm.m[3][0];
        m_planes[2].b = mvp_rm.m[1][1] + mvp_rm.m[3][1];
        m_planes[2].c = mvp_rm.m[1][2] + mvp_rm.m[3][2];
        m_planes[2].d = mvp_rm.m[1][3] + mvp_rm.m[3][3];

        /* Top */
        m_planes[3].a = -mvp_rm.m[1][0] + mvp_rm.m[3][0];
        m_planes[3].b = -mvp_rm.m[1][1] + mvp_rm.m[3][1];
        m_planes[3].c = -mvp_rm.m[1][2] + mvp_rm.m[3][2];
        m_planes[3].d = -mvp_rm.m[1][3] + mvp_rm.m[3][3];

        /* Near */
        m_planes[4].a = mvp_rm.m[2][0] + mvp_rm.m[3][0];
        m_planes[4].b = mvp_rm.m[2][1] + mvp_rm.m[3][1];
        m_planes[4].c = mvp_rm.m[2][2] + mvp_rm.m[3][2];
        m_planes[4].d = mvp_rm.m[2][3] + mvp_rm.m[3][3];

        /* Far */
        m_planes[5].a = -mvp_rm.m[2][0] + mvp_rm.m[3][0];
        m_planes[5].b = -mvp_rm.m[2][1] + mvp_rm.m[3][1];
        m_planes[5].c = -mvp_rm.m[2][2] + mvp_rm.m[3][2];
        m_planes[5].d = -mvp_rm.m[2][3] + mvp_rm.m[3][3];

#endif

        planes[0].normalize();
        planes[1].normalize();
        planes[2].normalize();
        planes[3].normalize();
        planes[4].normalize();
        planes[5].normalize();
    }

    inline void updatePlanes(const CTransform& modelview, const CProjection& projection)
    {
        updatePlanes(modelview.toMatrix4f(), projection.getCachedMatrix());
    }

    inline bool aabbFrustumTest(const CAABox& aabb) const
    {
        CVector3f vmin, vmax;
        for (uint32_t i = 0; i < 6; ++i)
        {
            const CPlane& plane = planes[i];

            /* X axis */
            if (plane.a >= 0)
            {
                vmin[0] = aabb.min[0];
                vmax[0] = aabb.max[0];
            }
            else
            {
                vmin[0] = aabb.max[0];
                vmax[0] = aabb.min[0];
            }
            /* Y axis */
            if (plane.b >= 0)
            {
                vmin[1] = aabb.min[1];
                vmax[1] = aabb.max[1];
            }
            else
            {
                vmin[1] = aabb.max[1];
                vmax[1] = aabb.min[1];
            }
            /* Z axis */
            if (plane.c >= 0)
            {
                vmin[2] = aabb.min[2];
                vmax[2] = aabb.max[2];
            }
            else
            {
                vmin[2] = aabb.max[2];
                vmax[2] = aabb.min[2];
            }
            float dadot = plane.vec.dot(vmax);
            if (dadot + plane.d < 0)
                return false;
        }
        return true;
    }

    inline bool sphereFrustumTest(const CSphere& sphere)
    {
        for (uint32_t i = 0 ; i<6 ; ++i)
        {
            float dadot = planes[i].vec.dot(sphere.position);
            if ((dadot + planes[i].d + sphere.radius) < 0)
                return false;
        }
        return true;
    }

    inline bool pointFrustumTest(const CVector3f& point)
    {
        for (uint32_t i = 0 ; i<6 ; ++i)
        {
            float dadot = planes[i].vec.dot(point);
            if ((dadot + planes[i].d) < 0)
                return false;
        }
        return true;
    }
};
}
#endif // CFRUSTUM_HPP
