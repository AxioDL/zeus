#ifndef CPROJECTION_HPP
#define CPROJECTION_HPP

#include "Global.hpp"
#include "CMatrix4f.hpp"
#include <stdio.h>

#define _USE_MATH_DEFINES 1
#include <math.h>
namespace Zeus
{
enum class EProjType
{
    None = 0,
    Orthographic = 1,
    Perspective = 2
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

class alignas(16) CProjection
{
    void _updateCachedMatrix();
public:
    ZE_DECLARE_ALIGNED_ALLOCATOR();

    CProjection()
    {
        m_projType = EProjType::Orthographic;
        m_ortho = SProjOrtho();
        m_mtx = CMatrix4f::skIdentityMatrix4f;
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
    {m_projType = EProjType::Orthographic; m_ortho = ortho; _updateCachedMatrix();}
    inline void setPersp(const SProjPersp& persp)
    {m_projType = EProjType::Perspective; m_persp = persp; _updateCachedMatrix();}
    
    inline EProjType getType() const {return m_projType;}
    inline const SProjOrtho& getOrtho() const
    {
        if (m_projType != EProjType::Orthographic)
        {
            fprintf(stderr, "attempted to access orthographic structure of non-ortho projection");
            abort();
        }
        return m_ortho;
    }
    inline const SProjPersp& getPersp() const
    {
        if (m_projType != EProjType::Perspective)
        {
            fprintf(stderr, "attempted to access perspective structure of non-persp projection");
            abort();
        }
        return m_persp;
    }
    
    inline const CMatrix4f& getCachedMatrix() const {return m_mtx;}
    
protected:

    /* Projection type */
    EProjType m_projType;
    
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
    CMatrix4f m_mtx;
    
};
}

#endif // CMATRIX3F_HPP
