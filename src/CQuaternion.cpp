#include "CQuaternion.hpp"
#include <math.h>

namespace Zeus
{
void CQuaternion::fromVector3f(const CVector3f& vec)
{
    float cosX = cosf(0.5 * vec.x);
    float cosY = cosf(0.5 * vec.y);
    float cosZ = cosf(0.5 * vec.z);

    float sinX = sinf(0.5 * vec.x);
    float sinY = sinf(0.5 * vec.y);
    float sinZ = sinf(0.5 * vec.z);

    r   = cosZ * cosY * cosX + sinZ * sinY * sinX;
    v.x = cosZ * cosY * sinX - sinZ * sinY * cosX;
    v.y = cosZ * sinY * cosX + sinZ * cosY * sinX;
    v.z = sinZ * cosY * cosX - cosZ * sinY * sinX;
}

CQuaternion& CQuaternion::operator=(const CQuaternion& q)
{
    r = q.r;
    v = q.v;
    return *this;
}

CQuaternion CQuaternion::operator+(const CQuaternion& q) const
{
    return CQuaternion(r + q.r, v+q.v);
}

CQuaternion CQuaternion::operator-(const CQuaternion& q) const
{
    return CQuaternion(r - q.r, v-q.v);
}

CQuaternion CQuaternion::operator*(const CQuaternion& q) const
{
    return CQuaternion(r*q.r - v.dot(q.v),
                       v.y * q.v.z - v.z * q.v.y + r * q.v.x + v.x*q.r,
                       v.z * q.v.x - v.x * q.v.z + r * q.v.y + v.y*q.r,
                       v.x * q.v.y - v.y * q.v.x + r * q.v.z + v.z*q.r);
}

CQuaternion CQuaternion::operator/(const CQuaternion& q) const
{
    CQuaternion p(q);
    p.invert();
    return *this * p;
}

CQuaternion CQuaternion::operator*(float scale) const
{
    return CQuaternion(r*scale, v*scale);
}

CQuaternion CQuaternion::operator/(float scale) const
{
    return CQuaternion(r/scale, v/scale);
}

CQuaternion CQuaternion::operator-() const
{
    return CQuaternion(-r, -v);
}

const CQuaternion& CQuaternion::operator+=(const CQuaternion& q)
{
    r += q.r;
    v += q.v;
    return *this;
}

const CQuaternion& CQuaternion::operator-=(const CQuaternion& q)
{
    r -= q.r;
    v -= q.v;
    return *this;
}

const CQuaternion& CQuaternion::operator *=(const CQuaternion& q)
{
   r = r*q.r - v.dot(q.v);
   v.x = v.y * q.v.z - v.z * q.v.y + r * q.v.x + v.x*q.r;
   v.y = v.z * q.v.x - v.x * q.v.z + r * q.v.y + v.y*q.r;
   v.z = v.x * q.v.y - v.y * q.v.x + r * q.v.z + v.z*q.r;

   return *this;
}

const CQuaternion& CQuaternion::operator *=(float scale)
{
    r *= scale;
    v *= scale;
    return *this;
}

const CQuaternion& CQuaternion::operator/=(float scale)
{
    r /= scale;
    v /= scale;
    return *this;
}

float CQuaternion::magnitude() const
{
    return sqrt(magSquared());
}

float CQuaternion::magSquared() const
{
    return (r*r + (v.dot(v)));
}

void CQuaternion::normalize()
{
    *this /= magnitude();
}

CQuaternion CQuaternion::normalized() const
{
    return *this/magnitude();
}

void CQuaternion::invert()
{
    v = -v;
}

CQuaternion CQuaternion::inverse() const
{
    return CQuaternion(r, -v);
}

CAxisAngle CQuaternion::toAxisAngle()
{
    CAxisAngle ret;
    ret.angle = acosf(r);

    float thetaInv = 1.0f/sinf(ret.angle);

    ret.axis.x = v.x * thetaInv;
    ret.axis.y = v.y * thetaInv;
    ret.axis.z = v.z * thetaInv;

    ret.angle *= 2;

    return ret;
}

CQuaternion CQuaternion::log() const
{
    float a = acosf(r);
    float sina = sinf(a);
    CQuaternion ret;

    ret.r = 0;

    if (sina > 0)
    {
        ret.v.x = a * v.x / sina;
        ret.v.y = a * v.y / sina;
        ret.v.z = a * v.z / sina;
    }
    else
        ret.v = CVector3f(0);

    return ret;
}

CQuaternion CQuaternion::exp() const
{
    float a = (v.magnitude());
    float sina = sinf(a);
    float cosa = cos(a);
    CQuaternion ret;

    ret.r = cosa;
    if (a > 0)
    {
        ret.v.x = sina * v.x / a;
        ret.v.y = sina * v.y / a;
        ret.v.z = sina * v.z / a;
    }
    else
        ret.v = CVector3f(0);

    return ret;
}

float CQuaternion::dot(const CQuaternion& b)
{
    return v.x * b.v.x + v.y * b.v.y + v.z * b.v.z + r * b.r;
}

CQuaternion CQuaternion::lerp(CQuaternion& a,  CQuaternion& b, double t)
{
    return (a + t * (b - a));
}

CQuaternion CQuaternion::nlerp(CQuaternion& a, CQuaternion& b, double t)
{
    return lerp(a, b, t).normalized();
}

CQuaternion CQuaternion::slerp(CQuaternion& a, CQuaternion& b, double t)
{
    if (t <= 0.0f)
        return a;
    if (t >= 1.0f)
        return b;

    CQuaternion ret;

    float mag = sqrtf(a.dot(a) * b.dot(b));

    float prod = a.dot(b) / mag;

    if (fabsf(prod) < 1.0)
    {
        const double sign = (prod < 0.0) ? -1.0 : 1.0;

        const double theta = acos(sign * prod);
        const double s1 = sin (sign * t * theta);
        const double d = 1.0 / sin(theta);
        const double s0 = sin((1.0 - t) * theta);

        ret.v.x = (float)(a.v.x * s0 + b.v.x * s1) * d;
        ret.v.y = (float)(a.v.y * s0 + b.v.y * s1) * d;
        ret.v.z = (float)(a.v.z * s0 + b.v.z * s1) * d;
        ret.r   = (float)(a.r   * s0 + b.r   * s1) * d;

        return ret;
    }
    return a;
}

CQuaternion operator+(float lhs, const CQuaternion& rhs)
{
    return CQuaternion(lhs + rhs.r, lhs * rhs.v);
}

CQuaternion operator-(float lhs, const CQuaternion& rhs)
{
    return CQuaternion(lhs - rhs.r, lhs * rhs.v);
}

CQuaternion operator*(float lhs, const CQuaternion& rhs)
{
    return CQuaternion(lhs * rhs.r, lhs * rhs.v);
}
}
