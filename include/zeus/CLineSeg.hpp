#pragma once

#include "Global.hpp"
#include "zeus/CVector3f.hpp"

namespace zeus
{
class CLineSeg
{
public:
    CLineSeg(const CVector3f& start, const CVector3f& end) : x0_start(start), x18_end(end)
    {
        CVector3f tmp = (end - start).normalized();
        if (tmp.x != 0 || tmp.y != 0 || tmp.z != 0)
            xc_dir = tmp.normalized();
        else
            xc_dir = CVector3f::skZero;
    }

    CVector3f x0_start;
    CVector3f xc_dir;
    CVector3f x18_end;
};
}

