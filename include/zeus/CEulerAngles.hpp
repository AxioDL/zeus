#ifndef CEULERANGES_HPP
#define CEULERANGES_HPP

#include "zeus/CVector3f.hpp"

namespace zeus
{
class CQuaternion;

class CEulerAngles : public CVector3f
{
public:
    CEulerAngles(const CQuaternion& quat);
};

}

#endif // CEULERANGES_HPP
