#include "CVector2f.hpp"
#include <memory.h>
#include <cmath>
#include <assert.h>
#include "Math.hpp"

const CVector2f CVector2f::skOne = CVector2f(1.0);
const CVector2f CVector2f::skNegOne = CVector2f(-1.0);
const CVector2f CVector2f::skZero;

float CVector2f::getAngleDiff(const CVector2f& a, const CVector2f& b)
{
    float mag1 = a.length();
    float mag2 = b.length();

    if (!mag1 || !mag2)
        return 0;

    float theta = acosf(a.dot(b) / (mag1 * mag2));
    return theta;
}

CVector2f CVector2f::slerp(const CVector2f& a, const CVector2f& b, float t)
{
    if (t <= 0.0f)
        return a;
    if (t >= 1.0f)
        return b;

    CVector2f ret;

    float mag = sqrtf(a.dot(a) * b.dot(b));

    float prod = a.dot(b) / mag;

    if (fabsf(prod) < 1.0)
    {
        const double sign = (prod < 0.0) ? -1.0 : 1.0;

        const double theta = acos(sign * prod);
        const double s1 = sin (sign * t * theta);
        const double d = 1.0 / sin(theta);
        const double s0 = sin((1.0 - t) * theta);

        ret.x = (float)(a.x * s0 + b.x * s1) * d;
        ret.y = (float)(a.y * s0 + b.y * s1) * d;

        return ret;
    }
    return a;
}

