/*
 * Copywrite 2021 Dodge Lafnitzegger
 */

#include "Window/Window.h"

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
