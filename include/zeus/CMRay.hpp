#ifndef CMRAY_HPP
#define CMRAY_HPP
#include "zeus/CVector3f.hpp"
#include "zeus/CTransform.hpp"
#include "zeus/Math.hpp"

namespace zeus
{
struct CMRay
{
    CMRay(const CVector3f& start, const CVector3f& end, float d) : start(start), d(d), invD(1.f / d), end(end)
    {
        normal = start + (d * end);
        delta = normal - start;
    }

    CMRay(const CVector3f& start, const CVector3f& norm, float d, float invD) : start(start), normal(norm), d(d), invD(invD)
    {
        delta = normal - start;
        end = invD * delta;
    }

    CMRay getInvUnscaledTransformRay(const CTransform& xfrm) const
    {
        const CTransform inv = xfrm.inverse();
        return CMRay(inv * start, inv * normal, d, invD);
    }

    CVector3f start; // x0
    CVector3f normal; // xc
    CVector3f delta; // x18
    float d; // x24
    float invD; // x28
    CVector3f end; // x2c
};
}

#endif
