#ifndef CMRAY_HPP
#define CMRAY_HPP
#include "zeus/CVector3f.hpp"
#include "zeus/CTransform.hpp"
#include "zeus/Math.hpp"

namespace zeus
{
struct alignas(16) CMRay
{
    CMRay(const CVector3f& start, const CVector3f& end, float dir)
        : start(start),
          dir(dir),
          unk3(1.0 / dir),
          unk4(end)
    {
        unk1 = start + (dir * end);
        unk2 = (unk1 - start);
    }

    CMRay(const CVector3f &start, const CVector3f &end, float dir, float unk)
        : start(start),
          dir(dir),
          unk3(unk),
          unk4(end)
    {
        unk1 = start + (dir * end);
        unk2 = (unk1 - start);
    }

    CMRay GetInvUnscaledTransformRay(const CTransform& xfrm)
    {
        CTransform inv = xfrm.inverse();

        return CMRay(newStart, newEnd, dir, unk3);
    }

    CVector3f start;
    CVector3f unk1;
    CVector3f unk2;
    float dir;
    float unk3;
    CVector3f unk4;
};
}

#endif
