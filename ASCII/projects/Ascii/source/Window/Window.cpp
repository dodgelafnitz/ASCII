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

#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "Window/BlockFont.h"

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
      case VK_NUMLOCK:    return AsciiButton::NumLock;
      case VK_DIVIDE:     return AsciiButton::NumPadDivide;
      case VK_MULTIPLY:   return AsciiButton::NumPadMultiply;
      case VK_SUBTRACT:   return AsciiButton::NumPadMinus;
      case VK_ADD:        return AsciiButton::NumPadPlus;
      case VK_DECIMAL:    return AsciiButton::NumPadPeriod;
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
    }

    return AsciiButton::Invalid;
  }

  int GetGlfwModFromAsciiState(AsciiState state) {
    switch (state) {
      case AsciiState::CapsLock:    return GLFW_MOD_CAPS_LOCK;
      case AsciiState::NumLock:     return GLFW_MOD_NUM_LOCK;
      case AsciiState::Shift:       return GLFW_MOD_SHIFT;
      case AsciiState::Control:     return GLFW_MOD_CONTROL;
      case AsciiState::Alt:         return GLFW_MOD_ALT;
      default:                      return 0;
    }
  }

  AsciiButton GetButtonFromGlfwKey(int key) {
    switch (key) {
      case GLFW_KEY_0:             return AsciiButton::Key0;
      case GLFW_KEY_1:             return AsciiButton::Key1;
      case GLFW_KEY_2:             return AsciiButton::Key2;
      case GLFW_KEY_3:             return AsciiButton::Key3;
      case GLFW_KEY_4:             return AsciiButton::Key4;
      case GLFW_KEY_5:             return AsciiButton::Key5;
      case GLFW_KEY_6:             return AsciiButton::Key6;
      case GLFW_KEY_7:             return AsciiButton::Key7;
      case GLFW_KEY_8:             return AsciiButton::Key8;
      case GLFW_KEY_9:             return AsciiButton::Key9;
      case GLFW_KEY_F1:            return AsciiButton::F1;
      case GLFW_KEY_F2:            return AsciiButton::F2;
      case GLFW_KEY_F3:            return AsciiButton::F3;
      case GLFW_KEY_F4:            return AsciiButton::F4;
      case GLFW_KEY_F5:            return AsciiButton::F5;
      case GLFW_KEY_F6:            return AsciiButton::F6;
      case GLFW_KEY_F7:            return AsciiButton::F7;
      case GLFW_KEY_F8:            return AsciiButton::F8;
      case GLFW_KEY_F9:            return AsciiButton::F9;
      case GLFW_KEY_F10:           return AsciiButton::F10;
      case GLFW_KEY_F11:           return AsciiButton::F11;
      case GLFW_KEY_F12:           return AsciiButton::F12;
      case GLFW_KEY_KP_0:          return AsciiButton::NumPad0;
      case GLFW_KEY_KP_1:          return AsciiButton::NumPad1;
      case GLFW_KEY_KP_2:          return AsciiButton::NumPad2;
      case GLFW_KEY_KP_3:          return AsciiButton::NumPad3;
      case GLFW_KEY_KP_4:          return AsciiButton::NumPad4;
      case GLFW_KEY_KP_5:          return AsciiButton::NumPad5;
      case GLFW_KEY_KP_6:          return AsciiButton::NumPad6;
      case GLFW_KEY_KP_7:          return AsciiButton::NumPad7;
      case GLFW_KEY_KP_8:          return AsciiButton::NumPad8;
      case GLFW_KEY_KP_9:          return AsciiButton::NumPad9;
      case GLFW_KEY_NUM_LOCK:      return AsciiButton::NumLock;
      case GLFW_KEY_KP_DIVIDE:     return AsciiButton::NumPadDivide;
      case GLFW_KEY_KP_MULTIPLY:   return AsciiButton::NumPadMultiply;
      case GLFW_KEY_KP_SUBTRACT:   return AsciiButton::NumPadMinus;
      case GLFW_KEY_KP_ADD:        return AsciiButton::NumPadPlus;
      case GLFW_KEY_KP_DECIMAL:    return AsciiButton::NumPadPeriod;
      case GLFW_KEY_A:             return AsciiButton::A;
      case GLFW_KEY_B:             return AsciiButton::B;
      case GLFW_KEY_C:             return AsciiButton::C;
      case GLFW_KEY_D:             return AsciiButton::D;
      case GLFW_KEY_E:             return AsciiButton::E;
      case GLFW_KEY_F:             return AsciiButton::F;
      case GLFW_KEY_G:             return AsciiButton::G;
      case GLFW_KEY_H:             return AsciiButton::H;
      case GLFW_KEY_I:             return AsciiButton::I;
      case GLFW_KEY_J:             return AsciiButton::J;
      case GLFW_KEY_K:             return AsciiButton::K;
      case GLFW_KEY_L:             return AsciiButton::L;
      case GLFW_KEY_M:             return AsciiButton::M;
      case GLFW_KEY_N:             return AsciiButton::N;
      case GLFW_KEY_O:             return AsciiButton::O;
      case GLFW_KEY_P:             return AsciiButton::P;
      case GLFW_KEY_Q:             return AsciiButton::Q;
      case GLFW_KEY_R:             return AsciiButton::R;
      case GLFW_KEY_S:             return AsciiButton::S;
      case GLFW_KEY_T:             return AsciiButton::T;
      case GLFW_KEY_U:             return AsciiButton::U;
      case GLFW_KEY_V:             return AsciiButton::V;
      case GLFW_KEY_W:             return AsciiButton::W;
      case GLFW_KEY_X:             return AsciiButton::X;
      case GLFW_KEY_Y:             return AsciiButton::Y;
      case GLFW_KEY_Z:             return AsciiButton::Z;
      case GLFW_KEY_LEFT:          return AsciiButton::Left;
      case GLFW_KEY_RIGHT:         return AsciiButton::Right;
      case GLFW_KEY_UP:            return AsciiButton::Up;
      case GLFW_KEY_DOWN:          return AsciiButton::Down;
      case GLFW_KEY_DELETE:        return AsciiButton::Delete;
      case GLFW_KEY_BACKSPACE:     return AsciiButton::Backspace;
      case GLFW_KEY_SPACE:         return AsciiButton::Space;
      case GLFW_KEY_TAB:           return AsciiButton::Tab;
      case GLFW_KEY_ENTER:         return AsciiButton::Return;
      case GLFW_KEY_GRAVE_ACCENT:  return AsciiButton::Grave;
      case GLFW_KEY_ESCAPE:        return AsciiButton::Escape;
      case GLFW_KEY_INSERT:        return AsciiButton::Insert;
      case GLFW_KEY_CAPS_LOCK:     return AsciiButton::CapsLock;
      case GLFW_KEY_MINUS:         return AsciiButton::Dash;
      case GLFW_KEY_EQUAL:         return AsciiButton::Equal;
      case GLFW_KEY_LEFT_BRACKET:  return AsciiButton::LeftBracket;
      case GLFW_KEY_RIGHT_BRACKET: return AsciiButton::RightBracket;
      case GLFW_KEY_BACKSLASH:     return AsciiButton::BackSlash;
      case GLFW_KEY_SLASH:         return AsciiButton::ForwardSlash;
      case GLFW_KEY_LEFT_SHIFT:    return AsciiButton::LeftShift;
      case GLFW_KEY_RIGHT_SHIFT:   return AsciiButton::RightShift;
      case GLFW_KEY_LEFT_CONTROL:  return AsciiButton::LeftControl;
      case GLFW_KEY_RIGHT_CONTROL: return AsciiButton::RightControl;
      case GLFW_KEY_LEFT_ALT:      return AsciiButton::LeftAlt;
      case GLFW_KEY_RIGHT_ALT:     return AsciiButton::RightAlt;
      case GLFW_KEY_SEMICOLON:     return AsciiButton::Semicolon;
      case GLFW_KEY_APOSTROPHE:    return AsciiButton::Apostrophe;
      case GLFW_KEY_COMMA:         return AsciiButton::Comma;
      case GLFW_KEY_PERIOD:        return AsciiButton::Period;
      case GLFW_KEY_HOME:          return AsciiButton::Home;
      case GLFW_KEY_END:           return AsciiButton::End;
      case GLFW_KEY_PAGE_UP:       return AsciiButton::PageUp;
      case GLFW_KEY_PAGE_DOWN:     return AsciiButton::PageDown;
      default:                     return AsciiButton::Count;
    }
  }

  AsciiButton GetButtonFromGlfwMouseButton(int mouse) {
    switch (mouse) {
      case GLFW_MOUSE_BUTTON_1: return AsciiButton::Mouse1;
      case GLFW_MOUSE_BUTTON_2: return AsciiButton::Mouse2;
      case GLFW_MOUSE_BUTTON_3: return AsciiButton::Mouse3;
      case GLFW_MOUSE_BUTTON_4: return AsciiButton::Mouse4;
      case GLFW_MOUSE_BUTTON_5: return AsciiButton::Mouse5;
      default:                  return AsciiButton::Count;
    }
  }

  int const MaxTitleSize = 0x1000;

  char const * s_asciiButtonNames[int(AsciiButton::Count) + 2] = {
    "Invalid",
    "Mouse1",
    "Mouse2",
    "Mouse3",
    "Mouse4",
    "Mouse5",
    "Key1",
    "Key2",
    "Key3",
    "Key4",
    "Key5",
    "Key6",
    "Key7",
    "Key8",
    "Key9",
    "Key0",
    "F1",
    "F2",
    "F3",
    "F4",
    "F5",
    "F6",
    "F7",
    "F8",
    "F9",
    "F10",
    "F11",
    "F12",
    "NumPad0",
    "NumPad1",
    "NumPad2",
    "NumPad3",
    "NumPad4",
    "NumPad5",
    "NumPad6",
    "NumPad7",
    "NumPad8",
    "NumPad9",
    "NumLock",
    "NumPadDivide",
    "NumPadMultiply",
    "NumPadMinus",
    "NumPadPlus",
    "NumPadEnter",
    "NumPadPeriod",
    "A",
    "B",
    "C",
    "D",
    "E",
    "F",
    "G",
    "H",
    "I",
    "J",
    "K",
    "L",
    "M",
    "N",
    "O",
    "P",
    "Q",
    "R",
    "S",
    "T",
    "U",
    "V",
    "W",
    "X",
    "Y",
    "Z",
    "Left",
    "Right",
    "Up",
    "Down",
    "Delete",
    "Backspace",
    "Space",
    "Tab",
    "Return",
    "Grave",
    "Escape",
    "Insert",
    "CapsLock",
    "Dash",
    "Equal",
    "LeftBracket",
    "RightBracket",
    "BackSlash",
    "ForwardSlash",
    "LeftShift",
    "RightShift",
    "LeftControl",
    "RightControl",
    "LeftAlt",
    "RightAlt",
    "Semicolon",
    "Apostrophe",
    "Comma",
    "Period",
    "Home",
    "End",
    "PageUp",
    "PageDown",
    "Count",
  };
  static_assert(int(AsciiButton::Count) == 103);

  char const * s_asciiStateNames[int(AsciiState::Count) + 1] = {
    "CapsLock",
    "NumLock",
    "Insert",
    "Shift",
    "Control",
    "Alt",
    "Count",
  };
  static_assert(int(AsciiState::Count) == 6);

  static int  s_windowCount = 0;
  static bool s_gladInitialized = false;

  static std::vector<AsciiInputEvent> s_pollingInput;

  static int c_defaultWindowWidth  = 640;
  static int c_defaultWindowHeight = 360;
}

