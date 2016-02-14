#ifndef CMATRIX4F
#define CMATRIX4F
#include "CMatrix3f.hpp"
#include "CVector4f.hpp"
#include "CVector3f.hpp"

namespace Zeus
{
class alignas(16) CMatrix4f
{
public:
    static const CMatrix4f skIdentityMatrix4f;
    ZE_DECLARE_ALIGNED_ALLOCATOR();
    explicit CMatrix4f(bool zero = false)
    {
        memset(m, 0, sizeof(m));

        if (!zero)
        {
            m[0][0] = 1.0;
            m[1][1] = 1.0;
            m[2][2] = 1.0;
            m[3][3] = 1.0;
        }
    }
    CMatrix4f(float m00, float m01, float m02, float m03,
              float m10, float m11, float m12, float m13,
              float m20, float m21, float m22, float m23,
              float m30, float m31, float m32, float m33)
    {
        m[0][0] = m00, m[1][0] = m01, m[2][0] = m02, m[3][0] = m03;
        m[0][1] = m10, m[1][1] = m11, m[2][1] = m12, m[3][1] = m13;
        m[0][2] = m20, m[1][2] = m21, m[2][2] = m22, m[3][2] = m23;
        m[0][3] = m30, m[1][3] = m31, m[2][3] = m32, m[3][3] = m33;
    }
    CMatrix4f(const CVector3f& scaleVec)
    {
        memset(m, 0, sizeof(m));
        m[0][0] = scaleVec[0];
        m[1][1] = scaleVec[1];
        m[2][2] = scaleVec[2];
        m[3][3] = 1.0f;
    }
    CMatrix4f(const CVector4f& r0, const CVector4f& r1, const CVector4f& r2, const CVector4f& r3)
    {vec[0] = r0; vec[1] = r1; vec[2] = r2; vec[3] = r3;}
    CMatrix4f(const CMatrix4f& other)
    {vec[0] = other.vec[0]; vec[1] = other.vec[1]; vec[2] = other.vec[2]; vec[3] = other.vec[3];}
#if __SSE__
    CMatrix4f(const __m128& r0, const __m128& r1, const __m128& r2, const __m128& r3)
    {vec[0].mVec128 = r0; vec[1].mVec128 = r1; vec[2].mVec128 = r2; vec[3].mVec128 = r3;}
#endif
    CMatrix4f(const CMatrix3f& other)
    {
        memset(m, 0, sizeof(m));
        vec[0] = other.vec[0];
        vec[1] = other.vec[1];
        vec[2] = other.vec[2];
        vec[3] = CVector4f(0, 0, 0, 1.0f);
    }
    inline CMatrix4f& operator=(const CMatrix4f& other)
    {
        vec[0] = other.vec[0];
        vec[1] = other.vec[1];
        vec[2] = other.vec[2];
        vec[3] = other.vec[3];
        return *this;
    }
    inline CVector4f operator*(const CVector4f& other) const
    {
#if __SSE__
        TVectorUnion res;
        res.mVec128 = _mm_add_ps(_mm_add_ps(_mm_mul_ps(vec[0].mVec128, ze_splat_ps(other.mVec128, 0)),
                                            _mm_mul_ps(vec[1].mVec128, ze_splat_ps(other.mVec128, 1))),
                                 _mm_add_ps(_mm_mul_ps(vec[2].mVec128, ze_splat_ps(other.mVec128, 2)),
                                            _mm_mul_ps(vec[3].mVec128, ze_splat_ps(other.mVec128, 3))));

        return CVector4f(res.mVec128);
#else
        return CVector4f(
                   m[0][0] * other.v[0] + m[1][0] * other.v[1] + m[2][0] * other.v[2] + m[3][0] * other.v[3],
                   m[0][1] * other.v[0] + m[1][1] * other.v[1] + m[2][1] * other.v[2] + m[3][1] * other.v[3],
                   m[0][2] * other.v[0] + m[1][2] * other.v[1] + m[2][2] * other.v[2] + m[3][2] * other.v[3],
                   m[0][3] * other.v[0] + m[1][3] * other.v[1] + m[2][3] * other.v[2] + m[3][3] * other.v[3]);
#endif
    }

