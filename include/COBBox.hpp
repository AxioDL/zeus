#ifndef COBBOX_HPP
#define COBBOX_HPP

#include "CTransform.hpp"
#include "CVector3f.hpp"
#include "CAABox.hpp"

namespace Zeus
{
class alignas(16) COBBox
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
        CVector3f p = m_extents * basis[0];

        ret.accumulateBounds(trans * p);
        p = m_extents * basis[1];
        ret.accumulateBounds(trans * p);
        p = m_extents * basis[2];
        ret.accumulateBounds(trans * p);
        p = m_extents * basis[3];
        ret.accumulateBounds(trans * p);
        p = m_extents * basis[4];
        ret.accumulateBounds(trans * p);
        p = m_extents * basis[5];
        ret.accumulateBounds(trans * p);
        p = m_extents * basis[6];
        ret.accumulateBounds(trans * p);
        p = m_extents * basis[7];
        ret.accumulateBounds(trans * p);

        return ret;
    }
};
}

#endif
