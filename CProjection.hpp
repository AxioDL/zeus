#ifndef CPROJECTION_HPP
#define CPROJECTION_HPP

#include "Global.hpp"
#include <stdexcept>

#define _USE_MATH_DEFINES 1
#include <math.h>

union TMatrix4f
{
    float m[4][4];
#if __SSE__
    __m128 mVec128[4];
#endif
    inline TMatrix4f transposed()
    {
        TMatrix4f ret;
#if __SSE__
        __m128 T0 = _mm_unpacklo_ps(mVec128[0], mVec128[1]);
        __m128 T2 = _mm_unpacklo_ps(mVec128[2], mVec128[3]);
        __m128 T1 = _mm_unpackhi_ps(mVec128[0], mVec128[1]);
        __m128 T3 = _mm_unpackhi_ps(mVec128[2], mVec128[3]);
        ret.mVec128[0] = _mm_movelh_ps(T0, T2);
        ret.mVec128[1] = _mm_movehl_ps(T2, T0);
        ret.mVec128[2] = _mm_movelh_ps(T1, T3);
        ret.mVec128[3] = _mm_movehl_ps(T3, T1);
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
    inline TMatrix4f& operator=(const TMatrix4f& other)
    {
#if __SSE__
        mVec128[0] = other.mVec128[0];
        mVec128[1] = other.mVec128[1];
        mVec128[2] = other.mVec128[2];
        mVec128[3] = other.mVec128[3];
#else
        m[0][0] = other.m[0][0];
        m[0][1] = other.m[0][1];
        m[0][2] = other.m[0][2];
        m[0][3] = other.m[0][3];
        m[1][0] = other.m[1][0];
        m[1][1] = other.m[1][1];
        m[1][2] = other.m[1][2];
        m[1][3] = other.m[1][3];
        m[2][0] = other.m[2][0];
        m[2][1] = other.m[2][1];
        m[2][2] = other.m[2][2];
        m[2][3] = other.m[2][3];
        m[3][0] = other.m[3][0];
        m[3][1] = other.m[3][1];
        m[3][2] = other.m[3][2];
        m[3][3] = other.m[3][3];
#endif
        return *this;
    }
};
static inline TMatrix4f operator*(const TMatrix4f& lhs, const TMatrix4f& rhs)
{
    TMatrix4f ret;
#if __SSE__
    unsigned i;
    for (i=0 ; i<4 ; ++i) {
        ret.mVec128[i] =
        _mm_add_ps(_mm_add_ps(_mm_add_ps(
                   _mm_mul_ps(lhs.mVec128[0], _mm_shuffle_ps(rhs.mVec128[i], rhs.mVec128[i], _MM_SHUFFLE(0, 0, 0, 0))),
                   _mm_mul_ps(lhs.mVec128[1], _mm_shuffle_ps(rhs.mVec128[i], rhs.mVec128[i], _MM_SHUFFLE(1, 1, 1, 1)))),
                   _mm_mul_ps(lhs.mVec128[2], _mm_shuffle_ps(rhs.mVec128[i], rhs.mVec128[i], _MM_SHUFFLE(2, 2, 2, 2)))),
                   _mm_mul_ps(lhs.mVec128[3], _mm_shuffle_ps(rhs.mVec128[i], rhs.mVec128[i], _MM_SHUFFLE(3, 3, 3, 3))));
    }
#else
    ret.m[0][0] = lhs.m[0][0]*rhs.m[0][0] + lhs.m[1][0]*rhs.m[0][1] + lhs.m[2][0]*rhs.m[0][2] + lhs.m[3][0]*rhs.m[0][3];
    ret.m[1][0] = lhs.m[0][0]*rhs.m[1][0] + lhs.m[1][0]*rhs.m[1][1] + lhs.m[2][0]*rhs.m[1][2] + lhs.m[3][0]*rhs.m[1][3];
    ret.m[2][0] = lhs.m[0][0]*rhs.m[2][0] + lhs.m[1][0]*rhs.m[2][1] + lhs.m[2][0]*rhs.m[2][2] + lhs.m[3][0]*rhs.m[2][3];
    ret.m[3][0] = lhs.m[0][0]*rhs.m[3][0] + lhs.m[1][0]*rhs.m[3][1] + lhs.m[2][0]*rhs.m[3][2] + lhs.m[3][0]*rhs.m[3][3];
    
    ret.m[0][1] = lhs.m[0][1]*rhs.m[0][0] + lhs.m[1][1]*rhs.m[0][1] + lhs.m[2][1]*rhs.m[0][2] + lhs.m[3][1]*rhs.m[0][3];
    ret.m[1][1] = lhs.m[0][1]*rhs.m[1][0] + lhs.m[1][1]*rhs.m[1][1] + lhs.m[2][1]*rhs.m[1][2] + lhs.m[3][1]*rhs.m[1][3];
    ret.m[2][1] = lhs.m[0][1]*rhs.m[2][0] + lhs.m[1][1]*rhs.m[2][1] + lhs.m[2][1]*rhs.m[2][2] + lhs.m[3][1]*rhs.m[2][3];
    ret.m[3][1] = lhs.m[0][1]*rhs.m[3][0] + lhs.m[1][1]*rhs.m[3][1] + lhs.m[2][1]*rhs.m[3][2] + lhs.m[3][1]*rhs.m[3][3];
    
    ret.m[0][2] = lhs.m[0][2]*rhs.m[0][0] + lhs.m[1][2]*rhs.m[0][1] + lhs.m[2][2]*rhs.m[0][2] + lhs.m[3][2]*rhs.m[0][3];
    ret.m[1][2] = lhs.m[0][2]*rhs.m[1][0] + lhs.m[1][2]*rhs.m[1][1] + lhs.m[2][2]*rhs.m[1][2] + lhs.m[3][2]*rhs.m[1][3];
    ret.m[2][2] = lhs.m[0][2]*rhs.m[2][0] + lhs.m[1][2]*rhs.m[2][1] + lhs.m[2][2]*rhs.m[2][2] + lhs.m[3][2]*rhs.m[2][3];
    ret.m[3][2] = lhs.m[0][2]*rhs.m[3][0] + lhs.m[1][2]*rhs.m[3][1] + lhs.m[2][2]*rhs.m[3][2] + lhs.m[3][2]*rhs.m[3][3];
    
    ret.m[0][3] = lhs.m[0][3]*rhs.m[0][0] + lhs.m[1][3]*rhs.m[0][1] + lhs.m[2][3]*rhs.m[0][2] + lhs.m[3][3]*rhs.m[0][3];
    ret.m[1][3] = lhs.m[0][3]*rhs.m[1][0] + lhs.m[1][3]*rhs.m[1][1] + lhs.m[2][3]*rhs.m[1][2] + lhs.m[3][3]*rhs.m[1][3];
    ret.m[2][3] = lhs.m[0][3]*rhs.m[2][0] + lhs.m[1][3]*rhs.m[2][1] + lhs.m[2][3]*rhs.m[2][2] + lhs.m[3][3]*rhs.m[2][3];
    ret.m[3][3] = lhs.m[0][3]*rhs.m[3][0] + lhs.m[1][3]*rhs.m[3][1] + lhs.m[2][3]*rhs.m[2][2] + lhs.m[3][3]*rhs.m[3][3];
#endif
    return ret;
}
extern const TMatrix4f kIdentityMtx4;

enum EProjType
{
    PROJ_NONE = 0,
    PROJ_ORTHO = 1,
    PROJ_PERSP = 2
};
struct SProjOrtho
{
    float m_top, m_bottom, m_left, m_right, m_near, m_far;
    SProjOrtho(float p_top=1.0f, float p_bottom=-1.0f, float p_left=-1.0f, float p_right=1.0f,
               float p_near=1.0f, float p_far=-1.0f) :
    m_top(p_top), m_bottom(p_bottom), m_left(p_left), m_right(p_right), m_near(p_near), m_far(p_far) {}
};
struct SProjPersp
{
    float m_fov, m_aspect, m_near, m_far;
    SProjPersp(float p_fov=55.0f * M_PI / 180.0f, float p_aspect=1.0f, float p_near=0.1f, float p_far=4096.f) :
    m_fov(p_fov), m_aspect(p_aspect), m_near(p_near), m_far(p_far) {}
};
extern const SProjOrtho kOrthoIdentity;

class ZE_ALIGN(16) CProjection
{
    void _updateCachedMatrix();
public:
    ZE_DECLARE_ALIGNED_ALLOCATOR();

