cmake_minimum_required(VERSION 3.19.0)

project(ASCII C CXX)

set("ENV{DCG_ENABLE_TESTS}" TRUE)

string(TIMESTAMP currentYear "%Y")
set("ENV{DCG_COPYWRITE}" "/*\n * Copywrite ${currentYear} Dodge Lafnitzegger\n */\n\n")

include("./dcg_setup.cmake")

set(CMAKE_EXE_LINKER_FLAGS "-static")

DCG_BEGIN()
  DCG_PROJECT_INTERFACE("DcTestUtility")
    DCG_FOLDER("Helpers")
      DCG_FILE_CPP_HEADER_ONLY("TestHelpers")
    DCG_END_FOLDER()
  DCG_END_PROJECT()

  DCG_PROJECT_LIB("DcUtility"
    TEST_DEPENDS
      "DcTestUtility"
  )
    DCG_FOLDER("Math")
      DCG_FILE_CPP_HEADER_ONLY("Vector")
      DCG_FILE_CPP_HEADER_ONLY("Matrix")
      DCG_FOLDER("Shapes2D")
        DCG_FILE_CPP("LineSegment")
        DCG_FILE_CPP("Line")
        DCG_FILE_CPP("Ray")
        DCG_FILE_CPP("Circle")
        DCG_FILE_CPP("Rect")
        DCG_FILE_CPP("ConvexHull")
        DCG_FILE_CPP("Polygon")
        DCG_FILE_CPP("Collision")
        DCG_FILE_CPP_HEADER_ONLY_NO_TEST("ForwardDeclarations")
      DCG_END_FOLDER()
    DCG_END_FOLDER()
    DCG_FOLDER("Containers")
      DCG_FILE_CPP_HEADER_ONLY("Grid")
      DCG_FILE_CPP_HEADER_ONLY("DynamicArray")
    DCG_END_FOLDER()
    DCG_FOLDER("General")
      DCG_FILE_CPP_NO_TEST("Color")
      DCG_FILE_CPP_HEADER_ONLY("Delagate")
    DCG_END_FOLDER()
  DCG_END_PROJECT()

  DCG_PROJECT_LIB("Ascii"
    PUBLIC_DEPENDS
      "DcUtility"
    TEST_DEPENDS
      "DcTestUtility"
  )
    DCG_FOLDER("Window")
      DCG_FILE_CPP("Window")
    DCG_END_FOLDER()
    DCG_FOLDER("Widget")
      DCG_FILE_CPP("Widget")
    DCG_END_FOLDER()
    DCG_FOLDER("Systems")
      DCG_FOLDER("Input")
        DCG_FILE_CPP("ButtonManager")
        DCG_FILE_CPP("MouseManager")
        DCG_FILE_CPP("TextManager")
        DCG_FILE_CPP("StateManager")
        DCG_FILE_CPP("InputManager")
      DCG_END_FOLDER()
      DCG_FILE_CPP("UpdateManager")
      DCG_FILE_CPP("FocusManager")
    DCG_END_FOLDER()
  DCG_END_PROJECT()

  DCG_PROJECT_EXE("Game" PRIVATE_DEPENDS "Ascii")
    DCG_FILE_CPP_APPLICATION("Main")
  DCG_END_PROJECT()

  DCG_PROJECT_EXE("Cardad" PRIVATE_DEPENDS "DcUtility")
    DCG_FILE_CPP_APPLICATION("Main")
  DCG_END_PROJECT()

  DCG_PROJECT_EXE("VisualTest" PRIVATE_DEPENDS "Ascii" "DcUtility")
    DCG_FILE_CPP_APPLICATION("Main")
  DCG_END_PROJECT()

  DCG_PROJECT_EXE("WaveFunctionCollapse" PRIVATE_DEPENDS "Ascii" "DcUtility")
    DCG_FILE_CPP_APPLICATION("Main")
  DCG_END_PROJECT()
DCG_END()
