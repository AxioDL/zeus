#include "zeus/CProjection.hpp"
#include "zeus/Math.hpp"
#include <cassert>

namespace zeus {
void CProjection::_updateCachedMatrix() {
  assert(m_projType == EProjType::Orthographic || m_projType == EProjType::Perspective);
  if (m_projType == EProjType::Orthographic) {
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

    tmp = 1.0f / (m_ortho.zfar - m_ortho.znear);
    m_mtx.m[0][2] = 0.0f;
    m_mtx.m[1][2] = 0.0f;
    m_mtx.m[2][2] = -tmp;
    m_mtx.m[3][2] = -m_ortho.zfar * tmp;

    m_mtx.m[0][3] = 0.0f;
    m_mtx.m[1][3] = 0.0f;
    m_mtx.m[2][3] = 0.0f;
    m_mtx.m[3][3] = 1.0f;
  } else if (m_projType == EProjType::Perspective) {
    float tfov = std::tan(m_persp.fov * 0.5f);
    float top = m_persp.znear * tfov;
    float bottom = -top;
    float right = m_persp.aspect * m_persp.znear * tfov;
    float left = -right;

    float rml = right - left;
    float rpl = right + left;
    float tmb = top - bottom;
    float tpb = top + bottom;
    float fpn = m_persp.zfar + m_persp.znear;
    float fmn = m_persp.zfar - m_persp.znear;

    m_mtx.m[0][0] = 2.f * m_persp.znear / rml;
    m_mtx.m[1][0] = 0.0f;
    m_mtx.m[2][0] = rpl / rml;
    m_mtx.m[3][0] = 0.0f;

    m_mtx.m[0][1] = 0.0f;
    m_mtx.m[1][1] = 2.f * m_persp.znear / tmb;
    m_mtx.m[2][1] = tpb / tmb;
    m_mtx.m[3][1] = 0.0f;

    m_mtx.m[0][2] = 0.0f;
    m_mtx.m[1][2] = 0.0f;
    m_mtx.m[2][2] = -fpn / fmn;
    m_mtx.m[3][2] = -2.f * m_persp.zfar * m_persp.znear / fmn;

    m_mtx.m[0][3] = 0.0f;
    m_mtx.m[1][3] = 0.0f;
    m_mtx.m[2][3] = -1.0f;
    m_mtx.m[3][3] = 0.0f;
  }
}
} // namespace zeus
