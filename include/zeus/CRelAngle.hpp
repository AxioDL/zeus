#ifndef CRELANGLE_HPP
#define CRELANGLE_HPP

#include "zeus/CVector3f.hpp"
#include "zeus/Math.hpp"

namespace zeus
{
/**
 * @brief The CRelAngle class represents relative angles in radians
 */
class alignas(16) CRelAngle : public CVector3f
{
public:
    /**
     * @brief CRelAngle
     * @param angles In degrees
     */
    CRelAngle(const CVector3f& angles)
    {
        x = degToRad(angles.x);
        y = degToRad(angles.y);
        z = degToRad(angles.z);
    }

    CRelAngle(float x, float y, float z)
        : CRelAngle(CVector3f{x, y, z})
    {
    }
};
}

#endif // CRELANGLE_HPP
