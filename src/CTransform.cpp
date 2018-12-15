#include "zeus/CTransform.hpp"

namespace zeus {
const CTransform CTransform::skIdentityTransform;

CTransform CTransformFromEditorEuler(const CVector3f& eulerVec) {
  CTransform result;
  double ti, tj, th, ci, cj, ch, si, sj, sh, cc, cs, sc, ss;

  ti = eulerVec[0];
  tj = eulerVec[1];
  th = eulerVec[2];

  ci = std::cos(ti);
  cj = std::cos(tj);
  ch = std::cos(th);
  si = std::sin(ti);
  sj = std::sin(tj);
  sh = std::sin(th);

  cc = ci * ch;
  cs = ci * sh;
  sc = si * ch;
  ss = si * sh;

  result.basis.m[0][0] = float(cj * ch);
  result.basis.m[1][0] = float(sj * sc - cs);
  result.basis.m[2][0] = float(sj * cc + ss);
  result.basis.m[0][1] = float(cj * sh);
  result.basis.m[1][1] = float(sj * ss + cc);
  result.basis.m[2][1] = float(sj * cs - sc);
  result.basis.m[0][2] = float(-sj);
  result.basis.m[1][2] = float(cj * si);
  result.basis.m[2][2] = float(cj * ci);

  return result;
}

CTransform CTransformFromAxisAngle(const CVector3f& axis, float angle) {
  CTransform result;
  CVector3f axisN = axis.normalized();

  float c = std::cos(angle);
  float s = std::sin(angle);
  float t = 1.f - c;

  result.basis.m[0][0] = t * axisN[0] * axisN[0] + c;
  result.basis.m[1][0] = t * axisN[0] * axisN[1] - axisN[2] * s;
  result.basis.m[2][0] = t * axisN[0] * axisN[2] + axisN[1] * s;

  result.basis.m[0][1] = t * axisN[0] * axisN[1] + axisN[2] * s;
  result.basis.m[1][1] = t * axisN[1] * axisN[1] + c;
  result.basis.m[2][1] = t * axisN[1] * axisN[2] - axisN[0] * s;

  result.basis.m[0][2] = t * axisN[0] * axisN[2] - axisN[1] * s;
  result.basis.m[1][2] = t * axisN[1] * axisN[2] + axisN[0] * s;
  result.basis.m[2][2] = t * axisN[2] * axisN[2] + c;

  return result;
}

CTransform CTransformFromEditorEulers(const CVector3f& eulerVec, const CVector3f& origin) {
  CTransform ret = CTransformFromEditorEuler(eulerVec);
  ret.origin = origin;
  return ret;
}
} // namespace zeus
