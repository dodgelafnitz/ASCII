/*
 * Copywrite 2021 Dodge Lafnitzegger
 */

#include "Window/Window.h"

#include<cstring>

#define _WIN32_TINNT 0x500
#define WIN32_LEAN_AND_MEAN
#ifndef NOMINMAX
  #define NOMINMAX
#endif
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

  AsciiButton GetButtonFromWindowsKey(int key) {
    switch (key) {
      case '0':           return AsciiButton::Key0;
      case '1':           return AsciiButton::Key1;
      case '2':           return AsciiButton::Key2;
      case '3':           return AsciiButton::Key3;
      case '4':           return AsciiButton::Key4;
      case '5':           return AsciiButton::Key5;
      case '6':           return AsciiButton::Key6;
      case '7':           return AsciiButton::Key7;
      case '8':           return AsciiButton::Key8;
      case '9':           return AsciiButton::Key9;
      case VK_F1:         return AsciiButton::F1;
      case VK_F2:         return AsciiButton::F2;
      case VK_F3:         return AsciiButton::F3;
      case VK_F4:         return AsciiButton::F4;
      case VK_F5:         return AsciiButton::F5;
      case VK_F6:         return AsciiButton::F6;
      case VK_F7:         return AsciiButton::F7;
      case VK_F8:         return AsciiButton::F8;
      case VK_F9:         return AsciiButton::F9;
      case VK_F10:        return AsciiButton::F10;
      case VK_F11:        return AsciiButton::F11;
      case VK_F12:        return AsciiButton::F12;
      case VK_NUMPAD0:    return AsciiButton::NumPad0;
      case VK_NUMPAD1:    return AsciiButton::NumPad1;
      case VK_NUMPAD2:    return AsciiButton::NumPad2;
      case VK_NUMPAD3:    return AsciiButton::NumPad3;
      case VK_NUMPAD4:    return AsciiButton::NumPad4;
      case VK_NUMPAD5:    return AsciiButton::NumPad5;
      case VK_NUMPAD6:    return AsciiButton::NumPad6;
      case VK_NUMPAD7:    return AsciiButton::NumPad7;
      case VK_NUMPAD8:    return AsciiButton::NumPad8;
      case VK_NUMPAD9:    return AsciiButton::NumPad9;
      case 'A':           return AsciiButton::A;
      case 'B':           return AsciiButton::B;
      case 'C':           return AsciiButton::C;
      case 'D':           return AsciiButton::D;
      case 'E':           return AsciiButton::E;
      case 'F':           return AsciiButton::F;
      case 'G':           return AsciiButton::G;
      case 'H':           return AsciiButton::H;
      case 'I':           return AsciiButton::I;
      case 'J':           return AsciiButton::J;
      case 'K':           return AsciiButton::K;
      case 'L':           return AsciiButton::L;
      case 'M':           return AsciiButton::M;
      case 'N':           return AsciiButton::N;
      case 'O':           return AsciiButton::O;
      case 'P':           return AsciiButton::P;
      case 'Q':           return AsciiButton::Q;
      case 'R':           return AsciiButton::R;
      case 'S':           return AsciiButton::S;
      case 'T':           return AsciiButton::T;
      case 'U':           return AsciiButton::U;
      case 'V':           return AsciiButton::V;
      case 'W':           return AsciiButton::W;
      case 'X':           return AsciiButton::X;
      case 'Y':           return AsciiButton::Y;
      case 'Z':           return AsciiButton::Z;
      case VK_LEFT:       return AsciiButton::Left;
      case VK_RIGHT:      return AsciiButton::Right;
      case VK_UP:         return AsciiButton::Up;
      case VK_DOWN:       return AsciiButton::Down;
      case VK_DELETE:     return AsciiButton::Delete;
      case VK_BACK:       return AsciiButton::Backspace;
      case VK_SPACE:      return AsciiButton::Space;
      case VK_TAB:        return AsciiButton::Tab;
      case VK_RETURN:     return AsciiButton::Return;
      case VK_OEM_3:      return AsciiButton::Grave;
      case VK_ESCAPE:     return AsciiButton::Escape;
      case VK_INSERT:     return AsciiButton::Insert;
      case VK_CAPITAL:    return AsciiButton::CapsLock;
      case VK_OEM_MINUS:  return AsciiButton::Dash;
      case VK_OEM_PLUS:   return AsciiButton::Equal;
      case VK_OEM_4:      return AsciiButton::LeftBracket;
      case VK_OEM_6:      return AsciiButton::RightBracket;
      case VK_OEM_5:      return AsciiButton::BackSlash;
      case VK_OEM_2:      return AsciiButton::ForwardSlash;
      case VK_LSHIFT:     return AsciiButton::LeftShift;
      case VK_RSHIFT:     return AsciiButton::RightShift;
      case VK_LCONTROL:   return AsciiButton::LeftControl;
      case VK_RCONTROL:   return AsciiButton::RightControl;
      case VK_LMENU:      return AsciiButton::LeftAlt;
      case VK_RMENU:      return AsciiButton::RightAlt;
      case VK_OEM_1:      return AsciiButton::Semicolon;
      case VK_OEM_7:      return AsciiButton::Apostrophe;
      case VK_OEM_COMMA:  return AsciiButton::Comma;
      case VK_OEM_PERIOD: return AsciiButton::Period;
      case VK_HOME:       return AsciiButton::Home;
      case VK_END:        return AsciiButton::End;
      case VK_PRIOR:      return AsciiButton::PageUp;
      case VK_NEXT:       return AsciiButton::PageDown;
      case VK_NUMLOCK:    return AsciiButton::NumLock;
      case VK_DIVIDE:     return AsciiButton::NumPadDivide;
      case VK_MULTIPLY:   return AsciiButton::NumPadMultiply;
      case VK_SUBTRACT:   return AsciiButton::NumPadMinus;
      case VK_ADD:        return AsciiButton::NumPadPlus;
      case VK_DECIMAL:    return AsciiButton::NumPadPeriod;
    }

    return AsciiButton::Invalid;
  }

  int const MaxTitleSize = 0x1000;
}

