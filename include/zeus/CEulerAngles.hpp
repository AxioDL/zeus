#ifndef CEULERANGES_HPP
#define CEULERANGES_HPP

#include "zeus/CVector3f.hpp"

namespace zeus
{
class CQuaternion;

class CEulerAngles : public CVector3f
{
public:
    CEulerAngles(float x, float y, float z) { assign(x, y, z); }
    CEulerAngles(const CQuaternion& quat);
};

}

#endif // CEULERANGES_HPP
