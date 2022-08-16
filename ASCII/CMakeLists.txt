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
      DCG_FILE_CPP_HEADER_ONLY_NO_TEST("Direction")
    DCG_END_FOLDER()
  DCG_END_PROJECT()

  DCG_PROJECT_LIB("Ascii"
    PUBLIC_DEPENDS
      "DcUtility"
    PRIVATE_DEPENDS
      "glfw"
      "glad"
      "stb"
    TEST_DEPENDS
      "DcTestUtility"
  )
    DCG_FOLDER("Window")
      DCG_FILE_CPP("Window")
      DCG_FILE_CPP_NO_TEST("BlockFont")
    DCG_END_FOLDER()
    DCG_FOLDER("Widget")
      DCG_FILE_CPP("Widget")
      DCG_FILE_CPP("DrawParams")
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
      DCG_FILE_CPP("WidgetManager")
    DCG_END_FOLDER()
  DCG_END_PROJECT()

  DCG_PROJECT_EXE("Game" PRIVATE_DEPENDS "Ascii")
    DCG_FILE_CPP_APPLICATION("Main")
  DCG_END_PROJECT()

  DCG_PROJECT_EXE("Gridgame" PRIVATE_DEPENDS "Ascii")
    DCG_FILE_CPP_APPLICATION("Main")
  DCG_END_PROJECT()

  DCG_PROJECT_EXE("Metagame" PRIVATE_DEPENDS "Ascii")
    DCG_FILE_CPP_APPLICATION("Main")
  DCG_END_PROJECT()

  DCG_PROJECT_EXE("Idlegame" PRIVATE_DEPENDS "Ascii")
    DCG_FILE_CPP_APPLICATION("Main")
  DCG_END_PROJECT()

  DCG_PROJECT_EXE("SolifortGame")
    DCG_FILE_CPP_APPLICATION("Main")
  DCG_END_PROJECT()

  DCG_PROJECT_EXE("Numbergame")
    DCG_FILE_CPP_APPLICATION("Main")
  DCG_END_PROJECT()

  DCG_PROJECT_EXE("VisualTest" PRIVATE_DEPENDS "Ascii" "DcUtility")
    DCG_FILE_CPP_APPLICATION("Main")
  DCG_END_PROJECT()

  DCG_PROJECT_EXE("VisualTest2" PRIVATE_DEPENDS "glfw" "DcUtility")
    DCG_FILE_CPP_APPLICATION("Main")
  DCG_END_PROJECT()

  DCG_PROJECT_EXE("VisualTest3" PRIVATE_DEPENDS "stb" "DcUtility" "Ascii")
    DCG_FILE_CPP_APPLICATION("Main")
  DCG_END_PROJECT()

  DCG_PROJECT_EXE("WaveFunctionCollapse" PRIVATE_DEPENDS "Ascii" "DcUtility")
    DCG_FILE_CPP_APPLICATION("Main")
  DCG_END_PROJECT()

  DCG_PROJECT_EXE("BlockFontConversion" PRIVATE_DEPENDS "stb")
    DCG_FILE_CPP_APPLICATION("Main")
  DCG_END_PROJECT()

  DCG_EXTERNAL_FILES("glad")
  DCG_EXTERNAL_SUBMODULE("GLFW" "https://github.com/glfw/glfw.git")
  DCG_EXTERNAL_SUBMODULE_CUSTOM_CMAKE("stb" "https://github.com/nothings/stb.git")
DCG_END()
