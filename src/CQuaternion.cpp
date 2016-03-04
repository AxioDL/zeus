#include "zeus/CQuaternion.hpp"
#include "zeus/Math.hpp"

namespace zeus
{
void CQuaternion::fromVector3f(const CVector3f& vec)
{
    float cosX = std::cos(0.5f * vec.x);
    float cosY = std::cos(0.5f * vec.y);
    float cosZ = std::cos(0.5f * vec.z);

    float sinX = std::sin(0.5f * vec.x);
    float sinY = std::sin(0.5f * vec.y);
    float sinZ = std::sin(0.5f * vec.z);

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
    return std::sqrt(magSquared());
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
//    CAxisAngle ret;
//    ret.angle = std::acos(r);

//    float thetaInv = 1.0f/std::sin(ret.angle);

//    ret.axis.x = v.x * thetaInv;
//    ret.axis.y = v.y * thetaInv;
//    ret.axis.z = v.z * thetaInv;

//    ret.angle *= 2.f;

//    return ret;
    return CAxisAngle();
}

CQuaternion CQuaternion::log() const
{
    float a = std::acos(r);
    float sina = std::sin(a);
    CQuaternion ret;

    ret.r = 0.f;

    if (sina > 0.f)
    {
        ret.v.x = a * v.x / sina;
        ret.v.y = a * v.y / sina;
        ret.v.z = a * v.z / sina;
    }
    else
        ret.v = CVector3f(0.f);

    return ret;
}

CQuaternion CQuaternion::exp() const
{
    float a = (v.magnitude());
    float sina = std::sin(a);
    float cosa = std::cos(a);
    CQuaternion ret;

    ret.r = cosa;
    if (a > 0.f)
    {
        ret.v.x = sina * v.x / a;
        ret.v.y = sina * v.y / a;
        ret.v.z = sina * v.z / a;
    }
    else
        ret.v = CVector3f(0.f);

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

    float mag = std::sqrt(a.dot(a) * b.dot(b));

    float prod = a.dot(b) / mag;

    if (std::fabs(prod) < 1.0f)
    {
        const double sign = (prod < 0.0f) ? -1.0f : 1.0f;

        const double theta = std::acos(sign * prod);
        const double s1 = std::sin(sign * t * theta);
        const double d = 1.0 / std::sin(theta);
        const double s0 = std::sin((1.0 - t) * theta);

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
