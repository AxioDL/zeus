#ifndef CAABOX_HPP
#define CAABOX_HPP

#include "CVector3f.hpp"
#include "CTransform.hpp"
#include <Athena/IStreamReader.hpp>

class ZE_ALIGN(16) CAABox
{
public:
    ZE_DECLARE_ALIGNED_ALLOCATOR();

    enum EBoxEdgeID
    {
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

    CAABox(Athena::io::IStreamReader& in) {readBoundingBox(in);}
    
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
        if (m_max[0] < other.m_min[0]) return false;
        if (m_min[0] > other.m_max[0]) return false;
        if (m_max[1] < other.m_min[1]) return false;
        if (m_min[1] > other.m_max[1]) return false;
        if (m_max[2] < other.m_min[2]) return false;
        if (m_min[2] > other.m_max[2]) return false;
        return true;
    }

    CVector3f center() const
    {
        return (m_min + m_max) * 0.5f;
    }
    CVector3f extents() const
    {
        return (m_max - m_min) * 0.5f;
    }

    inline CAABox getTransformedAABox(const CTransform& xfrm)
    {
        CAABox box;
        for (int i = 0; i < 8; i+=2)
        {
            CVector3f point = xfrm * getPoint(i);
            box.accumulateBounds(point);
            point = xfrm * getPoint(i+1);
            box.accumulateBounds(point);
        }

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
        return {(other.x >= -0.f) ? m_min.x : m_max.x,
                (other.y >= -0.f) ? m_min.y : m_max.y,
                (other.z >= -0.f) ? m_min.z : m_max.z};
    }

    inline CVector3f furthestPointAlongVector(const CVector3f& other)
    {
        return {(other.x >= -0.f) ? m_max.x : m_min.x,
                (other.y >= -0.f) ? m_max.y : m_min.y,
                (other.z >= -0.f) ? m_max.z : m_min.z};
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
        if (ret.x < m_min.x)
            ret.x = m_min.x;
        if (ret.y < m_min.y)
            ret.y = m_min.y;
        if (ret.z < m_min.z)
            ret.z = m_min.z;
        if (ret.x > m_max.x)
            ret.x = m_max.x;
        if (ret.y > m_max.y)
            ret.y = m_max.y;
        if (ret.z > m_max.z)
            ret.z = m_max.z;
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

    inline bool invalid()
    {
        return (m_max.x < m_min.x || m_max.y < m_min.y || m_max.z < m_min.z);
    }
};

inline bool operator ==(const CAABox& left, const CAABox& right)
{
    return (left.m_min == right.m_min && left.m_max == right.m_max);
}
inline bool operator !=(const CAABox& left, const CAABox& right)
{
    return (left.m_min != right.m_min || left.m_max != right.m_max);

}

#endif // CAABOX_HPP
