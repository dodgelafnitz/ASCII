/*
 * Copywrite 2022 Dodge Lafnitzegger
 */

#include "Widget/DrawParams.h"

DrawParams DrawParams::Constrain(DrawParams const & childParams) const {
  ivec2 const combinedOffset = m_drawOffset + childParams.m_drawOffset;

  ivec2 combinedClampOffset;
  ivec2 combinedClampSize;
  {
    ivec2 const childCorrectedOffset = m_drawOffset + childParams.m_clampZoneOffset;

    ivec2 const clampMin = ivec2(
      m_clampZoneSize.x < 0 ? std::numeric_limits<int>::min() : m_clampZoneOffset.x,
      m_clampZoneSize.y < 0 ? std::numeric_limits<int>::min() : m_clampZoneOffset.y
    );

    ivec2 const childClampMin = ivec2(
      childParams.m_clampZoneSize.x < 0 ? std::numeric_limits<int>::min() : childCorrectedOffset.x,
      childParams.m_clampZoneSize.y < 0 ? std::numeric_limits<int>::min() : childCorrectedOffset.y
    );

    ivec2 const clampMax = ivec2(
      m_clampZoneSize.x < 0 ? std::numeric_limits<int>::max() : m_clampZoneOffset.x + m_clampZoneSize.x,
      m_clampZoneSize.y < 0 ? std::numeric_limits<int>::max() : m_clampZoneOffset.y + m_clampZoneSize.y
    );

    ivec2 const childClampMax = ivec2(
      childParams.m_clampZoneSize.x < 0 ? std::numeric_limits<int>::max() : childCorrectedOffset.x + childParams.m_clampZoneSize.x,
      childParams.m_clampZoneSize.y < 0 ? std::numeric_limits<int>::max() : childCorrectedOffset.y + childParams.m_clampZoneSize.y
    );

    ivec2 const combinedClampMin = clampMin.Max(childClampMin);
    ivec2 const combinedClampMax = clampMax.Min(childClampMax);

    bool const noClampX = combinedClampMin.x == std::numeric_limits<int>::min() || combinedClampMin.x == std::numeric_limits<int>::max();
    bool const noClampY = combinedClampMin.y == std::numeric_limits<int>::min() || combinedClampMin.y == std::numeric_limits<int>::max();

    combinedClampOffset = combinedClampMin;
    combinedClampSize = ivec2(
      noClampX ? NoClamp : combinedClampMax.x - combinedClampMin.x,
      noClampX ? NoClamp : combinedClampMax.y - combinedClampMin.y
    );
  }

  std::shared_ptr<DisplayMapping> newMapping;
  {

    if (m_displayMapping == nullptr) {
      newMapping = childParams.m_displayMapping;
    }
    else {
      if (childParams.m_displayMapping) {
        auto const childDisplayMapping = *childParams.m_displayMapping;
        auto const displayMapping      = *m_displayMapping;

        newMapping = std::make_shared<DisplayMapping>([displayMapping, childDisplayMapping](AsciiCell const & cell) {
          return displayMapping(childDisplayMapping(cell));
        });
      }
      else {
        newMapping = m_displayMapping;
      }
    }
  }

  return DrawParams(
    combinedOffset,
    combinedClampOffset,
    combinedClampSize,
    newMapping,
    m_screen,
    int()
  );
}

void DrawParams::SetCell(ivec2 const & position, AsciiCell const & cell, int flags) const {
  if (m_screen == nullptr) {
    return;
  }

  ivec2 const drawPosition = position + m_drawOffset;

  if (drawPosition.x < 0 || drawPosition.x >= m_screen->GetSize().x || drawPosition.y < 0 || drawPosition.y >= m_screen->GetSize().y) {
    return;
  }

  if (!(flags & IgnoreClamp)) {
    ivec2 const max = m_clampZoneOffset + m_clampZoneSize;

    if (m_clampZoneSize.x >= 0 && (drawPosition.x < m_clampZoneOffset.x || drawPosition.x >= max.x)) {
      return;
    }

    if (m_clampZoneSize.y >= 0 && (drawPosition.y < m_clampZoneOffset.y || drawPosition.y >= max.y)) {
      return;
    }
  }

  if (m_displayMapping) {
    AsciiCell drawCell = (*m_displayMapping)(cell);

    if (flags & ForceCharacter) {
      drawCell.character = cell.character;
    }

    if (flags & ForceForeground) {
      drawCell.foregroundColor = cell.foregroundColor;
    }

    if (flags & ForceBackground) {
      drawCell.backgroundColor = cell.backgroundColor;
    }

    (*m_screen)[drawPosition] = drawCell;
  }
  else {
    (*m_screen)[drawPosition] = cell;
  }
}

void DrawParams::ModifyCell(ivec2 const & position, DisplayMapping const & mapping, int flags) const {
  if (mapping == nullptr) {
    return;
  }

  ivec2 const drawPosition = position + m_drawOffset;

  if (drawPosition.x < 0 || drawPosition.x >= m_screen->GetSize().x || drawPosition.y < 0 || drawPosition.y >= m_screen->GetSize().y) {
    return;
  }

  AsciiCell result = mapping((*m_screen)[drawPosition]);

  SetCell(position, result, flags);
}
