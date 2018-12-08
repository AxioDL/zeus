#include "zeus/CMatrix4f.hpp"

namespace zeus
{

const CMatrix4f CMatrix4f::skIdentityMatrix4f = CMatrix4f();

CMatrix4f CMatrix4f::transposed() const
{
    CMatrix4f ret;
#if __SSE__
    __m128 T0 = _mm_unpacklo_ps(m[0].mSimd.native(), m[1].mSimd.native());
    __m128 T2 = _mm_unpacklo_ps(m[2].mSimd.native(), m[3].mSimd.native());
    __m128 T1 = _mm_unpackhi_ps(m[0].mSimd.native(), m[1].mSimd.native());
    __m128 T3 = _mm_unpackhi_ps(m[2].mSimd.native(), m[3].mSimd.native());
    ret.m[0].mSimd = _mm_movelh_ps(T0, T2);
    ret.m[1].mSimd = _mm_movehl_ps(T2, T0);
    ret.m[2].mSimd = _mm_movelh_ps(T1, T3);
    ret.m[3].mSimd = _mm_movehl_ps(T3, T1);
#elif __ARM_NEON
    float32x4x2_t P0 = vzipq_f32( M.r[0], M.r[2] );
    float32x4x2_t P1 = vzipq_f32( M.r[1], M.r[3] );

    float32x4x2_t T0 = vzipq_f32( P0.val[0], P1.val[0] );
    float32x4x2_t T1 = vzipq_f32( P0.val[1], P1.val[1] );

    ret.m[0].mSimd = T0.val[0];
    ret.m[1].mSimd = T0.val[1];
    ret.m[2].mSimd = T1.val[0];
    ret.m[3].mSimd = T1.val[1];
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
