# CMakeLists.txt file generated with DCG.

# To stop file regeneration, remove the following line.
#!DCG_REGENERATE_THIS_FILE

project(Numbergame C CXX)

add_executable(Numbergame)

set_property(TARGET Numbergame PROPERTY FOLDER executables)

set_property(TARGET Numbergame PROPERTY CMAKE_CXX_STANDARD 20)
set_property(TARGET Numbergame PROPERTY CXX_STANDARD_REQUIRED ON)
target_compile_features(Numbergame PUBLIC cxx_std_20)

target_link_libraries(Numbergame
)

target_include_directories(Numbergame
PUBLIC
  "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

set(src
  "${CMAKE_CURRENT_SOURCE_DIR}/source/Main.cpp"
)

target_sources(Numbergame
PRIVATE
  ${src}
)

source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/source" PREFIX "source" FILES ${src})
