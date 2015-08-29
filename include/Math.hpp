#ifndef MATH_HPP
#define MATH_HPP

#define _USE_MATH_DEFINES 1
#include <math.h>
#include "CVector3f.hpp"
#include "CTransform.hpp"

/* Macros for min/max. */
#ifndef MIN
#define	MIN(a,b) (((a)<(b))?(a):(b))
#endif /* MIN */
#ifndef MAX
#define	MAX(a,b) (((a)>(b))?(a):(b))
#endif	/* MAX */

namespace Math
{
    template<typename T>
    inline T clamp(T min, T val, T max) {return MAX(min, MIN(max, val));}
    inline float radToDeg(float rad) {return rad * 180.f / M_PI;}
    inline float degToRad(float deg) {return deg * M_PI / 180;}

    extern const CVector3f kRadToDegVec;
    extern const CVector3f kDegToRadVec;
    inline CVector3f radToDeg(CVector3f rad) {return rad * kRadToDegVec;}
    inline CVector3f degToRad(CVector3f deg) {return deg * kDegToRadVec;}

    extern const CVector3f kUpVec;
    CTransform lookAt(const CVector3f& pos, const CVector3f& lookPos, const CVector3f& up=kUpVec);
    inline CVector3f baryToWorld(const CVector3f& p0, const CVector3f& p1, const CVector3f& p2, const CVector3f& bary)
    { return bary.x * p0 + bary.y * p1 + bary.z * p2; }
}

#endif // MATH_HPP
