#include "zeus/CAABox.hpp"
#include "zeus/CVector3f.hpp"

namespace zeus {
const CAABox CAABox::skInvertedBox = CAABox();
const CAABox CAABox::skNullBox = CAABox(CVector3f::skZero, CVector3f::skZero);
} // namespace zeus
