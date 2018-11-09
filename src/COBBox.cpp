#include "zeus/COBBox.hpp"

namespace zeus
{

CAABox COBBox::calculateAABox(const CTransform& transform) const
{
    CAABox ret = CAABox::skInvertedBox;

    CTransform trans = transform * transform;
    static const CVector3f basis[8] = {{1.f, 1.f, 1.f},    {1.f, 1.f, -1.f},  {1.f, -1.f, 1.f},  {1.f, -1.f, -1.f},
                                       {-1.f, -1.f, -1.f}, {-1.f, -1.f, 1.f}, {-1.f, 1.f, -1.f}, {-1.f, 1.f, 1.f}};
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

bool COBBox::OBBIntersectsBox(const COBBox& other) const
{
    CVector3f v = other.transform.origin - transform.origin;
    CVector3f T = CVector3f(v.dot(transform.basis[0]),
                            v.dot(transform.basis[1]),
                            v.dot(transform.basis[2]));

    CMatrix3f R;

    float ra, rb, t;

    for (int i = 0; i < 3; ++i)
        for (int k = 0; k < 3; ++k)
            R[i][k] = transform.basis[i].dot(other.transform.basis[k]);

    for (int i = 0; i < 3; ++i)
    {
        ra = extents[i];
        rb = (other.extents[0] * std::fabs(R[i][0])) +
             (other.extents[1] * std::fabs(R[i][1])) +
             (other.extents[2] * std::fabs(R[i][2]));
        t = std::fabs(T[i]);

        if (t > (ra + rb + FLT_EPSILON))
            return false;
    }

    for (int k = 0; k < 3; ++k)
    {
        ra = (extents[0] * std::fabs(R[0][k])) +
             (extents[1] * std::fabs(R[1][k])) +
             (extents[2] * std::fabs(R[2][k]));
        rb = other.extents[k];

        t = std::fabs(T[0] * R[0][k] + T[1] * R[1][k] + T[2] * R[2][k]);

        if (t > (ra + rb + FLT_EPSILON))
            return false;
    }

    /* A0 x B0 */
    ra = (extents[1] * std::fabs(R[2][0])) + (extents[2] * std::fabs(R[1][0]));
    rb = (other.extents[1] * std::fabs(R[0][2])) + (other.extents[2] * std::fabs(R[0][1]));
    t = std::fabs((T[2] * R[1][0]) - (T[1] * R[2][0]));
    if (t > (ra + rb + FLT_EPSILON))
        return false;

    /* A0 x B1 */
    ra = (extents[1] * std::fabs(R[2][1])) + (extents[2] * std::fabs(R[1][1]));
    rb = (other.extents[0] * std::fabs(R[0][2])) + (other.extents[2] * std::fabs(R[0][0]));
    t = std::fabs((T[2] * R[1][1]) - (T[1] * R[2][1]));
    if (t > (ra + rb + FLT_EPSILON))
        return false;

    /* A0 x B2 */
    ra = (extents[1] * std::fabs(R[2][2])) + (extents[2] * std::fabs(R[1][2]));
    rb = (other.extents[0] * std::fabs(R[0][1])) + (other.extents[1] * std::fabs(R[0][0]));
    t = std::fabs((T[2] * R[1][2]) - (T[1] * R[2][2]));
    if (t > (ra + rb + FLT_EPSILON))
        return false;

    /* A1 x B0 */
    ra = (extents[0] * std::fabs(R[2][0])) + (extents[2] * std::fabs(R[0][0]));
    rb = (other.extents[1] * std::fabs(R[1][2])) + (other.extents[2] * std::fabs(R[1][1]));
    t = std::fabs((T[0] * R[2][0]) - (T[2] * R[0][0]));
    if (t > (ra + rb + FLT_EPSILON))
        return false;

    /* A1 x B1 */
    ra = (extents[0] * std::fabs(R[2][1])) + (extents[2] * std::fabs(R[0][1]));
    rb = (other.extents[0] * std::fabs(R[1][2])) + (other.extents[2] * std::fabs(R[1][0]));
    t = std::fabs((T[0] * R[2][1]) - (T[2] * R[0][1]));
    if (t > (ra + rb + FLT_EPSILON))
        return false;

    /* A1 x B2 */
    ra = (extents[0] * std::fabs(R[2][2])) + (extents[2] * std::fabs(R[0][2]));
    rb = (other.extents[0] * std::fabs(R[1][1])) + (other.extents[1] * std::fabs(R[1][0]));
    t = std::fabs((T[0] * R[2][2]) - (T[2] * R[0][2]));
    if (t > (ra + rb + FLT_EPSILON))
        return false;

    /* A2 x B0 */
    ra = (extents[0] * std::fabs(R[1][0])) + (extents[1] * std::fabs(R[0][0]));
    rb = (other.extents[1] * std::fabs(R[2][2])) + (other.extents[2] * std::fabs(R[2][1]));
    t = std::fabs((T[1] * R[0][0]) - (T[0] * R[1][0]));
    if (t > (ra + rb + FLT_EPSILON))
        return false;

    /* A2 x B1 */
    ra = (extents[0] * std::fabs(R[1][1])) + (extents[1] * std::fabs(R[0][1]));
    rb = (other.extents[0] * std::fabs(R[2][2])) + (other.extents[2] * std::fabs(R[2][0]));
    t = std::fabs((T[1] * R[0][1]) - (T[0] * R[1][1]));
    if (t > (ra + rb + FLT_EPSILON))
        return false;

    /* A2 x B2 */
    ra = (extents[0] * std::fabs(R[1][2])) + (extents[1] * std::fabs(R[0][2]));
    rb = (other.extents[0] * std::fabs(R[2][1])) + (other.extents[1] * std::fabs(R[2][0]));
    t = std::fabs((T[1] * R[0][2]) - (T[0] * R[1][2]));
    if (t > (ra + rb + FLT_EPSILON))
        return false;

    return true;
}

}
