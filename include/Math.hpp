#ifndef MATH_HPP
#define MATH_HPP

#undef min
#undef max
#ifndef NOMINMAX
#define NOMINMAX 1
#endif
#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES 1
#endif
#include <math.h>
#include <algorithm>

namespace Zeus
{
struct CPUInfo
{
const char cpuBrand [32] = {0};
const char cpuVendor[32] = {0};
const bool isIntel       = false;
const bool SSE1          = false;
const bool SSE2          = false;
const bool SSE3          = false;
const bool SSSE3         = false;
const bool SSE41         = false;
const bool SSE42         = false;
const bool SSE4a         = false;
const bool AESNI         = false;
};
/**
 * Detects CPU capabilities and returns true if SSE4.1 or SSE4.2 is available
 */
void detectCPU();
const CPUInfo cpuFeatures();
class CVector3f;
class CTransform;
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
    CVector3f radToDeg(const CVector3f& rad);
    CVector3f degToRad(const CVector3f& deg);

    extern const CVector3f kUpVec;
    CTransform lookAt(const CVector3f& pos, const CVector3f& lookPos, const CVector3f& up=kUpVec);
    CVector3f baryToWorld(const CVector3f& p0, const CVector3f& p1, const CVector3f& p2, const CVector3f& bary);

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
    inline float arcCosineR(float val)   { return float(acos(val)); }
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
    inline double powD(float a, float b) { return exp(b * log(a)); }

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
