# CMakeLists.txt file generated with DCG.

# To stop file regeneration, remove the following line.
#!DCG_REGENERATE_THIS_FILE

project(VisualTest C CXX)

add_executable(VisualTest)

set_property(TARGET VisualTest PROPERTY FOLDER executables)

set_property(TARGET VisualTest PROPERTY CMAKE_CXX_STANDARD 20)
set_property(TARGET VisualTest PROPERTY CXX_STANDARD_REQUIRED ON)
target_compile_features(VisualTest PUBLIC cxx_std_20)

target_link_libraries(VisualTest
PRIVATE
  Ascii
  DcUtility
)

target_include_directories(VisualTest
PUBLIC
  "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

set(src
  "${CMAKE_CURRENT_SOURCE_DIR}/source/Main.cpp"
)

target_sources(VisualTest
PRIVATE
  ${src}
)

source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/source" PREFIX "source" FILES ${src})

DCG_add_interface_source_group(Ascii)
DCG_add_interface_source_group(DcUtility)
