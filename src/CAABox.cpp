#include "zeus/CAABox.hpp"
#include "zeus/CVector3f.hpp"

namespace zeus {
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

bool CAABox::intersects(const CSphere& other) const {
  float dist = 0.f;
  int comps = 0;
  for (int i = 0; i < 3; i++) {
    if (other.position[i] < min[i]) {
      if (other.position[i] + other.radius >= min[i]) {
        float comp = other.position[i] - min[i];
        dist += comp * comp;
        comps |= 1 << i;
      } else {
        return false;
      }
    } else if (other.position[i] > max[i]) {
      if (other.position[i] - other.radius <= max[i]) {
        float comp = other.position[i] - max[i];
        dist += comp * comp;
        comps |= 1 << i;
      } else {
        return false;
      }
    }
  }
  if (comps == 0)
    return true;
  return dist <= other.radius * other.radius;
}

float CAABox::intersectionRadius(const CSphere& other) const {
  float dist = 0.f;
  int comps = 0;
  for (int i = 0; i < 3; i++) {
    if (other.position[i] < min[i]) {
      if (other.position[i] + other.radius >= min[i]) {
        float comp = other.position[i] - min[i];
        dist += comp * comp;
        comps |= 1 << i;
      } else {
        return -1.f;
      }
    } else if (other.position[i] > max[i]) {
      if (other.position[i] - other.radius <= max[i]) {
        float comp = other.position[i] - max[i];
        dist += comp * comp;
        comps |= 1 << i;
      } else {
        return -1.f;
      }
    }
  }
  if (comps == 0)
    return dist;
  return (dist <= other.radius * other.radius) ? dist : -1.f;
}

CAABox CAABox::booleanIntersection(const CAABox& other) const {
  CVector3f minVec;
  CVector3f maxVec;

  for (int i = 0; i < 3; ++i) {
    if (min[i] <= other.min[i] && max[i] >= other.max[i]) {
      minVec[i] = other.min[i];
      maxVec[i] = other.max[i];
    } else if (other.min[i] <= min[i] && other.max[i] >= max[i]) {
      minVec[i] = min[i];
      maxVec[i] = max[i];
    } else if (other.min[i] <= min[i] && other.max[i] >= min[i]) {
      minVec[i] = min[i];
      maxVec[i] = other.max[i];
    } else if (other.min[i] <= max[i] && other.max[i] >= max[i]) {
      minVec[i] = other.min[i];
      maxVec[i] = max[i];
    }
  }

  return {minVec, maxVec};
}

} // namespace zeus
