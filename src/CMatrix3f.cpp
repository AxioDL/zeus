#include "zeus/CMatrix3f.hpp"
#include "zeus/CQuaternion.hpp"
#include "zeus/Global.hpp"

namespace zeus
{
const CMatrix3f CMatrix3f::skIdentityMatrix3f = CMatrix3f();

CMatrix3f::CMatrix3f(const CQuaternion& quat)
{
    CQuaternion nq = quat.normalized();
    float x2 = nq.x * nq.x;
    float y2 = nq.y * nq.y;
    float z2 = nq.z * nq.z;
    
    m[0][0] = 1.0 - 2.0 * y2 - 2.0 * z2;
    m[1][0] = 2.0 * nq.x * nq.y - 2.0 * nq.z * nq.w;
    m[2][0] = 2.0 * nq.x * nq.z + 2.0 * nq.y * nq.w;
    
    m[0][1] = 2.0 * nq.x * nq.y + 2.0 * nq.z * nq.w;
    m[1][1] = 1.0 - 2.0 * x2 - 2.0 * z2;
    m[2][1] = 2.0 * nq.y * nq.z - 2.0 * nq.x * nq.w;
    
    m[0][2] = 2.0 * nq.x * nq.z - 2.0 * nq.y * nq.w;
    m[1][2] = 2.0 * nq.y * nq.z + 2.0 * nq.x * nq.w;
    m[2][2] = 1.0 - 2.0 * x2 - 2.0 * y2;
    
    m[0][3] = 0.0f;
    m[1][3] = 0.0f;
    m[2][3] = 0.0f;
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
}
