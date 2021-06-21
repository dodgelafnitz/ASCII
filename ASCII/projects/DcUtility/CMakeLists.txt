# CMakeLists.txt file generated with DCG.

# To stop file regeneration, remove the following line.
#!DCG_REGENERATE_THIS_FILE

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
  "${CMAKE_CURRENT_SOURCE_DIR}/include/Containers/Grid.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/include/General/Color.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/include/General/Delagate.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/include/Helpers/TemplateHelpers.h"
)

set(src
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
