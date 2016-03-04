#include "zeus/CVector4f.hpp"
#include "zeus/CColor.hpp"

namespace zeus
{
CVector4f::CVector4f(const zeus::CColor& other)
    : x(other.r), y(other.g), z(other.b), w(other.a)
{
}

CVector4f& CVector4f::operator=(const CColor& other)
{
    x = other.r;
    y = other.g;
    z = other.b;
    w = other.a;

    return *this;
}
}
