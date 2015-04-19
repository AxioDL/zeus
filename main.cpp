#include <iostream>
#include "CVector3d.hpp"
#include "CQuaternion.hpp"
#include "Math.hpp"
#include <math.h>
#include <iomanip>
#include <immintrin.h>

#if !__SSE__
#error "SSE instruction set not enabled"
#endif

class Test
{
    char t;
    __m128 v;
};

int main()
{
    CQuaternion slerp1 = CQuaternion::fromAxisAngle(CVector3f(1.0, .5, 0.0), Math::degToRad(45.f));
# if 1
    CQuaternion slerp2 = CQuaternion::fromAxisAngle(CVector3f(0.5, 256.0, 4096.0), Math::degToRad(90.f));
    CQuaternion slerpQuat = CQuaternion::slerp(slerp1, slerp2, 0.5);

    std::cout << Math::radToDeg(slerpQuat.r) << " " << slerpQuat.v.x << " " << slerpQuat.v.y << " " << slerpQuat.v.z << std::endl;

    CQuaternion quat = CQuaternion::fromAxisAngle(CVector3f(1.0, .5, 0.0), Math::degToRad(45.f));
    quat.invert();
    CAxisAngle axisAngle = quat.toAxisAngle();
    std::cout << axisAngle.axis.x << " " << axisAngle.axis.y << " " << axisAngle.axis.z << " " << Math::radToDeg(axisAngle.angle) << "d" << std::endl;

    std::cout << sizeof (Test) << std::endl;
#endif
    return 0;
}

