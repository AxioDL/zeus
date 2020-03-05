#pragma once

#include "zeus/CTransform.hpp"
#include "zeus/CVector3f.hpp"

namespace zeus {
struct CMRay {
  CMRay(const CVector3f& start, const CVector3f& dirin, float len)
  : start(start), length(len), invLength(1.f / len), dir(dirin) {
    end = start + (len * dirin);
    delta = end - start;
  }

  CMRay(const CVector3f& start, const CVector3f& end, float len, float invLen)
  : start(start), end(end), length(len), invLength(invLen) {
    delta = end - start;
    dir = invLen * delta;
  }

  [[nodiscard]] CMRay getInvUnscaledTransformRay(const CTransform& xfrm) const {
    const CTransform inv = xfrm.inverse();
    return CMRay(inv * start, inv * end, length, invLength);
  }

  CVector3f start; // x0
  CVector3f end;   // xc
  CVector3f delta; // x18
  float length;    // x24
  float invLength; // x28
  CVector3f dir;   // x2c
};
} // namespace zeus
