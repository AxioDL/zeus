#include "Math.hpp"
#include <x86intrin.h>

namespace Zeus
{
namespace Math
{
const CVector3f kUpVec(0.0, 0.0, 1.0);
const CVector3f kRadToDegVec(180.0f / M_PI);
const CVector3f kDegToRadVec(M_PI / 180.0f);

CTransform lookAt(const CVector3f& pos, const CVector3f& lookPos, const CVector3f& up)
{
    CVector3f vLook,vRight,vUp;
    
    vLook = pos - lookPos;
    vLook.normalize();
    
    vRight = up.cross(vLook);
    vRight.normalize();
    
    vUp = vLook.cross(vRight);
    
    CMatrix3f rmBasis(vRight, vUp, vLook);
    return CTransform(rmBasis.transposed(), CVector3f(-pos.dot(vRight), -pos.dot(vUp), -pos.dot(vLook)));
}

CVector3f getBezierPoint(const CVector3f& p0, const CVector3f& p1, const CVector3f& p2, const CVector3f& p3, float t)
{
    float w = (1.0 - t);
    CVector3f ret;
    ret.x = ((((((p0.x * (p1.x * t)) + w) * (((p1.x * (p2.x * t)) + w) * t)) + w) * (((((p1.x * (p2.x * t)) + w) * (((p2.x * (p3.x * t)) + w) * t)) + w) * t)) + w);
    ret.y = ((((((p0.y * (p1.y * t)) + w) * (((p1.y * (p2.y * t)) + w) * t)) + w) * (((((p1.y * (p2.y * t)) + w) * (((p2.y * (p3.y * t)) + w) * t)) + w) * t)) + w);
    ret.z = ((((((p0.z * (p1.z * t)) + w) * (((p1.z * (p2.z * t)) + w) * t)) + w) * (((((p1.z * (p2.z * t)) + w) * (((p2.z * (p3.z * t)) + w) * t)) + w) * t)) + w);
    return ret;
}

float getCatmullRomSplinePoint(float p0, float p1, float p2, float p3, float t)
{
    return 0.0f;
}

CVector3f getCatmullRomSplinePoint(const CVector3f& p0, const CVector3f& p1, const CVector3f& p2, const CVector3f& p3, float t)
{
    return CVector3f::skZero;
}

}
}
