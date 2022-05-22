/*
 * Copywrite 2021 Dodge Lafnitzegger
 */

#include "Window/Window.h"

#ifdef WIN32
  #define _WIN32_TINNT 0x500
  #define WIN32_LEAN_AND_MEAN
  #ifndef NOMINMAX
    #define NOMINMAX
  #endif
  #include <Windows.h>
#endif

#include "glad.h"
#include "GLFW/glfw3.h"
#include "Window/BlockFont.h"

namespace {

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

  static std::vector<AsciiInputEvent> s_pollingInput;

  static const int c_defaultWindowWidth  = 640 * 2;
  static const int c_defaultWindowHeight = 360 * 2;

  static const int c_errorMesageBufferSize = 0x1 << 10;

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
  static_assert(int(AsciiState::Count) == 6);

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
  static_assert(int(AsciiButton::Count) == 103);

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
  static_assert(int(AsciiButton::Count) == 103);

  void nop() {}

  void CheckGlError(void const * func, char const * name) {
    if (func != glad_glGetError) {
      GLint errorCode = glGetError();

      if (errorCode != GLFW_NO_ERROR) {
        nop();
      }
    }
  }

  void CheckGlShaderError(GLuint shader)
  {
    GLint status;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);

    if (status != GL_TRUE) {
      char buffer[c_errorMesageBufferSize];
      int  logLength;
      glGetShaderInfoLog(shader, c_errorMesageBufferSize, &logLength, buffer);

      nop();
    }
  }

  void CheckGlShaderProgramError(GLuint program) {
    GLint status;
    glGetProgramiv(program, GL_LINK_STATUS, &status);

    if (status != GL_TRUE) {
      char buffer[c_errorMesageBufferSize];
      int  logLength;
      glGetProgramInfoLog(program, c_errorMesageBufferSize, &logLength, buffer);

      nop();
    }
  }
}

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
    event.buttonEvent.isDown = (action != GLFW_RELEASE);
    event.buttonEvent.button = GetButtonFromGlfwKey(key);

    s_pollingInput.emplace_back(event);
  }

  static void MouseButtonCallback(GLFWwindow * window, int button, int action, int mods) {
    Impl * impl = reinterpret_cast<Impl *>(glfwGetWindowUserPointer(window));

    EvaluateMods(impl, mods);

    AsciiInputEvent event;
    event.type               = AsciiInputType::Button;
    event.buttonEvent.isDown = (action != GLFW_RELEASE);
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

    Impl * impl = reinterpret_cast<Impl *>(glfwGetWindowUserPointer(window));

    AsciiInputEvent event;

    event.type               = AsciiInputType::MousePosition;

    int width;
    int height;
    glfwGetWindowSize(window, &width, &height);

    int const xCoord = int((xpos / width) * impl->size.x);
    int const yCoord = int((ypos / height) * impl->size.y);

    event.mousePositionEvent = ivec2(xCoord, yCoord);

    s_pollingInput.emplace_back(event);
  }

  static void SetImpl(std::shared_ptr<Impl> const & impl) {
    if (s_impl.expired()) {

#ifdef WIN32
      FreeConsole();
#endif

      s_impl = impl;

      glfwInit();

      glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
      glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
      glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
      glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

      impl->window = glfwCreateWindow(c_defaultWindowWidth, c_defaultWindowHeight, "", nullptr, nullptr);
      glfwMakeContextCurrent(impl->window);

      // Must happen after creating our context current.
      gladLoadGLLoader(GLADloadproc(glfwGetProcAddress));

      // Enable to solve graphics weirdness. Turns out glGetError is expensive.
      //gladSetPostGlCallCallback(CheckGlError);

      glfwSetWindowUserPointer(impl->window, impl.get());

      glfwSetKeyCallback(impl->window, Impl::KeyCallback);
      glfwSetMouseButtonCallback(impl->window, Impl::MouseButtonCallback);
      glfwSetScrollCallback(impl->window, Impl::MouseScrollCallback);
      glfwSetCursorPosCallback(impl->window, Impl::MousePositionCallback);

      glGenVertexArrays(1, &impl->vao);
      glBindVertexArray(impl->vao);

      char const * vertShaderSource = R"(
        #version 330

        uniform ivec2 gridSize;
        uniform ivec2 glyphSize;
        uniform ivec2 fontSheetSize;

        in int   dispChar;
        in ivec2 colorIndices;

        out Data {
          vec2       glyphCenter;
          flat ivec2 colorIndices;
        } outData;

        void main() {
          ivec2 pos = ivec2(gl_VertexID % gridSize.x, gl_VertexID / gridSize.x);

          gl_Position = vec4(
            (float( 2 * pos.x + 1) / float(gridSize.x)) - 1.0,
            (float(-2 * pos.y - 1) / float(gridSize.y)) + 1.0,
            0.0,
            1.0
          );

          ivec2 characterPos = ivec2(dispChar % fontSheetSize.x, dispChar / fontSheetSize.x);
          outData.glyphCenter  = glyphSize * (characterPos + 0.5);
          outData.colorIndices = colorIndices;
        }
      )";

      char const * geoShaderSource = R"(
        #version 330

        uniform ivec2 gridSize;
        uniform ivec2 glyphSize;
        uniform ivec2 fontSheetSize;

        layout(points) in;
        layout(triangle_strip, max_vertices = 4) out;

        in Data {
          vec2       glyphCenter;
          flat ivec2 colorIndices;
        } inData[];

        out Data {
          vec2       texelPos;
          flat ivec2 colorIndices;
        } outData;

        void main() {
          vec2 halfVal     = 1.0 / gridSize;
          vec4 halfSize    = vec4(halfVal.x,  halfVal.y, 0.0, 0.0);
          vec4 halfSizeOff = vec4(halfVal.x, -halfVal.y, 0.0, 0.0);

          vec2 halfGlyphSize    = vec2(glyphSize) * 0.5;
          vec2 halfGlyphSizeOff = vec2(halfGlyphSize.x, -halfGlyphSize.y);

          gl_Position          = gl_in[0].gl_Position - halfSizeOff;
          outData.texelPos     = inData[0].glyphCenter - halfGlyphSize;
          outData.colorIndices = inData[0].colorIndices;
          EmitVertex();

          gl_Position          = gl_in[0].gl_Position - halfSize;
          outData.texelPos     = inData[0].glyphCenter - halfGlyphSizeOff;
          outData.colorIndices = inData[0].colorIndices;
          EmitVertex();

          gl_Position          = gl_in[0].gl_Position + halfSize;
          outData.texelPos     = inData[0].glyphCenter + halfGlyphSizeOff;
          outData.colorIndices = inData[0].colorIndices;
          EmitVertex();

          gl_Position          = gl_in[0].gl_Position + halfSizeOff;
          outData.texelPos     = inData[0].glyphCenter + halfGlyphSize;
          outData.colorIndices = inData[0].colorIndices;
          EmitVertex();

          EndPrimitive();
        }
      )";

      char const * fragShaderSource = R"(
        #version 330

        uniform vec3           colorPalette[8];
        uniform isampler2DRect fontSheet;

        in Data {
          vec2       texelPos;
          flat ivec2 colorIndices;
        } inData;

        out vec4 outColor;

        void main() {
          vec3 backgroundColor = colorPalette[inData.colorIndices[1]];
          vec3 foregroundColor = colorPalette[inData.colorIndices[0]];
          vec3 colorDiff       = foregroundColor - backgroundColor;

          bool shouldDraw = texture(fontSheet, inData.texelPos)[0] > 0;
          //bool shouldDraw = bool(int(inData.texelPos.x + inData.texelPos.y) % 2);
          outColor        = vec4(float(shouldDraw) * colorDiff + backgroundColor, 1.0);
          //outColor        = vec4(inData.texelPos.y / 100, inData.texelPos.y / 100, inData.texelPos.y / 100, 1.0);
        }
      )";

      GLuint vertShader = glCreateShader(GL_VERTEX_SHADER);
      glShaderSource(vertShader, 1, &vertShaderSource, nullptr);
      glCompileShader(vertShader);
      CheckGlShaderError(vertShader);

      GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);
      glShaderSource(fragShader, 1, &fragShaderSource, nullptr);
      glCompileShader(fragShader);
      CheckGlShaderError(fragShader);

      GLuint geoShader = glCreateShader(GL_GEOMETRY_SHADER);
      glShaderSource(geoShader, 1, &geoShaderSource, nullptr);
      glCompileShader(geoShader);
      CheckGlShaderError(geoShader);

      impl->shader = glCreateProgram();
      glAttachShader(impl->shader, vertShader);
      glAttachShader(impl->shader, geoShader);
      glAttachShader(impl->shader, fragShader);
      glLinkProgram(impl->shader);

      glUseProgram(impl->shader);
      CheckGlShaderProgramError(impl->shader);

      glGenBuffers(1, &impl->vertexBuffer);

      GLuint fontSheet;
      glGenTextures(1, &fontSheet);
      glBindTexture(GL_TEXTURE_RECTANGLE, fontSheet);
      glTexImage2D(GL_TEXTURE_RECTANGLE, 0, GL_R8, 16 * GetGlyphWidth(), 16 * GetGlyphHeight(), 0, GL_RED, GL_UNSIGNED_BYTE, GetGlyphSheet());

      glBindFragDataLocation(impl->shader, 0, "outColor");

      glBindBuffer(GL_ARRAY_BUFFER, impl->vertexBuffer);

      impl->charAttr = glGetAttribLocation(impl->shader, "dispChar");
      glEnableVertexAttribArray(impl->charAttr);

      impl->colorAttr = glGetAttribLocation(impl->shader, "colorIndices");
      glEnableVertexAttribArray(impl->colorAttr);

      impl->gridSizeUniform      = glGetUniformLocation(impl->shader, "gridSize");
      impl->glyphSizeUniform     = glGetUniformLocation(impl->shader, "glyphSize");
      impl->fontSheetSizeUniform = glGetUniformLocation(impl->shader, "fontSheetSize");
      impl->colorPaletteUniform  = glGetUniformLocation(impl->shader, "colorPalette");
      impl->fontSheetUniform     = glGetUniformLocation(impl->shader, "fontSheet");
    }
  }

  ~Impl(void) {
    glfwDestroyWindow(window);
    glfwMakeContextCurrent(nullptr);
    glfwTerminate();
  }

  // Only 1 window allowed. Context switching is too expensive (for now).
  static std::weak_ptr<Impl> s_impl;

  GLFWwindow * window                                                                         = nullptr;
  int          startTime                                                                      = 0;
  bool         currentMouseButtons[int(AsciiButton::MouseEnd) - int(AsciiButton::MouseBegin)] = { 0 };
  bool         currentState[int(AsciiState::Count)]                                           = { 0 };
  int          cachedModState                                                                 = 0;
  ivec2        size                                                                           = ivec2(1, 1);
  GLuint       vao                                                                            = GL_INVALID_INDEX;
  GLuint       vertexBuffer                                                                   = GL_INVALID_INDEX;
  GLuint       shader                                                                         = GL_INVALID_INDEX;
  GLint        charAttr                                                                       = GL_INVALID_INDEX;
  GLint        colorAttr                                                                      = GL_INVALID_INDEX;
  GLint        gridSizeUniform                                                                = GL_INVALID_INDEX;
  GLint        glyphSizeUniform                                                               = GL_INVALID_INDEX;
  GLint        fontSheetSizeUniform                                                           = GL_INVALID_INDEX;
  GLint        colorPaletteUniform                                                            = GL_INVALID_INDEX;
  GLint        fontSheetUniform                                                               = GL_INVALID_INDEX;
  AsciiFont    font;
  std::string  name;
};

