#ifndef CQUATERNION_HPP
#define CQUATERNION_HPP

#include "Global.hpp"
#include "CAxisAngle.hpp"
#include "zeus/CVector3f.hpp"
#include "zeus/CVector4f.hpp"
#include "zeus/CMatrix3f.hpp"
#include "zeus/Math.hpp"
#include "zeus/CRelAngle.hpp"
#include "zeus/CTransform.hpp"
#if ZE_ATHENA_TYPES
#include <athena/IStreamReader.hpp>
#endif

namespace zeus
{

static inline float normalize_angle(float angle)
{
    if (angle > M_PIF)
        angle = -((2.f * angle) - M_PIF);
    else if (angle < -M_PIF)
        angle = 2.f * angle + M_PIF;

    return angle;
}

class alignas(16) CQuaternion
{
#if __atdna__ && ZE_ATHENA_TYPES
    float clangVec __attribute__((__vector_size__(16)));
#endif
public:
    ZE_DECLARE_ALIGNED_ALLOCATOR();

    CQuaternion() : w(1.0f), x(0.0f), y(0.0f), z(0.0f) {}
    CQuaternion(float wi, float xi, float yi, float zi) : w(wi), x(xi), y(yi), z(zi) {}
    CQuaternion(float xi, float yi, float zi) { fromVector3f(CVector3f(xi, yi, zi)); }
    CQuaternion(float wi, const CVector3f& vec) : w(wi), x(vec.x), y(vec.y), z(vec.z) {}
#if ZE_ATHENA_TYPES
    inline void readBig(athena::io::IStreamReader& input)
    {
        w = input.readFloatBig();
        x = input.readFloatBig();
        y = input.readFloatBig();
        z = input.readFloatBig();
    }
    CQuaternion(const atVec4f& vec)
    {
#if __SSE__
        mVec128 = vec.mVec128;
#else
        x = vec.vec[1];
        y = vec.vec[2];
        z = vec.vec[3];
        w = vec.vec[0];
#endif
    }

    CQuaternion(const CMatrix3f& mat)
    {
        float trace = mat[0][0] + mat[1][1] + mat[2][2];
        if (trace > 0)
        {
            float s = 0.5f / std::sqrt(trace + 1.0f);
            w = 0.25f / s;
            x = (mat[1][2] - mat[2][1]) * s;
            y = (mat[2][0] - mat[0][2]) * s;
            z = (mat[0][1] - mat[1][0]) * s;
        }
        else
        {
            if (mat[0][0] > mat[1][1] && mat[0][0] > mat[2][2])
            {
                float s = 2.0f * std::sqrt(1.0f + mat[0][0] - mat[1][1] - mat[2][2]);
                w = (mat[1][2] - mat[2][1]) / s;
                x = 0.25f * s;
                y = (mat[1][0] + mat[0][1]) / s;
                z = (mat[2][0] + mat[0][2]) / s;
            }
            else if (mat[1][1] > mat[2][2])
            {
                float s = 2.0f * std::sqrt(1.0f + mat[1][1] - mat[0][0] - mat[2][2]);
                w = (mat[2][0] - mat[0][2]) / s;
                x = (mat[1][0] + mat[0][1]) / s;
                y = 0.25f * s;
                z = (mat[2][1] + mat[1][2]) / s;
            }
            else
            {
                float s = 2.0f * std::sqrt(1.0f + mat[2][2] - mat[0][0] - mat[1][1]);
                w = (mat[0][1] - mat[1][0]) / s;
                x = (mat[2][0] + mat[0][2]) / s;
                y = (mat[2][1] + mat[1][2]) / s;
                z = 0.25f * s;
            }
        }
    }

    operator atVec4f()
    {
        atVec4f ret;
#if __SSE__
        ret.mVec128 = mVec128;
#else
        ret.vec[0] = w;
        ret.vec[1] = x;
        ret.vec[2] = y;
        ret.vec[3] = z;
#endif
        return ret;
    }
    operator atVec4f() const
    {
        atVec4f ret;
#if __SSE__
        ret.mVec128 = mVec128;
#else
        ret.vec[0] = w;
        ret.vec[1] = x;
        ret.vec[2] = y;
        ret.vec[3] = z;
#endif
        return ret;
    }

#endif
    CQuaternion(const CVector3f& vec) { fromVector3f(vec); }
    CQuaternion(const CVector4f& vec)
    {
#if __SSE__
        mVec128 = vec.mVec128;
#else
        x = vec[1];
        y = vec[2];
        z = vec[3];
        w = vec[0];
#endif
    }

    CQuaternion(const CVector3f& vecA, const CVector3f& vecB)
    {
        CVector3f vecAN = vecA.normalized();
        CVector3f vecBN = vecB.normalized();
        CVector3f w = vecAN.cross(vecBN);
        *this = CQuaternion(1.f + vecAN.dot(vecBN), w.x, w.y, w.z).normalized();
    }

    void fromVector3f(const CVector3f& vec);

