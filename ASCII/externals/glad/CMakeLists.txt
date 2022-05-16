add_library(glad STATIC)

set_property(TARGET glad PROPERTY FOLDER libraries)

#set_property(TARGET glad PROPERTY CMAKE_CXX_STANDARD 20)
#set_property(TARGET glad PROPERTY CXX_STANDARD_REQUIRED ON)
#target_compile_features(glad PUBLIC cxx_std_20)

target_include_directories(glad
PUBLIC
  "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

set(inc
  "${CMAKE_CURRENT_SOURCE_DIR}/include/glad/glad.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/include/KHR/khrplatform.h"
)

set(src
  "${CMAKE_CURRENT_SOURCE_DIR}/src/glad.c"
)

target_sources(glad
PUBLIC
  ${inc}
PRIVATE
  ${src}
)

source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/include" PREFIX "include" FILES ${inc})
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/src" PREFIX "source" FILES ${src})
