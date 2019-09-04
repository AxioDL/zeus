#pragma once

#include <cstdint>
#include "zeus/CVector2f.hpp"

namespace zeus {

class CVector2i {
public:
  int32_t x = 0;
  int32_t y = 0;

  constexpr CVector2i() = default;

  constexpr CVector2i(int32_t xin, int32_t yin) : x(xin), y(yin) {}

  CVector2i(const CVector2f& vec) : x(int32_t(vec.x())), y(int32_t(vec.y())) {}

  CVector2f toVec2f() const { return CVector2f(x, y); }

  CVector2i operator+(const CVector2i& val) const { return CVector2i(x + val.x, y + val.y); }

  CVector2i operator-(const CVector2i& val) const { return CVector2i(x - val.x, y - val.y); }

  CVector2i operator*(const CVector2i& val) const { return CVector2i(x * val.x, y * val.y); }

  CVector2i operator/(const CVector2i& val) const { return CVector2i(x / val.x, y / val.y); }

  bool operator==(const CVector2i& other) const { return x == other.x && y == other.y; }

  bool operator!=(const CVector2i& other) const { return x != other.x || y != other.y; }

  CVector2i operator*(int32_t val) const { return CVector2i(x * val, y * val); }
};
} // namespace zeus
