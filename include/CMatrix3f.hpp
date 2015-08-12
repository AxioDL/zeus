#ifndef CMATRIX3F_HPP
#define CMATRIX3F_HPP

#include "Global.hpp"
#include "CVector3f.hpp"
#include <assert.h>

/* Column-major matrix class */

class CQuaternion;
class ZE_ALIGN(16) CMatrix3f
{
public:
    ZE_DECLARE_ALIGNED_ALLOCATOR();
    
    explicit CMatrix3f(bool zero = false)
    {
        memset(m, 0, sizeof(m));
        if (!zero)
        {
            m[0][0] = 1.0;
            m[1][1] = 1.0;
            m[2][2] = 1.0;
        }
    }
    CMatrix3f(float m00, float m01, float m02,
              float m10, float m11, float m12,
              float m20, float m21, float m22)
    {
        m[0][0] = m00, m[1][0] = m01, m[2][0] = m02;
        m[0][1] = m10, m[1][1] = m11, m[2][1] = m12;
        m[0][2] = m20, m[1][2] = m21, m[2][2] = m22;
    }
    CMatrix3f(const CVector3f& scaleVec)
    {
        memset(m, 0, sizeof(m));
        m[0][0] = scaleVec[0];
        m[1][1] = scaleVec[1];
        m[2][2] = scaleVec[2];
    }
    CMatrix3f(const CVector3f& r0, const CVector3f& r1, const CVector3f& r2)
    {vec[0] = r0; vec[1] = r1; vec[2] = r2;}
    CMatrix3f(const CMatrix3f& other)
    {vec[0] = other.vec[0]; vec[1] = other.vec[1]; vec[2] = other.vec[2];}
#if __SSE__
    CMatrix3f(const __m128& r0, const __m128& r1, const __m128& r2)
    {vec[0].mVec128 = r0; vec[1].mVec128 = r1; vec[2].mVec128 = r2;}
#endif
    CMatrix3f(const CVector3f& axis, float angle);
    CMatrix3f(const CQuaternion& quat);
    
    inline CMatrix3f& operator=(const CMatrix3f& other)
    {
        vec[0] = other.vec[0];
        vec[1] = other.vec[1];
        vec[2] = other.vec[2];
        return *this;
    }
    
    inline CVector3f operator*(const CVector3f& other) const
    {
#if __SSE__
        TVectorUnion res;
        res.mVec128 =
        _mm_add_ps(_mm_add_ps(
                   _mm_mul_ps(vec[0].mVec128, ze_splat_ps(other.mVec128, 0)),
                   _mm_mul_ps(vec[1].mVec128, ze_splat_ps(other.mVec128, 1))),
                   _mm_mul_ps(vec[2].mVec128, ze_splat_ps(other.mVec128, 2)));
        return CVector3f(res.mVec128);
#else
        return CVector3f(m[0][0] * other.v[0] + m[1][0] * other.v[1] + m[2][0] * other.v[2],
                         m[0][1] * other.v[0] + m[1][1] * other.v[1] + m[2][1] * other.v[2],
                         m[0][2] * other.v[0] + m[1][2] * other.v[1] + m[2][2] * other.v[2]);
#endif
    }
    
    inline CVector3f& operator[](int i)
    {
        assert(0 <= i && i < 3);
        return vec[i];
    }
    
    inline const CVector3f& operator[](int i) const
    {
        assert(0 <= i && i < 3);
        return vec[i];
    }

    static const CMatrix3f skIdentityMatrix3f;
    
    void transpose();
    CMatrix3f transposed() const;

    inline void invert() {*this = inverted();}
    CMatrix3f inverted() const;
    
    union
    {
        float m[3][4]; /* 4th row for union-alignment */
        struct
        {
            CVector3f vec[3];
        };
    };
};

CMatrix3f operator*(const CMatrix3f& lhs, const CMatrix3f& rhs);

#endif // CMATRIX3F_HPP
