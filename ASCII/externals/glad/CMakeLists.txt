project(glad C CXX)

add_library(glad STATIC)

target_include_directories(glad
PUBLIC
  "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

set(inc
  "${CMAKE_CURRENT_SOURCE_DIR}/include/glad.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/include/khrplatform.h"
)

set(src
  "${CMAKE_CURRENT_SOURCE_DIR}/source/glad.c"
)

target_sources(glad
PUBLIC
  ${inc}
PRIVATE
  ${src}
)

source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/include" PREFIX "include" FILES ${inc})
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/source" PREFIX "source" FILES ${src})
