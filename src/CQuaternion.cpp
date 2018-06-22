#include "zeus/CQuaternion.hpp"
#include "zeus/Math.hpp"

namespace zeus
{
const CQuaternion CQuaternion::skNoRotation;

CQuaternion::CQuaternion(const CMatrix3f& mat)
{
    float trace = mat[0][0] + mat[1][1] + mat[2][2];
    if (trace >= 0.f)
    {
        float st = std::sqrt(trace + 1.0f);
        float s = 0.5f / st;
        w = 0.5f * st;
        x = (mat[1][2] - mat[2][1]) * s;
        y = (mat[2][0] - mat[0][2]) * s;
        z = (mat[0][1] - mat[1][0]) * s;
    }
    else
    {
        int idx = 0;
        if (mat[1][1] > mat[0][0])
        {
            idx = 1;
            if (mat[2][2] > mat[1][1])
                idx = 2;
        }
        else if (mat[2][2] > mat[0][0])
        {
            idx = 2;
        }

        switch (idx)
        {
        case 0:
        {
            float st = std::sqrt(mat[0][0] - (mat[1][1] + mat[2][2]) + 1.f);
            float s = 0.5f / st;
            w = (mat[1][2] - mat[2][1]) * s;
            x = 0.5f * st;
            y = (mat[1][0] + mat[0][1]) * s;
            z = (mat[2][0] + mat[0][2]) * s;
            break;
        }
        case 1:
        {
            float st = std::sqrt(mat[1][1] - (mat[2][2] + mat[0][0]) + 1.f);
            float s = 0.5f / st;
            w = (mat[2][0] - mat[0][2]) * s;
            x = (mat[1][0] + mat[0][1]) * s;
            y = 0.5f * st;
            z = (mat[2][1] + mat[1][2]) * s;
            break;
        }
        case 2:
        {
            float st = std::sqrt(mat[2][2] - (mat[0][0] + mat[1][1]) + 1.f);
            float s = 0.5f / st;
            w = (mat[0][1] - mat[1][0]) * s;
            x = (mat[2][0] + mat[0][2]) * s;
            y = (mat[2][1] + mat[1][2]) * s;
            z = 0.5f * st;
            break;
        }
        default:
            w = 0.f;
            x = 0.f;
            y = 0.f;
            z = 0.f;
            break;
        }
    }
}

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

CQuaternion CQuaternion::operator+(const CQuaternion& q) const { return CQuaternion(w + q.w, x + q.x, y + q.y, z + q.z); }

CQuaternion CQuaternion::operator-(const CQuaternion& q) const { return CQuaternion(w - q.w, x - q.x, y - q.y, z - q.z); }

CQuaternion CQuaternion::operator*(const CQuaternion& q) const
{
    return CQuaternion(w * q.w - CVector3f(x, y, z).dot({q.x, q.y, q.z}),
                       y * q.z - z * q.y + w * q.x + x * q.w,
                       z * q.x - x * q.z + w * q.y + y * q.w,
                       x * q.y - y * q.x + w * q.z + z * q.w);
}

CNUQuaternion CNUQuaternion::operator*(const CNUQuaternion& q) const
{
    return CNUQuaternion(w * q.w - CVector3f(x, y, z).dot({q.x, q.y, q.z}),
                         y * q.z - z * q.y + w * q.x + x * q.w,
                         z * q.x - x * q.z + w * q.y + y * q.w,
                         x * q.y - y * q.x + w * q.z + z * q.w);
}

CQuaternion CQuaternion::operator/(const CQuaternion& q) const
{
    CQuaternion p(q);
    p.invert();
    return *this * p;
}

CQuaternion CQuaternion::operator*(float scale) const { return CQuaternion(w * scale, x * scale, y * scale, z * scale); }

CNUQuaternion CNUQuaternion::operator*(float scale) const { return CNUQuaternion(w * scale, x * scale, y * scale, z * scale); }

CQuaternion CQuaternion::operator/(float scale) const { return CQuaternion(w / scale, x / scale, y / scale, z / scale); }

CQuaternion CQuaternion::operator-() const { return CQuaternion(-w, -x, -y, -z); }

const CQuaternion& CQuaternion::operator+=(const CQuaternion& q)
{
    w += q.w;
    x += q.x;
    y += q.y;
    z += q.z;
    return *this;
}

const CNUQuaternion& CNUQuaternion::operator+=(const CNUQuaternion& q)
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

const CQuaternion& CQuaternion::operator*=(const CQuaternion& q)
{
    CQuaternion orig = *this;

    w = orig.w * q.w - CVector3f(orig.x, orig.y, orig.z).dot({q.x, q.y, q.z});
    x = orig.y * q.z - orig.z * q.y + orig.w * q.x + orig.x * q.w;
    y = orig.z * q.x - orig.x * q.z + orig.w * q.y + orig.y * q.w;
    z = orig.x * q.y - orig.y * q.x + orig.w * q.z + orig.z * q.w;

    return *this;
}

const CQuaternion& CQuaternion::operator*=(float scale)
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

void CQuaternion::invert()
{
    x = -x;
    y = -y;
    z = -z;
}

CQuaternion CQuaternion::inverse() const { return CQuaternion(w, -x, -y, -z); }

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

CQuaternion CQuaternion::lerp(const CQuaternion& a, const CQuaternion& b, double t) { return (a + t * (b - a)); }

CQuaternion CQuaternion::nlerp(const CQuaternion& a, const CQuaternion& b, double t) { return lerp(a, b, t).normalized(); }

CQuaternion CQuaternion::slerp(const CQuaternion& a, const CQuaternion& b, double t)
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

