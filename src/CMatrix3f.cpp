#include "zeus/CMatrix3f.hpp"
#include "zeus/CQuaternion.hpp"
#include "zeus/Global.hpp"

namespace zeus {
const CMatrix3f CMatrix3f::skIdentityMatrix3f = CMatrix3f();

CMatrix3f::CMatrix3f(const CQuaternion& quat) {
  CQuaternion nq = quat.normalized();
  float x2 = nq.x() * nq.x();
  float y2 = nq.y() * nq.y();
  float z2 = nq.z() * nq.z();

  m[0][0] = 1.0 - 2.0 * y2 - 2.0 * z2;
  m[1][0] = 2.0 * nq.x() * nq.y() - 2.0 * nq.z() * nq.w();
  m[2][0] = 2.0 * nq.x() * nq.z() + 2.0 * nq.y() * nq.w();

  m[0][1] = 2.0 * nq.x() * nq.y() + 2.0 * nq.z() * nq.w();
  m[1][1] = 1.0 - 2.0 * x2 - 2.0 * z2;
  m[2][1] = 2.0 * nq.y() * nq.z() - 2.0 * nq.x() * nq.w();

  m[0][2] = 2.0 * nq.x() * nq.z() - 2.0 * nq.y() * nq.w();
  m[1][2] = 2.0 * nq.y() * nq.z() + 2.0 * nq.x() * nq.w();
  m[2][2] = 1.0 - 2.0 * x2 - 2.0 * y2;
}

void CMatrix3f::transpose() {
#if __SSE__
  __m128 zero = _mm_xor_ps(m[0].mSimd.native(), m[0].mSimd.native());
  __m128 T0 = _mm_unpacklo_ps(m[0].mSimd.native(), m[1].mSimd.native());
  __m128 T2 = _mm_unpacklo_ps(m[2].mSimd.native(), zero);
  __m128 T1 = _mm_unpackhi_ps(m[0].mSimd.native(), m[1].mSimd.native());
  __m128 T3 = _mm_unpackhi_ps(m[2].mSimd.native(), zero);
  m[0].mSimd = _mm_movelh_ps(T0, T2);
  m[1].mSimd = _mm_movehl_ps(T2, T0);
  m[2].mSimd = _mm_movelh_ps(T1, T3);
#elif __ARM_NEON
  float32x4x2_t P0 = vzipq_f32(M.r[0], M.r[2]);
  float32x4x2_t P1 = vzipq_f32(M.r[1], M.r[3]);

  float32x4x2_t T0 = vzipq_f32(P0.val[0], P1.val[0]);
  float32x4x2_t T1 = vzipq_f32(P0.val[1], P1.val[1]);

  m[0].mSimd = T0.val[0];
  m[1].mSimd = T0.val[1];
  m[2].mSimd = T1.val[0];
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

CMatrix3f CMatrix3f::transposed() const {
#if __SSE__
  __m128 zero = _mm_xor_ps(m[0].mSimd.native(), m[0].mSimd.native());
  __m128 T0 = _mm_unpacklo_ps(m[0].mSimd.native(), m[1].mSimd.native());
  __m128 T2 = _mm_unpacklo_ps(m[2].mSimd.native(), zero);
  __m128 T1 = _mm_unpackhi_ps(m[0].mSimd.native(), m[1].mSimd.native());
  __m128 T3 = _mm_unpackhi_ps(m[2].mSimd.native(), zero);
  return CMatrix3f(_mm_movelh_ps(T0, T2), _mm_movehl_ps(T2, T0), _mm_movelh_ps(T1, T3));
#elif __ARM_NEON
  float32x4x2_t P0 = vzipq_f32(M.r[0], M.r[2]);
  float32x4x2_t P1 = vzipq_f32(M.r[1], M.r[3]);

  float32x4x2_t T0 = vzipq_f32(P0.val[0], P1.val[0]);
  float32x4x2_t T1 = vzipq_f32(P0.val[1], P1.val[1]);

  return CMatrix3f(T0.val[0], T0.val[1], T1.val[0]);
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

CMatrix3f CMatrix3f::inverted() const {
  float det = m[0][0] * m[1][1] * m[2][2] + m[1][0] * m[2][1] * m[0][2] + m[2][0] * m[0][1] * m[1][2] -
              m[0][2] * m[1][1] * m[2][0] - m[1][2] * m[2][1] * m[0][0] - m[2][2] * m[0][1] * m[1][0];

  if (det == 0.0)
    return CMatrix3f();

  det = 1.0f / det;
  return CMatrix3f((m[1][1] * m[2][2] - m[1][2] * m[2][1]) * det, -(m[1][0] * m[2][2] - m[1][2] * m[2][0]) * det,
                   (m[1][0] * m[2][1] - m[1][1] * m[2][0]) * det, -(m[0][1] * m[2][2] - m[0][2] * m[2][1]) * det,
                   (m[0][0] * m[2][2] - m[0][2] * m[2][0]) * det, -(m[0][0] * m[2][1] - m[0][1] * m[2][0]) * det,
                   (m[0][1] * m[1][2] - m[0][2] * m[1][1]) * det, -(m[0][0] * m[1][2] - m[0][2] * m[1][0]) * det,
                   (m[0][0] * m[1][1] - m[0][1] * m[1][0]) * det);
}
} // namespace zeus
