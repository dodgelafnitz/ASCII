# CMakeLists.txt file generated with DCG.

#!NO_DCG_REGENERATE_THIS_FILE

project(DcUtility C CXX)

add_library(DcUtility STATIC)

set_property(TARGET DcUtility PROPERTY FOLDER libraries)

set_property(TARGET DcUtility PROPERTY CMAKE_CXX_STANDARD 20)
set_property(TARGET DcUtility PROPERTY CXX_STANDARD_REQUIRED ON)
target_compile_features(DcUtility PUBLIC cxx_std_20)

target_link_libraries(DcUtility
)

target_include_directories(DcUtility
PUBLIC
  "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

set(inc
  "${CMAKE_CURRENT_SOURCE_DIR}/include/Math/Vector.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/include/Math/Matrix.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/include/Math/Shapes2D/LineSegment.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/include/Math/Shapes2D/Line.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/include/Math/Shapes2D/Ray.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/include/Math/Shapes2D/Circle.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/include/Math/Shapes2D/Rect.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/include/Math/Shapes2D/ConvexHull.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/include/Math/Shapes2D/Polygon.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/include/Math/Shapes2D/Collision.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/include/Math/Shapes2D/ForwardDeclarations.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/include/Containers/Grid.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/include/Containers/DynamicArray.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/include/General/Color.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/include/General/Delagate.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/include/DcUtility.natvis"
)

set(src
  "${CMAKE_CURRENT_SOURCE_DIR}/source/Math/Shapes2D/LineSegment.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/source/Math/Shapes2D/Line.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/source/Math/Shapes2D/Ray.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/source/Math/Shapes2D/Circle.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/source/Math/Shapes2D/Rect.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/source/Math/Shapes2D/ConvexHull.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/source/Math/Shapes2D/Polygon.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/source/Math/Shapes2D/Collision.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/source/General/Color.cpp"
)

target_sources(DcUtility
PUBLIC
  ${inc}
PRIVATE
  ${src}
)

source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/include" PREFIX "include" FILES ${inc})
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/source" PREFIX "source" FILES ${src})

add_subdirectory("${CMAKE_SOURCE_DIR}/projects/DcUtility/test")
