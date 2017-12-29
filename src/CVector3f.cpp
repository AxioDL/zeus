#include "zeus/CVector3f.hpp"
#include "zeus/CVector3d.hpp"
#include <memory.h>
#include <cmath>
#include <cassert>
#include "zeus/Math.hpp"

namespace zeus
{
const CVector3f CVector3f::skOne(1.f);
const CVector3f CVector3f::skNegOne(-1.f);
const CVector3f CVector3f::skZero;
const CVector3f CVector3f::skForward(0.f, 1.f, 0.f);
const CVector3f CVector3f::skBack(0.f, -1.f, 0.f);
const CVector3f CVector3f::skLeft(-1.f, 0.f, 0.f);
const CVector3f CVector3f::skRight(1.f, 0.f, 0.f);
const CVector3f CVector3f::skUp(0.f, 0.f, 1.f);
const CVector3f CVector3f::skDown(0.f, 0.f, -1.f);
const CVector3f CVector3f::skRadToDegVec(180.0f / M_PIF);
const CVector3f CVector3f::skDegToRadVec(M_PIF / 180.0f);
const CVector3d CVector3d::skZero(0.0, 0.0, 0.0);

CVector3f::CVector3f(const CVector3d& vec)
{
#if __SSE__
    mVec128 = _mm_cvtpd_ps(vec.mVec128[0]);
    v[2] = vec.v[2];
#else
    v[0] = vec.v[0];
    v[1] = vec.v[1];
    v[2] = vec.v[2];
#endif
}

float CVector3f::getAngleDiff(const CVector3f& a, const CVector3f& b)
{
    float mag1 = a.magnitude();
    float mag2 = b.magnitude();

    if (!mag1 || !mag2)
        return 0.f;

    float dot = a.dot(b);
    float theta = std::acos(dot / (mag1 * mag2));
    return theta;
}

CVector3f CVector3f::slerp(const CVector3f& a, const CVector3f& b, float t)
{
    if (t <= 0.0f)
        return a;
    if (t >= 1.0f)
        return b;

    CVector3f ret;

    float mag = std::sqrt(a.dot(a) * b.dot(b));

    float prod = a.dot(b) / mag;

    if (std::fabs(prod) < 1.0f)
    {
        const double sign = (prod < 0.0f) ? -1.0f : 1.0f;

        const double theta = acos(sign * prod);
        const double s1 = sin(sign * t * theta);
        const double d = 1.0 / sin(theta);
        const double s0 = sin((1.0 - t) * theta);

        ret.x = (float)(a.x * s0 + b.x * s1) * d;
        ret.y = (float)(a.y * s0 + b.y * s1) * d;
        ret.z = (float)(a.z * s0 + b.z * s1) * d;

        return ret;
    }
    return a;
}
}
