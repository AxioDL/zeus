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
  static float MakeRelativeAngle(float angle) noexcept {
    float ret = angle - std::trunc(angle / (2.f * M_PIF)) * (2.f * M_PIF);
    if (ret < 0.f)
      ret += 2.f * M_PIF;
    return ret;
  }

  constexpr CRelAngle() noexcept = default;

  constexpr CRelAngle(float angle) noexcept : angle(angle) {}

  constexpr CRelAngle(const CRelAngle&) noexcept = default;

  constexpr CRelAngle& operator=(float ang) noexcept {
    angle = ang;
    return *this;
  }

  constexpr CRelAngle& operator=(const CRelAngle& ang) noexcept = default;

  constexpr float asDegrees() const noexcept { return radToDeg(angle); }

  constexpr float asRadians() const noexcept { return angle; }

  float arcCosine() const noexcept { return std::acos(angle); }

  static constexpr CRelAngle FromDegrees(float angle) noexcept {
    CRelAngle ret;
    ret.angle = degToRad(angle);
    return ret;
  }

  constexpr operator float() const noexcept { return angle; }

  static constexpr CRelAngle FromRadians(float angle) noexcept { return CRelAngle(angle); }

  constexpr bool operator<(const CRelAngle& other) const noexcept { return angle < other.angle; }

  constexpr CRelAngle& operator+=(const CRelAngle& other) noexcept {
    angle += other.angle;
    return *this;
  }

  constexpr CRelAngle& operator+=(float r) noexcept {
    angle += r;
    return *this;
  }

  constexpr CRelAngle& operator-=(const CRelAngle& other) noexcept {
    angle -= other.angle;
    return *this;
  }

  constexpr CRelAngle& operator-=(float r) noexcept {
    angle -= r;
    return *this;
  }

  constexpr CRelAngle& operator*=(const CRelAngle& other) noexcept {
    angle *= other.angle;
    return *this;
  }

  constexpr CRelAngle& operator*=(float r) noexcept {
    angle *= r;
    return *this;
  }

  constexpr CRelAngle& operator/=(const CRelAngle& other) noexcept {
    angle /= other.angle;
    return *this;
  }

  constexpr CRelAngle& operator/=(float r) noexcept {
    angle /= r;
    return *this;
  }

  void makeRel() noexcept { angle = MakeRelativeAngle(angle); }

  CRelAngle asRel() const noexcept { return CRelAngle(MakeRelativeAngle(angle)); }
};
} // namespace zeus
