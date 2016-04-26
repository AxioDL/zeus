cmake_minimum_required(VERSION 3.0)
project(zeus)

if (NOT DEFINED ATHENA_INCLUDE_DIR)
    set(ATHENA_INCLUDE_DIR ../athena/include)
endif()
include_directories(include ${ATHENA_INCLUDE_DIR})

if(NOT MSVC)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -msse4.1 -msse4.2 -std=c++14")
endif()

add_library(zeus
    src/CVector3f.cpp
    src/Math.cpp
    src/CQuaternion.cpp
    src/CMatrix3f.cpp
    src/CProjection.cpp
    src/CPlane.cpp
    src/CTransform.cpp
    src/CColor.cpp
    src/CVector2f.cpp
    src/CRectangle.cpp
    src/CVector4f.cpp
    src/CMatrix4f.cpp
    src/CAABox.cpp

    include/zeus/Math.hpp
    include/zeus/CQuaternion.hpp
    include/zeus/CMatrix3f.hpp
    include/zeus/CProjection.hpp
    include/zeus/CAxisAngle.hpp
    include/zeus/CRelAngle.hpp
    include/zeus/CPlane.hpp
    include/zeus/CTransform.hpp
    include/zeus/CColor.hpp
    include/zeus/Global.hpp
    include/zeus/zeus.hpp
    include/zeus/TVectorUnion.hpp
    include/zeus/CVector2i.hpp
    include/zeus/CVector2f.hpp
    include/zeus/CVector3f.hpp
    include/zeus/CVector3d.hpp
    include/zeus/CVector4f.hpp
    include/zeus/CRectangle.hpp
    include/zeus/CMatrix4f.hpp
    include/zeus/CFrustum.hpp
    include/zeus/CAABox.hpp
    include/zeus/COBBox.hpp
    include/zeus/CLineSeg.hpp
    include/zeus/CSphere.hpp
    include/zeus/CUnitVector.hpp
    include/zeus/CMRay.hpp)

add_subdirectory(test)

