#ifndef CAABOX_HPP
#define CAABOX_HPP

#include "zeus/CVector3f.hpp"
#include "zeus/CTransform.hpp"
#include "zeus/CPlane.hpp"
#include "zeus/CLineSeg.hpp"
#include "zeus/CSphere.hpp"
#include "zeus/Math.hpp"
#if ZE_ATHENA_TYPES
#include <athena/IStreamReader.hpp>
#endif

namespace zeus
{
class alignas(16) CAABox
{
public:
    ZE_DECLARE_ALIGNED_ALLOCATOR();

    enum class EBoxEdgeId
    {
        UnknownEdge0,
        UnknownEdge1,
        UnknownEdge2,
        UnknownEdge3,
        UnknownEdge4,
        UnknownEdge5,
        UnknownEdge6,
        UnknownEdge7,
        UnknownEdge8,
        UnknownEdge9,
        UnknownEdge10,
        UnknownEdge11
    };

    enum class EBoxFaceID
    {
    };

    static const CAABox skInvertedBox;
    static const CAABox skNullBox;

    CVector3f min;
    CVector3f max;

    // set default AABox to insane inverse min/max to allow for accumulation
    inline CAABox() : min(1e16f), max(-1e16f) {}

    CAABox(const CVector3f& min, const CVector3f& max) : min(min), max(max) {}

    CAABox(float minX, float minY, float minZ, float maxX, float maxY, float maxZ)
    : min(minX, minY, minZ), max(maxX, maxY, maxZ)
    {
    }
#if ZE_ATHENA_TYPES
    inline void readBoundingBoxBig(athena::io::IStreamReader& in)
    {
        min.readBig(in);
        max.readBig(in);
    }
    static inline CAABox ReadBoundingBoxBig(athena::io::IStreamReader& in)
    {
        CAABox ret;
        ret.readBoundingBoxBig(in);
        return ret;
    }

#endif

    float distanceFromPointSquared(const CVector3f& other) const
    {
        float dist = 0;
        for (int i = 0; i < 3; i++)
        {
            if (other[i] < min[i])
            {
                const float tmp = (min[i] - other[i]);
                dist += tmp * tmp;
            }
            else if (other[i] > max[i])
            {
                const float tmp = (other[i] - max[i]);
                dist += tmp * tmp;
            }
        }

        return dist;
    }

    float distanceFromPoint(const CVector3f& other) const { return sqrtF(distanceFromPointSquared(other)); }

    inline bool intersects(const CAABox& other) const
    {
        bool x1 = (max[0] > other.min[0]);
        bool x2 = (min[0] < other.max[0]);
        bool y1 = (max[1] > other.min[1]);
        bool y2 = (min[1] < other.max[1]);
        bool z1 = (max[2] > other.min[2]);
        bool z2 = (min[2] < other.max[2]);
        return x1 && x2 && y1 && y2 && z1 && z2;
    }

    bool intersects(const CSphere& other) const
    {
        return distanceFromPointSquared(other.position) <= other.radius * other.radius;
    }

    float intersectionRadius(const CSphere& other) const
    {
        float dist = distanceFromPoint(other.position);
        return (dist < other.radius) ? dist : -1.f;
    }

    inline CAABox booleanIntersection(const CAABox& other) const
    {
        CVector3f minVec = CVector3f::skZero;
        CVector3f maxVec = CVector3f::skZero;

        for (int i = 0; i < 3; ++i)
        {
            if (min[i] <= other.min[i] && max[i] >= other.max[i])
            {
                minVec[i] = other.min[i];
                maxVec[i] = other.max[i];
            }
            else if (other.min[i] <= min[i] && other.max[i] >= max[i])
            {
                minVec[i] = min[i];
                maxVec[i] = max[i];
            }
            else if (other.min[i] <= min[i] && other.max[i] >= min[i])
            {
                minVec[i] = min[i];
                maxVec[i] = other.max[i];
            }
            else if (other.min[i] <= max[i] && other.max[i] >= max[i])
            {
                minVec[i] = other.min[i];
                maxVec[i] = max[i];
            }
        }

        return {minVec, maxVec};
    }

    inline bool inside(const CAABox& other) const
    {
        bool x = min[0] >= other.min[0] && max[0] <= other.max[0];
        bool y = min[1] >= other.min[1] && max[1] <= other.max[1];
        bool z = min[2] >= other.min[2] && max[2] <= other.max[2];
        return x && y && z;
    }

