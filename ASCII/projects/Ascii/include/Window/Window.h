/*
 * Copywrite 2021 Dodge Lafnitzegger
 */

#ifndef ASCII_WINDOW_WINDOW_H
#define ASCII_WINDOW_WINDOW_H

#include <string>

#include "Color.h"
#include "Containers/Grid.h"
#include "Math/Vector.h"

static int const FontColorCount = 8;

struct AsciiFont {
  AsciiFont(void) = default;

  ivec2 size;
  Color colors[FontColorCount];
};

struct AsciiCell {
  AsciiCell(void) : AsciiCell(0, 0, 1, 1) {}

  AsciiCell(
    unsigned char foregroundColor,
    unsigned char backgroundColor,
    bool          visible         = true
  ) :
    AsciiCell(foregroundColor, backgroundColor, visible, visible)
  {}

  AsciiCell(
    unsigned char foregroundColor,
    unsigned char backgroundColor,
    bool          foregroundVisible,
    bool          backgroundVisible
  ) :
    foregroundColor(foregroundColor),
    backgroundColor(backgroundColor),
    foregroundAlpha(foregroundVisible),
    backgroundAlpha(backgroundVisible)
  {}

  unsigned char foregroundColor : 3;
  unsigned char backgroundColor : 3;
  unsigned char foregroundAlpha : 1;
  unsigned char backgroundAlpha : 1;
  char          character;
};

struct AsciiInputEvent {
  // TODO
};

class AsciiWindow {
public:
  void Draw(Grid<AsciiCell, 2> const & draw);

  std::vector<AsciiInputEvent> PollInput(void);

  std::string GetClipboard(void) const;
  void SetClipboard(std::string const & clipboard);

  std::string GetTitle(void) const;
  void SetTitle(std::string const & title);

  AsciiFont GetFont(void) const;
  void SetFont(AsciiFont const & font);

  int GetRunMs(void) const;
  void Sleep(int milliseconds);
};

#endif // ASCII_WINDOW_WINDOW_H
