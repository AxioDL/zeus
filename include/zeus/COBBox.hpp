#ifndef COBBOX_HPP
#define COBBOX_HPP

#include "zeus/CTransform.hpp"
#include "zeus/CVector3f.hpp"
#include "zeus/CAABox.hpp"
#include "zeus/CMRay.hpp"

namespace zeus
{
class alignas(16) COBBox
{
public:
    ZE_DECLARE_ALIGNED_ALLOCATOR();
#if ZE_ATHENA_TYPES
    void readBig(athena::io::IStreamReader& in)
    {
        transform.read34RowMajor(in);
        extents.readBig(in);
    }
    static COBBox ReadBig(athena::io::IStreamReader& in)
    {
        COBBox out;
        out.readBig(in);
        return out;
    }

#endif

    CTransform transform;
    CVector3f extents;

    COBBox() {}

    COBBox(const CAABox& aabb) : extents(aabb.extents()) { transform.origin = aabb.center(); }

    COBBox(const CTransform& xf, const CVector3f& extents) : transform(xf), extents(extents) {}

    CAABox calculateAABox(const CTransform& transform = CTransform()) const
    {
        CAABox ret = CAABox::skInvertedBox;

        CTransform trans = transform * transform;
        static const CVector3f basis[8] = {{1.0, 1.0, 1.0},    {1.0, 1.0, -1.0},  {1.0, -1.0, 1.0},  {1.0, -1.0, -1.0},
                                           {-1.0, -1.0, -1.0}, {-1.0, -1.0, 1.0}, {-1.0, 1.0, -1.0}, {-1.0, 1.0, 1.0}};
        CVector3f p = extents * basis[0];

        ret.accumulateBounds(trans * p);
        p = extents * basis[1];
        ret.accumulateBounds(trans * p);
        p = extents * basis[2];
        ret.accumulateBounds(trans * p);
        p = extents * basis[3];
        ret.accumulateBounds(trans * p);
        p = extents * basis[4];
        ret.accumulateBounds(trans * p);
        p = extents * basis[5];
        ret.accumulateBounds(trans * p);
        p = extents * basis[6];
        ret.accumulateBounds(trans * p);
        p = extents * basis[7];
        ret.accumulateBounds(trans * p);

        return ret;
    }

    static COBBox FromAABox(const CAABox& box, const CTransform& xf)
    {
        const CVector3f extents = xf.toMatrix4f().transposed().vec[1].toVec3f() - box.center();
        const CTransform newXf = CTransform::Translate(box.center()) * xf;
        return COBBox(newXf, extents);
    }

    bool OBBIntersectsBox(const COBBox& other);
};
}

#endif
