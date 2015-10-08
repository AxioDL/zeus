#ifndef CRELANGLE_HPP
#define CRELANGLE_HPP

#include "CVector3f.hpp"
#include "Math.hpp"

namespace Zeus
{
/**
 * @brief The CRelAngle class represents relative angles in radians
 */
class ZE_ALIGN(16) CRelAngle : public CVector3f
{
public:
    /**
     * @brief CRelAngle
     * @param angles In degrees
     */
    CRelAngle(const CVector3f& angles)
    {
        x = Math::degToRad(angles.x);
        y = Math::degToRad(angles.y);
        z = Math::degToRad(angles.z);
    }

    CRelAngle(float x, float y, float z)
        : CRelAngle(CVector3f{x, y, z})
    {
    }
};
}

#endif // CRELANGLE_HPP
