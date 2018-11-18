#include "zeus/Math.hpp"
#include "zeus/CTransform.hpp"
#include "zeus/CVector3f.hpp"
#include "zeus/CVector2f.hpp"
#if _WIN32
#include <intrin.h>
#else
#include <cpuid.h>
#endif

namespace zeus
{

static bool isCPUInit = false;
static CPUInfo g_cpuFeatures = {};
static CPUInfo g_missingFeatures = {};

void getCpuInfo(int eax, int regs[4])
{
#if !GEKKO
#if _WIN32
    __cpuid(regs, eax);
#else
    __cpuid(eax, regs[0], regs[1], regs[2], regs[3]);
#endif
#endif
}

void getCpuInfoEx(int eax, int ecx, int regs[4])
{
#if !GEKKO
#if _WIN32
    __cpuidex(regs, eax, ecx);
#else
    __cpuid_count(eax, ecx, regs[0], regs[1], regs[2], regs[3]);
#endif
#endif
}

void detectCPU()
{
#if !GEKKO
    if (isCPUInit)
        return;

    int regs[4];
    getCpuInfo(0, regs);
    int highestFeature = regs[0];
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

    if (highestFeature >= 1)
    {
        getCpuInfo(1, regs);
        memset((bool*)&g_cpuFeatures.AESNI, ((regs[2] & 0x02000000) != 0), 1);
        memset((bool*)&g_cpuFeatures.SSE1, ((regs[3] & 0x02000000) != 0), 1);
        memset((bool*)&g_cpuFeatures.SSE2, ((regs[3] & 0x04000000) != 0), 1);
        memset((bool*)&g_cpuFeatures.SSE3, ((regs[2] & 0x00000001) != 0), 1);
        memset((bool*)&g_cpuFeatures.SSSE3, ((regs[2] & 0x00000200) != 0), 1);
        memset((bool*)&g_cpuFeatures.SSE41, ((regs[2] & 0x00080000) != 0), 1);
        memset((bool*)&g_cpuFeatures.SSE42, ((regs[2] & 0x00100000) != 0), 1);
        memset((bool*)&g_cpuFeatures.AVX, ((regs[2] & 0x10000000) != 0), 1);
    }

    if (highestFeature >= 7)
    {
        getCpuInfoEx(7, 0, regs);
        memset((bool*)&g_cpuFeatures.AVX2, ((regs[1] & 0x00000020) != 0), 1);
    }

    isCPUInit = true;
#endif
}

const CPUInfo& cpuFeatures() { detectCPU(); return g_cpuFeatures; }

std::pair<bool, const CPUInfo&> validateCPU()
{
    detectCPU();
    bool ret = true;

#if __AVX2__
    if (!g_cpuFeatures.AVX2)
    {
        *(bool*) &g_missingFeatures.AVX2 = true;
        ret = false;
    }
#endif
#if __AVX__
    if (!g_cpuFeatures.AVX)
    {
        *(bool*) &g_missingFeatures.AVX = true;
        ret = false;
    }
#endif
#if __SSE4A__
    if (!g_cpuFeatures.SSE4a)
    {
        *(bool*) &g_missingFeatures.SSE4a = true;
        ret = false;
    }
#endif
#if __SSE4_2__
    if (!g_cpuFeatures.SSE42)
    {
        *(bool*) &g_missingFeatures.SSE42 = true;
        ret = false;
    }
#endif
#if __SSE4_1__
    if (!g_cpuFeatures.SSE41)
    {
        *(bool*) &g_missingFeatures.SSE41 = true;
        ret = false;
    }
#endif
#if __SSSE3__
    if (!g_cpuFeatures.SSSE3)
    {
        *(bool*) &g_missingFeatures.SSSE3 = true;
        ret = false;
    }
#endif
#if __SSE3__
    if (!g_cpuFeatures.SSE3)
    {
        *(bool*) &g_missingFeatures.SSE3 = true;
        ret = false;
    }
#endif
#if __SSE2__
    if (!g_cpuFeatures.SSE2)
    {
        *(bool*) &g_missingFeatures.SSE2 = true;
        ret = false;
    }
#endif
#if __SSE__
    if (!g_cpuFeatures.SSE1)
    {
        *(bool*) &g_missingFeatures.SSE1 = true;
        ret = false;
    }
#endif

    return {ret, g_missingFeatures};
}

CTransform lookAt(const CVector3f& pos, const CVector3f& lookPos, const CVector3f& up)
{
    CVector3f vLook, vRight, vUp;

    vLook = lookPos - pos;
    if (vLook.magnitude() <= FLT_EPSILON)
        vLook = {0.f, 1.f, 0.f};
    else
        vLook.normalize();

    vUp = up - vLook * clamp(-1.f, up.dot(vLook), 1.f);
    if (vUp.magnitude() <= FLT_EPSILON)
    {
        vUp = CVector3f(0.f, 0.f, 1.f) - vLook * vLook.z;
        if (vUp.magnitude() <= FLT_EPSILON)
            vUp = CVector3f(0.f, 1.f, 0.f) - vLook * vLook.y;
    }
    vUp.normalize();
    vRight = vLook.cross(vUp);

    CMatrix3f rmBasis(vRight, vLook, vUp);
    return CTransform(rmBasis, pos);
}

CVector3f getBezierPoint(const CVector3f& a, const CVector3f& b,
                         const CVector3f& c, const CVector3f& d, float t)
{
    const float omt = 1.f - t;
    return ((a * omt + b * t) * omt + (b * omt + c * t) * t) * omt +
           ((b * omt + c * t) * omt + (c * omt + d * t) * t) * t;
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

bool close_enough(const CVector3f& a, const CVector3f &b, float epsilon)
{
    if (std::fabs(a.x - b.x) < epsilon && std::fabs(a.y - b.y) < epsilon && std::fabs(a.z - b.z) < epsilon)
        return true;
    return false;
}

bool close_enough(const CVector2f& a, const CVector2f& b, float epsilon)
{
    if (std::fabs(a.x - b.x) < epsilon && std::fabs(a.y - b.y) < epsilon)
        return true;
    return false;
}

template <> CVector3f min(const CVector3f& a, const CVector3f& b)
{
    return {min(a.x, b.x), min(a.y, b.y), min(a.z, b.z)};
}

template <> CVector3f max(const CVector3f& a, const CVector3f& b)
{
    return {max(a.x, b.x), max(a.y, b.y), max(a.z, b.z)};
}
}
