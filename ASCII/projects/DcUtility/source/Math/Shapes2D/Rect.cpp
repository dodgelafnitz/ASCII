/*
 * Copywrite 2021 Dodge Lafnitzegger
 */

#include "Math/Shapes2D/Rect.h"

#include <cmath>

Rect::Rect(fvec2 const & center, fvec2 const & dimensions) :
  m_center(center),
  m_dims(std::abs(dimensions.x), std::abs(dimensions.y))
{}

fvec2 Rect::GetCenter(void) const {
  return m_center;
}

void Rect::SetCenter(fvec2 const & center) {
  m_center = center;
}

fvec2 Rect::GetDimensions(void) const {
  return m_dims;
}

void Rect::SetDimensions(fvec2 const & dimensions) {
  m_dims.x = std::abs(dimensions.x);
  m_dims.y = std::abs(dimensions.y);
}

bool Rect::Contains(fvec2 const & target) const {
  return Clamp(target) == target;
}

float Rect::DistanceTo(fvec2 const & target) const {
  return (target - Clamp(target)).Length();
}

fvec2 Rect::Clamp(fvec2 const & target) const {
  fvec2 const halfDims = m_dims * 0.5f;

  return fvec2(
    std::min(std::max(target.x, m_center.x - halfDims.x), m_center.x + halfDims.x),
    std::min(std::max(target.y, m_center.y - halfDims.y), m_center.y + halfDims.y)
  );
}

fvec2 Rect::ClampToBoundary(fvec2 const & target) const {
  fvec2 const clamp = Clamp(target);

  if (clamp != target) {
    return clamp;
  }

  float const nearX = m_center.x + m_dims.x * (target.x < m_center.x ? -0.5f : 0.5f);
  float const nearY = m_center.y + m_dims.y * (target.y < m_center.y ? -0.5f : 0.5f);

  float const xDiff = std::abs(target.x - nearX);
  float const yDiff = std::abs(target.y - nearY);

  if (xDiff < yDiff) {
    return fvec2(nearX, target.y);
  }
  else {
    return fvec2(target.x, nearY);
  }
}