    inline bool insidePlane(const CPlane& plane) const
    {
        CVector3f vmin, vmax;
        /* X axis */
        if (plane.a >= 0)
        {
            vmin[0] = min[0];
            vmax[0] = max[0];
        }
        else
        {
            vmin[0] = max[0];
            vmax[0] = min[0];
        }
        /* Y axis */
        if (plane.b >= 0)
        {
            vmin[1] = min[1];
            vmax[1] = max[1];
        }
        else
        {
            vmin[1] = max[1];
            vmax[1] = min[1];
        }
        /* Z axis */
        if (plane.c >= 0)
        {
            vmin[2] = min[2];
            vmax[2] = max[2];
        }
        else
        {
            vmin[2] = max[2];
            vmax[2] = min[2];
        }
        float dadot = plane.vec.dot(vmax);
        if (dadot + plane.d < 0)
            return false;
        return true;
    }

    CVector3f center() const { return (min + max) * 0.5f; }

    CVector3f extents() const { return (max - min) * 0.5f; }

    float volume() const { return (max.x - min.x) * (max.y - min.y) * (max.z - min.z); }

    inline CLineSeg getEdge(EBoxEdgeId id)
    {
        switch (id)
        {
        case EBoxEdgeId::UnknownEdge0:
            return CLineSeg({min.x, min.y, min.z}, {min.x, min.y, max.z});
        case EBoxEdgeId::UnknownEdge1:
            return CLineSeg({max.x, min.y, min.z}, {min.x, min.y, min.z});
        case EBoxEdgeId::UnknownEdge2:
            return CLineSeg({max.x, min.y, max.z}, {max.x, min.y, max.z});
        case EBoxEdgeId::UnknownEdge3:
            return CLineSeg({min.x, min.y, max.z}, {max.x, min.y, max.z});
        case EBoxEdgeId::UnknownEdge4:
            return CLineSeg({max.x, max.y, min.z}, {max.x, max.y, max.z});
        case EBoxEdgeId::UnknownEdge5:
            return CLineSeg({min.x, max.y, min.z}, {max.x, max.y, min.z});
        case EBoxEdgeId::UnknownEdge6:
            return CLineSeg({min.x, max.y, max.z}, {min.x, max.y, min.z});
        case EBoxEdgeId::UnknownEdge7:
            return CLineSeg({max.x, max.y, max.z}, {min.x, max.y, max.z});
        case EBoxEdgeId::UnknownEdge8:
            return CLineSeg({min.x, max.y, max.z}, {min.x, min.y, max.z});
        case EBoxEdgeId::UnknownEdge9:
            return CLineSeg({min.x, max.y, min.z}, {min.x, min.y, min.z});
        case EBoxEdgeId::UnknownEdge10:
            return CLineSeg({max.x, max.y, min.z}, {max.x, min.y, min.z});
        case EBoxEdgeId::UnknownEdge11:
            return CLineSeg({max.x, max.y, max.z}, {max.x, min.y, max.z});
        default:
            return CLineSeg({min.x, min.y, min.z}, {min.x, min.y, max.z});
        }
    }

    inline CAABox getTransformedAABox(const CTransform& xfrm) const
    {
        CAABox box;
        CVector3f point = xfrm * getPoint(0);
        box.accumulateBounds(point);
        point = xfrm * getPoint(1);
        box.accumulateBounds(point);
        point = xfrm * getPoint(2);
        box.accumulateBounds(point);
        point = xfrm * getPoint(3);
        box.accumulateBounds(point);
        point = xfrm * getPoint(4);
        box.accumulateBounds(point);
        point = xfrm * getPoint(5);
        box.accumulateBounds(point);
        point = xfrm * getPoint(6);
        box.accumulateBounds(point);
        point = xfrm * getPoint(7);
        box.accumulateBounds(point);
        return box;
    }

    inline void accumulateBounds(const CVector3f& point)
    {
        if (min.x > point.x)
            min.x = point.x;
        if (min.y > point.y)
            min.y = point.y;
        if (min.z > point.z)
            min.z = point.z;
        if (max.x < point.x)
            max.x = point.x;
        if (max.y < point.y)
            max.y = point.y;
        if (max.z < point.z)
            max.z = point.z;
    }

    inline void accumulateBounds(const CAABox& other)
    {
        accumulateBounds(other.min);
        accumulateBounds(other.max);
    }