        ret.x = float((a.x * s0 + b.x * s1) * d);
        ret.y = float((a.y * s0 + b.y * s1) * d);
        ret.z = float((a.z * s0 + b.z * s1) * d);
        ret.w = float((a.w * s0 + b.w * s1) * d);

        return ret;
    }
    return a;
}

CQuaternion CQuaternion::shortestRotationArc(const zeus::CVector3f& v0, const zeus::CVector3f& v1)
{
    CVector3f v0N = v0;
    CVector3f v1N = v1;

    if (!v0N.isZero())
        v0N.normalize();
    if (!v1N.isZero())
        v1N.normalize();

    CVector3f cross = v0N.cross(v1N);

    if (cross.magSquared() < 0.001f)
    {
        if (v0N.dot(v1N) > 0.f)
            return CQuaternion::skNoRotation;
        if (cross.canBeNormalized())
            return CQuaternion(0.0f, cross.normalized());
        return CQuaternion::skNoRotation;
    }
    else
    {
        float w = std::sqrt((1.f + zeus::clamp(-1.f, v0N.dot(v1N), 1.f)) * 2.f);
        return CQuaternion(0.5f * w, cross * (1.f / w));
    }
}

CQuaternion CQuaternion::clampedRotateTo(const zeus::CUnitVector3f& v0, const zeus::CUnitVector3f& v1,
                                         const zeus::CRelAngle& angle)
{
    CQuaternion arc = shortestRotationArc(v0, v1);
    if (angle >= 2.f * std::acos(arc.w))
        return arc;

    return fromAxisAngle(arc.getImaginary(), angle);
}

CQuaternion CQuaternion::slerpShort(const CQuaternion& a, const CQuaternion& b, double t)
{
    return zeus::CQuaternion::slerp((b.dot(a) >= 0.f) ? a : a.buildEquivalent(), b, t);
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

CNUQuaternion operator*(float lhs, const CNUQuaternion& rhs)
{
    return CNUQuaternion(lhs * rhs.w, lhs * rhs.x, lhs * rhs.y, lhs * rhs.z);
}

CQuaternion CQuaternion::buildEquivalent() const
{
    float tmp = std::acos(clamp(-1.f, w, 1.f)) * 2.0;
    if (std::fabs(tmp) < 1.0e-7)
        return {-1.f, 0.f, 0.f, 0.f};
    else
        return CQuaternion::fromAxisAngle(CUnitVector3f(x, y, z), tmp + 2.0 * M_PI);
}

CRelAngle CQuaternion::angleFrom(const zeus::CQuaternion& other)
{
    return std::acos(zeus::clamp(-1.f, dot(other), 1.f));
}

CQuaternion CQuaternion::lookAt(const CUnitVector3f& source, const CUnitVector3f& dest, const CRelAngle& maxAng)
{
    CQuaternion q = skNoRotation;
    zeus::CVector3f destNoZ = dest;
    zeus::CVector3f sourceNoZ = source;
    destNoZ.z = 0.f;
    sourceNoZ.z = 0.f;
    zeus::CVector3f tmp;
    if (sourceNoZ.magSquared() > 0.0001f && destNoZ.magSquared() > 0.0001f)
    {
        sourceNoZ.normalize();
        destNoZ.normalize();

        float angleBetween =
            normalize_angle(std::atan2(destNoZ.x, destNoZ.y) - std::atan2(sourceNoZ.x, sourceNoZ.y));
        float realAngle = zeus::clamp(-maxAng.asRadians(), angleBetween, maxAng.asRadians());
        CQuaternion tmpQ;
        tmpQ.rotateZ(-realAngle);
        q = tmpQ;
        tmp = q.transform(sourceNoZ);
    }
    else if (sourceNoZ.magSquared() > 0.0001f)
        tmp = sourceNoZ.normalized();
    else if (destNoZ.magSquared() > 0.0001f)
        tmp = destNoZ.normalized();
    else
        return skNoRotation;

    float realAngle =
        zeus::clamp(-maxAng.asRadians(), normalize_angle(std::acos(dest.z) - std::acos(source.z)), maxAng.asRadians());
    return CQuaternion::fromAxisAngle(tmp.cross(CVector3f::skUp), -realAngle) * q;
}

}
