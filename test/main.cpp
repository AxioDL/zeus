#include <iostream>
#include <iomanip>
#include <MathLib.hpp>

// This is only for testing, do NOT do this normally
using namespace Zeus;

union Color
{
    struct { Zeus::Comp8 r, g, b, a; };
    Zeus::Comp32 rgba;
};

int main()
{
    Zeus::detectCPU();
    assert(!CAABox({100, 100, 100}, {100, 100, 100}).invalid());
    assert(CAABox().invalid());
    CVector3f vec{320, 632162.f, 800.f};
    assert(vec.canBeNormalized());
    assert(!vec.isZero());
    assert(CVector3f().isZero());
    assert(!vec.normalized().canBeNormalized());
    float blarg = 5.f;
    CVector3f t{100, 100, 200};
    blarg = Math::clamp(0.f, blarg, 1.f);
    CAABox test{{-100, -100, -100}, {100, 100, 100}};
    CAABox test2{{-100, -100, -100}, {100, 100, 100}};
    CAABox test3{{-50, -50, -50},  {50, 50, 50}};
    CAABox test4{{-50, -50, -105}, {50, 50, 105}};
    CVector3f point(-90, 67, -105);
    CVector3f closestPoint = test.closestPointAlongVector(point);
    CVector3d doubleVec(100, -100, -200);

    assert(t.isEqu(t));
    assert(test.inside(test));
    assert(test2.inside(test));
    assert(test3.inside(test));
    assert(!test4.inside(test));

    CAABox aabb({-1}, {1});
    CSphere s1({0}, 1);
    CSphere s2({1, 0, 0}, 1);
    CSphere s3({3, 0, 0}, 1);

    std::cout << Math::min(1, 3) << std::endl;
    std::cout << Math::min(2, 1) << std::endl;
    std::cout << Math::max(1, 3) << std::endl;
    std::cout << Math::max(2, 1) << std::endl;
    std::cout << Math::clamp(-50,  100, 50) << std::endl;
    std::cout << Math::clamp(-50, -100, 50) << std::endl;
    std::cout << Math::powF(6.66663489, 2) << std::endl;
    std::cout << Math::invSqrtF(1) << std::endl;
    std::cout << Math::floorPowerOfTwo(256) << std::endl;
    std::cout << " Test 1 " << ( aabb.intersects(s1) ? "succeeded" : "failed" ) << std::endl;
    std::cout << " Test 2 " << ( aabb.intersects(s2) ? "succeeded" : "failed" ) << std::endl;
    std::cout << " Test 3 " << ( aabb.intersects(s3) ? "succeeded" : "failed" ) << std::endl;
    CLine line({-89.120926, 59.328712, 3.265882}, CUnitVector3f({-90.120926, 59.328712, 3.265882}));

    CColor ctest1;
    ctest1.fromHSV(0, 255/255.f, .5);
    float h, s, v;
    ctest1.toHSV(h, s, v);
    std::cout << (int)ctest1.r << " " << (int)ctest1.g << " " << (int)ctest1.b << " " << (int)ctest1.a << std::endl;
    std::cout << h << " " << s << " " << v << " " << (float)(ctest1.a / 255.f) << std::endl;
    return 0;
}
