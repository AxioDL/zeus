#pragma once

#include "zeus/CVector3f.hpp"

namespace zeus {
class CQuaternion;
class CTransform;

class CEulerAngles : public CVector3f {
public:
  constexpr CEulerAngles(float x, float y, float z) : CVector3f(x, y, z) {}
  CEulerAngles(const CQuaternion& quat);
  CEulerAngles(const CTransform& xf);
};

} // namespace zeus