    inline CVector4f& operator[](int i)
    {
        assert(0 <= i && i < 4);
        return vec[i];
    }

    inline const CVector4f& operator[](int i) const
    {
        assert(0 <= i && i < 4);
        return vec[i];
    }

    inline CMatrix4f transposed()
    {
        CMatrix4f ret;
#if __SSE__
        __m128 T0 = _mm_unpacklo_ps(vec[0].mVec128, vec[1].mVec128);
        __m128 T2 = _mm_unpacklo_ps(vec[2].mVec128, vec[3].mVec128);
        __m128 T1 = _mm_unpackhi_ps(vec[0].mVec128, vec[1].mVec128);
        __m128 T3 = _mm_unpackhi_ps(vec[2].mVec128, vec[3].mVec128);
        ret.vec[0].mVec128 = _mm_movelh_ps(T0, T2);
        ret.vec[1].mVec128 = _mm_movehl_ps(T2, T0);
        ret.vec[2].mVec128 = _mm_movelh_ps(T1, T3);
        ret.vec[3].mVec128 = _mm_movehl_ps(T3, T1);
#else
        ret.m[0][0] = m[0][0];
        ret.m[1][0] = m[0][1];
        ret.m[2][0] = m[0][2];
        ret.m[3][0] = m[0][3];

        ret.m[0][1] = m[1][0];
        ret.m[1][1] = m[1][1];
        ret.m[2][1] = m[1][2];
        ret.m[3][1] = m[1][3];

        ret.m[0][2] = m[2][0];
        ret.m[1][2] = m[2][1];
        ret.m[2][2] = m[2][2];
        ret.m[3][2] = m[2][3];

        ret.m[0][3] = m[3][0];
        ret.m[1][3] = m[3][1];
        ret.m[2][3] = m[3][2];
        ret.m[3][3] = m[3][3];
#endif
        return ret;
    }

    inline CVector3f multiplyOneOverW(const CVector3f& point) const
    {
        CVector4f xfVec = *this * point;
        return xfVec.toVec3f() / xfVec.w;
    }