AsciiWindow::AsciiWindow(void) {
  if (!s_windowInitialized) {
    CONSOLE_CURSOR_INFO cursorInfo;
    cursorInfo.bVisible = FALSE;
    cursorInfo.dwSize = sizeof(cursorInfo);
    SetConsoleCursorInfo(s_outputHandle, &cursorInfo);

    DWORD consoleMode;
    GetConsoleMode(s_inputHandle, &consoleMode);
    SetConsoleMode(s_inputHandle, consoleMode | ENABLE_MOUSE_INPUT);

    LONG const windowStyle = GetWindowLong(s_windowHandle, GWL_STYLE);
    SetWindowLong(s_windowHandle, GWL_STYLE, windowStyle & ~(WS_MAXIMIZEBOX | WS_SIZEBOX));

    s_windowInitialized = true;
  }

  CONSOLE_SCREEN_BUFFER_INFOEX screenInfo = { 0 };
  screenInfo.cbSize = sizeof(screenInfo);
  GetConsoleScreenBufferInfoEx(s_outputHandle, &screenInfo);

  static_assert(FontColorCount <= sizeof(screenInfo.ColorTable) / sizeof(*screenInfo.ColorTable));

  for (int i = 0; i < FontColorCount; ++i) {
    COLORREF const color = screenInfo.ColorTable[i];

    m_font.colors[i].r = GetRValue(color);
    m_font.colors[i].g = GetGValue(color);
    m_font.colors[i].b = GetBValue(color);
  }

  CONSOLE_FONT_INFOEX fontInfo;
  fontInfo.cbSize = sizeof(fontInfo);
  GetCurrentConsoleFontEx(s_outputHandle, FALSE, &fontInfo);

  m_font.size.x = fontInfo.dwFontSize.X;
  m_font.size.y = fontInfo.dwFontSize.Y;

  m_startTime = GetCurrentMs();
}

void AsciiWindow::Draw(Grid<AsciiCell, 2> const & draw) {
  COORD size;
  size.X = draw.GetSize().x;
  size.Y = draw.GetSize().y;

  COORD upperLeft;
  upperLeft.X = 0;
  upperLeft.Y = 0;

  SMALL_RECT grid;
  grid.Top    = 0;
  grid.Left   = 0;
  grid.Bottom = size.Y - 1;
  grid.Right  = size.X - 1;
  
  std::vector<CHAR_INFO> buffer(draw.Count());

  for (ivec2 pos; pos != draw.GetSize(); pos = draw.GetNextCoord(pos, draw.GetSize())) {
    AsciiCell const & cell = draw[pos];
    CHAR_INFO & bufferCell = buffer[pos.x + int(size.X) * pos.y];

    bufferCell.Attributes     = ((0xF & cell.backgroundColor) << 4) | (0xF & cell.foregroundColor);
    bufferCell.Char.AsciiChar = cell.character;
  }

  SetConsoleWindowInfo(s_outputHandle, true, &grid);
  SetConsoleScreenBufferSize(s_outputHandle, size);
  SetConsoleCursorPosition(s_outputHandle, upperLeft);

  WriteConsoleOutput(s_outputHandle, buffer.data(), size, upperLeft, &grid);
}

