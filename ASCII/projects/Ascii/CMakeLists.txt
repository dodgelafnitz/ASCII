# CMakeLists.txt file generated with DCG.

# To stop file regeneration, remove the following line.
#!DCG_REGENERATE_THIS_FILE

project(Ascii C CXX)

add_library(Ascii STATIC)

set_property(TARGET Ascii PROPERTY FOLDER libraries)

set_property(TARGET Ascii PROPERTY CMAKE_CXX_STANDARD 20)
set_property(TARGET Ascii PROPERTY CXX_STANDARD_REQUIRED ON)
target_compile_features(Ascii PUBLIC cxx_std_20)

target_link_libraries(Ascii
PRIVATE
  glfw
  glad
  stb
PUBLIC
  DcUtility
)

target_include_directories(Ascii
PUBLIC
  "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

set(inc
  "${CMAKE_CURRENT_SOURCE_DIR}/include/Window/Window.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/include/Window/BlockFont.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/include/Widget/Widget.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/include/Systems/Input/ButtonManager.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/include/Systems/Input/MouseManager.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/include/Systems/Input/TextManager.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/include/Systems/Input/StateManager.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/include/Systems/Input/InputManager.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/include/Systems/UpdateManager.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/include/Systems/FocusManager.h"
)

set(src
  "${CMAKE_CURRENT_SOURCE_DIR}/source/Window/Window.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/source/Window/BlockFont.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/source/Widget/Widget.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/source/Systems/Input/ButtonManager.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/source/Systems/Input/MouseManager.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/source/Systems/Input/TextManager.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/source/Systems/Input/StateManager.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/source/Systems/Input/InputManager.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/source/Systems/UpdateManager.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/source/Systems/FocusManager.cpp"
)

target_sources(Ascii
PUBLIC
  ${inc}
PRIVATE
  ${src}
)

source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/include" PREFIX "include" FILES ${inc})
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/source" PREFIX "source" FILES ${src})

DCG_add_interface_source_group(DcUtility)

add_subdirectory("${CMAKE_SOURCE_DIR}/projects/Ascii/test")
