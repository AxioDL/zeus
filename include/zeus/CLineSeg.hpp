#ifndef CLINE_HPP
#define CLINE_HPP

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
            normal = tmp.normalized();
        else
            normal = CVector3f::skZero;
    }

    CVector3f normal;
    CVector3f start;
    CVector3f end;
};
}

#endif