GLFWkeyfun;

struct AsciiWindow::Impl {
  static void EvaluateMods(Impl * impl, int mods) {
    if (mods != impl->cachedModState) {
      for (int i = 0; i < int(AsciiState::Count); ++i) {
        AsciiState const state = AsciiState(i);
        int const glfwMod = GetGlfwModFromAsciiState(state);
        if (glfwMod == 0) {
          continue;
        }

        bool const isActive = mods & glfwMod;

        if (isActive != impl->currentState[i]) {
          AsciiInputEvent newStateEvent;

          newStateEvent.type = AsciiInputType::State;
          newStateEvent.stateEvent.isActive = isActive;
          newStateEvent.stateEvent.state = state;

          impl->currentState[i] = isActive;
          s_pollingInput.emplace_back(newStateEvent);
        }
      }

      impl->cachedModState = mods;
    }
  }

  static void KeyCallback(GLFWwindow * window, int key, int scancode, int action, int mods) {
    Impl * impl = reinterpret_cast<Impl *>(glfwGetWindowUserPointer(window));

    EvaluateMods(impl, mods);

    AsciiInputEvent event;
    event.type               = AsciiInputType::Button;
    event.buttonEvent.isDown = (action == GLFW_PRESS);
    event.buttonEvent.button = GetButtonFromGlfwKey(key);

    s_pollingInput.emplace_back(event);
  }

