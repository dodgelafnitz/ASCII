# CMakeLists.txt file generated with DCG.

# To stop file regeneration, remove the following line.
#!DCG_REGENERATE_THIS_FILE

add_executable(DcUtilityTest)

set_property(TARGET DcUtilityTest PROPERTY FOLDER tests)

set_property(TARGET DcUtilityTest PROPERTY CMAKE_CXX_STANDARD 20)
set_property(TARGET DcUtilityTest PROPERTY CXX_STANDARD_REQUIRED ON)
target_compile_features(DcUtilityTest PUBLIC cxx_std_20)

target_link_libraries(DcUtilityTest
PRIVATE
  DcUtility
  gmock
  gtest_main
  DcTestUtility
)

set(tst
  "${CMAKE_CURRENT_SOURCE_DIR}/Math/VectorTest.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/Math/MatrixTest.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/Math/Shapes2D/LineSegmentTest.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/Math/Shapes2D/LineTest.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/Math/Shapes2D/RayTest.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/Math/Shapes2D/CircleTest.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/Math/Shapes2D/RectTest.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/Math/Shapes2D/ConvexHullTest.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/Math/Shapes2D/PolygonTest.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/Math/Shapes2D/CollisionTest.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/Containers/GridTest.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/Containers/DynamicArrayTest.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/General/DelagateTest.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/Helpers/TemplateHelpersTest.cpp"
)

target_sources(DcUtilityTest
PRIVATE
  ${tst}
)

source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" PREFIX "test" FILES ${tst})

DCG_add_interface_source_group(DcUtility)
DCG_add_interface_source_group(DcTestUtility)
