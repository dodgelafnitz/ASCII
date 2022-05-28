/*
 * Copywrite 2022 Dodge Lafnitzegger
 */

#include "Widget/DrawParams.h"

void DrawParams::Draw(Grid<AsciiCell, 2> & io_screen, ivec2 const & position, AsciiCell const & cell, int flags) {
  ivec2 const drawPosition = position + drawOffset;

  if (drawPosition.x < 0 || drawPosition.x >= io_screen.GetSize().x || drawPosition.y < 0 || drawPosition.y >= io_screen.GetSize().y) {
    return;
  }

  if (!(flags & IgnoreClamp)) {
    if (clampZoneSize.x < 0 || clampZoneSize.y < 0) {
      return;
    }

    ivec2 const max = clampZoneOffset + clampZoneSize;

    if (drawPosition.x < clampZoneOffset.x || drawPosition.x >= max.x || drawPosition.y < clampZoneOffset.y || drawPosition.y >= max.y) {
      return;
    }
  }

  if (displayMapping) {
    AsciiCell drawCell = displayMapping(cell);

    if (flags & ForceCharacter) {
      drawCell.character = cell.character;
    }

    if (flags & ForceForeground) {
      drawCell.foregroundColor = cell.foregroundColor;
    }

    if (flags & ForceBackground) {
      drawCell.backgroundColor = cell.backgroundColor;
    }

    io_screen[drawPosition] = drawCell;
  }
  else {
    io_screen[drawPosition] = cell;
  }
  
}
