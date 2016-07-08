#include "zeus/Math.hpp"
#include "zeus/CTransform.hpp"
#include "zeus/CVector3f.hpp"
#if _WIN32
#include <intrin.h>
#else
#include <cpuid.h>
#endif

namespace zeus
{

static CPUInfo g_cpuFeatures;

void getCpuInfo(int level, int regs[4])
{
#if !GEKKO
#if _WIN32
    __cpuid(regs, level);
#else
    __cpuid(level, regs[0], regs[1], regs[2], regs[3]);
#endif
#endif
}

void detectCPU()
{
#if !GEKKO
    static bool isInit = false;
    if (isInit)
        return;

    int regs[4];
    getCpuInfo(0, regs);
    *reinterpret_cast<int*>((char*)g_cpuFeatures.cpuVendor) = regs[1];
    *reinterpret_cast<int*>((char*)g_cpuFeatures.cpuVendor + 4) = regs[3];
    *reinterpret_cast<int*>((char*)g_cpuFeatures.cpuVendor + 8) = regs[2];
    getCpuInfo(0x80000000, regs);
    if (regs[0] >= 0x80000004)
    {
        for (unsigned int i = 0x80000002; i <= 0x80000004; i++)
        {
            getCpuInfo(i, regs);
            // Interpret CPU brand string and cache information.
            if (i == 0x80000002)
                memcpy((char*)g_cpuFeatures.cpuBrand, regs, sizeof(regs));
            else if (i == 0x80000003)
                memcpy((char*)g_cpuFeatures.cpuBrand + 16, regs, sizeof(regs));
            else if (i == 0x80000004)
                memcpy((char*)g_cpuFeatures.cpuBrand + 32, regs, sizeof(regs));
        }
    }

    getCpuInfo(1, regs);

    memset((bool*)&g_cpuFeatures.AESNI, ((regs[2] & 0x02000000) != 0), 1);
    memset((bool*)&g_cpuFeatures.SSE1, ((regs[3] & 0x02000000) != 0), 1);
    memset((bool*)&g_cpuFeatures.SSE2, ((regs[3] & 0x04000000) != 0), 1);
    memset((bool*)&g_cpuFeatures.SSE3, ((regs[2] & 0x00000001) != 0), 1);
    memset((bool*)&g_cpuFeatures.SSSE3, ((regs[2] & 0x00000200) != 0), 1);
    memset((bool*)&g_cpuFeatures.SSE41, ((regs[2] & 0x00080000) != 0), 1);
    memset((bool*)&g_cpuFeatures.SSE42, ((regs[2] & 0x00100000) != 0), 1);

    isInit = true;
#endif
}

const CPUInfo& cpuFeatures() { return g_cpuFeatures; }

CTransform lookAt(const CVector3f& pos, const CVector3f& lookPos, const CVector3f& up)
{
    CVector3f vLook, vRight, vUp;

    vLook = lookPos - pos;
    if (vLook.magnitude() < FLT_EPSILON)
        vLook = {0.f, 1.f, 0.f};
    vLook.normalize();

    vRight = vLook.cross(up);
    vRight.normalize();

    vUp = vRight.cross(vLook);

    CMatrix3f rmBasis(vRight, vLook, vUp);
    return CTransform(rmBasis, pos);
}

CVector3f getBezierPoint(const CVector3f& a, const CVector3f& b, const CVector3f& c, const CVector3f& d, float t)
{
    const float oneMinusTime = (1.0 - t);
    return (a * oneMinusTime * oneMinusTime) + (b * 3.f * t * oneMinusTime) + (c * 3.f * t * t * oneMinusTime) +
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
    union {
        __m128d v;
        double d[2];
    } qv = {val};
    qv.v = _mm_sqrt_sd(qv.v, qv.v);
    q = qv.d[0];
#else
    // le sigh, let's use Carmack's inverse square -.-
    union {
        double v;
        int i;
    } p;

    double x = val * 0.5F;
    p.v = val;
    p.i = 0x5fe6eb50c7b537a9 - (p.i >> 1);
    p.v *= (1.5f - (x * p.v * p.v));
    p.v *= (1.5f - (x * p.v * p.v));
    q = p.v;

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
#endif
    return q;
}

float fastArcCosF(float val)
{
    /* If we're not at a low enough value,
     * the approximation below won't provide any benefit,
     * and we simply fall back to the standard implementation
     */
    if (std::fabs(val) >= 0.925000011920929f)
        return std::acos(val);

    /* Fast Arc Cosine approximation using Taylor Polynomials
     * while this implementation is fast, it's also not as accurate.
     * This is a straight reimplementation of Retro's CFastArcCosR
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

float fastCosF(float val)
{
    if (std::fabs(val) > M_PIF)
    {
        float rVal = float(uint32_t(val));
        val = -((rVal * val) - 6.2831855f);
        if (val <= M_PIF && val < -M_PIF)
            val += 6.2831855f;
        else
            val -= 6.2831855f;
    }

    float sq = val * val;
    float b = sq * sq;
    val = sq + -0.4999803f;
    val = (b * val) + 0.041620344f;
    b = b * sq;
    val = (b * val) + -0.0013636103f;
    b = b * sq;
    val = (b * val) + 0.000020169435f;
    return val;
}

float fastSinF(float val)
{
    if (std::fabs(val) > M_PIF)
    {
        float rVal = float(uint32_t(val));
        val = -((rVal * val) - 6.2831855f);
        if (val <= M_PIF && val < -M_PIF)
            val += 6.2831855f;
        else
            val -= 6.2831855f;
    }

    float sq = val * val;
    float ret = val * 0.99980587f;
    val = val * sq;
    ret = (val * ret) + -0.16621658f;
    val = val * sq;
    ret = (val * ret) + 0.0080871079f;
    val = val * sq;
    ret = (val * ret) + -0.00015297699f;
    return ret;
}

float getCatmullRomSplinePoint(float a, float b, float c, float d, float t)
{
    if (t <= 0.0f)
        return b;
    if (t >= 1.0f)
        return c;

    const float t2 = t * t;
    const float t3 = t2 * t;

    return (a * (-0.5f * t3 + t2 - 0.5f * t) + b * (1.5f * t3 + -2.5f * t2 + 1.0f) + c * (-1.5f * t3 + 2.0f * t2 + 0.5f * t) +
            d * (0.5f * t3 - 0.5f * t2));
}

CVector3f getCatmullRomSplinePoint(const CVector3f& a, const CVector3f& b, const CVector3f& c, const CVector3f& d, float t)
{
    if (t <= 0.0f)
        return b;
    if (t >= 1.0f)
        return c;

    const float t2 = t * t;
    const float t3 = t2 * t;

    return (a * (-0.5f * t3 + t2 - 0.5f * t) + b * (1.5f * t3 + -2.5f * t2 + 1.0f) + c * (-1.5f * t3 + 2.0f * t2 + 0.5f * t) +
            d * (0.5f * t3 - 0.5f * t2));
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
    return zeus::getCatmullRomSplinePoint(b, c, bVelocity * cbDistance, cVelocity * cbDistance, t);
}

CVector3f baryToWorld(const CVector3f& p0, const CVector3f& p1, const CVector3f& p2, const CVector3f& bary)
{
    return bary.x * p0 + bary.y * p1 + bary.z * p2;
}
}