std::vector<AsciiInputEvent> AsciiWindow::PollInput(void) {
  DWORD eventCount;
  GetNumberOfConsoleInputEvents(s_inputHandle, &eventCount);

  std::vector<INPUT_RECORD> buffer(eventCount);
  ReadConsoleInput(s_inputHandle, buffer.data(), eventCount, &eventCount);

  std::vector<AsciiInputEvent> result;

  bool state[int(AsciiState::Count)];
  for (int i = 0; i < int(AsciiState::Count); ++i) {
    state[i] = m_currentState[i];
  }

  for (INPUT_RECORD const & record : buffer) {
    switch (record.EventType) {
      case KEY_EVENT: {
        AsciiInputEvent event;

        event.type = AsciiInputType::Button;
        event.buttonEvent.button = GetButtonFromWindowsKey(record.Event.KeyEvent.wVirtualKeyCode);
        event.buttonEvent.isDown = record.Event.KeyEvent.bKeyDown;

        result.emplace_back(event);

        state[int(AsciiState::ScrollLock)] = record.Event.KeyEvent.dwControlKeyState & SCROLLLOCK_ON;
        state[int(AsciiState::CapsLock)]   = record.Event.KeyEvent.dwControlKeyState & CAPSLOCK_ON;
        state[int(AsciiState::NumLock)]    = record.Event.KeyEvent.dwControlKeyState & NUMLOCK_ON;

        if (
          record.Event.KeyEvent.bKeyDown && (
            (record.Event.KeyEvent.wVirtualKeyCode == VK_INSERT) ||
            (state[int(AsciiState::NumLock)] && record.Event.KeyEvent.wVirtualKeyCode == VK_NUMPAD0)
          )
        ) {
          state[int(AsciiState::Insert)] = !state[int(AsciiState::Insert)];
        }
        else {
          switch (record.Event.KeyEvent.wVirtualKeyCode) {
            case VK_SHIFT: {
              state[int(AsciiState::Shift)] = record.Event.KeyEvent.bKeyDown;
            } break;
            case VK_CONTROL: {
              state[int(AsciiState::Control)] = record.Event.KeyEvent.bKeyDown;
            } break;
            case VK_MENU: {
              state[int(AsciiState::Alt)] = record.Event.KeyEvent.bKeyDown;
            } break;
          }
        }
      } break;
      case MOUSE_EVENT: {
        if (record.Event.MouseEvent.dwEventFlags & MOUSE_MOVED) {
          AsciiInputEvent event;

          event.type = AsciiInputType::MousePosition;
          event.mousePositionEvent.x = record.Event.MouseEvent.dwMousePosition.X;
          event.mousePositionEvent.y = record.Event.MouseEvent.dwMousePosition.Y;

          result.emplace_back(event);
        }
        if (record.Event.MouseEvent.dwEventFlags & MOUSE_WHEELED) {
          AsciiInputEvent event;

          event.type = AsciiInputType::MouseScroll;
          event.mouseScrollEvent = HIWORD(record.Event.MouseEvent.dwButtonState);

          result.emplace_back(event);
        }

        bool mouseButtonState[int(AsciiButton::MouseCount)];
        mouseButtonState[int(AsciiButton::Mouse1)] = record.Event.MouseEvent.dwButtonState & FROM_LEFT_1ST_BUTTON_PRESSED;
        mouseButtonState[int(AsciiButton::Mouse2)] = record.Event.MouseEvent.dwButtonState & RIGHTMOST_BUTTON_PRESSED;
        mouseButtonState[int(AsciiButton::Mouse3)] = record.Event.MouseEvent.dwButtonState & FROM_LEFT_2ND_BUTTON_PRESSED;
        mouseButtonState[int(AsciiButton::Mouse4)] = record.Event.MouseEvent.dwButtonState & FROM_LEFT_3RD_BUTTON_PRESSED;
        mouseButtonState[int(AsciiButton::Mouse5)] = record.Event.MouseEvent.dwButtonState & FROM_LEFT_4TH_BUTTON_PRESSED;

        for (int i = 0; i < int(AsciiButton::MouseCount); ++i) {
          if (m_currentMouseButtons[i] != mouseButtonState[i]) {
            AsciiInputEvent event;

            event.type = AsciiInputType::Button;
            event.buttonEvent.button = AsciiButton(i);
            event.buttonEvent.isDown = mouseButtonState[i];

            result.emplace_back(event);

            m_currentMouseButtons[i] = mouseButtonState[i];
          }
        }

        state[int(AsciiState::ScrollLock)] = record.Event.MouseEvent.dwControlKeyState & SCROLLLOCK_ON;
        state[int(AsciiState::CapsLock)]   = record.Event.MouseEvent.dwControlKeyState & CAPSLOCK_ON;
        state[int(AsciiState::NumLock)]    = record.Event.MouseEvent.dwControlKeyState & NUMLOCK_ON;

      } break;
    }

    for (int i = 0; i < int(AsciiState::Count); ++i) {
      if (m_currentState[i] != state[i]) {
        AsciiInputEvent event;

        event.type = AsciiInputType::State;
        event.stateEvent.state    = AsciiState(i);
        event.stateEvent.isActive = state[i];

        result.emplace_back(event);

        m_currentState[i] = state[i];
      }
    }
  }

  return result;
}

