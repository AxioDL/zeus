#pragma once

#include <cstdint>
#include "zeus/CVector2f.hpp"

namespace zeus {

class CVector2i {
public:
  int32_t x = 0;
  int32_t y = 0;

  constexpr CVector2i() noexcept = default;

  constexpr CVector2i(int32_t xin, int32_t yin) noexcept : x(xin), y(yin) {}

  CVector2i(const CVector2f& vec) noexcept : x(int32_t(vec.x())), y(int32_t(vec.y())) {}

  [[nodiscard]] constexpr CVector2f toVec2f() const noexcept { return CVector2f(float(x), float(y)); }

  [[nodiscard]] constexpr CVector2i operator+(const CVector2i& val) const noexcept {
    return CVector2i(x + val.x, y + val.y);
  }
  [[nodiscard]] constexpr CVector2i operator-(const CVector2i& val) const noexcept {
    return CVector2i(x - val.x, y - val.y);
  }
  [[nodiscard]] constexpr CVector2i operator*(const CVector2i& val) const noexcept {
    return CVector2i(x * val.x, y * val.y);
  }
  [[nodiscard]] constexpr CVector2i operator/(const CVector2i& val) const noexcept {
    return CVector2i(x / val.x, y / val.y);
  }

  [[nodiscard]] constexpr bool operator==(const CVector2i& other) const noexcept {
    return x == other.x && y == other.y;
  }
  [[nodiscard]] constexpr bool operator!=(const CVector2i& other) const noexcept { return !operator==(other); }

  [[nodiscard]] constexpr CVector2i operator*(int32_t val) const noexcept { return CVector2i(x * val, y * val); }
};
static_assert(sizeof(CVector2i) == sizeof(int32_t) * 2);

} // namespace zeus
