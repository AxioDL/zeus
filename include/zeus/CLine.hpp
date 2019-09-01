#pragma once

#include "zeus/CVector3f.hpp"

namespace zeus {
class CLine {
public:
  constexpr CLine(const CVector3f& origin, const CVector3f& dir) : origin(origin), dir(dir) {}

  CVector3f origin;
  CVector3f dir;
};
} // namespace zeus
