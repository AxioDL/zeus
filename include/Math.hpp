#ifndef MATH_HPP
#define MATH_HPP

#ifndef NOMINMAX
#define NOMINMAX 1
#endif
#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES 1
#endif
#include <math.h>
#include <algorithm>

#include "CVector3f.hpp"
#include "CTransform.hpp"

namespace Zeus
{
namespace Math
{
    template<typename T>
    inline T min(T a, T b) { return a < b ? a : b; }
    template<typename T>
    inline T max(T a, T b) { return a > b ? a : b; }

    template<typename T>
    inline T clamp(T a, T val, T b) {return max<T>(a, min<T>(b, val));}
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

    CVector3f getBezierPoint(const CVector3f& a, const CVector3f& b,
                             const CVector3f& c, const CVector3f& d, float t);
    float getCatmullRomSplinePoint(float a, float b,
                                   float c, float d, float t);
    CVector3f getCatmullRomSplinePoint(const CVector3f& a, const CVector3f& b,
                                       const CVector3f& c, const CVector3f& d, float t);
    CVector3f getRoundCatmullRomSplinePoint(const CVector3f& a, const CVector3f& b,
                                            const CVector3f& c, const CVector3f& d, float t);

    inline float slowCosineR(float val)  { return float(cos(val));  }
    inline float slowSineR(float val)    { return float(sin(val));  }
    inline float slowTangentR(float val) { return float(tan(val)); }
    inline float arcSineR(float val)     { return float(asin(val)); }
    inline float arcTangentR(float val)  { return float(atan(val)); }
    inline float arcCosineR(float val)      { return float(acos(val)); }
    inline float powF(float a, float b)  { return float(exp(b * log(a))); }
    inline float floorF(float val)       { return float(floor(val)); }
    inline float ceilingF(float val)
    {
        float tmp = floorF(val);
        return (tmp == val ? tmp : tmp + 1.0);
    }

    // Since round(double) doesn't exist in some <cmath> implementations
    // we'll define our own
    inline double round(double val) { return (val < 0.0 ? ceilingF(val - 0.5) : floorF(val + 0.5)); }
    inline double powD(float a, float b) { return exp(a * log(b)); }

    double sqrtD(double val);
    inline double invSqrtD(double val)  { return 1.0 / sqrtD(val); }
    inline float invSqrtF(float val)    { return float(1.0 / sqrtD(val)); }
    inline float sqrtF(float val)       { return float(sqrtD(val)); }
    float fastArcCosR(float val);
    float fastCosR(float val);
    float fastSinR(float val);
    int floorPowerOfTwo(int x);
}
}

#endif // MATH_HPP