  static void MouseButtonCallback(GLFWwindow * window, int button, int action, int mods) {
    Impl * impl = reinterpret_cast<Impl *>(glfwGetWindowUserPointer(window));

    EvaluateMods(impl, mods);

    AsciiInputEvent event;
    event.type               = AsciiInputType::Button;
    event.buttonEvent.isDown = (action == GLFW_PRESS);
    event.buttonEvent.button = GetButtonFromGlfwMouseButton(button);

    s_pollingInput.emplace_back(event);
  }

  static void MouseScrollCallback(GLFWwindow * window, double xoffset, double yoffset) {
    AsciiInputEvent event;

    event.type             = AsciiInputType::MouseScroll;
    event.mouseScrollEvent = int(yoffset);

    s_pollingInput.emplace_back(event);
  }

  static void MousePositionCallback(GLFWwindow * window, double xpos, double ypos) {
    AsciiInputEvent event;

    event.type               = AsciiInputType::MousePosition;
    event.mousePositionEvent = ivec2(int(xpos), int(ypos)); // TODO: Scaling based on token size/font and size of window.

    s_pollingInput.emplace_back(event);
  }

  GLFWwindow * window                                                                         = nullptr;
  int          startTime                                                                      = 0;
  bool         currentMouseButtons[int(AsciiButton::MouseEnd) - int(AsciiButton::MouseBegin)] = { 0 };
  bool         currentState[int(AsciiState::Count)]                                           = { 0 };
  int          cachedModState                                                                 = 0;
  AsciiFont    font;
  std::string  name;
};


