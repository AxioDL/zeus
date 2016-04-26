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

    CVector3f m_min;
    CVector3f m_max;

    // set default AABox to insane inverse min/max to allow for accumulation
    inline CAABox()
        : m_min(1e16f), m_max(-1e16f)
    {}
    
    CAABox(const CVector3f& min, const CVector3f& max)
        : m_min(min),
          m_max(max)
    {
    }

    CAABox(float minX, float minY, float minZ,
                 float maxX, float maxY, float maxZ)
        : m_min(minX, minY, minZ),
          m_max(maxX, maxY, maxZ)
    {
    }
#if ZE_ATHENA_TYPES
    inline void readBoundingBoxBig(athena::io::IStreamReader& in)
    {
        m_min.readBig(in);
        m_max.readBig(in);
    }
#endif

    float distanceFromPointSquared(const CVector3f& other) const
    {
        float dist = 0;
        for (int i = 0; i < 3; i++)
        {
            if (other[i] < m_min[i])
            {
                const float tmp = (m_min[i] - other[i]);
                dist += tmp * tmp;
            }
            else if (other[i] > m_max[i])
            {
                const float tmp = (other[i] - m_max[i]);
                dist += tmp * tmp;
            }
        }

        return dist;
    }

    float distanceFromPoint(const CVector3f &other) const
    {
        return sqrtF(distanceFromPointSquared(other));
    }
    
    inline bool intersects(const CAABox& other) const
    {
        bool x1 = (m_max[0] < other.m_min[0]);
        bool x2 = (m_min[0] > other.m_max[0]);
        bool y1 = (m_max[1] < other.m_min[1]);
        bool y2 = (m_min[1] > other.m_max[1]);
        bool z1 = (m_max[2] < other.m_min[2]);
        bool z2 = (m_min[2] > other.m_max[2]);
        return x1 && x2 && y1 && y2 && z1 && z2;
    }
    bool intersects(const CSphere& other) const
    {
        return distanceFromPointSquared(other.position) <= other.radius * other.radius;
    }

    inline bool inside(const CAABox& other) const
    {
        bool x = m_min[0] >= other.m_min[0] && m_max[0] <= other.m_max[0];
        bool y = m_min[1] >= other.m_min[1] && m_max[1] <= other.m_max[1];
        bool z = m_min[2] >= other.m_min[2] && m_max[2] <= other.m_max[2];
        return x && y && z;
    }

    inline bool insidePlane(const CPlane& plane) const
    {
        CVector3f vmin, vmax;
        /* X axis */
        if (plane.a >= 0) {
            vmin[0] = m_min[0];
            vmax[0] = m_max[0];
        } else {
            vmin[0] = m_max[0];
            vmax[0] = m_min[0];
        }
        /* Y axis */
        if (plane.b >= 0) {
            vmin[1] = m_min[1];
            vmax[1] = m_max[1];
        } else {
            vmin[1] = m_max[1];
            vmax[1] = m_min[1];
        }
        /* Z axis */
        if (plane.c >= 0) {
            vmin[2] = m_min[2];
            vmax[2] = m_max[2];
        } else {
            vmin[2] = m_max[2];
            vmax[2] = m_min[2];
        }
        float dadot = plane.vec.dot(vmax);
        if (dadot + plane.d < 0)
            return false;
        return true;
    }

    CVector3f center() const {return (m_min + m_max) * 0.5f;}

    CVector3f volume() const {return (m_max - m_min) * 0.5f;}

    inline CLineSeg getEdge(EBoxEdgeId id)
    {
        switch (id)
        {
            case EBoxEdgeId::UnknownEdge0:
                return CLineSeg({m_min.x, m_min.y, m_min.z},
                                {m_min.x, m_min.y, m_max.z});
            case EBoxEdgeId::UnknownEdge1:
                return CLineSeg({m_max.x, m_min.y, m_min.z},
                                {m_min.x, m_min.y, m_min.z});
            case EBoxEdgeId::UnknownEdge2:
                return CLineSeg({m_max.x, m_min.y, m_max.z},
                                {m_max.x, m_min.y, m_max.z});
            case EBoxEdgeId::UnknownEdge3:
                return CLineSeg({m_min.x, m_min.y, m_max.z},
                                {m_max.x, m_min.y, m_max.z});
            case EBoxEdgeId::UnknownEdge4:
                return CLineSeg({m_max.x, m_max.y, m_min.z},
                                {m_max.x, m_max.y, m_max.z});
            case EBoxEdgeId::UnknownEdge5:
                return CLineSeg({m_min.x, m_max.y, m_min.z},
                                {m_max.x, m_max.y, m_min.z});
            case EBoxEdgeId::UnknownEdge6:
                return CLineSeg({m_min.x, m_max.y, m_max.z},
                                {m_min.x, m_max.y, m_min.z});
            case EBoxEdgeId::UnknownEdge7:
                return CLineSeg({m_max.x, m_max.y, m_max.z},
                                {m_min.x, m_max.y, m_max.z});
            case EBoxEdgeId::UnknownEdge8:
                return CLineSeg({m_min.x, m_max.y, m_max.z},
                                {m_min.x, m_min.y, m_max.z});
            case EBoxEdgeId::UnknownEdge9:
                return CLineSeg({m_min.x, m_max.y, m_min.z},
                                {m_min.x, m_min.y, m_min.z});
            case EBoxEdgeId::UnknownEdge10:
                return CLineSeg({m_max.x, m_max.y, m_min.z},
                                {m_max.x, m_min.y, m_min.z});
            case EBoxEdgeId::UnknownEdge11:
                return CLineSeg({m_max.x, m_max.y, m_max.z},
                                {m_max.x, m_min.y, m_max.z});
            default:
                return CLineSeg({m_min.x, m_min.y, m_min.z},
                                {m_min.x, m_min.y, m_max.z});
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
        if (m_min.x > point.x)
            m_min.x = point.x;
        if (m_min.y > point.y)
            m_min.y = point.y;
        if (m_min.z > point.z)
            m_min.z = point.z;
        if (m_max.x < point.x)
            m_max.x = point.x;
        if (m_max.y < point.y)
            m_max.y = point.y;
        if (m_max.z < point.z)
            m_max.z = point.z;
    }

    inline void accumulateBounds(const CAABox& other)
    {
        accumulateBounds(other.m_min);
        accumulateBounds(other.m_max);
    }

    inline bool pointInside(const CVector3f& other) const
    {
        return (m_min.x <= other.x && other.x <= m_max.z &&
                m_min.y <= other.y && other.y <= m_max.z &&
                m_min.z <= other.z && other.z <= m_max.z);
    }

    inline CVector3f closestPointAlongVector(const CVector3f& other) const
    {
        CVector3f center = this->center();
        return {(other.x < center.x ? m_min.x : m_max.x),
                (other.y < center.y ? m_min.y : m_max.y),
                (other.z < center.z ? m_min.z : m_max.z)};
    }

    inline CVector3f furthestPointAlongVector(const CVector3f& other) const
    {
        CVector3f center = this->center();
        return {(other.x < center.x ? m_max.x : m_min.x),
                (other.y < center.y ? m_max.y : m_min.y),
                (other.z < center.z ? m_max.z : m_min.z)};
    }

    inline CVector3f getPoint(const int point) const
    {
        int zOff = point & 4;
        int yOff = (point * 2) & 4;
        int xOff = (point * 4) & 4;
        float z = ((float*)(&m_min.x) + zOff)[2];
        float y = ((float*)(&m_min.x) + yOff)[1];
        float x = ((float*)(&m_min.x) + xOff)[0];
        return CVector3f(x, y, z);
    }

    inline CVector3f clampToBox(const CVector3f& vec)
    {
        CVector3f ret = vec;
        clamp(m_min.x, ret.x, m_max.x);
        clamp(m_min.y, ret.y, m_max.y);
        clamp(m_min.z, ret.z, m_max.z);
        return ret;
    }

    inline void splitX(CAABox& posX, CAABox& negX) const
    {
        float midX = (m_max.x - m_min.x) * .5 + m_min.x;
        posX.m_max = m_max;
        posX.m_min = m_min;
        posX.m_min.x = midX;
        negX.m_max = m_max;
        negX.m_max.x = midX;
        negX.m_min = m_min;
    }
    
    inline void splitY(CAABox& posY, CAABox& negY) const
    {
        float midY = (m_max.y - m_min.y) * .5 + m_min.y;
        posY.m_max = m_max;
        posY.m_min = m_min;
        posY.m_min.y = midY;
        negY.m_max = m_max;
        negY.m_max.y = midY;
        negY.m_min = m_min;
    }
    
    inline void splitZ(CAABox& posZ, CAABox& negZ) const
    {
        float midZ = (m_max.z - m_min.z) * .5 + m_min.z;
        posZ.m_max = m_max;
        posZ.m_min = m_min;
        posZ.m_min.z = midZ;
        negZ.m_max = m_max;
        negZ.m_max.z = midZ;
        negZ.m_min = m_min;
    }


    inline bool invalid() {return (m_max.x < m_min.x || m_max.y < m_min.y || m_max.z < m_min.z);}
};

inline bool operator ==(const CAABox& left, const CAABox& right) {return (left.m_min == right.m_min && left.m_max == right.m_max);}
inline bool operator !=(const CAABox& left, const CAABox& right) {return (left.m_min != right.m_min || left.m_max != right.m_max);}
}

#endif // CAABOX_HPP
