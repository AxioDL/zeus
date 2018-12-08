#include "zeus/CEulerAngles.hpp"
#include "zeus/CQuaternion.hpp"

namespace zeus
{

CEulerAngles::CEulerAngles(const CQuaternion& quat)
{
    float quatDot = quat.dot(quat);
    float t0 = 0.f;
    if (quatDot > 0.f)
        t0 = 2.f / quatDot;
    double t1 = 1.0 - (t0 * quat.x() * quat.x() + t0 * quat.z() * quat.z());
    double t2 = t0 * quat.y() * quat.x() - t0 * quat.z() * quat.w();
    double t3 = t1 * t1 + t2 * t2;

    double t4 = 0.0;
    if (t3 > 0.0)
        t4 = std::sqrt(t3);

    double t5 = t0 * quat.z() * quat.y() + t0 * quat.x() * quat.w();

    if (std::abs(t4) > 0.00001)
    {
        x() = -std::atan2(-t5, t4);
        y() = -std::atan2(t0 * quat.z() * quat.x() - t0 * quat.y() * quat.w(),
                          1.0 - (t0 * quat.x() * quat.x() + t0 * quat.y() * quat.y()));
        z() = -std::atan2(t2, t1);
    }
    else
    {
        x() = -std::atan2(-t5, t4);
        y() = -std::atan2(-(t0 * quat.z() * quat.x() + t0 * quat.y() * quat.w()),
                          1.0 - (t0 * quat.y() * quat.y() + t0 * quat.z() * quat.z()));
        z() = 0.f;
    }
}

CEulerAngles::CEulerAngles(const CTransform& xf)
{
    float xyMagSq = xf.basis[1][1] * xf.basis[1][1] + xf.basis[1][0] * xf.basis[1][0];
    float f1 = 0.f;
    if (xyMagSq > 0.f)
    {
        f1 = 1.f / std::sqrt(xyMagSq);
        float f0;

        for (int i=0 ; i<4 ; ++i)
        {
            f0 = f1 * f1;
            f1 *= 0.5f;
            f0 = 3.f - xyMagSq * f0;
            f1 *= f0;
        }

        f1 = xyMagSq * f0;
    }

    if (std::fabs(f1) >= 0.00001)
    {
        x() = -std::atan2(-xf.basis[1][2], f1);
        y() = -std::atan2(xf.basis[0][2], xf.basis[2][2]);
        z() = -std::atan2(xf.basis[1][0], xf.basis[1][1]);
    }
    else
    {
        x() = -std::atan2(-xf.basis[1][2], f1);
        y() = -std::atan2(-xf.basis[2][0], xf.basis[0][0]);
        z() = 0.f;
    }
}

}
