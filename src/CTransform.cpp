#include "CTransform.hpp"

namespace Zeus
{
CTransform CTransformFromEditorEuler(const CVector3f& eulerVec)
{
    CTransform result;
    double ti, tj, th, ci, cj, ch, si, sj, sh, cc, cs, sc, ss;
    
    ti = eulerVec[0];
    tj = eulerVec[1];
    th = eulerVec[2];
    
    ci = cos(ti);
    cj = cos(tj);
    ch = cos(th);
    si = sin(ti);
    sj = sin(tj);
    sh = sin(th);
    
    cc = ci * ch;
    cs = ci * sh;
    sc = si * ch;
    ss = si * sh;
    
    result.m_basis.m[0][0] = (float)(cj * ch);
    result.m_basis.m[1][0] = (float)(sj * sc - cs);
    result.m_basis.m[2][0] = (float)(sj * cc + ss);
    result.m_basis.m[0][1] = (float)(cj * sh);
    result.m_basis.m[1][1] = (float)(sj * ss + cc);
    result.m_basis.m[2][1] = (float)(sj * cs - sc);
    result.m_basis.m[0][2] = (float)(-sj);
    result.m_basis.m[1][2] = (float)(cj * si);
    result.m_basis.m[2][2] = (float)(cj * ci);
    
    return result;
}

CTransform CTransformFromAxisAngle(const CVector3f& axis, float angle)
{
    CTransform result;
    CVector3f axisN = axis.normalized();
    
    float c = cosf(angle);
    float s = sinf(angle);
    float t = 1 - c;
    
    result.m_basis.m[0][0] = t * axisN.v[0] * axisN.v[0] + c;
    result.m_basis.m[1][0] = t * axisN.v[0] * axisN.v[1] - axisN.v[2] * s;
    result.m_basis.m[2][0] = t * axisN.v[0] * axisN.v[2] + axisN.v[1] * s;
    
    result.m_basis.m[0][1] = t * axisN.v[0] * axisN.v[1] + axisN.v[2] * s;
    result.m_basis.m[1][1] = t * axisN.v[1] * axisN.v[1] + c;
    result.m_basis.m[2][1] = t * axisN.v[1] * axisN.v[2] - axisN.v[0] * s;
    
    result.m_basis.m[0][2] = t * axisN.v[0] * axisN.v[2] - axisN.v[1] * s;
    result.m_basis.m[1][2] = t * axisN.v[1] * axisN.v[2] + axisN.v[0] * s;
    result.m_basis.m[2][2] = t * axisN.v[2] * axisN.v[2] + c;
    
    return result;
}

CTransform CTransformFromEditorEulers(const CVector3f& eulerVec, const CVector3f& origin)
{
    CTransform ret = CTransformFromEditorEuler(eulerVec);
    ret.m_origin = origin;
    return ret;
}
}
