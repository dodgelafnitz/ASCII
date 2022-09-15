# CMakeLists.txt file generated with DCG.

# To stop file regeneration, remove the following line.
#!DCG_REGENERATE_THIS_FILE

project(Metagame2 C CXX)

add_executable(Metagame2)

set_property(TARGET Metagame2 PROPERTY FOLDER executables)

set_property(TARGET Metagame2 PROPERTY CMAKE_CXX_STANDARD 20)
set_property(TARGET Metagame2 PROPERTY CXX_STANDARD_REQUIRED ON)
target_compile_features(Metagame2 PUBLIC cxx_std_20)

target_link_libraries(Metagame2
PRIVATE
  Ascii
)

target_include_directories(Metagame2
PUBLIC
  "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

set(src
  "${CMAKE_CURRENT_SOURCE_DIR}/source/Main.cpp"
)

target_sources(Metagame2
PRIVATE
  ${src}
)

source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/source" PREFIX "source" FILES ${src})

DCG_add_interface_source_group(Ascii)
