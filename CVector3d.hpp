#ifndef CVECTOR3D_HPP
#define CVECTOR3D_HPP

#include <Athena/IStreamReader.hpp>

#include "CVector3f.hpp"

class ZE_ALIGN(16) CVector3d
{
public:
    ZE_DECLARE_ALIGNED_ALLOCATOR();
    
    CVector3d();
    CVector3d(const CVector3f& vec3);
    CVector3d(double x, double y, double z);
    CVector3d(Athena::io::IStreamReader& input);
    ~CVector3d();

    bool operator ==(const CVector3d& rhs);
    CVector3d operator+(const CVector3d& rhs);
    CVector3d operator-(const CVector3d& rhs);
    CVector3d operator-() const;
    CVector3d operator*(const CVector3d& rhs);
    CVector3d operator/(const CVector3d& rhs);
    CVector3d operator+(double val);
    CVector3d operator-(double val);
    CVector3d operator*(double val);
    CVector3d operator/(double val);
    CVector3d normalized();
    CVector3d cross(const CVector3d& rhs) const;
    double    dot(const CVector3d& rhs) const;
    double    magnitude() const;

    CVector3f asVector3f();
    union
    {
        struct
        {
            double x, y, z;
        };
        __m128 v;
    };
};

#endif // CVECTOR3D_HPP
