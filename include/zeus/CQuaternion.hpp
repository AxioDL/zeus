#pragma once

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
        angle -= 2.f * M_PIF;
    else if (angle < -M_PIF)
        angle += 2.f * M_PIF;

    return angle;
}

class CNUQuaternion;

/** Unit quaternion, used for all quaternion arithmetic */
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

    operator atVec4f&()
    {
        return *reinterpret_cast<atVec4f*>(v);
    }
    operator const atVec4f&() const
    {
        return *reinterpret_cast<const atVec4f*>(v);
    }

#endif
    
    CQuaternion(const CMatrix3f& mat);
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
    float magnitude() const { return std::sqrt(magSquared()); }
    float magSquared() const { return w * w + x * x + y * y + z * z; }
    void normalize() { *this /= magnitude(); }
    CQuaternion normalized() const { return *this / magnitude(); }
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

    static CQuaternion lookAt(const CUnitVector3f& source, const CUnitVector3f& dest, const CRelAngle& maxAng);

    CVector3f transform(const CVector3f& v) const
    {
        CQuaternion r(0.f, v);
        return (*this * r * inverse()).getImaginary();
    }

    CQuaternion log() const;

    CQuaternion exp() const;

    inline CTransform toTransform() const { return CTransform(CMatrix3f(*this)); }
    inline CTransform toTransform(const zeus::CVector3f& origin) const { return CTransform(CMatrix3f(*this), origin); }
    inline float dot(const CQuaternion& rhs) const
    {
#if __SSE__
        TVectorUnion result;
#if __SSE4_1__
            result.mVec128 = _mm_dp_ps(mVec128, rhs.mVec128, 0xF1);
            return result.v[0];
#else
        result.mVec128 = _mm_mul_ps(mVec128, rhs.mVec128);
        return result.v[0] + result.v[1] + result.v[2] + result.v[3];
#endif
#else
        return (x * rhs.x) + (y * rhs.y) + (z * rhs.z) + (w * rhs.w);
#endif
    }

    static CQuaternion lerp(const CQuaternion& a, const CQuaternion& b, double t);
    static CQuaternion slerp(const CQuaternion& a, const CQuaternion& b, double t);
    static CQuaternion slerpShort(const CQuaternion& a, const CQuaternion& b, double t);
    static CQuaternion nlerp(const CQuaternion& a, const CQuaternion& b, double t);
    static CQuaternion shortestRotationArc(const zeus::CVector3f& v0, const zeus::CVector3f& v1);
    static CQuaternion clampedRotateTo(const zeus::CUnitVector3f& v0, const zeus::CUnitVector3f& v1,
                                       const zeus::CRelAngle& angle);

    inline float roll() const { return std::atan2(2.f * (x * y + w * z), w * w + x * x - y * y - z * z); }

    inline float pitch() const { return std::atan2(2.f * (y * z + w * x), w * w - x * x - y * y + z * z); }

    inline float yaw() const { return std::asin(-2.f * (x * z - w * y)); }

    CQuaternion buildEquivalent() const;

    zeus::CVector3f getImaginary() const { return {x, y, z}; }
    void setImaginary(const zeus::CVector3f& i) { x = i.x; y = i.y; z = i.z; }

    CRelAngle angleFrom(const zeus::CQuaternion& other);

    inline float& operator[](size_t idx) { assert(idx < 4); return (&w)[idx]; }
    inline const float& operator[](size_t idx) const { assert(idx < 4); return (&w)[idx]; }

    union
    {
        __m128 mVec128;
        struct
        {
            float w, x, y, z;
        };
        float v[4];
    };

    static const CQuaternion skNoRotation;

    static CQuaternion fromNUQuaternion(const CNUQuaternion& q);
};

/** Non-unit quaternion, no guarantee that it's normalized.
 *  Converting to CQuaternion will perform normalize operation.
 */
class alignas(16) CNUQuaternion
{
public:
    CNUQuaternion() : w(1.0f), x(0.0f), y(0.0f), z(0.0f) {}
    CNUQuaternion(float wi, float xi, float yi, float zi) : w(wi), x(xi), y(yi), z(zi) {}
    CNUQuaternion(float win, const zeus::CVector3f& vec) { w = win; x = vec.x; y = vec.y; z = vec.z; }
    CNUQuaternion(const CQuaternion& other) { w = other.w; x = other.x; y = other.y; z = other.z; }
    CNUQuaternion(const CMatrix3f& mtx) : CNUQuaternion(CQuaternion(mtx)) {}
    static inline CNUQuaternion fromAxisAngle(const CUnitVector3f& axis, const CRelAngle& angle)
    {
        return CNUQuaternion(CQuaternion::fromAxisAngle(axis, angle));
    }

    float magnitude() const { return std::sqrt(magSquared()); }
    float magSquared() const { return w * w + x * x + y * y + z * z; }
    void normalize()
    {
        float magDiv = 1.f / magnitude();
        w *= magDiv;
        x *= magDiv;
        y *= magDiv;
        z *= magDiv;
    }
    CNUQuaternion normalized() const
    {
        float magDiv = 1.f / magnitude();
        return { w * magDiv, x * magDiv, y * magDiv, z * magDiv };
    }

    CNUQuaternion operator*(const CNUQuaternion& q) const;
    CNUQuaternion operator*(float f) const;
    const CNUQuaternion& operator+=(const CNUQuaternion& q);

    inline float& operator[](size_t idx) { assert(idx < 4); return (&w)[idx]; }
    inline const float& operator[](size_t idx) const { assert(idx < 4); return (&w)[idx]; }

    union
    {
        __m128 mVec128;
        struct
        {
            float w, x, y, z;
        };
    };
};

inline CQuaternion CQuaternion::fromNUQuaternion(const CNUQuaternion& q)
{
    auto norm = q.normalized();
    return { norm.w, norm.x, norm.y, norm.z };
}

CQuaternion operator+(float lhs, const CQuaternion& rhs);
CQuaternion operator-(float lhs, const CQuaternion& rhs);
CQuaternion operator*(float lhs, const CQuaternion& rhs);
CNUQuaternion operator*(float lhs, const CNUQuaternion& rhs);
}
