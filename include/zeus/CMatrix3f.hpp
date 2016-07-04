#ifndef CMATRIX3F_HPP
#define CMATRIX3F_HPP

#include "Global.hpp"
#include "zeus/CVector3f.hpp"
#include <assert.h>
#include <string.h>

/* Column-major matrix class */
namespace zeus
{
class CQuaternion;
class alignas(16) CMatrix3f
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
    CMatrix3f(float scale)
    : CMatrix3f(CVector3f(scale)) {}
    CMatrix3f(const CVector3f& r0, const CVector3f& r1, const CVector3f& r2)
    {vec[0] = r0; vec[1] = r1; vec[2] = r2;}
    CMatrix3f(const CMatrix3f& other)
    {vec[0] = other.vec[0]; vec[1] = other.vec[1]; vec[2] = other.vec[2];}
#if __SSE__
    CMatrix3f(const __m128& r0, const __m128& r1, const __m128& r2)
    {vec[0].mVec128 = r0; vec[1].mVec128 = r1; vec[2].mVec128 = r2;}
#endif
#if ZE_ATHENA_TYPES
    CMatrix3f(const atVec4f& r0, const atVec4f& r1, const atVec4f& r2)
    {
#if __SSE__
        vec[0].mVec128 = r0.mVec128; vec[1].mVec128 = r1.mVec128; vec[2].mVec128 = r2.mVec128;
#else
        vec[0].x = r0.vec[0]; vec[0].y = r0.vec[1]; vec[0].z = r0.vec[2];
        vec[1].x = r1.vec[0]; vec[1].y = r1.vec[1]; vec[1].z = r1.vec[2];
        vec[2].x = r2.vec[0]; vec[2].y = r2.vec[1]; vec[2].z = r2.vec[2];
#endif
    }
    void readBig(athena::io::IStreamReader& input)
    {
        m[0][0] = input.readFloatBig();
        m[1][0] = input.readFloatBig();
        m[2][0] = input.readFloatBig();
        m[0][1] = input.readFloatBig();
        m[1][1] = input.readFloatBig();
        m[2][1] = input.readFloatBig();
        m[0][2] = input.readFloatBig();
        m[1][2] = input.readFloatBig();
        m[2][2] = input.readFloatBig();
    }
#endif
    CMatrix3f(const CVector3f& axis, float angle);
    CMatrix3f(const CQuaternion& quat);
    CMatrix3f(const TVectorUnion& r0, const TVectorUnion& r1, const TVectorUnion& r2)
    {
#if __SSE__
        vec[0].mVec128 = r0.mVec128; vec[1].mVec128 = r1.mVec128; vec[2].mVec128 = r2.mVec128;
#else
        vec[0].x = r0.vec[0]; vec[0].y = r0.vec[1]; vec[0].z = r0.vec[2];
        vec[1].x = r1.vec[0]; vec[1].y = r1.vec[1]; vec[1].z = r1.vec[2];
        vec[2].x = r2.vec[0]; vec[2].y = r2.vec[1]; vec[2].z = r2.vec[2];
#endif
    }

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

    inline CMatrix3f orthonormalized() const
    {
        CMatrix3f ret;
        ret[0] = vec[0].normalized();
        ret[2] = ret[0].cross(vec[1]);
        ret[2].normalize();
        ret[1] = ret[2].cross(ret[0]);
        return ret;
    }

    static const CMatrix3f skIdentityMatrix3f;
    
    void transpose();
    void transposeSSE3();
    CMatrix3f transposed() const;
    CMatrix3f transposedSSE3() const;

    inline void invert() {*this = inverted();}
    CMatrix3f inverted() const;

    void addScaledMatrix(const CMatrix3f& other, float scale)
    {
        CVector3f scaleVec(scale);
        vec[0] += other.vec[0] * scaleVec;
        vec[1] += other.vec[1] * scaleVec;
        vec[2] += other.vec[2] * scaleVec;
    }
    
    union
    {
        float m[3][4]; /* 4th row for union-alignment */
        struct
        {
            CVector3f vec[3];
        };
    };
};

static CMatrix3f operator*(const CMatrix3f& lhs, const CMatrix3f& rhs)
{
#if __SSE__
    unsigned i;
    TVectorUnion resVec[3];
    for (i=0 ; i<3 ; ++i) {
        resVec[i].mVec128 =
        _mm_add_ps(_mm_add_ps(
                   _mm_mul_ps(lhs[0].mVec128, ze_splat_ps(rhs[i].mVec128, 0)),
                   _mm_mul_ps(lhs[1].mVec128, ze_splat_ps(rhs[i].mVec128, 1))),
                   _mm_mul_ps(lhs[2].mVec128, ze_splat_ps(rhs[i].mVec128, 2)));
        resVec[i].v[3] = 0.0;
    }
    return CMatrix3f(resVec[0].mVec128, resVec[1].mVec128, resVec[2].mVec128);
#else
    return CMatrix3f(lhs[0][0] * rhs[0][0] + lhs[1][0] * rhs[0][1] + lhs[2][0] * rhs[0][2],
                     lhs[0][0] * rhs[1][0] + lhs[1][0] * rhs[1][1] + lhs[2][0] * rhs[1][2],
                     lhs[0][0] * rhs[2][0] + lhs[1][0] * rhs[2][1] + lhs[2][0] * rhs[2][2],
                     lhs[0][1] * rhs[0][0] + lhs[1][1] * rhs[0][1] + lhs[2][1] * rhs[0][2],
                     lhs[0][1] * rhs[1][0] + lhs[1][1] * rhs[1][1] + lhs[2][1] * rhs[1][2],
                     lhs[0][1] * rhs[2][0] + lhs[1][1] * rhs[2][1] + lhs[2][1] * rhs[2][2],
                     lhs[0][2] * rhs[0][0] + lhs[1][2] * rhs[0][1] + lhs[2][2] * rhs[0][2],
                     lhs[0][2] * rhs[1][0] + lhs[1][2] * rhs[1][1] + lhs[2][2] * rhs[1][2],
                     lhs[0][2] * rhs[2][0] + lhs[1][2] * rhs[2][1] + lhs[2][2] * rhs[2][2]);
#endif
}

}

#endif // CMATRIX3F_HPP
