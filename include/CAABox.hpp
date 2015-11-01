#ifndef CAABOX_HPP
#define CAABOX_HPP

#include "CVector3f.hpp"
#include "CUnitVector.hpp"
#include "CTransform.hpp"
#include "CPlane.hpp"
#include "CLine.hpp"
#include "Math.hpp"
#if ZE_ATHENA_TYPES
#include <Athena/IStreamReader.hpp>
#endif

namespace Zeus
{
class alignas(16) CAABox
{
public:
    ZE_DECLARE_ALIGNED_ALLOCATOR();

    enum EBoxEdgeId
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

    enum EBoxFaceID
    {
    };

    static const CAABox skInvertedBox;

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
    CAABox(Athena::io::IStreamReader& in) {readBoundingBox(in);}
#endif
    
    inline void readBoundingBox(Athena::io::IStreamReader& in)
    {
        m_min[0] = in.readFloat();
        m_min[1] = in.readFloat();
        m_min[2] = in.readFloat();
        m_max[0] = in.readFloat();
        m_max[1] = in.readFloat();
        m_max[2] = in.readFloat();
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

    inline CLine getEdge(EBoxEdgeId id)
    {
        switch (id)
        {
            case UnknownEdge0:
                return CLine({m_min.x, m_min.y, m_min.z},
                             CUnitVector3f({m_min.x, m_min.y, m_max.z}));
            case UnknownEdge1:
                return CLine({m_max.x, m_min.y, m_min.z},
                             CUnitVector3f({m_min.x, m_min.y, m_min.z}));
            case UnknownEdge2:
                return CLine({m_max.x, m_min.y, m_max.z},
                             CUnitVector3f({m_max.x, m_min.y, m_max.z}));
            case UnknownEdge3:
                return CLine({m_min.x, m_min.y, m_max.z},
                             CUnitVector3f({m_max.x, m_min.y, m_max.z}));
            case UnknownEdge4:
                return CLine({m_max.x, m_max.y, m_min.z},
                             CUnitVector3f({m_max.x, m_max.y, m_max.z}));
            case UnknownEdge5:
                return CLine({m_min.x, m_max.y, m_min.z},
                             CUnitVector3f({m_max.x, m_max.y, m_min.z}));
            case UnknownEdge6:
                return CLine({m_min.x, m_max.y, m_max.z},
                             CUnitVector3f({m_min.x, m_max.y, m_min.z}));
            case UnknownEdge7:
                return CLine({m_max.x, m_max.y, m_max.z},
                             CUnitVector3f({m_min.x, m_max.y, m_max.z}));
            case UnknownEdge8:
                return CLine({m_min.x, m_max.y, m_max.z},
                             CUnitVector3f({m_min.x, m_min.y, m_max.z}));
            case UnknownEdge9:
                return CLine({m_min.x, m_max.y, m_min.z},
                             CUnitVector3f({m_min.x, m_min.y, m_min.z}));
            case UnknownEdge10:
                return CLine({m_max.x, m_max.y, m_min.z},
                             CUnitVector3f({m_max.x, m_min.y, m_min.z}));
            case UnknownEdge11:
                return CLine({m_max.x, m_max.y, m_max.z},
                             CUnitVector3f({m_max.x, m_min.y, m_max.z}));
            default:
                return CLine({m_min.x, m_min.y, m_min.z},
                             CUnitVector3f({m_min.x, m_min.y, m_max.z}));
        }
    }

    inline CAABox getTransformedAABox(const CTransform& xfrm)
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

    inline bool pointInside(const CVector3f& other) const
    {
        return (m_min.x <= other.x && other.x <= m_max.z &&
                m_min.y <= other.y && other.y <= m_max.z &&
                m_min.z <= other.z && other.z <= m_max.z);
    }

    inline CVector3f closestPointAlongVector(const CVector3f& other)
    {
        CVector3f center = this->center();
        return {(other.x < center.x ? m_min.x : m_max.x),
                (other.y < center.y ? m_min.y : m_max.y),
                (other.z < center.z ? m_min.z : m_max.z)};
    }

    inline CVector3f furthestPointAlongVector(const CVector3f& other)
    {
        CVector3f center = this->center();
        return {(other.x < center.x ? m_max.x : m_min.x),
                (other.y < center.y ? m_max.y : m_min.y),
                (other.z < center.z ? m_max.z : m_min.z)};
    }

    inline CVector3f getPoint(const int point)
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
        Math::clamp(m_min.x, ret.x, m_max.x);
        Math::clamp(m_min.y, ret.y, m_max.y);
        Math::clamp(m_min.z, ret.z, m_max.z);
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