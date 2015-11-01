#ifndef CQUATERNION_HPP
#define CQUATERNION_HPP

#include "Global.hpp"
#include "CAxisAngle.hpp"
#include "CVector3f.hpp"
#include "CVector4f.hpp"
#include <math.h>
#if ZE_ATHENA_TYPES
#include <Athena/IStreamReader.hpp>
#endif

namespace Zeus
{
class alignas(16) CQuaternion
{
public:
    ZE_DECLARE_ALIGNED_ALLOCATOR();
    
    CQuaternion() : r(1.0f) {}
    CQuaternion(float r, float x, float y, float z) : r(r), v(x, y, z){}
    CQuaternion(float x, float y, float z) { fromVector3f(CVector3f(x, y, z)); }
    CQuaternion(float r, const CVector3f& vec) : r(r), v(vec){}
#if ZE_ATHENA_TYPES
    CQuaternion(Athena::io::IStreamReader& input) { r = input.readFloat(); v = CVector3f(input);}
#endif
    CQuaternion(const CVector3f& vec) { fromVector3f(vec); }
    CQuaternion(const CVector4f& vec)
          : r(vec.w)
    {
#if __SSE__
        v.mVec128 = vec.mVec128; v.v[3] = 0;
#else
        v.x = vec.x; v.y = vec.y; v.z = vec.z;
#endif
    }

    virtual ~CQuaternion() {}

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
    static inline CQuaternion fromAxisAngle(const CVector3f& axis, float angle)
    {
        return CQuaternion(cosf(angle/2), axis*sinf(angle/2));
    }

    CAxisAngle toAxisAngle();

    static inline CVector3f rotate(const CQuaternion& rotation, const CVector3f& v)
    {
        CQuaternion q = rotation * v;
        q *= rotation.inverse();

        return q.v;
    }

    CQuaternion log() const;

    CQuaternion exp() const;

    float dot(const CQuaternion& quat);

    static CQuaternion lerp(CQuaternion& a, CQuaternion& b, double t);
    static CQuaternion slerp(CQuaternion& a, CQuaternion& b, double t);
    static CQuaternion nlerp(CQuaternion& a, CQuaternion& b, double t);
    
    inline float roll() const
    {
        return atan2f(2.0 * (v.x * v.y + r * v.z), r * r + v.x * v.x - v.y * v.y - v.z * v.z);
    }
    
    inline float pitch() const
    {
        return atan2f(2.0 * (v.y * v.z + r * v.x), r * r - v.x * v.x - v.y * v.y + v.z * v.z);
    }
    
    inline float yaw() const
    {
        return asinf(-2.0 * (v.x * v.z - r * v.y));
    }

    float r;
    CVector3f v;
};

CQuaternion operator+(float lhs, const CQuaternion& rhs);
CQuaternion operator-(float lhs, const CQuaternion& rhs);
CQuaternion operator*(float lhs, const CQuaternion& rhs);
}
#endif // CQUATERNION_HPP
