/*
 * Copywrite 2021 Dodge Lafnitzegger
 */

#ifndef ASCII_WINDOW_WINDOW_H
#define ASCII_WINDOW_WINDOW_H

#include <string>

#include "General/Color.h"
#include "Containers/Grid.h"
#include "Math/Vector.h"

static int const FontColorCount = 8;

struct AsciiFont {
  AsciiFont(void) = default;

  ivec2 size;
  Color colors[FontColorCount];
};

struct AsciiCell {
  AsciiCell(void) : AsciiCell(0, 0) {}

  AsciiCell(
    unsigned char foregroundColor,
    unsigned char backgroundColor
  ) :
    foregroundColor(foregroundColor),
    backgroundColor(backgroundColor)
  {}

  unsigned char foregroundColor : 3;
  unsigned char backgroundColor : 3;
  char          character           = ' ';
};

enum class AsciiButton {
  Mouse1,
  Mouse2,
  Mouse3,
  Mouse4,
  Mouse5,
  Key1,
  Key2,
  Key3,
  Key4,
  Key5,
  Key6,
  Key7,
  Key8,
  Key9,
  Key0,
  F1,
  F2,
  F3,
  F4,
  F5,
  F6,
  F7,
  F8,
  F9,
  F10,
  F11,
  F12,
  NumPad0,
  NumPad1,
  NumPad2,
  NumPad3,
  NumPad4,
  NumPad5,
  NumPad6,
  NumPad7,
  NumPad8,
  NumPad9,
  A,
  B,
  C,
  D,
  E,
  F,
  G,
  H,
  I,
  J,
  K,
  L,
  M,
  N,
  O,
  P,
  Q,
  R,
  S,
  T,
  U,
  V,
  W,
  X,
  Y,
  Z,
  Left,
  Right,
  Up,
  Down,
  Delete,
  Backspace,
  Space,
  Tab,
  Return,
  Grave,
  Escape,
  Insert,
  CapsLock,
  Dash,
  Equal,
  LeftBracket,
  RighBracket,
  BackSlash,
  ForwardSlash,
  LeftShift,
  RightShift,
  LeftControl,
  RightControl,
  LeftAlt,
  RightAlt,
  Semicolon,
  Apostrophe,
  Comma,
  Period,
  Home,
  End,
  PageUp,
  PageDown,
  NumLock,
  NumPadDivide,
  NumPadMultiply,
  NumPadMinus,
  NumPadPlus,
  NumPadEnter,
  NumPadPeriod,
};

/*
enum class AsciiState {
  NumLock,
  ScrollLock,
  CapsLock,
  Insert,
  ShiftMod,
  ControlMod,
  AltMod,
};

enum class AsciiTextInputType {
  Character,
  Cut,
  Copy,
  Paste,
  MoveCursor,
  MoveSelection,
  Delete,
  Select,
  Indent,
  Outdent,
};

enum class AsciiTextAmount {
  Character,
  Word,
  Line,
  Page,
  All,
};

struct AsciiInputEvent {
  enum class EventType {
    MouseMove,
    MouseScroll,
    MouseClick,
    MouseHold,
    Button,
    State,
    Text,
  };

  EventType type;
  union {
    struct {
      int x;
      int y;
    } mouseMoveData;

    struct {
      int delta;
      int current;
    } mouseScrollData;

    struct {
      AsciiButton button;
      int         clickCount;
    } mouseClickData;

    struct {
      AsciiButton button;
      int         clickCount;
    } mouseHoldData;

    struct {
      AsciiButton button;
      bool        down;
    } buttonData;

    struct {
      AsciiState state;
      bool       on;
    } stateData;

    struct {
      AsciiTextInputType inputType;
      union {
        char character;

        struct {
          Direction       dir;
          AsciiTextAmount amount;
        } move;

        struct {
          AsciiTextAmount amount;
        } select;
      };
    } textData;
  };
};
*/

enum class AsciiInputType {
  Button,
  Mouse,
};

struct AsciiInputEvent {
  AsciiInputType type;

  union {
    struct {
      AsciiButton button;
      bool        isDown;
    } buttonEvent;

    struct {
      ivec2 position;
      int   scroll;
    } mouseEvent;
  };
};

class IAsciiWindow {
public:
  virtual ~IAsciiWindow(void) = default;

  virtual void Draw(Grid<AsciiCell, 2> const & draw) = 0;

  virtual std::vector<AsciiInputEvent> PollInput(void) = 0;

  virtual std::string GetClipboard(void) const = 0;
  virtual void SetClipboard(std::string const & clipboard) = 0;

  virtual std::string GetTitle(void) const = 0;
  virtual void SetTitle(std::string const & title) = 0;

  virtual AsciiFont GetFont(void) const = 0;
  virtual void SetFont(AsciiFont const & font) = 0;

  virtual int GetRunMs(void) const = 0;
  virtual void Sleep(int milliseconds) = 0;
};

class AsciiWindow : public IAsciiWindow {
public:
  virtual ~AsciiWindow(void) override = default;

  virtual void Draw(Grid<AsciiCell, 2> const & draw) override;

  virtual std::vector<AsciiInputEvent> PollInput(void) override;

  virtual std::string GetClipboard(void) const override;
  virtual void SetClipboard(std::string const & clipboard) override;

  virtual std::string GetTitle(void) const override;
  virtual void SetTitle(std::string const & title) override;

  virtual AsciiFont GetFont(void) const override;
  virtual void SetFont(AsciiFont const & font) override;

  virtual int GetRunMs(void) const override;
  virtual void Sleep(int milliseconds) override;
};

#endif // ASCII_WINDOW_WINDOW_H
