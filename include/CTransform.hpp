#ifndef CTRANSFORM_HPP
#define CTRANSFORM_HPP

#include "Global.hpp"
#include "CMatrix3f.hpp"
#include "CMatrix4f.hpp"
#include "CVector3f.hpp"

class ZE_ALIGN(16) CTransform
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
    
    inline CVector3f operator*(const CVector3f& other) const
    {return m_origin + m_basis * other;}
    
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

    CMatrix3f m_basis;
    CVector3f m_origin;
};

static inline CTransform CTransformFromScaleVector(const CVector3f& scale)
{
    return CTransform(CMatrix3f(scale));
}
CTransform CTransformFromEditorEuler(const CVector3f& eulerVec);
CTransform CTransformFromAxisAngle(const CVector3f& axis, float angle);

#endif // CTRANSFORM_HPP
