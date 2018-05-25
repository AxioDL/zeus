#include <iostream>
#include <iomanip>
#include <zeus/zeus.hpp>

// This is only for testing, do NOT do this normally
using namespace zeus;

union Color {
    struct
    {
        zeus::Comp8 r, g, b, a;
    };
    zeus::Comp32 rgba;
};

int main()
{
    zeus::detectCPU();
    assert(!CAABox({100, 100, 100}, {100, 100, 100}).invalid());
    assert(CAABox().invalid());
    CVector3f vec{320.f, 1.f, 0.66568f};
    assert(vec.canBeNormalized());
    assert(!vec.isZero());
    assert(CVector3f().isZero());
    assert(!vec.normalized().canBeNormalized());
    float blarg = 5.f;
    CVector3f t{100, 100, 200};
    blarg = clamp(0.f, blarg, 1.f);
    CAABox test{{-100, -100, -100}, {100, 100, 100}};
    CAABox test2{{-100, -100, -100}, {100, 100, 100}};
    CAABox test3{{-50, -50, -50}, {50, 50, 50}};
    CAABox test4{{-50, -50, -105}, {50, 50, 105}};
    CVector3f point(-90, 67, -105);
    test.closestPointAlongVector(point);
    CVector3d(100, -100, -200);
    zeus::CAABox aabb1{208.9f, -83.9f, 17.7f, 211.9f, -80.9f, 25.7f};
    aabb1.center();
    aabb1.extents() * 2.f;
    zeus::CAABox aabb2{211.8f, -81.4f, 22.3f, 212.8f, -80.4f, 25.0f};
    aabb2.center();
    aabb2.extents() * 2.f;

    aabb1.booleanIntersection(aabb2);
    aabb2.center();
    aabb2.extents() * 2.f;

    assert(t.isEqu(t));
    assert(test.inside(test));
    assert(test2.inside(test));
    assert(test3.inside(test));
    assert(!test4.inside(test));

    CAABox aabb(-1.f, 1.f);
    CSphere s1(zeus::CVector3f(0.f), 1);
    CSphere s2(zeus::CVector3f(1.f, 0.f, 0.f), 1);
    CSphere s3(zeus::CVector3f(3.f, 0.f, 0.f), 1);

    std::cout << min(1, 3) << std::endl;
    std::cout << min(2, 1) << std::endl;
    std::cout << max(1, 3) << std::endl;
    std::cout << max(2, 1) << std::endl;
    std::cout << clamp(-50, 100, 50) << std::endl;
    std::cout << clamp(-50, -100, 50) << std::endl;
    std::cout << powF(6.66663489, 2) << std::endl;
    std::cout << invSqrtF(1) << std::endl;
    std::cout << floorPowerOfTwo(256) << std::endl;
    std::cout << " Test 1 " << (aabb.intersects(s1) ? "succeeded" : "failed") << std::endl;
    std::cout << " Test 2 " << (aabb.intersects(s2) ? "succeeded" : "failed") << std::endl;
    std::cout << " Test 3 " << (aabb.intersects(s3) ? "succeeded" : "failed") << std::endl;
    CLineSeg({-89.120926f, 59.328712f, 3.265882f}, {-90.120926f, 59.328712f, 3.265882f});

    CColor ctest1;
    ctest1.fromHSV(0, 255 / 255.f, .5);
    float h, s, v;
    ctest1.toHSV(h, s, v);
    std::cout << (int)ctest1.r << " " << (int)ctest1.g << " " << (int)ctest1.b << " " << (int)ctest1.a << std::endl;
    std::cout << h << " " << s << " " << v << " " << (float)(ctest1.a / 255.f) << std::endl;
    return 0;
}
