#ifndef CVECTOR2i_HPP
#define CVECTOR2i_HPP

#include "Global.hpp"
#include "zeus/Math.hpp"

#if ZE_ATHENA_TYPES
#include <athena/IStreamReader.hpp>
#endif

namespace zeus
{

class CVector2i
{
public:
    union {
        struct
        {
            int x, y;
        };
        int v[2];
    };
    CVector2i() = default;
    CVector2i(int xin, int yin) : x(xin), y(yin) {}
};
}

#endif // CVECTOR2i_HPP
