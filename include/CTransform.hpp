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

    inline CTransform operator*(const CTransform& rhs) const
    {return CTransform(m_basis * rhs.m_basis, m_origin + (m_basis * rhs.m_origin));}
    
    inline CTransform inverse() const
    {
        CMatrix3f inv = m_basis.inverted();
        return CTransform(inv, inv * -m_origin);
    }

    inline void translate(const CVector3f& position)
    {
        m_basis = CMatrix3f::skIdentityMatrix3f;
        m_origin = position;
    }

    inline void translate(float x, float y, float z) { translate({x, y, z}); }

    inline void rotate(const CVector3f& euler) { *this = *this * CMatrix3f(CQuaternion(euler)); }

    inline void scaleBy(float factor)
    { CTransform xfrm(CMatrix3f(CVector3f(factor, factor, factor))); *this = *this * xfrm; }

    inline void scale(const CVector3f& factor)
    {
        m_basis = CMatrix3f(true);
        m_basis[0][0] = factor.x;
        m_basis[1][1] = factor.y;
        m_basis[2][2] = factor.z;
        m_origin.zeroOut();
    }


    inline void scale(float x, float y, float z) { scale({x, y, z}); }
    inline void scale(float factor) { scale({factor, factor, factor}); }

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
    
    inline void toMatrix4f(CMatrix4f& mat) const
    {
#if __SSE__
        mat.vec[0].mVec128 = m_basis[0].mVec128; mat.m[0][3] = 0.0f;
        mat.vec[1].mVec128 = m_basis[1].mVec128; mat.m[1][3] = 0.0f;
        mat.vec[2].mVec128 = m_basis[2].mVec128; mat.m[2][3] = 0.0f;
        mat.vec[3].mVec128 = m_origin.mVec128; mat.m[3][3] = 1.0f;
#else
        mat.m[0][0] = m_basis[0][0]; mat.m[0][1] = m_basis[0][1]; mat.m[0][2] = m_basis[0][2]; mat.m[0][3] = 0.0f;
        mat.m[1][0] = m_basis[1][0]; mat.m[1][1] = m_basis[1][1]; mat.m[1][2] = m_basis[1][2]; mat.m[1][3] = 0.0f;
        mat.m[2][0] = m_basis[2][0]; mat.m[2][1] = m_basis[2][1]; mat.m[2][2] = m_basis[2][2]; mat.m[2][3] = 0.0f;
        mat.m[3][0] = m_origin[0]; mat.m[3][1] = m_origin[1]; mat.m[3][2] = m_origin[2]; mat.m[3][3] = 1.0f;
#endif
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
