/*
 * Copywrite 2021 Dodge Lafnitzegger
 */

#include "Math/Shapes2D/Circle.h"

#include <cmath>

Circle::Circle(void) :
  m_radius(0.0f)
{}

Circle::Circle(fvec2 const & pos, float radius) :
  m_pos(pos),
  m_radius(std::abs(radius))
{}

fvec2 Circle::GetPosition(void) const {
  return m_pos;
}

void Circle::SetPosition(fvec2 const & position) {
  m_pos = position;
}

float Circle::GetRadius(void) const {
  return m_radius;
}

void Circle::SetRadius(float radius) {
  m_radius = std::abs(radius);
}

bool Circle::Contains(fvec2 const & target) const {
  return (target - m_pos).LengthSquared() <= m_radius * m_radius;
}

float Circle::DistanceTo(fvec2 const & target) const {
  return std::max((target - m_pos).Length() - m_radius, 0.0f);
}

fvec2 Circle::Clamp(fvec2 const & target) const {
  if (Contains(target)) {
    return target;
  }

  return ClampToBoundary(target);
}

fvec2 Circle::ClampToBoundary(fvec2 const & target) const {
  fvec2 dir = (target - m_pos).Normalize();

  if (dir.LengthSquared() == 0.0f) {
    dir = fvec2(1.0f, 0.0f);
  }

  return m_pos + dir * m_radius;
}
