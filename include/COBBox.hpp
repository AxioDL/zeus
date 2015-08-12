#ifndef COBBOX_HPP
#define COBBOX_HPP

#include "CTransform.hpp"
#include "CVector3f.hpp"
#include "CAABox.hpp"

class COBBox
{
public:
    CTransform m_transform;
    CVector3f  m_extents;

    COBBox()
    {}

    COBBox(const CAABox& aabb)
    { createFromAABox(aabb); }

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

        for (int i = 0; i < 8; i++)
        {
            CVector3f p = (m_extents * basis[i]);
            ret.accumulateBounds(trans * p);
        }

        return ret;
    }

    void createFromAABox(const CAABox& box)
    {
        m_extents = box.extents();
        m_transform.m_origin = box.center();
    }

    static inline COBBox fromAABox(const CAABox& box)
    {
        COBBox ret;
        ret.createFromAABox(box);
        return box;
    }
};

#endif
