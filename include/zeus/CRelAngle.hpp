#pragma once

#include <cmath>

#include "zeus/Math.hpp"

namespace zeus {
/**
 * @brief The CRelAngle class represents relative angle in radians
 */
class CRelAngle {
  float angle = 0.f;

public:
  static float MakeRelativeAngle(float angle) {
    float ret = angle - std::trunc(angle / (2.f * M_PIF)) * (2.f * M_PIF);
    if (ret < 0.f)
      ret += 2.f * M_PIF;
    return ret;
  }

  constexpr CRelAngle() = default;

  constexpr CRelAngle(float angle) : angle(angle) {}

  CRelAngle& operator=(float ang) {
    angle = ang;
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
    ret.angle = degToRad(angle);
    return ret;
  }

  operator float() const { return angle; }

  static CRelAngle FromRadians(float angle) { return CRelAngle(angle); }

  bool operator<(const CRelAngle& other) const { return angle < other.angle; }

  CRelAngle& operator+=(const CRelAngle& other) {
    angle = angle + other.angle;
    return *this;
  }

  CRelAngle& operator+=(float r) {
    angle = angle + r;
    return *this;
  }

  CRelAngle& operator-=(const CRelAngle& other) {
    angle = angle - other.angle;
    return *this;
  }

  CRelAngle& operator-=(float r) {
    angle = angle - r;
    return *this;
  }

  CRelAngle& operator*=(const CRelAngle& other) {
    angle = angle * other.angle;
    return *this;
  }

  CRelAngle& operator*=(float r) {
    angle = angle * r;
    return *this;
  }

  CRelAngle& operator/=(const CRelAngle& other) {
    angle = angle / other.angle;
    return *this;
  }

  CRelAngle& operator/=(float r) {
    angle = angle / r;
    return *this;
  }

  void makeRel() { angle = MakeRelativeAngle(angle); }

  CRelAngle asRel() const { return CRelAngle(MakeRelativeAngle(angle)); }
};
} // namespace zeus
