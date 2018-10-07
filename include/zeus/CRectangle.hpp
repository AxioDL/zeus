#pragma once
#include "zeus/CVector2f.hpp"

namespace zeus
{
class CRectangle
{
public:
    CRectangle() {}
    CRectangle(float x, float y, float w, float h) : position(x, y), size(w, h) {}

    inline bool contains(const CVector2f& point) const
    {
        if (point.x < position.x || point.x > position.x + size.x)
            return false;
        if (point.y < position.y || point.y > position.y + size.y)
            return false;

        return true;
    }

    inline bool intersects(const CRectangle& rect) const
    {
        return !(position.x > rect.position.x + rect.size.x || rect.position.x > position.x + size.x ||
                 position.y > rect.position.y + rect.size.y || rect.position.y > position.y + size.y);
    }

    CVector2f position;
    CVector2f size;
};
}

