#include "CMatrix3f.hpp"
#include "CQuaternion.hpp"
#include "Global.hpp"

const CMatrix3f CMatrix3f::skIdentityMatrix3f = CMatrix3f();

CMatrix3f::CMatrix3f(const CQuaternion& quat)
{
    CQuaternion nq = quat.normalized();
    float x2 = nq.v[0] * nq.v[0];
    float y2 = nq.v[1] * nq.v[1];
    float z2 = nq.v[2] * nq.v[2];
    
    m[0][0] = 1.0 - 2.0 * y2 - 2.0 * z2;
    m[1][0] = 2.0 * nq.v[0] * nq.v[1] - 2.0 * nq.v[2] * nq.r;
    m[2][0] = 2.0 * nq.v[0] * nq.v[2] + 2.0 * nq.v[1] * nq.r;
    
    m[0][1] = 2.0 * nq.v[0] * nq.v[1] + 2.0 * nq.v[2] * nq.r;
    m[1][1] = 1.0 - 2.0 * x2 - 2.0 * z2;
    m[2][1] = 2.0 * nq.v[1] * nq.v[2] - 2.0 * nq.v[0] * nq.r;
    
    m[0][2] = 2.0 * nq.v[0] * nq.v[2] - 2.0 * nq.v[1] * nq.r;
    m[1][2] = 2.0 * nq.v[1] * nq.v[2] + 2.0 * nq.v[0] * nq.r;
    m[2][2] = 1.0 - 2.0 * x2 - 2.0 * y2;
    
    m[0][3] = 0.0f;
    m[1][3] = 0.0f;
    m[2][3] = 0.0f;
}

CMatrix3f operator*(const CMatrix3f& lhs, const CMatrix3f& rhs)
{
#if __SSE__
    unsigned i;
    TVectorUnion resVec[3];
    for (i=0 ; i<3 ; ++i) {
        resVec[i].mVec128 =
        _mm_add_ps(_mm_add_ps(
                   _mm_mul_ps(lhs[0].mVec128, ze_splat_ps(rhs[i].mVec128, 0)),
                   _mm_mul_ps(lhs[1].mVec128, ze_splat_ps(rhs[i].mVec128, 1))),
                   _mm_mul_ps(lhs[2].mVec128, ze_splat_ps(rhs[i].mVec128, 2)));
        resVec[i].v[3] = 0.0;
    }
    return CMatrix3f(resVec[0].mVec128, resVec[1].mVec128, resVec[2].mVec128);
#else
    return CMatrix3f(lhs[0][0] * rhs[0][0] + lhs[1][0] * rhs[0][1] + lhs[2][0] * rhs[0][2],
                     lhs[0][0] * rhs[1][0] + lhs[1][0] * rhs[1][1] + lhs[2][0] * rhs[1][2],
                     lhs[0][0] * rhs[2][0] + lhs[1][0] * rhs[2][1] + lhs[2][0] * rhs[2][2],
                     lhs[0][1] * rhs[0][0] + lhs[1][1] * rhs[0][1] + lhs[2][1] * rhs[0][2],
                     lhs[0][1] * rhs[1][0] + lhs[1][1] * rhs[1][1] + lhs[2][1] * rhs[1][2],
                     lhs[0][1] * rhs[2][0] + lhs[1][1] * rhs[2][1] + lhs[2][1] * rhs[2][2],
                     lhs[0][2] * rhs[0][0] + lhs[1][2] * rhs[0][1] + lhs[2][2] * rhs[0][2],
                     lhs[0][2] * rhs[1][0] + lhs[1][2] * rhs[1][1] + lhs[2][2] * rhs[1][2],
                     lhs[0][2] * rhs[2][0] + lhs[1][2] * rhs[2][1] + lhs[2][2] * rhs[2][2]);
#endif
}

void CMatrix3f::transpose()
{
#if __SSE__
    __m128 zero = _mm_xor_ps(vec[0].mVec128, vec[0].mVec128);
    __m128 T0 = _mm_unpacklo_ps(vec[0].mVec128, vec[1].mVec128);
    __m128 T2 = _mm_unpacklo_ps(vec[2].mVec128, zero);
    __m128 T1 = _mm_unpackhi_ps(vec[0].mVec128, vec[1].mVec128);
    __m128 T3 = _mm_unpackhi_ps(vec[2].mVec128, zero);
    vec[0].mVec128 = _mm_movelh_ps(T0, T2);
    vec[1].mVec128 = _mm_movehl_ps(T2, T0);
    vec[2].mVec128 = _mm_movelh_ps(T1, T3);
#else
    float tmp;
    
    tmp = m[0][1];
    m[0][1] = m[1][0];
    m[1][0] = tmp;
    
    tmp = m[0][2];
    m[0][2] = m[2][0];
    m[2][0] = tmp;
    
    tmp = m[1][2];
    m[1][2] = m[2][1];
    m[2][1] = tmp;
#endif
}

CMatrix3f CMatrix3f::transposed() const
{
#if __SSE__
    __m128 zero = _mm_xor_ps(vec[0].mVec128, vec[0].mVec128);
    __m128 T0 = _mm_unpacklo_ps(vec[0].mVec128, vec[1].mVec128);
    __m128 T2 = _mm_unpacklo_ps(vec[2].mVec128, zero);
    __m128 T1 = _mm_unpackhi_ps(vec[0].mVec128, vec[1].mVec128);
    __m128 T3 = _mm_unpackhi_ps(vec[2].mVec128, zero);
    return CMatrix3f(_mm_movelh_ps(T0, T2), _mm_movehl_ps(T2, T0), _mm_movelh_ps(T1, T3));
#else
    CMatrix3f ret(*this);
    float tmp;
    
    tmp = ret.m[0][1];
    ret.m[0][1] = ret.m[1][0];
    ret.m[1][0] = tmp;
    
    tmp = m[0][2];
    ret.m[0][2] = ret.m[2][0];
    ret.m[2][0] = tmp;
    
    tmp = m[1][2];
    ret.m[1][2] = ret.m[2][1];
    ret.m[2][1] = tmp;
    
    return ret;
#endif
}

CMatrix3f CMatrix3f::inverted() const
{
    float det =
    m[0][0] * m[1][1] * m[2][2] +
    m[1][0] * m[2][1] * m[0][2] +
    m[2][0] * m[0][1] * m[1][2] -
    m[0][2] * m[1][1] * m[2][0] -
    m[1][2] * m[2][1] * m[0][0] -
    m[2][2] * m[0][1] * m[1][0];
    
    if (det == 0.0)
        return CMatrix3f();
    
    det = 1.0f / det;
    return CMatrix3f((m[1][1]*m[2][2] - m[1][2]*m[2][1]) * det,
                     -(m[1][0]*m[2][2] - m[1][2]*m[2][0]) * det,
                     (m[1][0]*m[2][1] - m[1][1]*m[2][0]) * det,
                     -(m[0][1]*m[2][2] - m[0][2]*m[2][1]) * det,
                     (m[0][0]*m[2][2] - m[0][2]*m[2][0]) * det,
                     -(m[0][0]*m[2][1] - m[0][1]*m[2][0]) * det,
                     (m[0][1]*m[1][2] - m[0][2]*m[1][1]) * det,
                     -(m[0][0]*m[1][2] - m[0][2]*m[1][0]) * det,
                     (m[0][0]*m[1][1] - m[0][1]*m[1][0]) * det);
}

