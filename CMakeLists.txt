cmake_minimum_required(VERSION 3.0)
project(zeus)

if (NOT DEFINED ATHENA_INCLUDE_DIR)
    set(ATHENA_INCLUDE_DIR ../athena/include)
endif()
include_directories(include ${ATHENA_INCLUDE_DIR})

set(SOURCES
    src/CAxisAngle.cpp
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
    src/CEulerAngles.cpp)

# SSELegacy.cpp compiled separately to escape the effects of link-time optimization
if(NOT MSVC)
set_source_files_properties(${SOURCES} PROPERTIES COMPILE_FLAGS "-msse4.1 -msse4.2 -std=c++14")
if(CUSTOM_FLAGS)
    string(REPLACE "-flto=thin" "" CUSTOM_FLAGS ${CMAKE_CXX_FLAGS})
endif(CUSTOM_FLAGS)
if (CMAKE_OSX_SYSROOT)
    set(CUSTOM_FLAGS "${CUSTOM_FLAGS} -isysroot ${CMAKE_OSX_SYSROOT}")
endif()
if (CMAKE_OSX_DEPLOYMENT_TARGET AND NOT CMAKE_OSX_DEPLOYMENT_TARGET STREQUAL "")
    set(CUSTOM_FLAGS "${CUSTOM_FLAGS} -mmacosx-version-min=${CMAKE_OSX_DEPLOYMENT_TARGET} -O3 -msse3 -std=c++14")
else()
    set(CUSTOM_FLAGS "${CUSTOM_FLAGS} -O3 -msse3 -std=c++14")
endif()

set(SSE_LEGACY_OBJ ${CMAKE_CURRENT_BINARY_DIR}/SSELegacy.o)
separate_arguments(CUSTOM_FLAGS UNIX_COMMAND ${CUSTOM_FLAGS})
add_custom_command(
    OUTPUT ${SSE_LEGACY_OBJ}
    COMMAND ${CMAKE_CXX_COMPILER}
    ARGS ${CUSTOM_FLAGS} -c ${CMAKE_CURRENT_SOURCE_DIR}/src/SSELegacy.cpp
                         -o ${SSE_LEGACY_OBJ}
                         -I ${CMAKE_CURRENT_SOURCE_DIR}/include
                         -I ${ATHENA_INCLUDE_DIR}
    MAIN_DEPENDENCY src/SSELegacy.cpp)

else()
set(SSE_LEGACY_OBJ src/SSELegacy.cpp)
endif()

add_library(zeus
    ${SOURCES}
    ${SSE_LEGACY_OBJ}

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
    include/zeus/CLine.hpp
    include/zeus/CLineSeg.hpp
    include/zeus/CSphere.hpp
    include/zeus/CUnitVector.hpp
    include/zeus/CMRay.hpp
    include/zeus/CEulerAngles.hpp)

add_subdirectory(test)

