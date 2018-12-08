#include "zeus/CVector4f.hpp"
#include "zeus/CColor.hpp"

namespace zeus {
const CVector4f CVector4f::skZero(0.f, 0.f, 0.f, 0.f);

CVector4f::CVector4f(const zeus::CColor& other) : mSimd(other.mSimd) {}

CVector4f& CVector4f::operator=(const CColor& other) {
  mSimd = other.mSimd;
  return *this;
}
} // namespace zeus
