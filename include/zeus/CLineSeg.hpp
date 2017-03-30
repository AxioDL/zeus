#ifndef CLINESEG_HPP
#define CLINESEG_HPP

#include "Global.hpp"
#include "zeus/CVector3f.hpp"

namespace zeus
{
class CLineSeg
{
public:
    CLineSeg(const CVector3f& start, const CVector3f& end) : start(start), end(end)
    {
        CVector3f tmp = (end - start).normalized();
        if (tmp.x != 0 || tmp.y != 0 || tmp.z != 0)
            dir = tmp.normalized();
        else
            dir = CVector3f::skZero;
    }

    CVector3f dir;
    CVector3f start;
    CVector3f end;
};
}

#endif