    inline bool pointInside(const CVector3f& other) const
    {
        return (min.x <= other.x && other.x <= max.z && min.y <= other.y && other.y <= max.z && min.z <= other.z &&
                other.z <= max.z);
    }

    inline CVector3f closestPointAlongVector(const CVector3f& other) const
    {
        CVector3f center = this->center();
        return {(other.x < center.x ? min.x : max.x), (other.y < center.y ? min.y : max.y),
                (other.z < center.z ? min.z : max.z)};
    }

    inline CVector3f furthestPointAlongVector(const CVector3f& other) const
    {
        CVector3f center = this->center();
        return {(other.x < center.x ? max.x : min.x), (other.y < center.y ? max.y : min.y),
                (other.z < center.z ? max.z : min.z)};
    }

    inline float distanceBetween(const CAABox& other)
    {
        int intersects = 0;
        if (max.x >= other.min.x && min.x <= other.max.x)
            intersects |= 0x1;
        if (max.y >= other.min.y && min.y <= other.max.y)
            intersects |= 0x2;
        if (max.z >= other.min.z && min.z <= other.max.z)
            intersects |= 0x4;

        float minX, maxX;
        if (max.x < other.min.x)
        {
            minX = max.x;
            maxX = other.min.x;
        }
        else
        {
            minX = min.x;
            maxX = other.max.x;
        }

        float minY, maxY;
        if (max.y < other.min.y)
        {
            minY = max.y;
            maxY = other.min.y;
        }
        else
        {
            minY = min.y;
            maxY = other.max.y;
        }

        float minZ, maxZ;
        if (max.z < other.min.z)
        {
            minZ = max.z;
            maxZ = other.min.z;
        }
        else
        {
            minZ = min.z;
            maxZ = other.max.z;
        }

        switch (intersects)
        {
        case 0:
            return zeus::CVector3f(maxX - minX, maxY - minY, maxZ - minZ).magnitude();
        case 1:
            return zeus::CVector2f(maxY - minY, maxZ - minZ).magnitude();
        case 2:
            return zeus::CVector2f(maxX - minX, maxZ - minZ).magnitude();
        case 3:
            return std::fabs(maxZ - minZ);
        case 4:
            return zeus::CVector2f(maxX - minX, maxY - minY).magnitude();
        case 5:
            return std::fabs(maxY - minY);
        case 6:
            return std::fabs(maxX - minX);
        case 7:
        default:
            return 0.f;
        }
    }

    inline CVector3f getPoint(const int point) const
    {
        int zOff = point & 4;
        int yOff = (point * 2) & 4;
        int xOff = (point * 4) & 4;
        float z = ((float*)(&min.x) + zOff)[2];
        float y = ((float*)(&min.x) + yOff)[1];
        float x = ((float*)(&min.x) + xOff)[0];
        return CVector3f(x, y, z);
    }

    inline CVector3f clampToBox(const CVector3f& vec)
    {
        CVector3f ret = vec;
        clamp(min.x, ret.x, max.x);
        clamp(min.y, ret.y, max.y);
        clamp(min.z, ret.z, max.z);
        return ret;
    }

    inline void splitX(CAABox& negX, CAABox& posX) const
    {
        float midX = (max.x - min.x) * .5 + min.x;
        posX.max = max;
        posX.min = min;
        posX.min.x = midX;
        negX.max = max;
        negX.max.x = midX;
        negX.min = min;
    }

    inline void splitY(CAABox& negY, CAABox& posY) const
    {
        float midY = (max.y - min.y) * .5 + min.y;
        posY.max = max;
        posY.min = min;
        posY.min.y = midY;
        negY.max = max;
        negY.max.y = midY;
        negY.min = min;
    }

    inline void splitZ(CAABox& negZ, CAABox& posZ) const
    {
        float midZ = (max.z - min.z) * .5 + min.z;
        posZ.max = max;
        posZ.min = min;
        posZ.min.z = midZ;
        negZ.max = max;
        negZ.max.z = midZ;
        negZ.min = min;
    }

    inline bool invalid() { return (max.x < min.x || max.y < min.y || max.z < min.z); }

    inline float operator[](size_t idx) const
    {
        if (idx < 3)
            return min[idx];
        else
            return max[idx-3];
    }
};

inline bool operator==(const CAABox& left, const CAABox& right)
{
    return (left.min == right.min && left.max == right.max);
}
inline bool operator!=(const CAABox& left, const CAABox& right)
{
    return (left.min != right.min || left.max != right.max);
}
}

#endif // CAABOX_HPP
