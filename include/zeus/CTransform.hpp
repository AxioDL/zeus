#ifndef CTRANSFORM_HPP
#define CTRANSFORM_HPP

#include "Global.hpp"
#include "zeus/CMatrix3f.hpp"
#include "zeus/CMatrix4f.hpp"
#include "zeus/CVector3f.hpp"
#include <stdint.h>
#include <stdio.h>

namespace zeus
{
class alignas(16) CTransform
{
public:
    ZE_DECLARE_ALIGNED_ALLOCATOR();

    CTransform() : basis(false) {}
    CTransform(const CMatrix3f& basis, const CVector3f& offset = CVector3f::skZero) : basis(basis), origin(offset) {}
#if ZE_ATHENA_TYPES
    CTransform(const atVec4f* mtx) : basis(mtx[0], mtx[1], mtx[2]), origin(mtx[0].vec[3], mtx[1].vec[3], mtx[2].vec[3]) {}

    void read34RowMajor(athena::io::IStreamReader& r)
    {
        atVec4f r0 = r.readVec4fBig();
        atVec4f r1 = r.readVec4fBig();
        atVec4f r2 = r.readVec4fBig();
        basis = CMatrix3f(r0, r1, r2);
        basis.transpose();
        origin = CVector3f(r0.vec[3], r1.vec[3], r2.vec[3]);
    }
#endif

    /* Column constructor */
    CTransform(const CVector3f& c0, const CVector3f& c1, const CVector3f& c2, const CVector3f& c3)
    : basis(c0, c1, c2), origin(c3) {}

    static inline CTransform Identity() { return CTransform(CMatrix3f::skIdentityMatrix3f); }

    inline bool operator ==(const CTransform& other) const
    {
        return origin == other.origin && basis == other.basis;
    }

    inline CTransform operator*(const CTransform& rhs) const
    {
        return CTransform(basis * rhs.basis, origin + (basis * rhs.origin));
    }

    inline CTransform inverse() const
    {
        CMatrix3f inv = basis.inverted();
        return CTransform(inv, inv * -origin);
    }

    static inline CTransform Translate(const CVector3f& position) { return {CMatrix3f::skIdentityMatrix3f, position}; }

    static inline CTransform Translate(float x, float y, float z) { return Translate({x, y, z}); }

    inline CTransform operator+(const CVector3f& other) { return CTransform(basis, origin + other); }

    inline CTransform& operator+=(const CVector3f& other)
    {
        origin += other;
        return *this;
    }

    inline CTransform operator-(const CVector3f& other) { return CTransform(basis, origin - other); }

    inline CTransform& operator-=(const CVector3f& other)
    {
        origin -= other;
        return *this;
    }

    inline zeus::CVector3f rotate(const CVector3f& vec) const { return *this * vec; }

    static inline CTransform RotateX(float theta)
    {
        float sinT = std::sin(theta);
        float cosT = std::cos(theta);
        return CTransform(CMatrix3f(TVectorUnion{1.f, 0.f, 0.f, 0.f}, TVectorUnion{0.f, cosT, sinT, 0.f},
                                    TVectorUnion{0.f, -sinT, cosT, 0.f}));
    }

    static inline CTransform RotateY(float theta)
    {
        float sinT = std::sin(theta);
        float cosT = std::cos(theta);
        return CTransform(CMatrix3f(TVectorUnion{cosT, 0.f, -sinT, 0.f}, TVectorUnion{0.f, 1.f, 0.f, 0.f},
                                    TVectorUnion{sinT, 0.f, cosT, 0.f}));
    }

    static inline CTransform RotateZ(float theta)
    {
        float sinT = std::sin(theta);
        float cosT = std::cos(theta);
        return CTransform(CMatrix3f(TVectorUnion{cosT, sinT, 0.f, 0.f}, TVectorUnion{-sinT, cosT, 0.f, 0.f},
                                    TVectorUnion{0.f, 0.f, 1.f, 0.f}));
    }

    inline void rotateLocalX(float theta)
    {
        float sinT = std::sin(theta);
        float cosT = std::cos(theta);

        zeus::CVector3f b2 = basis[2] * sinT;
        zeus::CVector3f b1 = basis[1] * sinT;
        zeus::CVector3f cosV(cosT);

        basis[1] *= cosV;
        basis[2] *= cosV;

        basis[1] += b2;
        basis[2] -= b1;
    }

    inline void rotateLocalY(float theta)
    {
        float sinT = std::sin(theta);
        float cosT = std::cos(theta);

        zeus::CVector3f b0 = basis[0] * sinT;
        zeus::CVector3f b2 = basis[2] * sinT;
        zeus::CVector3f cosV(cosT);

        basis[0] *= cosV;
        basis[2] *= cosV;

        basis[2] += b0;
        basis[0] -= b2;
    }