    CProjection()
    {
        m_projType = PROJ_ORTHO;
        m_ortho = SProjOrtho();
        m_mtx = kIdentityMtx4;
    }
    CProjection(const CProjection& other) {*this = other;}
    CProjection(const SProjOrtho& ortho) {setOrtho(ortho);}
    CProjection(const SProjPersp& persp) {setPersp(persp);}
    
    inline CProjection& operator=(const CProjection& other)
    {
        if (this != &other)
        {
            m_projType = other.m_projType;
            m_ortho = other.m_ortho;
            m_mtx = other.m_mtx;
        }
        return *this;
    }
    
    inline void setOrtho(const SProjOrtho& ortho)
    {m_projType = PROJ_ORTHO; m_ortho = ortho; _updateCachedMatrix();}
    inline void setPersp(const SProjPersp& persp)
    {m_projType = PROJ_PERSP; m_persp = persp; _updateCachedMatrix();}
    
    inline EProjType getType() const {return m_projType;}
    inline const SProjOrtho& getOrtho() const
    {
        if (m_projType != PROJ_ORTHO)
            throw std::runtime_error("attempted to access orthographic structure of non-ortho projection");
        return m_ortho;
    }
    inline const SProjPersp& getPersp() const
    {
        if (m_projType != PROJ_PERSP)
            throw std::runtime_error("attempted to access perspective structure of non-persp projection");
        return m_persp;
    }
    
    inline const TMatrix4f& getCachedMatrix() const {return m_mtx;}
    
protected:

    /* Projection type */
    enum EProjType m_projType;
    
    /* Projection intermediate */
    union
    {
#ifdef _MSC_VER
        struct
        {
            SProjOrtho m_ortho;
        };
        struct
        {
            SProjPersp m_persp;
        };
#else
        SProjOrtho m_ortho;
        SProjPersp m_persp;
#endif
    };

    /* Cached projection matrix */
    TMatrix4f m_mtx;
    
};

#endif // CMATRIX3F_HPP
