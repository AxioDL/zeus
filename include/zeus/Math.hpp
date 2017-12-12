#ifndef MATH_HPP
#define MATH_HPP

#include <float.h>
#undef min
#undef max

#undef M_PI
#define M_PI 3.14159265358979323846   /* pi */
#define M_PIF 3.14159265358979323846f /* pi */
#undef M_PI_2
#define M_PI_2 1.57079632679489661923 /* pi/2 */
#undef M_PI_4
#define M_PI_4 0.78539816339744830962 /* pi/4 */
#undef M_1_PI
#define M_1_PI 0.31830988618379067154 /* 1/pi */
#undef M_2_PI
#define M_2_PI 0.63661977236758134308 /* 2/pi */
#undef M_2_SQRTPI
#define M_2_SQRTPI 1.12837916709551257390 /* 2/sqrt(pi) */
#undef M_SQRT2
#define M_SQRT2 1.41421356237309504880 /* sqrt(2) */
#undef M_SQRT1_2
#define M_SQRT1_2 0.70710678118654752440 /* 1/sqrt(2) */
#define M_SQRT1_2F 0.70710678118654752440f /* 1/sqrt(2) */

#include <cmath>
#include <algorithm>

namespace zeus
{
struct CPUInfo
{
    const char cpuBrand[48] = {0};
    const char cpuVendor[32] = {0};
    const bool isIntel = false;
    const bool SSE1 = false;
    const bool SSE2 = false;
    const bool SSE3 = false;
    const bool SSSE3 = false;
    const bool SSE41 = false;
    const bool SSE42 = false;
    const bool SSE4a = false;
    const bool AESNI = false;
};
/**
 * Detects CPU capabilities and returns true if SSE4.1 or SSE4.2 is available
 */
void detectCPU();
const CPUInfo& cpuFeatures();
std::pair<bool, const CPUInfo&> validateCPU();
class CVector3f;
class CVector2f;
class CTransform;

template <typename T>
inline constexpr T min(const T& a, const T& b)
{
    return a < b ? a : b;
}
template <typename T>
inline constexpr T max(const T& a, const T& b)
{
    return a > b ? a : b;
}
template <> CVector3f min(const CVector3f& a, const CVector3f& b);
template <> CVector3f max(const CVector3f& a, const CVector3f& b);

template <typename T>
inline constexpr T clamp(const T& a, const T& val, const T& b)
{
    return max<T>(a, min<T>(b, val));
}

inline constexpr float radToDeg(float rad) { return rad * (180.f / M_PIF); }
inline constexpr float degToRad(float deg) { return deg * (M_PIF / 180.f); }
inline constexpr double radToDeg(double rad) { return rad * (180.0 / M_PI); }
inline constexpr double degToRad(double deg) { return deg * (M_PI / 180.0); }

CVector3f baryToWorld(const CVector3f& p0, const CVector3f& p1, const CVector3f& p2, const CVector3f& bary);

CVector3f getBezierPoint(const CVector3f& a, const CVector3f& b, const CVector3f& c, const CVector3f& d, float t);
float getCatmullRomSplinePoint(float a, float b, float c, float d, float t);
CVector3f getCatmullRomSplinePoint(const CVector3f& a, const CVector3f& b, const CVector3f& c, const CVector3f& d, float t);
CVector3f getRoundCatmullRomSplinePoint(const CVector3f& a, const CVector3f& b, const CVector3f& c, const CVector3f& d,
                                        float t);

inline float powF(float a, float b) { return std::pow(a, b); }
inline float floorF(float val) { return std::floor(val); }
inline float ceilingF(float val)
{
    float tmp = std::floor(val);
    return (tmp == val ? tmp : tmp + 1.0);
}

// Since round(double) doesn't exist in some <cmath> implementations
// we'll define our own
inline double round(double val) { return (val < 0.0 ? ceilingF(val - 0.5) : floorF(val + 0.5)); }
inline double powD(float a, float b) { return std::exp(b * std::log(a)); }

double sqrtD(double val);
inline double invSqrtD(double val) { return 1.0 / sqrtD(val); }
inline float invSqrtF(float val) { return float(1.0 / sqrtD(val)); }
inline float sqrtF(float val) { return float(sqrtD(val)); }
float fastArcCosF(float val);
float fastCosF(float val);
float fastSinF(float val);
int floorPowerOfTwo(int x);
int ceilingPowerOfTwo(int x);

template <typename U>
typename std::enable_if<!std::is_enum<U>::value && std::is_integral<U>::value, int>::type PopCount(U x)
{
#if __GNUC__ >= 4
    return __builtin_popcountll(x);
#else
    const U m1 = U(0x5555555555555555);  // binary: 0101...
    const U m2 = U(0x3333333333333333);  // binary: 00110011..
    const U m4 = U(0x0f0f0f0f0f0f0f0f);  // binary:  4 zeros,  4 ones ...
    const U h01 = U(0x0101010101010101); // the sum of 256 to the power of 0,1,2,3...

    x -= (x >> 1) & m1;                        // put count of each 2 bits into those 2 bits
    x = (x & m2) + ((x >> 2) & m2);            // put count of each 4 bits into those 4 bits
    x = (x + (x >> 4)) & m4;                   // put count of each 8 bits into those 8 bits
    return (x * h01) >> ((sizeof(U) - 1) * 8); // returns left 8 bits of x + (x<<8) + (x<<16) + (x<<24) + ...
#endif
}

template <typename E>
typename std::enable_if<std::is_enum<E>::value, int>::type PopCount(E e)
{
    return PopCount(static_cast<typename std::underlying_type<E>::type>(e));
}


bool close_enough(const CVector3f &a, const CVector3f &b, float epsilon = 0.000099999997f);
bool close_enough(const CVector2f& a, const CVector2f& b, float epsilon = 0.000099999997f);

inline bool close_enough(float a, float b, double epsilon = 0.000009999999747378752)
{
    return std::fabs(a - b) < epsilon;
}
}

#endif // MATH_HPP
