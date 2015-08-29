#ifndef COBBOX_HPP
#define COBBOX_HPP

#include "CTransform.hpp"
#include "CVector3f.hpp"
#include "CAABox.hpp"

class ZE_ALIGN(16) COBBox
{
public:
    ZE_DECLARE_ALIGNED_ALLOCATOR();
    CTransform m_transform;
    CVector3f  m_extents;

    COBBox()
    {}

    COBBox(const CAABox& aabb)
        : m_extents(aabb.volume())
    {
        m_transform.m_origin = aabb.center();
    }

    CAABox calculateAABox(const CTransform& transform = CTransform())
    {
        CAABox ret = CAABox::skInvertedBox;

        CTransform trans = transform * m_transform;
        static const CVector3f basis[8] ={
            { 1.0,  1.0,  1.0},
            { 1.0,  1.0, -1.0},
            { 1.0, -1.0,  1.0},
            { 1.0, -1.0, -1.0},
            {-1.0, -1.0, -1.0},
            {-1.0, -1.0,  1.0},
            {-1.0,  1.0, -1.0},
            {-1.0,  1.0,  1.0}
        };

        for (int i = 0; i < 8; i+=2)
        {
            CVector3f p = (m_extents * basis[i]);
            ret.accumulateBounds(trans * p);
            p = m_extents * basis[i + 1];
            ret.accumulateBounds(trans * p);
        }

        return ret;
    }
};

#endif