std::weak_ptr<AsciiWindow::Impl> AsciiWindow::Impl::s_impl;

char const * GetAsciiButtonName(AsciiButton button) {
  return s_asciiButtonNames[int(button) + 1];
}

char const * GetAsciiStateName(AsciiState state) {
  return s_asciiStateNames[int(state)];
}

AsciiWindow::AsciiWindow(void) {
  m_impl = std::make_shared<Impl>();

  if (Impl::s_impl.expired()) {
    Impl::SetImpl(m_impl);
  }

  m_impl->startTime = GetCurrentMs();
}

void AsciiWindow::Draw(Grid<AsciiCell, 2> const & draw) {
  {
    ivec2 const size = draw.GetSize();

    if (m_impl->size != size) {
      m_impl->size = size;

      glfwSetWindowSize(m_impl->window, size.x * GetGlyphWidth(), size.y * GetGlyphHeight());
      glViewport(0, 0, size.x * GetGlyphWidth(), size.y * GetGlyphHeight());
    }

    glBufferData(GL_ARRAY_BUFFER, draw.Count() * sizeof(AsciiCell), draw.Data(), GL_STREAM_DRAW);

    glVertexAttribIPointer(m_impl->charAttr, 1, GL_BYTE, sizeof(AsciiCell), &((AsciiCell *)0)->character);
    glVertexAttribIPointer(m_impl->colorAttr, 2, GL_BYTE, sizeof(AsciiCell), &((AsciiCell *)0)->foregroundColor);

    glUniform2i(m_impl->gridSizeUniform, size.x, size.y);
    glUniform2i(m_impl->glyphSizeUniform, GetGlyphWidth(), GetGlyphHeight());
    glUniform2i(m_impl->fontSheetSizeUniform, 16, 16);

    // Set colors
    {
      float colorPaletteValues[FontColorCount][3]; // rgb for 8 colors

      for (int i = 0; i < FontColorCount; ++i) {
        colorPaletteValues[i][0] = float(m_impl->font.colors[i].r) / 255.0f;
        colorPaletteValues[i][1] = float(m_impl->font.colors[i].g) / 255.0f;
        colorPaletteValues[i][2] = float(m_impl->font.colors[i].b) / 255.0f;
      }

      glUniform3fv(m_impl->colorPaletteUniform, 8, *colorPaletteValues);
    }

    glDrawArrays(GL_POINTS, 0, draw.Count());

    glfwSwapBuffers(m_impl->window);
  }
}

