# CMakeLists.txt file generated with DCG.

# To stop file regeneration, remove the following line.
#!DCG_REGENERATE_THIS_FILE

project(Cardad C CXX)

add_executable(Cardad)

set_property(TARGET Cardad PROPERTY FOLDER executables)

set_property(TARGET Cardad PROPERTY CMAKE_CXX_STANDARD 20)
set_property(TARGET Cardad PROPERTY CXX_STANDARD_REQUIRED ON)
target_compile_features(Cardad PUBLIC cxx_std_20)

target_link_libraries(Cardad
PRIVATE
  DcUtility
)

target_include_directories(Cardad
PUBLIC
  "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

set(src
  "${CMAKE_CURRENT_SOURCE_DIR}/source/Main.cpp"
)

target_sources(Cardad
PRIVATE
  ${src}
)

source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/source" PREFIX "source" FILES ${src})

DCG_add_interface_source_group(DcUtility)
