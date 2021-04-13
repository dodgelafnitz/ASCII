# CMakeLists.txt file generated with DCG.

# To stop file regeneration, remove the following line.
#!DCG_REGENERATE_THIS_FILE

add_executable(DcUtilityTest)

set_property(TARGET DcUtilityTest PROPERTY FOLDER tests)

target_compile_features(DcUtilityTest INTERFACE cxx_std_20)

target_link_libraries(DcUtilityTest
PRIVATE
  DcUtility
  gtest_main
)

set(tst
  "${CMAKE_CURRENT_SOURCE_DIR}/Math/VectorTest.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/Math/MatrixTest.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/Containers/GridTest.cpp"
)

target_sources(DcUtilityTest
PRIVATE
  ${tst}
)

source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" PREFIX "test" FILES ${tst})

DCG_add_interface_source_group(DcUtility)