std::vector<AsciiInputEvent> AsciiWindow::PollInput(void) {
  // This can be filled during sleep. We don't clear the input vector at the beginning to queue up those events too.
  glfwPollEvents();

  std::vector<AsciiInputEvent> const forReturn = std::move(s_pollingInput);
  s_pollingInput.clear();

  return forReturn;
}

std::string AsciiWindow::GetClipboard(void) const {
 return glfwGetClipboardString(m_impl->window);
}

void AsciiWindow::SetClipboard(std::string const & clipboardStr) {
  glfwSetClipboardString(m_impl->window, clipboardStr.c_str());
}

std::string AsciiWindow::GetTitle(void) const {
  return m_impl->name;
}

void AsciiWindow::SetTitle(std::string const & title) {
  m_impl->name = title;

  glfwSetWindowTitle(m_impl->window, title.c_str());
}

AsciiFont AsciiWindow::GetFont(void) const {
  return m_impl->font;
}

void AsciiWindow::SetFont(AsciiFont const & font) {
  if (font == m_impl->font) {
    return;
  }

  m_impl->font = font;
}

int AsciiWindow::GetRunMs(void) const {
  return GetCurrentMs() - m_impl->startTime;
}

void AsciiWindow::Sleep(int milliseconds) {
  double currentTime      = glfwGetTime();
  double const targetTime = currentTime + double(milliseconds) / 1000.0;

  do {
    glfwWaitEventsTimeout(targetTime - currentTime);

    currentTime = glfwGetTime();
  } while (currentTime < targetTime);
}

int AsciiWindow::GetCurrentMs(void) const {
  return int(glfwGetTime() * 1000);
}
