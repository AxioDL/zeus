#include "zeus/CAABox.hpp"
#include "zeus/CVector3f.hpp"

namespace zeus {
const CAABox CAABox::skInvertedBox = CAABox();
const CAABox CAABox::skNullBox = CAABox(CVector3f::skZero, CVector3f::skZero);

static const int ProjWindings[6][4] = {
  {2, 0, 4, 6}, // -X
  {0, 1, 5, 4}, // -Y
  {2, 3, 1, 0}, // -Z
  {1, 3, 7, 5}, // +X
  {3, 2, 6, 7}, // +Y
  {4, 5, 7, 6}, // +Z
};

static bool testProjectedLine(const CVector2f& a, const CVector2f& b, const CVector2f& point) {
  zeus::CVector2f normal = (b - a).perpendicularVector().normalized();
  return point.dot(normal) >= a.dot(normal);
}

bool CAABox::projectedPointTest(const CMatrix4f& mvp, const CVector2f& point) const {
  CVector2f projPoints[8];
  for (int i = 0; i < 8; ++i)
    projPoints[i] = mvp.multiplyOneOverW(getPoint(i)).toVec2f();

  for (int i = 0, j; i < 6; ++i) {
    for (j = 0; j < 3; ++j) {
      if (!testProjectedLine(projPoints[ProjWindings[i][j]], projPoints[ProjWindings[i][j + 1]], point))
        break;
    }
    if (j == 3 && testProjectedLine(projPoints[ProjWindings[i][3]], projPoints[ProjWindings[i][0]], point))
      return true;
  }

  return false;
}

} // namespace zeus
