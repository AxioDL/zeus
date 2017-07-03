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
        const CVector3f extents = box.max - box.center();
        const CTransform newXf = CTransform::Translate(box.center()) * xf;
        return COBBox(newXf, extents);
    }

    bool OBBIntersectsBox(const COBBox& other) const
    {
        CVector3f v = other.transform.origin - transform.origin;
        CVector3f T = CVector3f(v.dot(transform.basis[0]), v.dot(transform.basis[1]),
                v.dot(transform.basis[2]));

        CMatrix3f R;

        float ra, rb, t;

        for (int i = 0; i < 3; ++i)
            for (int k = 0; k < 3; ++k)
                R[i][k] = transform.basis[i].dot(other.transform.basis[k]);

        for (int i = 0; i < 3; ++i)
        {
            ra = extents[i];
            rb = (other.extents[0] * fabs(R[i][0])) +
                 (other.extents[1] * fabs(R[i][1])) +
                 (other.extents[2] * fabs(R[i][2]));
            t = fabs(T[i]);

            if (t > (ra + rb))
                return false;
        }

        for (int k = 0; k < 3; ++k)
        {
            ra = (extents[0] * fabs(R[0][k])) +
                 (extents[1] * fabs(R[1][k])) +
                 (extents[2] * fabs(R[2][k]));
            rb = other.extents[k];

            t = fabs(T[0] * R[0][k]) + (T[1] * R[1][k]) + (T[2] * R[2][k]);

            if (t > (ra + rb))
                return false;
        }

        /* A0 x B0 */
        ra = (extents[1] * fabs(R[2][0])) + (extents[2]*fabs(R[1][0]));
        rb = (other.extents[1] * fabs(R[0][2])) + (other.extents[2] * fabs(R[0][1]));
        t = fabs((T[2] * R[1][0]) - (T[1]*R[2][0]));
        if (t > (ra + rb))
            return false;

        /* A0 x B1 */
        ra = (extents[1] * fabs(R[2][1])) + (extents[2]*fabs(R[1][1]));
        rb = (other.extents[0] * fabs(R[0][2])) + (other.extents[2] * fabs(R[0][0]));
        t = fabs((T[2] * R[1][1]) - (T[1]*R[2][1]));
        if (t > (ra + rb))
            return false;

        /* A0 x B2 */
        ra = (extents[1] * fabs(R[2][2])) + (extents[2]*fabs(R[1][2]));
        rb = (other.extents[0] * fabs(R[0][1])) + (other.extents[1] * fabs(R[0][0]));
        t = fabs((T[2] * R[1][2]) - (T[1]*R[2][2]));
        if (t > (ra + rb))
            return false;

        /* A1 x B0 */
        ra = (extents[0] * fabs(R[2][0])) + (extents[2]*fabs(R[0][0]));
        rb = (other.extents[1] * fabs(R[1][2])) + (other.extents[2] * fabs(R[1][1]));
        t = fabs((T[0] * R[2][0]) - (T[2]*R[0][0]));
        if (t > (ra + rb))
            return false;

        /* A1 x B1 */
        ra = (extents[0] * fabs(R[2][1])) + (extents[2]*fabs(R[0][1]));
        rb = (other.extents[0] * fabs(R[1][2])) + (other.extents[2] * fabs(R[1][0]));
        t = fabs((T[0] * R[2][1]) - (T[2]*R[0][1]));
        if (t > (ra + rb))
            return false;

        /* A1 x B2 */
        ra = (extents[0] * fabs(R[2][2])) + (extents[2]*fabs(R[0][2]));
        rb = (other.extents[0] * fabs(R[1][1])) + (other.extents[1] * fabs(R[1][0]));
        t = fabs((T[0] * R[2][2]) - (T[2]*R[0][2]));
        if (t > (ra + rb))
            return false;

        /* A2 x B0 */
        ra = (extents[0] * fabs(R[1][0])) + (extents[1]*fabs(R[0][0]));
        rb = (other.extents[1] * fabs(R[2][2])) + (other.extents[2] * fabs(R[2][1]));
        t = fabs((T[1] * R[0][0]) - (T[0]*R[1][0]));
        if (t > (ra + rb))
            return false;

        /* A2 x B1 */
        ra = (extents[0] * fabs(R[1][1])) + (extents[1]*fabs(R[0][1]));
        rb = (other.extents[0] * fabs(R[2][2])) + (other.extents[2] * fabs(R[2][0]));
        t = fabs((T[1] * R[0][1]) - (T[0]*R[1][1]));
        if (t > (ra + rb))
            return false;

        /* A2 x B2 */
        ra = (extents[0] * fabs(R[1][2])) + (extents[1]*fabs(R[0][2]));
        rb = (other.extents[0] * fabs(R[2][1])) + (other.extents[1] * fabs(R[2][0]));
        t = fabs((T[1] * R[0][2]) - (T[0]*R[1][2]));
        if (t > (ra + rb))
            return false;

        return true;
    }

    bool AABoxIntersectsBox(const CAABox& other)
    {
        return OBBIntersectsBox(FromAABox(other, CTransform::Identity()));
    }
};
}

#endif
