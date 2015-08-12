#ifndef CFRUSTUM_HPP
#define CFRUSTUM_HPP

#include "CPlane.hpp"
#include "CAABox.hpp"

class CFrustum
{
    CPlane m_planes[6];
public:
    
    inline void updatePlanes(const CTransform& modelview, const CProjection& projection)
    {
        CMatrix4f mv;
        modelview.toMatrix4f(mv);
        CMatrix4f mvp = projection.getCachedMatrix() * mv;
        CMatrix4f mvp_rm = mvp.transposed();
        
#if __SSE__
        
        /* Left */
        m_planes[0].mVec128 = _mm_add_ps(mvp_rm.vec[0].mVec128, mvp_rm.vec[3].mVec128);
        
        /* Right */
        m_planes[1].mVec128 = _mm_add_ps(_mm_sub_ps(CVector3f::skZero.mVec128, mvp_rm.vec[0].mVec128), mvp_rm.vec[3].mVec128);
        
        /* Bottom */
        m_planes[2].mVec128 = _mm_add_ps(mvp_rm.vec[1].mVec128, mvp_rm.vec[3].mVec128);
        
        /* Top */
        m_planes[3].mVec128 = _mm_add_ps(_mm_sub_ps(CVector3f::skZero.mVec128, mvp_rm.vec[1].mVec128), mvp_rm.vec[3].mVec128);
        
        /* Near */
        m_planes[4].mVec128 = _mm_add_ps(mvp_rm.vec[2].mVec128, mvp_rm.vec[3].mVec128);
        
        /* Far */
        m_planes[5].mVec128 = _mm_add_ps(_mm_sub_ps(CVector3f::skZero.mVec128, mvp_rm.vec[2].mVec128), mvp_rm.vec[3].mVec128);
        
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
        
        m_planes[0].normalize();
        m_planes[1].normalize();
        m_planes[2].normalize();
        m_planes[3].normalize();
        m_planes[4].normalize();
        m_planes[5].normalize();
        
    }
    
    inline bool aabbFrustumTest(const CAABox& aabb) const
    {
        CVector3f vmin, vmax;
        int i;
        for (i=0 ; i<6 ; ++i) {
            const CPlane& plane = m_planes[i];
            
            /* X axis */
            if (plane.a >= 0) {
                vmin[0] = aabb.m_min[0];
                vmax[0] = aabb.m_max[0];
            } else {
                vmin[0] = aabb.m_max[0];
                vmax[0] = aabb.m_min[0];
            }
            /* Y axis */
            if (plane.b >= 0) {
                vmin[1] = aabb.m_min[1];
                vmax[1] = aabb.m_max[1];
            } else {
                vmin[1] = aabb.m_max[1];
                vmax[1] = aabb.m_min[1];
            }
            /* Z axis */
            if (plane.c >= 0) {
                vmin[2] = aabb.m_min[2];
                vmax[2] = aabb.m_max[2];
            } else {
                vmin[2] = aabb.m_max[2];
                vmax[2] = aabb.m_min[2];
            }
            float dadot = plane.vec.dot(vmax);
            if (dadot + plane.d < 0)
                return false;
        }
        return true;
    }
    
};

#endif // CFRUSTUM_HPP
