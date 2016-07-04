#include "zeus/CMatrix3f.hpp"
#include "zeus/CMatrix4f.hpp"

namespace zeus
{

void CMatrix3f::transposeSSE3()
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

CMatrix3f CMatrix3f::transposedSSE3() const
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

CMatrix4f CMatrix4f::transposedSSE3() const
{
    CMatrix3f ret;
#if __SSE__
    __m128 T0 = _mm_unpacklo_ps(vec[0].mVec128, vec[1].mVec128);
    __m128 T2 = _mm_unpacklo_ps(vec[2].mVec128, vec[3].mVec128);
    __m128 T1 = _mm_unpackhi_ps(vec[0].mVec128, vec[1].mVec128);
    __m128 T3 = _mm_unpackhi_ps(vec[2].mVec128, vec[3].mVec128);
    ret.vec[0].mVec128 = _mm_movelh_ps(T0, T2);
    ret.vec[1].mVec128 = _mm_movehl_ps(T2, T0);
    ret.vec[2].mVec128 = _mm_movelh_ps(T1, T3);
    ret.vec[3].mVec128 = _mm_movehl_ps(T3, T1);
#else
    ret.m[0][0] = m[0][0];
    ret.m[1][0] = m[0][1];
    ret.m[2][0] = m[0][2];
    ret.m[3][0] = m[0][3];

    ret.m[0][1] = m[1][0];
    ret.m[1][1] = m[1][1];
    ret.m[2][1] = m[1][2];
    ret.m[3][1] = m[1][3];

    ret.m[0][2] = m[2][0];
    ret.m[1][2] = m[2][1];
    ret.m[2][2] = m[2][2];
    ret.m[3][2] = m[2][3];

    ret.m[0][3] = m[3][0];
    ret.m[1][3] = m[3][1];
    ret.m[2][3] = m[3][2];
    ret.m[3][3] = m[3][3];
#endif
    return ret;
}

}
