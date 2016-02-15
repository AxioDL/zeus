#ifndef CTRANSFORM_HPP
#define CTRANSFORM_HPP

#include "Global.hpp"
#include "CMatrix3f.hpp"
#include "CMatrix4f.hpp"
#include "CVector3f.hpp"
#include "CQuaternion.hpp"

namespace Zeus
{
class alignas(16) CTransform
{
public:
    ZE_DECLARE_ALIGNED_ALLOCATOR();

    CTransform() : m_basis(false) {}
    CTransform(const CMatrix3f& basis, const CVector3f& offset=CVector3f::skZero) :
    m_basis(basis), m_origin(offset) {}
#if ZE_ATHENA_TYPES
    CTransform(const atVec4f* mtx)
    : m_basis(mtx[0], mtx[1], mtx[2]), m_origin(mtx[0].vec[3], mtx[1].vec[3], mtx[2].vec[3]) {}
#endif

    static inline CTransform Identity()
    {
        return CTransform(CMatrix3f::skIdentityMatrix3f);
    }

    inline CTransform operator*(const CTransform& rhs) const
    {return CTransform(m_basis * rhs.m_basis, m_origin + (m_basis * rhs.m_origin));}
    
    inline CTransform inverse() const
    {
        CMatrix3f inv = m_basis.inverted();
        return CTransform(inv, inv * -m_origin);
    }

    static inline CTransform Translate(const CVector3f& position)
    {
        return {CMatrix3f::skIdentityMatrix3f, position};
    }

    static inline CTransform Translate(float x, float y, float z) { return Translate({x, y, z}); }

    inline CTransform& operator+=(const CVector3f& other)
    {
        m_origin += other;
        return *this;
    }

    inline void rotate(const CVector3f& euler) { *this = *this * CMatrix3f(CQuaternion(euler)); }

    static inline CTransform RotateX(float theta)
    {
        float sinT = sinf(theta);
        float cosT = cosf(theta);
        return CTransform(CMatrix3f({1.f, 0.f, 0.f, 0.f},
                                    {0.f, cosT, sinT, 0.f},
                                    {0.f, -sinT, cosT, 0.f}));
    }

    static inline CTransform RotateY(float theta)
    {
        float sinT = sinf(theta);
        float cosT = cosf(theta);
        return CTransform(CMatrix3f({cosT, 0.f, -sinT, 0.f},
                                    {0.f, 1.f, 0.f, 0.f},
                                    {sinT, 0.f, cosT, 0.f}));
    }

    static inline CTransform RotateZ(float theta)
    {
        float sinT = sinf(theta);
        float cosT = cosf(theta);
        return CTransform(CMatrix3f({cosT, sinT, 0.f, 0.f},
                                    {-sinT, cosT, 0.f, 0.f},
                                    {0.f, 0.f, 1.f, 0.f}));
    }

    inline void rotateLocalX(float theta)
    {
        float sinT = sinf(theta);
        float cosT = cosf(theta);

        Zeus::CVector3f b2 = m_basis[2] * sinT;
        Zeus::CVector3f b1 = m_basis[1] * sinT;
        Zeus::CVector3f cosV(cosT);

        m_basis[1] *= cosV;
        m_basis[2] *= cosV;

        m_basis[1] += b2;
        m_basis[2] -= b1;
    }

    inline void rotateLocalY(float theta)
    {
        float sinT = sinf(theta);
        float cosT = cosf(theta);

        Zeus::CVector3f b0 = m_basis[0] * sinT;
        Zeus::CVector3f b2 = m_basis[2] * sinT;
        Zeus::CVector3f cosV(cosT);

        m_basis[0] *= cosV;
        m_basis[2] *= cosV;

        m_basis[2] += b0;
        m_basis[0] -= b2;
    }

