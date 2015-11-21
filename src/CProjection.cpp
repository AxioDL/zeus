#include "CProjection.hpp"
#include <math.h>
#include <stdio.h>

namespace Zeus
{
void CProjection::_updateCachedMatrix()
{
    if (m_projType == EProjType::Orthographic)
    {
        float tmp;
        
        tmp = 1.0f / (m_ortho.m_right - m_ortho.m_left);
        m_mtx.m[0][0] = 2.0f * tmp;
        m_mtx.m[1][0] = 0.0f;
        m_mtx.m[2][0] = 0.0f;
        m_mtx.m[3][0] = -(m_ortho.m_right + m_ortho.m_left) * tmp;
        
        tmp = 1.0f / (m_ortho.m_top - m_ortho.m_bottom);
        m_mtx.m[0][1] = 0.0f;
        m_mtx.m[1][1] = 2.0f * tmp;
        m_mtx.m[2][1] = 0.0f;
        m_mtx.m[3][1] = -(m_ortho.m_top + m_ortho.m_bottom) * tmp;
        
        tmp = 1.0f / (m_ortho.m_far - m_ortho.m_near);
        m_mtx.m[0][2] = 0.0f;
        m_mtx.m[1][2] = 0.0f;
        m_mtx.m[2][2] = -(1.0f) * tmp;
        m_mtx.m[3][2] = -m_ortho.m_far * tmp;
        
        m_mtx.m[0][3] = 0.0f;
        m_mtx.m[1][3] = 0.0f;
        m_mtx.m[2][3] = 0.0f;
        m_mtx.m[3][3] = 1.0f;
    }
    else if (m_projType == EProjType::Perspective)
    {
        float cot,tmp;
        float t_fovy = tanf(m_persp.m_fov / 2.0);
        
        cot = 1.0f / t_fovy;
        
        m_mtx.m[0][0] = cot/m_persp.m_aspect;
        m_mtx.m[1][0] = 0.0f;
        m_mtx.m[2][0] = 0.0f;
        m_mtx.m[3][0] = 0.0f;
        
        m_mtx.m[0][1] = 0.0f;
        m_mtx.m[1][1] = cot;
        m_mtx.m[2][1] = 0.0f;
        m_mtx.m[3][1] = 0.0f;
        
        tmp = 1.0f / (m_persp.m_far - m_persp.m_near);
        m_mtx.m[0][2] = 0.0f;
        m_mtx.m[1][2] = 0.0f;
        m_mtx.m[2][2] = -m_persp.m_far * tmp;
        m_mtx.m[3][2] = -(m_persp.m_far * m_persp.m_near) * tmp;
        
        m_mtx.m[0][3] = 0.0f;
        m_mtx.m[1][3] = 0.0f;
        m_mtx.m[2][3] = -1.0f;
        m_mtx.m[3][3] = 0.0f;
    }
    else
    {
        fprintf(stderr, "attempted to cache invalid projection type");
        abort();
    }
}
}

