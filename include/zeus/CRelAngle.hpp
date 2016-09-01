#ifndef CRELANGLE_HPP
#define CRELANGLE_HPP

#include "zeus/CVector3f.hpp"
#include "zeus/Math.hpp"

namespace zeus
{
/**
 * @brief The CRelAngle class represents relative angle in radians
 */
struct CRelAngle
{
    float angle = 0.f;

    CRelAngle() = default;
    CRelAngle(float angle) : angle(angle) {}
    float asDegrees() const { return radToDeg(angle); }
    float asRadians() const { return angle; }
    float arcCosine() const { return std::acos(angle); }

    static CRelAngle FromDegrees(float angle)
    {
        CRelAngle ret;
        ret.angle = degToRad(angle);
        return ret;
    }

    operator float() { return angle; }
    static CRelAngle FromRadians(float angle) { return CRelAngle(angle); }

    bool operator <(const CRelAngle& other) const { return angle < other.angle; }
    CRelAngle& operator +=(const CRelAngle& other) { angle += other.angle; return *this; }
    CRelAngle& operator +=(float r) { angle += r; return *this; }
    CRelAngle& operator *=(const CRelAngle& other) { angle *= other.angle; return *this; }
    CRelAngle& operator *=(float r) { angle *= r; return *this;}
};
}

#endif // CRELANGLE_HPP
