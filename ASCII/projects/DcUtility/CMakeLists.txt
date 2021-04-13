# CMakeLists.txt file generated with DCG.

# To stop file regeneration, remove the following line.
#!DCG_REGENERATE_THIS_FILE

project(DcUtility C CXX)

add_library(DcUtility INTERFACE)

set_property(TARGET DcUtility PROPERTY FOLDER libraries)

target_compile_features(DcUtility INTERFACE cxx_std_20)

target_link_libraries(DcUtility
)

target_include_directories(DcUtility
INTERFACE
  "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

set(inc
  "${CMAKE_CURRENT_SOURCE_DIR}/include/Math/Vector.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/include/Math/Matrix.h"
)

target_sources(DcUtility
INTERFACE
  ${inc}
)

source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/include" PREFIX "include" FILES ${inc})

add_subdirectory("${CMAKE_SOURCE_DIR}/projects/DcUtility/test")