char const * GetAsciiButtonName(AsciiButton button) {
  return s_asciiButtonNames[int(button) + 1];
}

char const * GetAsciiStateName(AsciiState state) {
  return s_asciiStateNames[int(state)];
}

AsciiWindow::AsciiWindow(void) {
  if (s_windowCount == 0) {
    glfwInit();
  }
  ++s_windowCount;

  m_impl = std::make_shared<Impl>();

  m_impl->window = glfwCreateWindow(c_defaultWindowWidth, c_defaultWindowHeight, "", nullptr, nullptr);

  glfwMakeContextCurrent(m_impl->window);

  // must happen after creating our first window.
  if (!s_gladInitialized) {
    gladLoadGLLoader(GLADloadproc(glfwGetProcAddress));
    s_gladInitialized = true;
  }

  glfwSetWindowUserPointer(m_impl->window, m_impl.get());

  glfwSetKeyCallback(m_impl->window, Impl::KeyCallback);
  glfwSetMouseButtonCallback(m_impl->window, Impl::MouseButtonCallback);
  glfwSetScrollCallback(m_impl->window, Impl::MouseScrollCallback);
  glfwSetCursorPosCallback(m_impl->window, Impl::MousePositionCallback);

  glfwMakeContextCurrent(nullptr);

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

    m_impl->font.colors[i].r = GetRValue(color);
    m_impl->font.colors[i].g = GetGValue(color);
    m_impl->font.colors[i].b = GetBValue(color);
  }

  CONSOLE_FONT_INFOEX fontInfo;
  fontInfo.cbSize = sizeof(fontInfo);
  GetCurrentConsoleFontEx(s_outputHandle, FALSE, &fontInfo);

  m_impl->font.size.x = fontInfo.dwFontSize.X;
  m_impl->font.size.y = fontInfo.dwFontSize.Y;

  m_impl->startTime = GetCurrentMs();
}

AsciiWindow::~AsciiWindow(void) {
  glfwDestroyWindow(m_impl->window);

  if (--s_windowCount == 0) {
    glfwTerminate();
  }
}

