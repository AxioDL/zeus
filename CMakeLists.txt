include_directories(../Athena/include)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -msse4.1")
add_library(Math
    CVector3f.cpp
    Math.cpp
    CQuaternion.cpp
    CMatrix3f.cpp
    CProjection.cpp
    CPlane.cpp
    CTransform.cpp
    CVector2f.cpp
    CRectangle.cpp
    CVector4f.cpp
    CMatrix4f.cpp

    CVector3f.hpp
    Math.hpp
    CQuaternion.hpp
    CMatrix3f.hpp
    CProjection.hpp
    CAxisAngle.hpp
    CPlane.hpp
    CTransform.hpp
    CColor.hpp
    Global.hpp
    MathLib.hpp
    CVector2f.hpp
    CRectangle.hpp
    CMatrix4f.hpp
    TVectorUnion.hpp
    CVector4f.hpp
    CFrustum.hpp
    SBoundingBox.hpp)

