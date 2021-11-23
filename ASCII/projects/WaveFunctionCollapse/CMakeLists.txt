# CMakeLists.txt file generated with DCG.

# To stop file regeneration, remove the following line.
#!DCG_REGENERATE_THIS_FILE

project(WaveFunctionCollapse C CXX)

add_executable(WaveFunctionCollapse)

set_property(TARGET WaveFunctionCollapse PROPERTY FOLDER executables)

set_property(TARGET WaveFunctionCollapse PROPERTY CMAKE_CXX_STANDARD 20)
set_property(TARGET WaveFunctionCollapse PROPERTY CXX_STANDARD_REQUIRED ON)
target_compile_features(WaveFunctionCollapse PUBLIC cxx_std_20)

target_link_libraries(WaveFunctionCollapse
PRIVATE
  Ascii
  DcUtility
)

target_include_directories(WaveFunctionCollapse
PUBLIC
  "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

set(src
  "${CMAKE_CURRENT_SOURCE_DIR}/source/Main.cpp"
)

target_sources(WaveFunctionCollapse
PRIVATE
  ${src}
)

source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/source" PREFIX "source" FILES ${src})

DCG_add_interface_source_group(Ascii)
DCG_add_interface_source_group(DcUtility)
