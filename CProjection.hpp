#ifndef CPROJECTION_HPP
#define CPROJECTION_HPP

#include "Global.hpp"
#include <stdexcept>

#define _USE_MATH_DEFINES 1
#include <math.h>

typedef union
{
    float m[4][4];
#if __SSE__
    __m128 mVec128[4];
#endif
} TMatrix4f;
static inline void copyMatrix4f(TMatrix4f& dest, const TMatrix4f& src)
{
#if __SSE__
    dest.mVec128[0] = src.mVec128[0];
    dest.mVec128[1] = src.mVec128[1];
    dest.mVec128[2] = src.mVec128[2];
    dest.mVec128[3] = src.mVec128[3];
#else
    dest.m[0][0] = src.m[0][0];
    dest.m[0][1] = src.m[0][1];
    dest.m[0][2] = src.m[0][2];
    dest.m[0][3] = src.m[0][3];
    dest.m[1][0] = src.m[1][0];
    dest.m[1][1] = src.m[1][1];
    dest.m[1][2] = src.m[1][2];
    dest.m[1][3] = src.m[1][3];
    dest.m[2][0] = src.m[2][0];
    dest.m[2][1] = src.m[2][1];
    dest.m[2][2] = src.m[2][2];
    dest.m[2][3] = src.m[2][3];
    dest.m[3][0] = src.m[3][0];
    dest.m[3][1] = src.m[3][1];
    dest.m[3][2] = src.m[3][2];
    dest.m[3][3] = src.m[3][3];
#endif
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
        copyMatrix4f(m_mtx, kIdentityMtx4);
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
            copyMatrix4f(m_mtx, other.m_mtx);
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
    
    inline const TMatrix4f& getCachedMatrix() {return m_mtx;}
    
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
