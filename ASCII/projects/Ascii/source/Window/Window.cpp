/*
 * Copywrite 2021 Dodge Lafnitzegger
 */

#include "Window/Window.h"

#define _WIN32_TINNT 0x500
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <Windows.h>

/*

Make Cursor Invisile:
  CONSOLE_CURSOR_INFO
  SetConsoleCursor()

Enable Mouse
  ENABLE_MOUSE_INPUT
  SetConsoleModw()

Set Console Properties
  WS_MAXIMIZEBOX
  WS_SIZEBOX
  GWL_STYLE
  SetWindowLong()

Get Time
  QueryPerformanceCounter()
  QueryPerformanceFrequency()

Set Font
  CONSOLE_FONT_INFOEX
  L"Terminal"
  FF_DONTCARE
  FW_NORMAL
  SetCurrentConsoleFontEx()

Draw
  WriteConsoleOutput()

Get Input
  GetNumberOfConsoleInputEvents()
  ReadConsoleInput()

Get or Set Clipboard
  OpenClipboard()
  CF_TEXT
  GetClipboardData()
  GMEM_MOVABLE
  GlobalAlloc()
  SetClipboardData()
  GlobalLock()
  GlobalUnlock()
  CloseClipboard

Set Title
  SetConsoleTitle()

*/

namespace {
  bool s_windowInitialized = false;

  HANDLE const s_outputHandle = GetStdHandle(STD_OUTPUT_HANDLE);
  HANDLE const s_inputHandle  = GetStdHandle(STD_INPUT_HANDLE);
  HWND const   s_windowHandle = GetConsoleWindow();


}

AsciiWindow::AsciiWindow(void) {
  if (!s_windowInitialized) {

    CONSOLE_CURSOR_INFO cursorInfo;
    cursorInfo.bVisible = FALSE;
    cursorInfo.dwSize = sizeof(cursorInfo);
    SetConsoleCursorInfo(s_outputHandle, &cursorInfo);

    //todo

    s_windowInitialized = true;
  }
}

void AsciiWindow::Draw(Grid<AsciiCell, 2> const & draw) {
}
