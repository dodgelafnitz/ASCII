# CMakeLists.txt file generated with DCG.

# To stop file regeneration, remove the following line.
#!DCG_REGENERATE_THIS_FILE

project(DcTestUtility C CXX)

add_library(DcTestUtility INTERFACE)

set_property(TARGET DcTestUtility PROPERTY FOLDER libraries)

target_compile_features(DcTestUtility INTERFACE cxx_std_20)

target_link_libraries(DcTestUtility
)

target_include_directories(DcTestUtility
INTERFACE
  "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

set(inc
  "${CMAKE_CURRENT_SOURCE_DIR}/include/Helpers/TestHelpers.h"
)

target_sources(DcTestUtility
INTERFACE
  ${inc}
)

source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/include" PREFIX "include" FILES ${inc})

add_subdirectory("${CMAKE_SOURCE_DIR}/projects/DcTestUtility/test")
