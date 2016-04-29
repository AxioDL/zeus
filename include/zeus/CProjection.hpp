#ifndef CPROJECTION_HPP
#define CPROJECTION_HPP

#include "Global.hpp"
#include "zeus/CMatrix4f.hpp"
#include <cstdlib>
#include <cstdio>
#include <cmath>

namespace zeus
{
enum class EProjType
{
    None = 0,
    Orthographic = 1,
    Perspective = 2
};
struct SProjOrtho
{
    float top, bottom, left, right, near, far;
    SProjOrtho(float p_top=1.0f, float p_bottom=-1.0f, float p_left=-1.0f, float p_right=1.0f,
               float p_near=1.0f, float p_far=-1.0f) :
    top(p_top), bottom(p_bottom), left(p_left), right(p_right), near(p_near), far(p_far) {}
};
struct SProjPersp
{
    float fov, aspect, near, far;
    SProjPersp(float p_fov=degToRad(55.0f), float p_aspect=1.0f, float p_near=0.1f, float p_far=4096.f) :
    fov(p_fov), aspect(p_aspect), near(p_near), far(p_far) {}
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
            std::fprintf(stderr, "attempted to access orthographic structure of non-ortho projection");
            std::abort();
        }
        return m_ortho;
    }
    inline const SProjPersp& getPersp() const
    {
        if (m_projType != EProjType::Perspective)
        {
            std::fprintf(stderr, "attempted to access perspective structure of non-persp projection");
            std::abort();
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
