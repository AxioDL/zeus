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
    double t1 = 1.0 - (t0 * quat.x * quat.x + t0 * quat.z * quat.z);
    double t2 = t0 * quat.y * quat.x - t0 * quat.z * quat.w;
    double t3 = t1 * t1 + t2 * t2;

    double t4 = 0.0;
    if (t3 > 0.0)
    {
        double sqrtT3 = std::sqrt(t3);
        double t5 = 0.5 * sqrtT3 * -(t3 * sqrtT3 * sqrtT3 - 3.0);
        double t6 = 0.5 * t5 * -(t5 * t5 - 3.0);
        double t7 = 0.5 * t6 * -(t3 * t6 * t6 - 3.0);
        t4 = t3 * 0.5 * t7 * -(t3 * t7 * t7 - 3.0);
    }

    if (std::abs(t4) > 0.00001)
    {
        x = -std::atan2(t0 * quat.z * quat.y + t0 * quat.x * quat.w, t4);
        y = -std::atan2(t0 * quat.z * quat.x - t0 * quat.y * quat.w,
                        1.0 - (t0 * quat.x * quat.x + t0 * quat.y * quat.y));
        z = -std::atan2(t2, t1);
    }
    else
    {
        x = -std::atan2(t0 * quat.z * quat.y + t0 * quat.x * quat.w, t4);
        y = -std::atan2(t0 * quat.z * quat.x + t0 * quat.y * quat.w,
                        1.0 - (t0 * quat.y * quat.y + t0 * quat.z * quat.z));
        z = 0.f;
    }
}

}
