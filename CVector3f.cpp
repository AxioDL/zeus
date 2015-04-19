#include "CVector3f.hpp"
#include <memory.h>
#include <cmath>
#include <assert.h>
#include "Math.hpp"

const CVector3f CVector3f::skOne = CVector3f(1);
const CVector3f CVector3f::skZero;

void CVector3f::normalize()
{
    float mag = length();
    assert(mag != 0.0);
    
    x /= mag;
    y /= mag;
    z /= mag;
}

CVector3f CVector3f::normalized() const
{
    CVector3f ret;
    float mag = length();
    assert(mag != 0.0);

    ret.x = x/mag;
    ret.y = y/mag;
    ret.z = z/mag;

    return ret;
}

float CVector3f::getAngleDiff(const CVector3f& a, const CVector3f& b)
{
    float mag1 = a.length();
    float mag2 = b.length();

    if (!mag1 || !mag2)
        return 0;

    float theta = acosf(a.dot(b) / (mag1 * mag2));
    return theta;
}

CVector3f CVector3f::slerp(const CVector3f& a, const CVector3f& b, float t)
{
    if (t <= 0.0f)
        return a;
    if (t >= 1.0f)
        return b;

    CVector3f ret;

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
        ret.z = (float)(a.z * s0 + b.z * s1) * d;

        return ret;
    }
    return a;
}
