/*
 * Copywrite 2022 Dodge Lafnitzegger
 */

#ifndef ASCII_WIDGET_DRAWPARAMS_H
#define ASCII_WIDGET_DRAWPARAMS_H

#include <functional>

#include "Containers/Grid.h"
#include "Math/Vector.h"
#include "Window/Window.h"

class DrawParams {
public:
  using DisplayMapping = std::function<AsciiCell(AsciiCell const &)>;

  enum DrawFlags {
    IgnoreClamp     = 0x1,
    ForceCharacter  = 0x2,
    ForceBackground = 0x4,
    ForceForeground = 0x8,
    ForceCell       = ForceCharacter | ForceBackground | ForceForeground,
  };

  static int const NoClamp = -1;

  DrawParams(void) = default;
  DrawParams(DrawParams const &) = default;
  DrawParams(DrawParams &&) = default;

  DrawParams(
    ivec2 const &          drawOffset,
    ivec2 const &          clampZoneOffset,
    ivec2 const &          clampZoneSize,
    DisplayMapping const & displayMapping,
    Grid<AsciiCell, 2> *   screen
  ) :
    DrawParams(
      drawOffset,
      clampZoneOffset,
      clampZoneSize,
      displayMapping ? std::make_shared<DisplayMapping>(displayMapping) : nullptr,
      screen,
      int()
    )
  {}

  DrawParams(
    ivec2 const &          drawOffset,
    DisplayMapping const & displayMapping = nullptr
  ) :
    DrawParams(drawOffset, ivec2(), ivec2(NoClamp, NoClamp), displayMapping, nullptr)
  {}

  DrawParams(
    ivec2 const &          drawOffset,
    ivec2 const &          clampZoneOffset,
    ivec2 const &          clampZoneSize,
    DisplayMapping const & displayMapping = nullptr
  ) :
    DrawParams(drawOffset, clampZoneOffset, clampZoneSize, displayMapping, nullptr)
  {}

  DrawParams & operator =(DrawParams const &) = default;
  DrawParams & operator =(DrawParams &&) = default;

  bool operator ==(DrawParams const &) const = default;

  DrawParams Constrain(DrawParams const & childParams) const;

  void SetCell(ivec2 const & position, AsciiCell const & cell, int flags = 0) const;
  void ModifyCell(ivec2 const & position, DisplayMapping const & mapping, int flags = 0) const;

private:

  DrawParams(
    ivec2 const &                           drawOffset,
    ivec2 const &                           clampZoneOffset,
    ivec2 const &                           clampZoneSize,
    std::shared_ptr<DisplayMapping> const & displayMapping,
    Grid<AsciiCell, 2> *                    screen,
    int
  ) :
    m_drawOffset(drawOffset),
    m_clampZoneOffset(clampZoneOffset),
    m_clampZoneSize(clampZoneSize),
    m_displayMapping(displayMapping),
    m_screen(screen)
  {}

  ivec2 m_drawOffset;
  ivec2 m_clampZoneOffset;
  ivec2 m_clampZoneSize   = ivec2(NoClamp, NoClamp);

  std::shared_ptr<DisplayMapping> m_displayMapping;

  Grid<AsciiCell, 2> * m_screen = nullptr;
};

#endif // ASCII_WIDGET_DRAWPARAMS_H
