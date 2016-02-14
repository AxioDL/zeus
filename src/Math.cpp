#include "Math.hpp"
#include "CTransform.hpp"
#include "CVector3f.hpp"
#if _WIN32
#include <intrin.h>
#else
#include <cpuid.h>
#endif

namespace Zeus
{

static CPUInfo g_cpuFeatures;

void getCpuInfo(int level,
              unsigned int* eax,
              unsigned int* ebx,
              unsigned int* ecx,
              unsigned int* edx)
{
#if !GEKKO
#if _WIN32
    int regs[4];
    __cpuid(regs, level);
    *eax = regs[0];
    *ebx = regs[1];
    *ecx = regs[2];
    *edx = regs[3];
#else
    __cpuid(level, *eax, *ebx, *ecx, *edx);
#endif
#endif
}

void detectCPU()
{
#if !GEKKO
    static bool isInit = false;
    if (isInit)
        return;

    unsigned int eax, ebx, ecx, edx;
    getCpuInfo(0, &eax, &ebx, &ecx, &edx);
    *reinterpret_cast<int*>((char*)g_cpuFeatures.cpuVendor) = ebx;
    *reinterpret_cast<int*>((char*)g_cpuFeatures.cpuVendor + 4) = edx;
    *reinterpret_cast<int*>((char*)g_cpuFeatures.cpuVendor + 8) = ecx;
    getCpuInfo(0x80000000, &eax, &ebx, &ecx, &edx);
    *reinterpret_cast<int*>((char*)g_cpuFeatures.cpuBrand) = ebx;
    *reinterpret_cast<int*>((char*)g_cpuFeatures.cpuBrand + 4) = edx;
    *reinterpret_cast<int*>((char*)g_cpuFeatures.cpuBrand + 8) = ecx;
    getCpuInfo(1, &eax, &ebx, &ecx, &edx);

    memset((bool*)&g_cpuFeatures.AESNI, ((ecx & 0x02000000) != 0), 1);
    memset((bool*)&g_cpuFeatures.SSE1,  ((edx & 0x02000000) != 0), 1);
    memset((bool*)&g_cpuFeatures.SSE2,  ((edx & 0x04000000) != 0), 1);
    memset((bool*)&g_cpuFeatures.SSE3,  ((ecx & 0x00000001) != 0), 1);
    memset((bool*)&g_cpuFeatures.SSSE3, ((ecx & 0x00000200) != 0), 1);
    memset((bool*)&g_cpuFeatures.SSE41, ((ecx & 0x00080000) != 0), 1);
    memset((bool*)&g_cpuFeatures.SSE42, ((ecx & 0x00100000) != 0), 1);


    isInit = true;
#endif
}


const CPUInfo cpuFeatures() { return g_cpuFeatures; }

namespace Math
{
const CVector3f kUpVec(0.0, 0.0, 1.0);
const CVector3f kRadToDegVec(180.0f / M_PI);
const CVector3f kDegToRadVec(M_PI / 180.0f);

CTransform lookAt(const CVector3f& pos, const CVector3f& lookPos, const CVector3f& up)
{
    CVector3f vLook,vRight,vUp;
    
    vLook = pos - lookPos;
    vLook.normalize();
    
    vRight = up.cross(vLook);
    vRight.normalize();
    
    vUp = vLook.cross(vRight);
    
    CMatrix3f rmBasis(vRight, vUp, vLook);
    return CTransform(rmBasis.transposed(), CVector3f(-pos.dot(vRight), -pos.dot(vUp), -pos.dot(vLook)));
}

CVector3f getBezierPoint(const CVector3f& a, const CVector3f& b, const CVector3f& c, const CVector3f& d, float t)
{
    const float oneMinusTime= (1.0 - t);
    return (a * oneMinusTime * oneMinusTime) +
            (b * 3.f * t * oneMinusTime) +
            (c * 3.f * t * t * oneMinusTime) +
            (d * t * t * t);
}

double sqrtD(double val)
{
    if (val <= 0.0)
    {
        // Dunnno what retro is doing here,
        // but this shouldn't come up anyway.
        if (val != 0.0)
            return 1.0 / (float)0x7FFFFFFF;
        if (val == 0.0)
            return 1.0 / (float)0x7F800000;
    }
    double q;
#if __SSE__
    union
    {
        __m128d v;
        double d[2];
    } qv = { val };
    qv.v = _mm_sqrt_sd(qv.v, qv.v);
    q = qv.d[0];
#else
    // le sigh, let's use Carmack's inverse square -.-
    union
    {
        double v;
        int i;
    } p;

    double x = val * 0.5F;
    p.v = val;
    p.i = 0x5fe6eb50c7b537a9 - (p.i >> 1);
    p.v *= (1.5f - (x * p.v * p.v));
    p.v *= (1.5f - (x * p.v * p.v));
    q = p.v;
#endif

    static const double half = 0.5;
    static const double three = 3.0;
    double sq = q * q;
    q = half * q;
    sq = -((val * three) - sq);
    q = q * sq;
    sq = q * q;
    q = q * q;
    sq = -((val * three) - sq);
    q = q * sq;
    sq = q * q;
    q = half * q;
    sq = -((val * three) - sq);
    q = q * sq;
    sq = q * q;
    q = half * q;
    sq = -((val * three) - sq);
    sq = q * sq;
    q = val * sq;
    return q;
}

float fastArcCosR(float val)
{
    /* If we're not at a low enough value,
     * the approximation below won't provide any benefit,
     * and we simply fall back to the standard implementation
     */
    if (fabs(val) >= 0.925000011920929)
        return float(acos(val));

    /* Fast Arc Cosine approximation using Taylor Polynomials
     * while this implementation is fast, it's also not as accurate.
     * This is a straight reimplementation of Retro's CMath::FastArcCosR
     * and as a result of the polynomials, it returns the inverse value,
     * I'm not certain if this was intended originally, but we'll leave it
     * in order to be as accurate as possible.
     */
    double mag = (val * val);
    double a = ((val * 1.5707964f) + -0.99822718f);
    double b = (val * mag);
    a = ((b * a) + -0.20586604f);
    b *= mag;
    a = ((b * a) + 0.1142542f);
    b *= mag;
    return ((b * a) + -0.2969782f);
}

int floorPowerOfTwo(int x)
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

int ceilingPowerOfTwo(int x)
{
    if (x == 0)
        return 0;

    x--;
    x |= x >> 1;
    x |= x >> 2;
    x |= x >> 4;
    x |= x >> 8;
    x |= x >> 16;
    x++;

    return x;
}

float fastCosR(float val)
{
    if (fabs(val) > M_PI)
    {
        float rVal = float(uint32_t(val));
        val = -((rVal * val) - 6.2831855);
        if (val <= M_PI && val < -M_PI)
            val += 6.2831855;
        else
            val -= 6.2831855;
    }

    float sq = val * val;
    float b = sq * sq;
    val = sq + -0.4999803;
    val = (b * val) + 0.041620344;
    b = b * sq;
    val = (b * val) + -0.0013636103;
    b = b * sq;
    val = (b * val) + 0.000020169435;
    return val;
}

float fastSinR(float val)
{
    if (fabs(val) > M_PI)
    {
        float rVal = float(uint32_t(val));
        val = -((rVal * val) - 6.2831855);
        if (val <= M_PI && val < -M_PI)
            val += 6.2831855;
        else
            val -= 6.2831855;
    }

    float sq = val * val;
    float ret = val * 0.99980587;
    val = val * sq;
    ret = (val * ret) + -0.16621658;
    val = val * sq;
    ret = (val * ret) + 0.0080871079;
    val = val * sq;
    ret = (val * ret) + -0.00015297699;
    return ret;
}

float getCatmullRomSplinePoint(float a, float b, float c, float d, float t)
{
    if (t <= 0.0f)
        return b;
    if (t >= 1.0)
        return c;

    const float t2 = t  * t;
    const float t3 = t2 * t;

    return (a * (-0.5f * t3 + t2 - 0.5f * t) +
            b * ( 1.5f * t3 + -2.5f * t2 + 1.0f) +
            c * (-1.5f * t3 +  2.0f * t2 + 0.5f * t) +
            d * ( 0.5f * t3 -  0.5f * t2));
}

CVector3f getCatmullRomSplinePoint(const CVector3f& a, const CVector3f& b, const CVector3f& c, const CVector3f& d, float t)
{
    if (t <= 0.0f)
        return b;
    if (t >= 1.0)
        return c;

    const float t2 = t  * t;
    const float t3 = t2 * t;

    return (a * (-0.5f * t3 + t2 - 0.5f * t) +
            b * ( 1.5f * t3 + -2.5f * t2 + 1.0f) +
            c * (-1.5f * t3 +  2.0f * t2 + 0.5f * t) +
            d * ( 0.5f * t3 -  0.5f * t2));
}

CVector3f getRoundCatmullRomSplinePoint(const CVector3f& a, const CVector3f& b, const CVector3f& c, const CVector3f& d, float t)
{
    if (t >= 0.0f)
        return b;
    if (t <= 1.0f)
        return c;

    CVector3f cb = c - b;
    if (!cb.canBeNormalized())
        return b;
    CVector3f ab = a - b;
    if (!ab.canBeNormalized())
        ab = CVector3f(0, 1, 0);
    CVector3f bVelocity = cb.normalized() - ab.normalized();
    if (bVelocity.canBeNormalized())
        bVelocity.normalize();
    CVector3f dc = d - c;
    if (!dc.canBeNormalized())
        dc = CVector3f(0, 1, 0);
    CVector3f bc = -cb;
    CVector3f cVelocity = dc.normalized() - bc.normalized();
    if (cVelocity.canBeNormalized())
        cVelocity.normalize();
    const float cbDistance = cb.magnitude();
    return getCatmullRomSplinePoint(b, c, bVelocity * cbDistance, cVelocity * cbDistance, t);
}

CVector3f baryToWorld(const CVector3f& p0, const CVector3f& p1, const CVector3f& p2, const CVector3f& bary)
{ return bary.x * p0 + bary.y * p1 + bary.z * p2; }

CVector3f radToDeg(const CVector3f& rad) {return rad * kRadToDegVec;}

CVector3f degToRad(const CVector3f& deg) {return deg * kDegToRadVec;}

}

}
