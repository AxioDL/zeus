#ifndef COBBOX_HPP
#define COBBOX_HPP

#include "zeus/CTransform.hpp"
#include "zeus/CVector3f.hpp"
#include "zeus/CAABox.hpp"

namespace zeus
{
class alignas(16) COBBox
{
public:
    ZE_DECLARE_ALIGNED_ALLOCATOR();
#if ZE_ATHENA_TYPES
    void readBig(athena::io::IStreamReader& in)
    {
        m_transform.read34RowMajor(in);
        m_extents.readBig(in);
    }
    static COBBox ReadBig(athena::io::IStreamReader& in)
    {
        COBBox out;
        out.readBig(in);
        return out;
    }

#endif

    CTransform m_transform;
    CVector3f  m_extents;

    COBBox()
    {}

    COBBox(const CAABox& aabb)
        : m_extents(aabb.volume())
    {
        m_transform.m_origin = aabb.center();
    }

    COBBox(const CTransform& xf, const CVector3f& point)
    {

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

    static COBBox FromAABox(const CAABox& box, const CTransform& xf)
    {
        const CVector3f extents = xf.toMatrix4f().transposed().vec[1].toVec3f() - box.center();
        const CTransform newXf = CTransform::Translate(box.center()) * xf;
        return COBBox(newXf, extents);
    }
};
}

#endif