    inline void rotateLocalZ(float theta)
    {
        float sinT = sinf(theta);
        float cosT = cosf(theta);

        Zeus::CVector3f b0 = m_basis[0] * sinT;
        Zeus::CVector3f b1 = m_basis[1] * sinT;
        Zeus::CVector3f cosV(cosT);

        m_basis[0] *= cosV;
        m_basis[1] *= cosV;

        m_basis[0] += b1;
        m_basis[1] -= b0;
    }

    inline CVector3f transposeRotate(const CVector3f& in) const
    {
        return CVector3f(m_basis[0].dot(in), m_basis[1].dot(in), m_basis[2].dot(in));
    }

    inline void scaleBy(float factor)
    { CTransform xfrm(CMatrix3f(CVector3f(factor, factor, factor))); *this = *this * xfrm; }

    static inline CTransform Scale(const CVector3f& factor)
    {
        return CTransform(CMatrix3f({factor.x, 0.f, 0.f, 0.f},
                                    {0.f, factor.y, 0.f, 0.f},
                                    {0.f, 0.f, factor.z, 0.f}));
    }

    static inline CTransform Scale(float x, float y, float z)
    {
        return CTransform(CMatrix3f({x, 0.f, 0.f, 0.f},
                                    {0.f, y, 0.f, 0.f},
                                    {0.f, 0.f, z, 0.f}));
    }

    static inline CTransform Scale(float factor)
    {
        return CTransform(CMatrix3f({factor, 0.f, 0.f, 0.f},
                                    {0.f, factor, 0.f, 0.f},
                                    {0.f, 0.f, factor, 0.f}));
    }

    inline void multiplyIgnoreTranslation(const CTransform& xfrm) { m_basis = m_basis*xfrm.m_basis; }

    inline CTransform getRotation() { CTransform ret = *this; ret.m_origin.zeroOut(); return ret; }
    void setRotation(const CMatrix3f& mat)   { m_basis = mat; }
    void setRotation(const CTransform& xfrm) { setRotation(xfrm.m_basis); }

    /**
     * @brief buildMatrix3f Returns the stored matrix
     * buildMatrix3f is here for compliance with Retro's Math API
     * @return The Matrix (Neo, you are the one)
     */
    inline CMatrix3f buildMatrix3f() { return m_basis; }

    inline CVector3f operator*(const CVector3f& other) const {return m_origin + m_basis * other;}
    
    inline CMatrix4f toMatrix4f() const
    {
        CMatrix4f ret(m_basis[0], m_basis[1], m_basis[2], m_origin);
        ret[0][3] = 0.0f;
        ret[1][3] = 0.0f;
        ret[2][3] = 0.0f;
        ret[3][3] = 1.0f;
        return ret;
    }

    static inline CTransform fromColumns(const CVector3f& m0, const CVector3f& m1, const CVector3f& m2, const CVector3f& m3)
    {
        CTransform ret;
        ret.m_basis[0][0] = m0[0];
        ret.m_basis[0][1] = m1[0];
        ret.m_basis[0][2] = m2[0];
        ret.m_origin[0]   = m3[0];
        ret.m_basis[1][0] = m0[1];
        ret.m_basis[1][1] = m1[1];
        ret.m_basis[1][2] = m2[1];
        ret.m_origin[1]   = m3[1];
        ret.m_basis[2][0] = m0[2];
        ret.m_basis[2][1] = m1[2];
        ret.m_basis[2][2] = m2[2];
        ret.m_origin[2]   = m3[2];
        return ret;
    }

    CMatrix3f m_basis;
    CVector3f m_origin;
};

static inline CTransform CTransformFromScaleVector(const CVector3f& scale)
{
    return CTransform(CMatrix3f(scale));
}
CTransform CTransformFromEditorEuler(const CVector3f& eulerVec);
CTransform CTransformFromEditorEulers(const CVector3f& eulerVec, const CVector3f& origin);
CTransform CTransformFromAxisAngle(const CVector3f& axis, float angle);
}

#endif // CTRANSFORM_HPP
