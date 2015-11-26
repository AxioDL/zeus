#include "CVector4f.hpp"
#include "CColor.hpp"

namespace Zeus
{
CVector4f::CVector4f(const Zeus::CColor& other)
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
