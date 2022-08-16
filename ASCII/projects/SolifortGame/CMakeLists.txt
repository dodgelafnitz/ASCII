# CMakeLists.txt file generated with DCG.

# To stop file regeneration, remove the following line.
#!DCG_REGENERATE_THIS_FILE

project(SolifortGame C CXX)

add_executable(SolifortGame)

set_property(TARGET SolifortGame PROPERTY FOLDER executables)

set_property(TARGET SolifortGame PROPERTY CMAKE_CXX_STANDARD 20)
set_property(TARGET SolifortGame PROPERTY CXX_STANDARD_REQUIRED ON)
target_compile_features(SolifortGame PUBLIC cxx_std_20)

target_link_libraries(SolifortGame
)

target_include_directories(SolifortGame
PUBLIC
  "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

set(src
  "${CMAKE_CURRENT_SOURCE_DIR}/source/Main.cpp"
)

target_sources(SolifortGame
PRIVATE
  ${src}
)

source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/source" PREFIX "source" FILES ${src})
