/*
 * Copywrite 2021 Dodge Lafnitzegger
 */

#include "Math/Shapes2D/Line.h"

#include <algorithm>

namespace {
  fvec2 const c_defaultNormal(0.0f, 1.0f);
}

Line::Line(void) :
  m_normal(c_defaultNormal),
  m_dist(0.0f)
{}

Line::Line(fvec2 const & normal, float distanceFromOrigin) :
  m_normal(normal.Normal()),
  m_dist(distanceFromOrigin)
{
  float const lengthSquared = m_normal.LengthSquared();

  if (lengthSquared == 0.0f) {
    m_normal = fvec2(c_defaultNormal);
  }
}

fvec2 Line::GetRoot(void) const {
  return m_normal * m_dist;
}

void Line::SetRoot(fvec2 const & root) {
  m_dist = root.Dot(m_normal);
}

fvec2 Line::GetNormal(void) const {
  return m_normal;
}

void Line::SetNormal(fvec2 const & normal) {
  if (normal.LengthSquared() != 0.0f) {
    m_normal = normal.Normal();
  }
}

float Line::GetDistanceFromOrigin(void) const {
  return m_dist;
}

void Line::SetDistanceFromOrigin(float distance) {
  m_dist = distance;
}

void Line::SetFacing(fvec2 const & target) {
  if (SignedDistanceTo(target) < 0) {
    m_normal = -m_normal;
    m_dist   = -m_dist;
  }
}

float Line::SignedDistanceTo(fvec2 const & point) const {
  return point.Dot(m_normal) - m_dist;
}

float Line::DistanceTo(fvec2 const & point) const {
  return std::abs(SignedDistanceTo(point));
}

fvec2 Line::ProjectPointOnto(fvec2 const & point) const {
  fvec2 const diff = -SignedDistanceTo(point) * m_normal;

  return point + diff;
}