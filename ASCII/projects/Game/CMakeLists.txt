# CMakeLists.txt file generated with DCG.

# To stop file regeneration, remove the following line.
#!DCG_REGENERATE_THIS_FILE

project(Game C CXX)

add_executable(Game)

set_property(TARGET Game PROPERTY FOLDER executables)

set_property(TARGET Game PROPERTY CMAKE_CXX_STANDARD 20)
set_property(TARGET Game PROPERTY CXX_STANDARD_REQUIRED ON)
target_compile_features(Game PUBLIC cxx_std_20)

target_link_libraries(Game
PRIVATE
  Ascii
)

target_include_directories(Game
PUBLIC
  "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

set(src
  "${CMAKE_CURRENT_SOURCE_DIR}/source/Main.cpp"
)

target_sources(Game
PRIVATE
  ${src}
)

source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/source" PREFIX "source" FILES ${src})

DCG_add_interface_source_group(Ascii)
