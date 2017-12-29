#include "zeus/CVector2f.hpp"
#include <memory.h>
#include <cmath>
#include <cassert>
#include "zeus/Math.hpp"

namespace zeus
{
const CVector2f CVector2f::skOne = CVector2f(1.0);
const CVector2f CVector2f::skNegOne = CVector2f(-1.0);
const CVector2f CVector2f::skZero(0.f, 0.f);

float CVector2f::getAngleDiff(const CVector2f& a, const CVector2f& b)
{
    float mag1 = a.magnitude();
    float mag2 = b.magnitude();

    if (!mag1 || !mag2)
        return 0;

    float dot = a.dot(b);
    float theta = std::acos(dot / (mag1 * mag2));
    return theta;
}

CVector2f CVector2f::slerp(const CVector2f& a, const CVector2f& b, float t)
{
    if (t <= 0.0f)
        return a;
    if (t >= 1.0f)
        return b;

    CVector2f ret;

    float mag = std::sqrt(a.dot(a) * b.dot(b));

    float prod = a.dot(b) / mag;

    if (std::fabs(prod) < 1.0f)
    {
        const double sign = (prod < 0.0f) ? -1.0f : 1.0f;

        const double theta = std::acos(sign * prod);
        const double s1 = std::sin(sign * t * theta);
        const double d = 1.0 / std::sin(theta);
        const double s0 = std::sin((1.0f - t) * theta);

        ret = (a * s0 + b * s1) * d;
        return ret;
    }
    return a;
}
}
