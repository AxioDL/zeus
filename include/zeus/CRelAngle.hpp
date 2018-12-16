#pragma once

#include "zeus/CVector3f.hpp"
#include "zeus/Math.hpp"
#include <cmath>

namespace zeus {
/**
 * @brief The CRelAngle class represents relative angle in radians
 */
struct CRelAngle {
  float angle = 0.f;

  static float MakeRelativeAngle(float angle) {
    if (angle == 2.f * M_PIF)
      return 2.f * M_PIF;
    float ret = angle - std::trunc(angle / (2.f * M_PIF)) * (2.f * M_PIF);
    if (ret < 0.f)
      ret += 2.f * M_PIF;
    return ret;
  }

  CRelAngle() = default;

  CRelAngle(float angle) : angle(MakeRelativeAngle(angle)) {}

  CRelAngle& operator=(float ang) {
    angle = MakeRelativeAngle(ang);
    return *this;
  }

  CRelAngle& operator=(const CRelAngle& ang) {
    angle = ang.angle;
    return *this;
  }

  float asDegrees() const { return radToDeg(angle); }

  float asRadians() const { return angle; }

  float arcCosine() const { return std::acos(angle); }

  static CRelAngle FromDegrees(float angle) {
    CRelAngle ret;
    ret.angle = MakeRelativeAngle(degToRad(angle));
    return ret;
  }

  operator float() const { return angle; }

  static CRelAngle FromRadians(float angle) { return CRelAngle(angle); }

  bool operator<(const CRelAngle& other) const { return angle < other.angle; }

  CRelAngle& operator+=(const CRelAngle& other) {
    angle = MakeRelativeAngle(angle + other.angle);
    return *this;
  }

  CRelAngle& operator+=(float r) {
    angle = MakeRelativeAngle(angle + r);
    return *this;
  }

  CRelAngle& operator-=(const CRelAngle& other) {
    angle = MakeRelativeAngle(angle - other.angle);
    return *this;
  }

  CRelAngle& operator-=(float r) {
    angle = MakeRelativeAngle(angle - r);
    return *this;
  }

  CRelAngle& operator*=(const CRelAngle& other) {
    angle = MakeRelativeAngle(angle * other.angle);
    return *this;
  }

  CRelAngle& operator*=(float r) {
    angle = MakeRelativeAngle(angle * r);
    return *this;
  }

  CRelAngle& operator/=(const CRelAngle& other) {
    angle = MakeRelativeAngle(angle / other.angle);
    return *this;
  }

  CRelAngle& operator/=(float r) {
    angle = MakeRelativeAngle(angle / r);
    return *this;
  }
};
} // namespace zeus
