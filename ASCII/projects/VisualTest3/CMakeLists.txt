# CMakeLists.txt file generated with DCG.

# To stop file regeneration, remove the following line.
#!DCG_REGENERATE_THIS_FILE

project(VisualTest3 C CXX)

add_executable(VisualTest3)

set_property(TARGET VisualTest3 PROPERTY FOLDER executables)

set_property(TARGET VisualTest3 PROPERTY CMAKE_CXX_STANDARD 20)
set_property(TARGET VisualTest3 PROPERTY CXX_STANDARD_REQUIRED ON)
target_compile_features(VisualTest3 PUBLIC cxx_std_20)

target_link_libraries(VisualTest3
PRIVATE
  stb
  DcUtility
  Ascii
)

target_include_directories(VisualTest3
PUBLIC
  "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

set(src
  "${CMAKE_CURRENT_SOURCE_DIR}/source/Main.cpp"
)

target_sources(VisualTest3
PRIVATE
  ${src}
)

source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/source" PREFIX "source" FILES ${src})

DCG_add_interface_source_group(DcUtility)
DCG_add_interface_source_group(Ascii)
