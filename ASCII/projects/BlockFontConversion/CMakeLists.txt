# CMakeLists.txt file generated with DCG.

# To stop file regeneration, remove the following line.
#!DCG_REGENERATE_THIS_FILE

project(BlockFontConversion C CXX)

add_executable(BlockFontConversion)

set_property(TARGET BlockFontConversion PROPERTY FOLDER executables)

set_property(TARGET BlockFontConversion PROPERTY CMAKE_CXX_STANDARD 20)
set_property(TARGET BlockFontConversion PROPERTY CXX_STANDARD_REQUIRED ON)
target_compile_features(BlockFontConversion PUBLIC cxx_std_20)

target_link_libraries(BlockFontConversion
PRIVATE
  stb
)

target_include_directories(BlockFontConversion
PUBLIC
  "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

set(src
  "${CMAKE_CURRENT_SOURCE_DIR}/source/Main.cpp"
)

target_sources(BlockFontConversion
PRIVATE
  ${src}
)

source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/source" PREFIX "source" FILES ${src})

