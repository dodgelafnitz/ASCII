/*
 * Copywrite 2022 Dodge Lafnitzegger
 */

#ifndef ASCII_WIDGET_DRAWPARAMS_H
#define ASCII_WIDGET_DRAWPARAMS_H

#include <functional>

#include "Containers/Grid.h"
#include "Math/Vector.h"
#include "Window/Window.h"

struct DrawParams {
  enum DrawFlags {
    IgnoreClamp     = 0x1,
    ForceCharacter  = 0x2,
    ForceBackground = 0x4,
    ForceForeground = 0x8,
    ForceCell       = ForceCharacter | ForceBackground | ForceForeground,
  };

  void Draw(Grid<AsciiCell, 2> & io_screen, ivec2 const & position, AsciiCell const & cell, int flags = 0);

  ivec2 drawOffset;
  ivec2 clampZoneOffset;
  ivec2 clampZoneSize;

  std::function<AsciiCell(AsciiCell const &)> displayMapping;
};


#endif // ASCII_WIDGET_DRAWPARAMS_H
