if (NOT DEFINED ATHENA_INCLUDE_DIR)
    set(ATHENA_INCLUDE_DIR ../Athena/include)
endif()
include_directories(include ${ATHENA_INCLUDE_DIR})

if(NOT WIN32)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -msse4.1")
endif()

add_library(Math
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

    include/CVector3f.hpp
    include/Math.hpp
    include/CQuaternion.hpp
    include/CMatrix3f.hpp
    include/CProjection.hpp
    include/CAxisAngle.hpp
    include/CPlane.hpp
    include/CTransform.hpp
    include/CColor.hpp
    include/Global.hpp
    include/MathLib.hpp
    include/CVector2f.hpp
    include/CRectangle.hpp
    include/CMatrix4f.hpp
    include/TVectorUnion.hpp
    include/CVector4f.hpp
    include/CFrustum.hpp
    include/CAABox.hpp
    include/COBBox.hpp)

