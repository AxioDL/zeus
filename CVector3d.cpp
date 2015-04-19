#include "CVector3d.hpp"
#include <memory.h>
#include <cmath>

CVector3d::CVector3d()
{
    memset(&v, 0, sizeof(v));
}

CVector3d::CVector3d(double x, double y, double z)
    : x(x),
      y(y),
      z(z)
{
}

CVector3d::CVector3d(Athena::io::IStreamReader& input)
{
    x = input.readDouble();
    y = input.readDouble();
    z = input.readDouble();
}

CVector3d::~CVector3d()
{
}

bool CVector3d::operator ==(const CVector3d& rhs)
{
    return (x == rhs.x && y == rhs.y && z == rhs.z);
}

CVector3d CVector3d::operator+(const CVector3d& rhs)
{
    return CVector3d(x + rhs.x, y + rhs.y, z + rhs.z);
}

CVector3d CVector3d::operator-(const CVector3d& rhs)
{
    return CVector3d(x - rhs.x, y - rhs.y, z - rhs.z);
}

CVector3d CVector3d::operator-() const
{
    return CVector3d(-x, -y, -z);
}

CVector3d CVector3d::operator*(const CVector3d& rhs)
{
    return CVector3d(x * rhs.x, y * rhs.y, z * rhs.z);
}

CVector3d CVector3d::operator/(const CVector3d& rhs)
{
    return CVector3d(x / rhs.x, y / rhs.y, z / rhs.z);
}

CVector3d CVector3d::operator+(double val)
{
    return CVector3d(x + val, y + val, z + val);
}

CVector3d CVector3d::operator-(double val)
{
    return CVector3d(x - val, y - val, z - val);
}

CVector3d CVector3d::operator*(double val)
{
    return CVector3d(x * val, y * val, z * val);
}

CVector3d CVector3d::operator/(double val)
{
    return CVector3d(x / val, y / val, z / val);
}

CVector3d CVector3d::normalized()
{
    CVector3d ret;
    double mag = magnitude();

    ret.x = x/mag;
    ret.y = y/mag;
    ret.z = z/mag;

    return ret;
}

CVector3d CVector3d::cross(const CVector3d& rhs) const
{
    return CVector3d(y * rhs.z - z * rhs.y, z * rhs.x - x * rhs.z, x * rhs.y - y * rhs.x);
}

double CVector3d::dot(const CVector3d& rhs) const
{
    return (x * rhs.x) + (y * rhs.y) + (z * rhs.z);
}

double CVector3d::magnitude() const
{
    return sqrt(x*x + y*y + z*z);
}

CVector3f CVector3d::asVector3f()
{
    return CVector3f((float)x, (float)y, (float)z);
}
