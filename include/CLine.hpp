#ifndef CLINE_HPP
#define CLINE_HPP

#include "Global.hpp"
#include "CVector3f.hpp"
#include "CUnitVector.hpp"

namespace Zeus
{
class alignas(16) CLine
{
public:
    CLine(const CVector3f& a, const CUnitVector3f& b)
    {
        CVector3f ab = (b - a).normalized();

        start = a;
        if (ab.x != 0.0f || ab.y != 0.0f || ab.z != 0.0f)
            normal = ab;

        end = b;
    }


    CVector3f start;
    CVector3f normal;
    CUnitVector3f end;
};
}

#endif