    CQuaternion& operator=(const CQuaternion& q);
    CQuaternion operator+(const CQuaternion& q) const;
    CQuaternion operator-(const CQuaternion& q) const;
    CQuaternion operator*(const CQuaternion& q) const;
    CQuaternion operator/(const CQuaternion& q) const;
    CQuaternion operator*(float scale) const;
    CQuaternion operator/(float scale) const;
    CQuaternion operator-() const;
    const CQuaternion& operator+=(const CQuaternion& q);
    const CQuaternion& operator-=(const CQuaternion& q);
    const CQuaternion& operator*=(const CQuaternion& q);
    const CQuaternion& operator*=(float scale);
    const CQuaternion& operator/=(float scale);
    float magnitude() const;
    float magSquared() const;
    void normalize();
    CQuaternion normalized() const;
    void invert();
    CQuaternion inverse() const;

    /**
     * @brief Set the rotation using axis angle notation
     * @param axis The axis to rotate around
     * @param angle The magnitude of the rotation in radians
     * @return
     */
    static inline CQuaternion fromAxisAngle(const CUnitVector3f& axis, const CRelAngle& angle)
    {
        return CQuaternion(std::cos(angle / 2.f), axis * std::sin(angle / 2.f));
    }

    void rotateX(const CRelAngle& angle) { *this *= fromAxisAngle({1.0f, 0.0f, 0.0f}, angle); }
    void rotateY(const CRelAngle& angle) { *this *= fromAxisAngle({0.0f, 1.0f, 0.0f}, angle); }
    void rotateZ(const CRelAngle& angle) { *this *= fromAxisAngle({0.0f, 0.0f, 1.0f}, angle); }

    static inline CVector3f rotate(const CQuaternion& rotation, const CAxisAngle& v)
    {
        CQuaternion q = rotation * v;
        q *= rotation.inverse();

        return {q.x, q.y, q.z};
    }

    static inline CQuaternion lookAt(const CUnitVector3f& target, const CUnitVector3f& up, const CRelAngle& c)
    {
        CQuaternion q = skNoRotation;
        zeus::CVector3f upCpy = up;
        zeus::CVector3f targetCpy = target;
        upCpy.z = 0.f;
        targetCpy.z = 0.f;
        zeus::CVector3f tmp;
        if (upCpy.magnitude() > 0.0009f && upCpy.magnitude() > 0.0009f)
        {
            targetCpy.normalize();
            upCpy.normalize();

            CRelAngle angleBetween =
                normalize_angle(std::atan2(targetCpy.x, targetCpy.y) - std::atan2(upCpy.x, upCpy.y));
            CRelAngle realAngle = zeus::clamp<CRelAngle>(-c, angleBetween, c);
            CQuaternion tmpQ;
            tmpQ.rotateZ(realAngle);
            q = tmpQ;
            CQuaternion q2 = (q * CQuaternion{0.f, targetCpy}) * -tmpQ;
            tmp.x = q2.x;
            tmp.y = q2.y;
            tmp.z = q2.z;
        }
        else if (upCpy.magnitude() > 0.0009f)
            tmp = targetCpy.normalized();
        else if (upCpy.magnitude() > 0.0009f)
            tmp = upCpy.normalized();
        else
            return skNoRotation;

        CRelAngle realAngle = zeus::clamp<CRelAngle>(-c, normalize_angle(std::acos(up.z) - std::acos(target.z)), c);
        return CQuaternion::fromAxisAngle(tmp.cross(kUpVec), realAngle) * q;
    }

    CVector3f transform(const CVector3f& v) const { return rotate(*this, v); }

    CQuaternion log() const;

    CQuaternion exp() const;

    inline CTransform toTransform() const { return CTransform(CMatrix3f(*this)); }
    inline CTransform toTransform(const zeus::CVector3f& origin) const { return CTransform(CMatrix3f(*this), origin); }
    float dot(const CQuaternion& quat) const;

    static CQuaternion lerp(const CQuaternion& a, const CQuaternion& b, double t);
    static CQuaternion slerp(const CQuaternion& a, const CQuaternion& b, double t);
    static CQuaternion nlerp(const CQuaternion& a, const CQuaternion& b, double t);

    inline float roll() const { return std::atan2(2.f * (x * y + w * z), w * w + x * x - y * y - z * z); }

    inline float pitch() const { return std::atan2(2.f * (y * z + w * x), w * w - x * x - y * y + z * z); }

    inline float yaw() const { return std::asin(-2.f * (x * z - w * y)); }

    inline float& operator[](size_t idx) { return (&w)[idx]; }
    inline const float& operator[](size_t idx) const { return (&w)[idx]; }

    union
    {
        __m128 mVec128;
        struct
        {
            float w, x, y, z;
        };
    };

    static const CQuaternion skNoRotation;
};

class alignas(16) CNUQuaternion : public CQuaternion
{
public:
    CNUQuaternion() = default;
    CNUQuaternion(const CMatrix3f& mtx) : CQuaternion(mtx) { normalize(); }

    CNUQuaternion(const CQuaternion& other) : CQuaternion(other) { normalize(); }
};

CQuaternion operator+(float lhs, const CQuaternion& rhs);
CQuaternion operator-(float lhs, const CQuaternion& rhs);
CQuaternion operator*(float lhs, const CQuaternion& rhs);
}
#endif // CQUATERNION_HPP
