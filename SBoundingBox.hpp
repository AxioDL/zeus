#ifndef BOUNDINGBOX_HPP
#define BOUNDINGBOX_HPP

#include "CVector3f.hpp"
#include <Athena/IStreamReader.hpp>

struct SBoundingBox
{
    CVector3f m_min;
    CVector3f m_max;
    
    inline SBoundingBox() {}
    
    SBoundingBox(const CVector3f& min, const CVector3f& max)
        : m_min(min),
          m_max(max)
    {
    }

    SBoundingBox(const float& minX,const float& minY,const float& minZ,
                 const float& maxX,const float& maxY,const float& maxZ)
        : m_min(minX, minY, minZ),
          m_max(maxX, maxY, maxZ)
    {
    }
    
    inline void readBoundingBox(Athena::io::IStreamReader& in)
    {
        m_min[0] = in.readFloat();
        m_min[1] = in.readFloat();
        m_min[2] = in.readFloat();
        m_max[0] = in.readFloat();
        m_max[1] = in.readFloat();
        m_max[2] = in.readFloat();
    }
    SBoundingBox(Athena::io::IStreamReader& in) {readBoundingBox(in);}
    
    inline bool intersects(SBoundingBox& other) const
    {
        if (m_max[0] < other.m_min[0]) return false;
        if (m_min[0] > other.m_max[0]) return false;
        if (m_max[1] < other.m_min[1]) return false;
        if (m_min[1] > other.m_max[1]) return false;
        if (m_max[2] < other.m_min[2]) return false;
        if (m_min[2] > other.m_max[2]) return false;
        return true;
    }
    
    inline void splitX(SBoundingBox& posX, SBoundingBox& negX) const
    {
        float midX = (m_max.x - m_min.x) / 2.0 + m_min.x;
        posX.m_max = m_max;
        posX.m_min = m_min;
        posX.m_min.x = midX;
        negX.m_max = m_max;
        negX.m_max.x = midX;
        negX.m_min = m_min;
    }
    
    inline void splitY(SBoundingBox& posY, SBoundingBox& negY) const
    {
        float midY = (m_max.y - m_min.y) / 2.0 + m_min.y;
        posY.m_max = m_max;
        posY.m_min = m_min;
        posY.m_min.y = midY;
        negY.m_max = m_max;
        negY.m_max.y = midY;
        negY.m_min = m_min;
    }
    
    inline void splitZ(SBoundingBox& posZ, SBoundingBox& negZ) const
    {
        float midZ = (m_max.z - m_min.z) / 2.0 + m_min.z;
        posZ.m_max = m_max;
        posZ.m_min = m_min;
        posZ.m_min.z = midZ;
        negZ.m_max = m_max;
        negZ.m_max.z = midZ;
        negZ.m_min = m_min;
    }
};

inline bool operator ==(const SBoundingBox& left, const SBoundingBox& right)
{
    return (left.m_min == right.m_min && left.m_max == right.m_max);
}
inline bool operator !=(const SBoundingBox& left, const SBoundingBox& right)
{
    return (left.m_min != right.m_min || left.m_max != right.m_max);

}

#endif // BOUNDINGBOX_HPP