    union
    {
        float m[4][4];
        struct
        {
            CVector4f vec[4];
        };
    };
};
static inline CMatrix4f operator*(const CMatrix4f& lhs, const CMatrix4f& rhs)
{
    CMatrix4f ret;
#if __SSE__
    unsigned i;

    for (i = 0 ; i < 4 ; ++i)
    {
        ret.vec[i].mVec128 =
            _mm_add_ps(_mm_add_ps(_mm_add_ps(
                                      _mm_mul_ps(lhs.vec[0].mVec128, _mm_shuffle_ps(rhs.vec[i].mVec128, rhs.vec[i].mVec128, _MM_SHUFFLE(0, 0, 0, 0))),
                                      _mm_mul_ps(lhs.vec[1].mVec128, _mm_shuffle_ps(rhs.vec[i].mVec128, rhs.vec[i].mVec128, _MM_SHUFFLE(1, 1, 1, 1)))),
                                  _mm_mul_ps(lhs.vec[2].mVec128, _mm_shuffle_ps(rhs.vec[i].mVec128, rhs.vec[i].mVec128, _MM_SHUFFLE(2, 2, 2, 2)))),
                       _mm_mul_ps(lhs.vec[3].mVec128, _mm_shuffle_ps(rhs.vec[i].mVec128, rhs.vec[i].mVec128, _MM_SHUFFLE(3, 3, 3, 3))));
    }

#else
    ret.m[0][0] = lhs.m[0][0] * rhs.m[0][0] + lhs.m[1][0] * rhs.m[0][1] + lhs.m[2][0] * rhs.m[0][2] + lhs.m[3][0] * rhs.m[0][3];
    ret.m[1][0] = lhs.m[0][0] * rhs.m[1][0] + lhs.m[1][0] * rhs.m[1][1] + lhs.m[2][0] * rhs.m[1][2] + lhs.m[3][0] * rhs.m[1][3];
    ret.m[2][0] = lhs.m[0][0] * rhs.m[2][0] + lhs.m[1][0] * rhs.m[2][1] + lhs.m[2][0] * rhs.m[2][2] + lhs.m[3][0] * rhs.m[2][3];
    ret.m[3][0] = lhs.m[0][0] * rhs.m[3][0] + lhs.m[1][0] * rhs.m[3][1] + lhs.m[2][0] * rhs.m[3][2] + lhs.m[3][0] * rhs.m[3][3];

    ret.m[0][1] = lhs.m[0][1] * rhs.m[0][0] + lhs.m[1][1] * rhs.m[0][1] + lhs.m[2][1] * rhs.m[0][2] + lhs.m[3][1] * rhs.m[0][3];
    ret.m[1][1] = lhs.m[0][1] * rhs.m[1][0] + lhs.m[1][1] * rhs.m[1][1] + lhs.m[2][1] * rhs.m[1][2] + lhs.m[3][1] * rhs.m[1][3];
    ret.m[2][1] = lhs.m[0][1] * rhs.m[2][0] + lhs.m[1][1] * rhs.m[2][1] + lhs.m[2][1] * rhs.m[2][2] + lhs.m[3][1] * rhs.m[2][3];
    ret.m[3][1] = lhs.m[0][1] * rhs.m[3][0] + lhs.m[1][1] * rhs.m[3][1] + lhs.m[2][1] * rhs.m[3][2] + lhs.m[3][1] * rhs.m[3][3];

    ret.m[0][2] = lhs.m[0][2] * rhs.m[0][0] + lhs.m[1][2] * rhs.m[0][1] + lhs.m[2][2] * rhs.m[0][2] + lhs.m[3][2] * rhs.m[0][3];
    ret.m[1][2] = lhs.m[0][2] * rhs.m[1][0] + lhs.m[1][2] * rhs.m[1][1] + lhs.m[2][2] * rhs.m[1][2] + lhs.m[3][2] * rhs.m[1][3];
    ret.m[2][2] = lhs.m[0][2] * rhs.m[2][0] + lhs.m[1][2] * rhs.m[2][1] + lhs.m[2][2] * rhs.m[2][2] + lhs.m[3][2] * rhs.m[2][3];
    ret.m[3][2] = lhs.m[0][2] * rhs.m[3][0] + lhs.m[1][2] * rhs.m[3][1] + lhs.m[2][2] * rhs.m[3][2] + lhs.m[3][2] * rhs.m[3][3];

    ret.m[0][3] = lhs.m[0][3] * rhs.m[0][0] + lhs.m[1][3] * rhs.m[0][1] + lhs.m[2][3] * rhs.m[0][2] + lhs.m[3][3] * rhs.m[0][3];
    ret.m[1][3] = lhs.m[0][3] * rhs.m[1][0] + lhs.m[1][3] * rhs.m[1][1] + lhs.m[2][3] * rhs.m[1][2] + lhs.m[3][3] * rhs.m[1][3];
    ret.m[2][3] = lhs.m[0][3] * rhs.m[2][0] + lhs.m[1][3] * rhs.m[2][1] + lhs.m[2][3] * rhs.m[2][2] + lhs.m[3][3] * rhs.m[2][3];
    ret.m[3][3] = lhs.m[0][3] * rhs.m[3][0] + lhs.m[1][3] * rhs.m[3][1] + lhs.m[2][3] * rhs.m[2][2] + lhs.m[3][3] * rhs.m[3][3];
#endif
    return ret;
}
}

#endif // CMATRIX4F

