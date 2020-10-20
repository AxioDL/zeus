cmake_minimum_required(VERSION 3.10 FATAL_ERROR) # because of c++17
project(zeus)

if (NOT MSVC)
  set(CMAKE_CXX_STANDARD 20)
  set(CMAKE_CXX_STANDARD_REQUIRED ON)
endif()

set(SOURCES
    src/CVector3f.cpp
    src/Math.cpp
    src/CQuaternion.cpp
    src/CMatrix3f.cpp
    src/CProjection.cpp
    src/CPlane.cpp
    src/CFrustum.cpp
    src/CTransform.cpp
    src/CColor.cpp
    src/CVector2f.cpp
    src/CMatrix4f.cpp
    src/CAABox.cpp
    src/COBBox.cpp
    src/CEulerAngles.cpp)

add_library(zeus
    ${SOURCES}
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
    include/zeus/CLine.hpp
    include/zeus/CLineSeg.hpp
    include/zeus/CSphere.hpp
    include/zeus/CUnitVector.hpp
    include/zeus/CMRay.hpp
    include/zeus/CEulerAngles.hpp
    include/zeus/simd/simd.hpp
    include/zeus/simd/simd_sse.hpp
    include/zeus/simd/simd_avx.hpp
    include/zeus/simd/simd_neon.hpp
    include/zeus/simd/parallelism_v2_simd.hpp)

target_include_directories(zeus PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>)
if(TARGET athena-core)
  target_link_libraries(zeus PUBLIC athena-core)
  target_compile_definitions(zeus PUBLIC ZE_ATHENA_TYPES=1)
endif()

if(TARGET hsh)
  target_link_libraries(zeus PUBLIC hsh)
  target_compile_definitions(zeus PUBLIC ZE_HSH_TYPES=1)
endif()

add_subdirectory(test)

