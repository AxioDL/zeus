#include "Math.hpp"

const CVector3f Math::kUpVec(0.0, 0.0, 1.0);
const CVector3f Math::kRadToDegVec(180.0f / M_PI);
const CVector3f Math::kDegToRadVec(M_PI / 180.0f);

CTransform Math::lookAt(const CVector3f& pos, const CVector3f& lookPos, const CVector3f& up)
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
