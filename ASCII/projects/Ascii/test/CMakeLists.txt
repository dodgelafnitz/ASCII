# CMakeLists.txt file generated with DCG.

# To stop file regeneration, remove the following line.
#!DCG_REGENERATE_THIS_FILE

add_executable(AsciiTest)

set_property(TARGET AsciiTest PROPERTY FOLDER tests)

set_property(TARGET AsciiTest PROPERTY CMAKE_CXX_STANDARD 20)
set_property(TARGET AsciiTest PROPERTY CXX_STANDARD_REQUIRED ON)
target_compile_features(AsciiTest PUBLIC cxx_std_20)

target_link_libraries(AsciiTest
PRIVATE
  Ascii
  gmock
  gtest_main
  DcTestUtility
)

set(tst
  "${CMAKE_CURRENT_SOURCE_DIR}/Window/WindowTest.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/Widget/WidgetTest.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/Systems/Input/ButtonManagerTest.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/Systems/Input/MouseManagerTest.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/Systems/Input/TextManagerTest.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/Systems/Input/StateManagerTest.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/Systems/Input/InputManagerTest.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/Systems/UpdateManagerTest.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/Systems/WidgetManagerTest.cpp"
)

target_sources(AsciiTest
PRIVATE
  ${tst}
)

source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" PREFIX "test" FILES ${tst})

DCG_add_interface_source_group(Ascii)
DCG_add_interface_source_group(DcTestUtility)
