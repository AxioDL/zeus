#ifndef MATH_HPP
#define MATH_HPP

#ifndef NOMINMAX
#define NOMINMAX 1
#endif
#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES 1
#endif
#include <math.h>
#include "CVector3f.hpp"
#include "CTransform.hpp"
#include <algorithm>

namespace Zeus
{
namespace Math
{
    template<typename T>
    inline T clamp(T min, T val, T max) {return std::max(min, std::min(max, val));}
    inline float radToDeg(float rad) {return rad * 180.f / M_PI;}
    inline float degToRad(float deg) {return deg * M_PI / 180;}

    extern const CVector3f kRadToDegVec;
    extern const CVector3f kDegToRadVec;
    inline CVector3f radToDeg(CVector3f rad) {return rad * kRadToDegVec;}
    inline CVector3f degToRad(CVector3f deg) {return deg * kDegToRadVec;}

    // Since round(double) doesn't exist in some <cmath> implementations
    // we'll define our own
    inline float round(double val) { return (val < 0.0 ? ceil(val - 0.5) : floor(val + 0.5)); }

    extern const CVector3f kUpVec;
    CTransform lookAt(const CVector3f& pos, const CVector3f& lookPos, const CVector3f& up=kUpVec);
    inline CVector3f baryToWorld(const CVector3f& p0, const CVector3f& p1, const CVector3f& p2, const CVector3f& bary)
    { return bary.x * p0 + bary.y * p1 + bary.z * p2; }

    CVector3f getBezierPoint(const CVector3f& a, const CVector3f& b, const CVector3f& c, const CVector3f& d, float t);
    float getCatmullRomSplinePoint(float p0, float p1,
                                   float p2, float p3, float t);
    CVector3f getCatmullRomSplinePoint(const CVector3f& p0, const CVector3f& p1,
                                       const CVector3f& p2, const CVector3f& p3, float t);

    inline float slowCosineR(float val) { return float(cos(val));  }
    inline float slowSineR(float val)   { return float(sin(val));  }
    inline float arcSineR(float val)    { return float(asin(val)); }
    inline float arcTangentR(float val) { return float(atan(val)); }
    inline float fastArcCosR(float val)
    {
        double f2 = fabs(val);
        if (f2 <= 0.925000011920929)
            return float(acos(val));

        float f4 = val * val;
        float f5 = 1.5707964f;
        float f0 = -0.99822718f;
        float f3 = -0.20586604f;
        f5 = (val * f5) + f0;
        f2 = 0.11425424f;
        float f1 = val * f4;
        f0 = -0.29697824f;
        f5 = (f1 * f5) + f3;
        f1 = (f1 * f4);
        f5 = (f1 * f5) + f2;
        f1 = (f1 * f4);
        f5 = (f1 * f5) + f0;
        return f5;
    }

    inline int floorPowerOfTwo(int x)
    {
        if (x == 0)
            return 0;
        /*
         * we want to ensure that we always get the previous power,
         * but if we have values like 256, we'll always get the same value,
         * x-1 ensures that we always get the previous power.
         */
        x = (x - 1) | (x >> 1);
        x = x | (x >> 2);
        x = x | (x >> 4);
        x = x | (x >> 8);
        x = x | (x >> 16);
        return x - (x >> 1);
    }
}
}

#endif // MATH_HPP
