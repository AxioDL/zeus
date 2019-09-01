#include "zeus/CQuaternion.hpp"

#include <cmath>

#include "zeus/Math.hpp"

namespace zeus {
CQuaternion::CQuaternion(const CMatrix3f& mat) {
  float trace = mat[0][0] + mat[1][1] + mat[2][2];
  if (trace >= 0.f) {
    float st = std::sqrt(trace + 1.0f);
    float s = 0.5f / st;
    w() = 0.5f * st;
    x() = (mat[1][2] - mat[2][1]) * s;
    y() = (mat[2][0] - mat[0][2]) * s;
    z() = (mat[0][1] - mat[1][0]) * s;
  } else {
    int idx = 0;
    if (mat[1][1] > mat[0][0]) {
      idx = 1;
      if (mat[2][2] > mat[1][1])
        idx = 2;
    } else if (mat[2][2] > mat[0][0]) {
      idx = 2;
    }

    switch (idx) {
    case 0: {
      float st = std::sqrt(mat[0][0] - (mat[1][1] + mat[2][2]) + 1.f);
      float s = 0.5f / st;
      w() = (mat[1][2] - mat[2][1]) * s;
      x() = 0.5f * st;
      y() = (mat[1][0] + mat[0][1]) * s;
      z() = (mat[2][0] + mat[0][2]) * s;
      break;
    }
    case 1: {
      float st = std::sqrt(mat[1][1] - (mat[2][2] + mat[0][0]) + 1.f);
      float s = 0.5f / st;
      w() = (mat[2][0] - mat[0][2]) * s;
      x() = (mat[1][0] + mat[0][1]) * s;
      y() = 0.5f * st;
      z() = (mat[2][1] + mat[1][2]) * s;
      break;
    }
    case 2: {
      float st = std::sqrt(mat[2][2] - (mat[0][0] + mat[1][1]) + 1.f);
      float s = 0.5f / st;
      w() = (mat[0][1] - mat[1][0]) * s;
      x() = (mat[2][0] + mat[0][2]) * s;
      y() = (mat[2][1] + mat[1][2]) * s;
      z() = 0.5f * st;
      break;
    }
    default:
      w() = 0.f;
      x() = 0.f;
      y() = 0.f;
      z() = 0.f;
      break;
    }
  }
}

void CQuaternion::fromVector3f(const CVector3f& vec) {
  float cosX = std::cos(0.5f * vec.x());
  float cosY = std::cos(0.5f * vec.y());
  float cosZ = std::cos(0.5f * vec.z());

  float sinX = std::sin(0.5f * vec.x());
  float sinY = std::sin(0.5f * vec.y());
  float sinZ = std::sin(0.5f * vec.z());

  simd_floats f;
  f[0] = cosZ * cosY * cosX + sinZ * sinY * sinX;
  f[1] = cosZ * cosY * sinX - sinZ * sinY * cosX;
  f[2] = cosZ * sinY * cosX + sinZ * cosY * sinX;
  f[3] = sinZ * cosY * cosX - cosZ * sinY * sinX;
  mSimd.copy_from(f);
}

CQuaternion CQuaternion::operator*(const CQuaternion& q) const {
  return CQuaternion(w() * q.w() - CVector3f(x(), y(), z()).dot({q.x(), q.y(), q.z()}),
                     y() * q.z() - z() * q.y() + w() * q.x() + x() * q.w(),
                     z() * q.x() - x() * q.z() + w() * q.y() + y() * q.w(),
                     x() * q.y() - y() * q.x() + w() * q.z() + z() * q.w());
}

CNUQuaternion CNUQuaternion::operator*(const CNUQuaternion& q) const {
  return CNUQuaternion(w() * q.w() - CVector3f(x(), y(), z()).dot({q.x(), q.y(), q.z()}),
                       y() * q.z() - z() * q.y() + w() * q.x() + x() * q.w(),
                       z() * q.x() - x() * q.z() + w() * q.y() + y() * q.w(),
                       x() * q.y() - y() * q.x() + w() * q.z() + z() * q.w());
}

CQuaternion CQuaternion::operator/(const CQuaternion& q) const {
  CQuaternion p(q);
  p.invert();
  return *this * p;
}

const CQuaternion& CQuaternion::operator*=(const CQuaternion& q) {
  CQuaternion orig = *this;

  w() = orig.w() * q.w() - CVector3f(orig.x(), orig.y(), orig.z()).dot({q.x(), q.y(), q.z()});
  x() = orig.y() * q.z() - orig.z() * q.y() + orig.w() * q.x() + orig.x() * q.w();
  y() = orig.z() * q.x() - orig.x() * q.z() + orig.w() * q.y() + orig.y() * q.w();
  z() = orig.x() * q.y() - orig.y() * q.x() + orig.w() * q.z() + orig.z() * q.w();

  return *this;
}

CQuaternion CQuaternion::log() const {
  float a = std::acos(w());
  float sina = std::sin(a);
  CQuaternion ret;

  if (sina > 0.f)
    ret = a * *this / sina;
  else
    ret = simd<float>(0.f);

  ret.w() = 0.f;

  return ret;
}

CQuaternion CQuaternion::exp() const {
  float a = (CVector3f(mSimd.shuffle<1, 2, 3, 3>()).magnitude());
  float sina = std::sin(a);
  float cosa = std::cos(a);
  CQuaternion ret;

  if (a > 0.f)
    ret = sina * *this / a;
  else
    ret = simd<float>(0.f);

  ret.w() = cosa;

  return ret;
}

CQuaternion CQuaternion::lerp(const CQuaternion& a, const CQuaternion& b, double t) { return (a + t * (b - a)); }

CQuaternion CQuaternion::nlerp(const CQuaternion& a, const CQuaternion& b, double t) {
  return lerp(a, b, t).normalized();
}

CQuaternion CQuaternion::slerp(const CQuaternion& a, const CQuaternion& b, double t) {
  if (t <= 0.0f)
    return a;
  if (t >= 1.0f)
    return b;

  CQuaternion ret;

  float mag = std::sqrt(a.dot(a) * b.dot(b));

  float prod = a.dot(b) / mag;

  if (std::fabs(prod) < 1.0f) {
    const double sign = (prod < 0.0f) ? -1.0f : 1.0f;

    const double theta = std::acos(sign * prod);
    const double s1 = std::sin(sign * t * theta);
    const double d = 1.0 / std::sin(theta);
    const double s0 = std::sin((1.0 - t) * theta);

    ret = (a * s0 + b * s1) * d;

    return ret;
  }
  return a;
}

CQuaternion CQuaternion::shortestRotationArc(const zeus::CVector3f& v0, const zeus::CVector3f& v1) {
  CVector3f v0N = v0;
  CVector3f v1N = v1;

  if (!v0N.isZero())
    v0N.normalize();
  if (!v1N.isZero())
    v1N.normalize();

  CVector3f cross = v0N.cross(v1N);

  if (cross.magSquared() < 0.001f) {
    if (v0N.dot(v1N) > 0.f)
      return CQuaternion();
    if (cross.canBeNormalized())
      return CQuaternion(0.0f, cross.normalized());
    return CQuaternion();
  } else {
    float w = std::sqrt((1.f + zeus::clamp(-1.f, v0N.dot(v1N), 1.f)) * 2.f);
    return CQuaternion(0.5f * w, cross * (1.f / w));
  }
}

CQuaternion CQuaternion::clampedRotateTo(const zeus::CUnitVector3f& v0, const zeus::CUnitVector3f& v1,
                                         const zeus::CRelAngle& angle) {
  CQuaternion arc = shortestRotationArc(v0, v1);
  if (angle >= 2.f * std::acos(arc.w()))
    return arc;

  return fromAxisAngle(arc.getImaginary(), angle);
}

CQuaternion CQuaternion::slerpShort(const CQuaternion& a, const CQuaternion& b, double t) {
  return zeus::CQuaternion::slerp((b.dot(a) >= 0.f) ? a : a.buildEquivalent(), b, t);
}

CQuaternion operator+(float lhs, const CQuaternion& rhs) { return simd<float>(lhs) + rhs.mSimd; }

CQuaternion operator-(float lhs, const CQuaternion& rhs) { return simd<float>(lhs) - rhs.mSimd; }

CQuaternion operator*(float lhs, const CQuaternion& rhs) { return simd<float>(lhs) * rhs.mSimd; }

CNUQuaternion operator*(float lhs, const CNUQuaternion& rhs) { return simd<float>(lhs) * rhs.mSimd; }

CQuaternion CQuaternion::buildEquivalent() const {
  float tmp = std::acos(clamp(-1.f, w(), 1.f)) * 2.f;
  if (std::fabs(tmp) < 1.0e-7)
    return {-1.f, 0.f, 0.f, 0.f};
  else
    return CQuaternion::fromAxisAngle(CUnitVector3f(mSimd.shuffle<1, 2, 3, 3>()), tmp + 2.0 * M_PI);
}

CRelAngle CQuaternion::angleFrom(const zeus::CQuaternion& other) {
  return std::acos(zeus::clamp(-1.f, dot(other), 1.f));
}

static float normalize_angle(float angle) {
  if (angle > M_PIF)
    angle -= 2.f * M_PIF;
  else if (angle < -M_PIF)
    angle += 2.f * M_PIF;

  return angle;
}

CQuaternion CQuaternion::lookAt(const CUnitVector3f& source, const CUnitVector3f& dest, const CRelAngle& maxAng) {
  CQuaternion q;
  zeus::CVector3f destNoZ = dest;
  zeus::CVector3f sourceNoZ = source;
  destNoZ.z() = 0.f;
  sourceNoZ.z() = 0.f;
  zeus::CVector3f tmp;
  if (sourceNoZ.magSquared() > 0.0001f && destNoZ.magSquared() > 0.0001f) {
    sourceNoZ.normalize();
    destNoZ.normalize();

    float angleBetween =
        normalize_angle(std::atan2(destNoZ.x(), destNoZ.y()) - std::atan2(sourceNoZ.x(), sourceNoZ.y()));
    float realAngle = zeus::clamp(-maxAng.asRadians(), angleBetween, maxAng.asRadians());
    CQuaternion tmpQ;
    tmpQ.rotateZ(-realAngle);
    q = tmpQ;
    tmp = q.transform(sourceNoZ);
  } else if (sourceNoZ.magSquared() > 0.0001f)
    tmp = sourceNoZ.normalized();
  else if (destNoZ.magSquared() > 0.0001f)
    tmp = destNoZ.normalized();
  else
    return CQuaternion();

  float realAngle = zeus::clamp(-maxAng.asRadians(), normalize_angle(std::acos(dest.z()) - std::acos(source.z())),
                                maxAng.asRadians());
  return CQuaternion::fromAxisAngle(tmp.cross(skUp), -realAngle) * q;
}

} // namespace zeus
