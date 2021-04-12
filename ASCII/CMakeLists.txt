cmake_minimum_required(VERSION 3.19.0)

project(ASCII C CXX)

set("ENV{DCG_ENABLE_TESTS}" TRUE)
set("ENV{DCG_DEBUG_PRINT_STRUCTURE}" TRUE)

string(TIMESTAMP currentYear "%Y")
set("ENV{DCG_COPYWRITE}" "/*\n * Copywrite ${currentYear} Dodge Lafnitzegger\n */\n\n")

include("./dcg_setup.cmake")

DCG_BEGIN()
  DCG_PROJECT_INTERFACE("DcUtility")
    DCG_FOLDER("Math")
      DCG_FILE_CPP_HEADER_ONLY("Vector")
      DCG_FILE_CPP_HEADER_ONLY("Matrix")
    DCG_END_FOLDER()
    DCG_FILE_CPP_HEADER_ONLY("Debug")
  DCG_END_PROJECT()

  DCG_PROJECT_LIB("Ascii" PUBLIC_DEPENDS "DcUtility")
    DCG_FOLDER("Window")
      DCG_FILE_CPP("Window")
    DCG_END_FOLDER()
    DCG_FOLDER("Widget")
      DCG_FILE_CPP("Widget")
    DCG_END_FOLDER()
    DCG_FOLDER("Systems")
      DCG_FILE_CPP("ButtonManager")
      DCG_FILE_CPP("FocusManager")
      DCG_FILE_CPP("MouseManager")
      DCG_FILE_CPP("TextManager")
    DCG_END_FOLDER()
  DCG_END_PROJECT()

  DCG_PROJECT_EXE("Game" PRIVATE_DEPENDS "Ascii")
    DCG_FILE_CPP_APPLICATION("Main")
    DCG_ASSET("assets/file.txt")
  DCG_END_PROJECT()

  DCG_EXTERNAL_FILES("ExternTest")
  #DCG_EXTERNAL_SUBMODULE("GoogleTest" "https://github.com/google/googletest.git")
DCG_END()