std::string AsciiWindow::GetClipboard(void) const {
  if (!OpenClipboard(nullptr)) {
    return std::string();
  }

  HANDLE clipboard = GetClipboardData(CF_TEXT);
  if (!clipboard) {
    CloseClipboard();
    return std::string();
  }

  char const * data = (char const *)(GlobalLock(clipboard));

  if (!data) {
    return std::string();
  }

  std::string const result(data);

  GlobalUnlock(clipboard);
  CloseClipboard();

  return result;
}

void AsciiWindow::SetClipboard(std::string const & clipboardStr) {
  if (!OpenClipboard(nullptr)) {
    return;
  }

  EmptyClipboard();

  if (clipboardStr.empty()) {
    CloseClipboard();
    return;
  }

  HGLOBAL clipboard = GlobalAlloc(GMEM_MOVEABLE, clipboardStr.size() + 1);
  if (!clipboard) {
    CloseClipboard();
    return;
  }

  std::memcpy(GlobalLock(clipboard), clipboardStr.c_str(), clipboardStr.size() + 1);
  GlobalUnlock(clipboard);

  SetClipboardData(CF_TEXT, clipboard);

  GlobalFree(clipboard);
  CloseClipboard();
}

std::string AsciiWindow::GetTitle(void) const {
  char buffer[MaxTitleSize];
  GetConsoleTitle(buffer, MaxTitleSize);

  return std::string(buffer);
}

void AsciiWindow::SetTitle(std::string const & title) {
  SetConsoleTitle(title.c_str());
}

AsciiFont AsciiWindow::GetFont(void) const {
  return m_font;
}

void AsciiWindow::SetFont(AsciiFont const & font) {
  if (font == m_font) {
    return;
  }

  CONSOLE_FONT_INFOEX fontInfo;
  fontInfo.cbSize = sizeof(fontInfo);
  fontInfo.dwFontSize.X = font.size.x;
  fontInfo.dwFontSize.Y = font.size.y;

  std::wcscpy(fontInfo.FaceName, L"Terminal");
  fontInfo.FontFamily = FF_DONTCARE;
  fontInfo.FontWeight = FW_NORMAL;
  SetCurrentConsoleFontEx(s_outputHandle, FALSE, &fontInfo);

  CONSOLE_SCREEN_BUFFER_INFOEX screenInfo = { 0 };
  screenInfo.cbSize = sizeof(screenInfo);
  GetConsoleScreenBufferInfoEx(s_outputHandle, &screenInfo);

  for (int i = 0; i < FontColorCount; ++i) {
    COLORREF const color = RGB(m_font.colors[i].r, m_font.colors[i].g, m_font.colors[i].b);

    screenInfo.ColorTable[i] = color;
  }

  SetConsoleScreenBufferInfoEx(s_outputHandle, &screenInfo);

  m_font = font;
}

int AsciiWindow::GetRunMs(void) const {
  return GetCurrentMs() - m_startTime;
}

void AsciiWindow::Sleep(int milliseconds) {
  ::Sleep(milliseconds);
}

int AsciiWindow::GetCurrentMs(void) const {
  LARGE_INTEGER counter;
  if (!QueryPerformanceCounter(&counter)) {
    return 0;
  }

  LARGE_INTEGER frequency;
  if (!QueryPerformanceFrequency(&frequency)) {
    return 0;
  }

  return int((counter.QuadPart * 1000) / frequency.QuadPart);
}