void AsciiWindow::Draw(Grid<AsciiCell, 2> const & draw) {
  {
    ivec2 const size = draw.GetSize() * ivec2(GetGlyphWidth(), GetGlyphHeight());

    glfwSetWindowAspectRatio(m_impl->window, draw.GetSize().x, draw.GetSize().y);

    // Set every pixel in the frame buffer to the current clear color.
    glClear(GL_COLOR_BUFFER_BIT);

    // Drawing is done by specifying a sequence of vertices.  The way these
    // vertices are connected (or not connected) depends on the argument to
    // glBegin.  GL_POLYGON constructs a filled polygon.
    glBegin(GL_POLYGON);
      glColor3f(1, 0, 0); glVertex3f(-0.6, -0.75, 0.5);
      glColor3f(0, 1, 0); glVertex3f(0.6, -0.75, 0);
      glColor3f(0, 0, 1); glVertex3f(0, 0.75, 0);
    glEnd();

  }

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
  s_pollingInput.clear();
  glfwPollEvents();

  DWORD eventCount;
  GetNumberOfConsoleInputEvents(s_inputHandle, &eventCount);

  std::vector<INPUT_RECORD> buffer(eventCount);
  ReadConsoleInput(s_inputHandle, buffer.data(), eventCount, &eventCount);

  std::vector<AsciiInputEvent> result;

  bool state[int(AsciiState::Count)];
  for (int i = 0; i < int(AsciiState::Count); ++i) {
    state[i] = m_impl->currentState[i];
  }

  for (INPUT_RECORD const & record : buffer) {
    switch (record.EventType) {
      case KEY_EVENT: {
        AsciiInputEvent event;

        event.type = AsciiInputType::Button;
        event.buttonEvent.button = GetButtonFromWindowsKey(record.Event.KeyEvent.wVirtualKeyCode);
        event.buttonEvent.isDown = record.Event.KeyEvent.bKeyDown;

        result.emplace_back(event);

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

        int const c_mouseCount = int(AsciiButton::MouseEnd) - int(AsciiButton::MouseBegin);

        bool mouseButtonState[c_mouseCount];
        mouseButtonState[int(AsciiButton::Mouse1) - int(AsciiButton::MouseBegin)] = record.Event.MouseEvent.dwButtonState & FROM_LEFT_1ST_BUTTON_PRESSED;
        mouseButtonState[int(AsciiButton::Mouse2) - int(AsciiButton::MouseBegin)] = record.Event.MouseEvent.dwButtonState & RIGHTMOST_BUTTON_PRESSED;
        mouseButtonState[int(AsciiButton::Mouse3) - int(AsciiButton::MouseBegin)] = record.Event.MouseEvent.dwButtonState & FROM_LEFT_2ND_BUTTON_PRESSED;
        mouseButtonState[int(AsciiButton::Mouse4) - int(AsciiButton::MouseBegin)] = record.Event.MouseEvent.dwButtonState & FROM_LEFT_3RD_BUTTON_PRESSED;
        mouseButtonState[int(AsciiButton::Mouse5) - int(AsciiButton::MouseBegin)] = record.Event.MouseEvent.dwButtonState & FROM_LEFT_4TH_BUTTON_PRESSED;

        for (int i = 0; i < c_mouseCount; ++i) {
          if (m_impl->currentMouseButtons[i] != mouseButtonState[i]) {
            AsciiInputEvent event;

            event.type = AsciiInputType::Button;
            event.buttonEvent.button = AsciiButton(i + int(AsciiButton::MouseBegin));
            event.buttonEvent.isDown = mouseButtonState[i];

            result.emplace_back(event);

            m_impl->currentMouseButtons[i] = mouseButtonState[i];
          }
        }

        state[int(AsciiState::CapsLock)]   = record.Event.MouseEvent.dwControlKeyState & CAPSLOCK_ON;
        state[int(AsciiState::NumLock)]    = record.Event.MouseEvent.dwControlKeyState & NUMLOCK_ON;

      } break;
    }

    for (int i = 0; i < int(AsciiState::Count); ++i) {
      if (m_impl->currentState[i] != state[i]) {
        AsciiInputEvent event;

        event.type = AsciiInputType::State;
        event.stateEvent.state    = AsciiState(i);
        event.stateEvent.isActive = state[i];

        result.emplace_back(event);

        m_impl->currentState[i] = state[i];
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
  return m_impl->name;
}

void AsciiWindow::SetTitle(std::string const & title) {
  m_impl->name = title;

  glfwSetWindowTitle(m_impl->window, title.c_str());
  SetConsoleTitle(title.c_str());
}

AsciiFont AsciiWindow::GetFont(void) const {
  return m_impl->font;
}

void AsciiWindow::SetFont(AsciiFont const & font) {
  if (font == m_impl->font) {
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
    COLORREF const color = RGB(m_impl->font.colors[i].r, m_impl->font.colors[i].g, m_impl->font.colors[i].b);

    screenInfo.ColorTable[i] = color;
  }

  SetConsoleScreenBufferInfoEx(s_outputHandle, &screenInfo);

  m_impl->font = font;
}

int AsciiWindow::GetRunMs(void) const {
  return GetCurrentMs() - m_impl->startTime;
}

void AsciiWindow::Sleep(int milliseconds) {
  ::Sleep(milliseconds);
}

int AsciiWindow::GetCurrentMs(void) const {
  return int(glfwGetTime() * 1000);
}
