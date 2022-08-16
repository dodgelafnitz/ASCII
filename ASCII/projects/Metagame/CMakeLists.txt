# CMakeLists.txt file generated with DCG.

# To stop file regeneration, remove the following line.
#!DCG_REGENERATE_THIS_FILE

project(Metagame C CXX)

add_executable(Metagame)

set_property(TARGET Metagame PROPERTY FOLDER executables)

set_property(TARGET Metagame PROPERTY CMAKE_CXX_STANDARD 20)
set_property(TARGET Metagame PROPERTY CXX_STANDARD_REQUIRED ON)
target_compile_features(Metagame PUBLIC cxx_std_20)

target_link_libraries(Metagame
PRIVATE
  Ascii
)

target_include_directories(Metagame
PUBLIC
  "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

set(src
  "${CMAKE_CURRENT_SOURCE_DIR}/source/Main.cpp"
)

target_sources(Metagame
PRIVATE
  ${src}
)

source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/source" PREFIX "source" FILES ${src})

DCG_add_interface_source_group(Ascii)
