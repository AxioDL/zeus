#pragma once

#include "Global.hpp"
#include "zeus/Math.hpp"
#include "CVector2f.hpp"

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
    CVector2i(const CVector2f& vec) : x(int(vec.x)), y(int(vec.y)) {}

    CVector2f toVec2f() const { return CVector2f(x, y); }

    inline CVector2i operator+(const CVector2i& val) const
    {
        return CVector2i(x + val.x, y + val.y);
    }
    inline CVector2i operator-(const CVector2i& val) const
    {
        return CVector2i(x - val.x, y - val.y);
    }
    inline CVector2i operator*(const CVector2i& val) const
    {
        return CVector2i(x * val.x, y * val.y);
    }
    inline CVector2i operator/(const CVector2i& val) const
    {
        return CVector2i(x / val.x, y / val.y);
    }
    inline bool operator==(const CVector2i& other) const
    {
        return x == other.x && y == other.y;
    }
    inline bool operator!=(const CVector2i& other) const
    {
        return x != other.x || y != other.y;
    }
    inline CVector2i operator*(int val) const
    {
        return CVector2i(x * val, y * val);
    }
};
}

