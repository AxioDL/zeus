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

    w = cosZ * cosY * cosX + sinZ * sinY * sinX;
    x = cosZ * cosY * sinX - sinZ * sinY * cosX;
    y = cosZ * sinY * cosX + sinZ * cosY * sinX;
    z = sinZ * cosY * cosX - cosZ * sinY * sinX;
}

CQuaternion& CQuaternion::operator=(const CQuaternion& q)
{
#if __SSE__
    mVec128 = q.mVec128;
#else
    w = q.w;
    x = q.x;
    y = q.y;
    z = q.z;
#endif
    return *this;
}

CQuaternion CQuaternion::operator+(const CQuaternion& q) const
{
    return CQuaternion(w + q.w, x + q.x, y + q.y, z + q.z);
}

CQuaternion CQuaternion::operator-(const CQuaternion& q) const
{
    return CQuaternion(w - q.w, x - q.x, y - q.y, z - q.z);
}

CQuaternion CQuaternion::operator*(const CQuaternion& q) const
{
    return CQuaternion(w*q.w - CVector3f(x, y, z).dot({q.x, q.y, q.z}),
                       y * q.z - z * q.y + w * q.x + x*q.w,
                       z * q.x - x * q.z + w * q.y + y*q.w,
                       x * q.y - y * q.x + w * q.z + z*q.w);
}

CQuaternion CQuaternion::operator/(const CQuaternion& q) const
{
    CQuaternion p(q);
    p.invert();
    return *this * p;
}

CQuaternion CQuaternion::operator*(float scale) const
{
    return CQuaternion(w*scale, x*scale, y*scale, z*scale);
}

CQuaternion CQuaternion::operator/(float scale) const
{
    return CQuaternion(w/scale, x/scale, y/scale, z/scale);
}

CQuaternion CQuaternion::operator-() const
{
    return CQuaternion(-w, -x, -y, -z);
}

const CQuaternion& CQuaternion::operator+=(const CQuaternion& q)
{
    w += q.w;
    x += q.x;
    y += q.y;
    z += q.z;
    return *this;
}

const CQuaternion& CQuaternion::operator-=(const CQuaternion& q)
{
    w -= q.w;
    x -= q.x;
    y -= q.y;
    z -= q.z;
    return *this;
}

const CQuaternion& CQuaternion::operator *=(const CQuaternion& q)
{
    w = w*q.w - CVector3f(x, y, z).dot({q.x, q.y, q.z});
    x = y * q.z - z * q.y + w * q.x + x*q.w;
    y = z * q.x - x * q.z + w * q.y + y*q.w;
    z = x * q.y - y * q.x + w * q.z + z*q.w;

    return *this;
}

const CQuaternion& CQuaternion::operator *=(float scale)
{
    w *= scale;
    x *= scale;
    y *= scale;
    z *= scale;
    return *this;
}

const CQuaternion& CQuaternion::operator/=(float scale)
{
    w /= scale;
    x /= scale;
    y /= scale;
    z /= scale;
    return *this;
}

float CQuaternion::magnitude() const
{
    return std::sqrt(magSquared());
}

float CQuaternion::magSquared() const
{
    return w*w + x*x + y*y + z*z;
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
    x = -x;
    y = -y;
    z = -z;
}

CQuaternion CQuaternion::inverse() const
{
    return CQuaternion(w, -x, -y, -z);
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
    float a = std::acos(w);
    float sina = std::sin(a);
    CQuaternion ret;

    ret.w = 0.f;

    if (sina > 0.f)
    {
        ret.x = a * x / sina;
        ret.y = a * y / sina;
        ret.z = a * z / sina;
    }
    else
    {
        ret.x = 0.f;
        ret.y = 0.f;
        ret.z = 0.f;
    }

    return ret;
}

CQuaternion CQuaternion::exp() const
{
    float a = (CVector3f(x, y, z).magnitude());
    float sina = std::sin(a);
    float cosa = std::cos(a);
    CQuaternion ret;

    ret.w = cosa;
    if (a > 0.f)
    {
        ret.x = sina * x / a;
        ret.y = sina * y / a;
        ret.z = sina * z / a;
    }
    else
    {
        ret.x = 0.f;
        ret.y = 0.f;
        ret.z = 0.f;
    }

    return ret;
}

float CQuaternion::dot(const CQuaternion& b)
{
    return x * b.x + y * b.y + z * b.z + w * b.w;
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

        ret.x = (float)(a.x * s0 + b.x * s1) * d;
        ret.y = (float)(a.y * s0 + b.y * s1) * d;
        ret.z = (float)(a.z * s0 + b.z * s1) * d;
        ret.w = (float)(a.w * s0 + b.w * s1) * d;

        return ret;
    }
    return a;
}

CQuaternion operator+(float lhs, const CQuaternion& rhs)
{
    return CQuaternion(lhs + rhs.w, lhs * rhs.x, lhs * rhs.y, lhs * rhs.z);
}

CQuaternion operator-(float lhs, const CQuaternion& rhs)
{
    return CQuaternion(lhs - rhs.w, lhs * rhs.x, lhs * rhs.y, lhs * rhs.z);
}

CQuaternion operator*(float lhs, const CQuaternion& rhs)
{
    return CQuaternion(lhs * rhs.w, lhs * rhs.x, lhs * rhs.y, lhs * rhs.z);
}
}
