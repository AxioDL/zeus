#include "zeus/CProjection.hpp"
#include "zeus/Math.hpp"
#include <cassert>

namespace zeus
{
void CProjection::_updateCachedMatrix()
{
    assert(m_projType == EProjType::Orthographic || m_projType == EProjType::Perspective);
    if (m_projType == EProjType::Orthographic)
    {
        float tmp;

        tmp = 1.0f / (m_ortho.right - m_ortho.left);
        m_mtx.m[0][0] = 2.0f * tmp;
        m_mtx.m[1][0] = 0.0f;
        m_mtx.m[2][0] = 0.0f;
        m_mtx.m[3][0] = -(m_ortho.right + m_ortho.left) * tmp;

        tmp = 1.0f / (m_ortho.top - m_ortho.bottom);
        m_mtx.m[0][1] = 0.0f;
        m_mtx.m[1][1] = 2.0f * tmp;
        m_mtx.m[2][1] = 0.0f;
        m_mtx.m[3][1] = -(m_ortho.top + m_ortho.bottom) * tmp;

        tmp = 1.0f / (m_ortho.far - m_ortho.near);
        m_mtx.m[0][2] = 0.0f;
        m_mtx.m[1][2] = 0.0f;
        m_mtx.m[2][2] = -(1.0f) * tmp;
        m_mtx.m[3][2] = -m_ortho.far * tmp;

        m_mtx.m[0][3] = 0.0f;
        m_mtx.m[1][3] = 0.0f;
        m_mtx.m[2][3] = 0.0f;
        m_mtx.m[3][3] = 1.0f;
    }
    else if (m_projType == EProjType::Perspective)
    {
        float cot, tmp;
        float t_fovy = std::tan(m_persp.fov / 2.0f);

        cot = 1.0f / t_fovy;

        m_mtx.m[0][0] = cot / m_persp.aspect;
        m_mtx.m[1][0] = 0.0f;
        m_mtx.m[2][0] = 0.0f;
        m_mtx.m[3][0] = 0.0f;

        m_mtx.m[0][1] = 0.0f;
        m_mtx.m[1][1] = cot;
        m_mtx.m[2][1] = 0.0f;
        m_mtx.m[3][1] = 0.0f;

        tmp = 1.0f / (m_persp.far - m_persp.near);
        m_mtx.m[0][2] = 0.0f;
        m_mtx.m[1][2] = 0.0f;
        m_mtx.m[2][2] = -m_persp.far * tmp;
        m_mtx.m[3][2] = -(m_persp.far * m_persp.near) * tmp;

        m_mtx.m[0][3] = 0.0f;
        m_mtx.m[1][3] = 0.0f;
        m_mtx.m[2][3] = -1.0f;
        m_mtx.m[3][3] = 0.0f;
    }
}
}