    inline void rotateLocalZ(float theta)
    {
        float sinT = std::sin(theta);
        float cosT = std::cos(theta);

        zeus::CVector3f b0 = basis[0] * sinT;
        zeus::CVector3f b1 = basis[1] * sinT;
        zeus::CVector3f cosV(cosT);

        basis[0] *= cosV;
        basis[1] *= cosV;

        basis[0] += b1;
        basis[1] -= b0;
    }

    inline CVector3f transposeRotate(const CVector3f& in) const
    {
        return CVector3f(basis[0].dot(in), basis[1].dot(in), basis[2].dot(in));
    }

    inline void scaleBy(float factor)
    {
        CTransform xfrm(CMatrix3f(CVector3f(factor, factor, factor)));
        *this = *this * xfrm;
    }

    static inline CTransform Scale(const CVector3f& factor)
    {
        return CTransform(CMatrix3f(TVectorUnion{factor.x, 0.f, 0.f, 0.f}, TVectorUnion{0.f, factor.y, 0.f, 0.f},
                                    TVectorUnion{0.f, 0.f, factor.z, 0.f}));
    }

    static inline CTransform Scale(float x, float y, float z)
    {
        return CTransform(
            CMatrix3f(TVectorUnion{x, 0.f, 0.f, 0.f}, TVectorUnion{0.f, y, 0.f, 0.f}, TVectorUnion{0.f, 0.f, z, 0.f}));
    }

    static inline CTransform Scale(float factor)
    {
        return CTransform(CMatrix3f(TVectorUnion{factor, 0.f, 0.f, 0.f}, TVectorUnion{0.f, factor, 0.f, 0.f},
                                    TVectorUnion{0.f, 0.f, factor, 0.f}));
    }

    inline CTransform multiplyIgnoreTranslation(const CTransform& xfrm)
    {
        CTransform ret;
        ret.basis = basis * xfrm.basis;
        return ret;
    }

    inline CTransform getRotation() const
    {
        CTransform ret = *this;
        ret.origin.zeroOut();
        return ret;
    }
    void setRotation(const CMatrix3f& mat) { basis = mat; }
    void setRotation(const CTransform& xfrm) { setRotation(xfrm.basis); }

    /**
     * @brief buildMatrix3f Returns the stored matrix
     * buildMatrix3f is here for compliance with Retro's Math API
     * @return The Matrix (Neo, you are the one)
     */
    inline const CMatrix3f& buildMatrix3f() const { return basis; }

    inline CVector3f operator*(const CVector3f& other) const { return origin + basis * other; }

    inline CMatrix4f toMatrix4f() const
    {
        CMatrix4f ret(basis[0], basis[1], basis[2], origin);
        ret[0][3] = 0.0f;
        ret[1][3] = 0.0f;
        ret[2][3] = 0.0f;
        ret[3][3] = 1.0f;
        return ret;
    }

    inline CVector3f upVector() const
    {
        return { basis.m[2][0], basis.m[2][1], basis.m[2][2] };
    }

    inline CVector3f frontVector() const
    {
        return { basis.m[1][0], basis.m[1][1], basis.m[1][2] };
    }

    inline CVector3f rightVector() const
    {
        return { basis.m[0][0], basis.m[0][1], basis.m[0][2] };
    }

    inline void orthonormalize()
    {
        basis[0].normalize();
        basis[2] = basis[0].cross(basis[1]);
        basis[2].normalize();
        basis[1] = basis[2].cross(basis[0]);
    }

    void printMatrix() const
    {
        printf("%f %f %f %f\n"
               "%f %f %f %f\n"
               "%f %f %f %f\n"
               "%f %f %f %f\n",
               basis[0][0], basis[1][0], basis[2][0], origin[0],
               basis[0][1], basis[1][1], basis[2][1], origin[1],
               basis[0][2], basis[1][2], basis[2][2], origin[2],
               0.f, 0.f, 0.f, 1.f);
    }

    CMatrix3f basis;
    CVector3f origin;
};

static inline CTransform CTransformFromScaleVector(const CVector3f& scale) { return CTransform(CMatrix3f(scale)); }
CTransform CTransformFromEditorEuler(const CVector3f& eulerVec);
CTransform CTransformFromEditorEulers(const CVector3f& eulerVec, const CVector3f& origin);
CTransform CTransformFromAxisAngle(const CVector3f& axis, float angle);
CTransform lookAt(const CVector3f& pos, const CVector3f& lookPos, const CVector3f& up = CVector3f::skUp);
}

#endif // CTRANSFORM_HPP
