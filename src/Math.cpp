#include "Math.hpp"

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

CVector3f getBezierPoint(const CVector3f& r4, const CVector3f& r5, const CVector3f& r6, const CVector3f& r7, float f1)
{
    //TODO: This isn't quite right, figure out what's really going on and reimplement
#if 0
    float inv = 1.0 - f1;
    CVector3f r3;
    r3.x = ((((((r4.x * (r5.x * f1)) + inv) * (((r5.x * (r6.x * f1)) + inv) * f1)) + inv) * (((((r5.x * (r6.x * f1)) + inv) * (((r6.x * (r7.x * f1)) + inv) * f1)) + inv) * f1)) + inv);
    r3.y = ((((((r4.y * (r5.y * f1)) + inv) * (((r5.y * (r6.y * f1)) + inv) * f1)) + inv) * (((((r5.y * (r6.y * f1)) + inv) * (((r6.y * (r7.y * f1)) + inv) * f1)) + inv) * f1)) + inv);
    r3.z = ((((((r4.z * (r5.z * f1)) + inv) * (((r5.z * (r6.z * f1)) + inv) * f1)) + inv) * (((((r5.z * (r6.z * f1)) + inv) * (((r6.z * (r7.z * f1)) + inv) * f1)) + inv) * f1)) + inv);

    return r3;
#endif
    return CVector3f::skZero;
}
}
}
