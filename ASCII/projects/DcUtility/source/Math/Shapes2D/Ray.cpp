/*
 * Copywrite 2021 Dodge Lafnitzegger
 */

#include "Math/Shapes2D/Ray.h"

namespace {
  fvec2 const c_defaultDirection(0.0f, 1.0f);
}

Ray::Ray(void) :
  m_dir(c_defaultDirection)
{}

Ray::Ray(fvec2 const & root, fvec2 const & diirection) :
  m_root(root),
  m_dir(diirection.Normal())
{}

fvec2 Ray::GetRoot(void) const {
  return m_root;
}

void Ray::SetRoot(fvec2 const & root) {
  m_root = root;
}

fvec2 Ray::GetDirection(void) const {
  return m_dir;
}

void Ray::SetDirection(fvec2 const & direction) {
  if (direction != fvec2()) {
    m_dir = direction.Normal();
  }
}

fvec2 Ray::GetNormal(void) const {
  return fvec2(-m_dir.y, m_dir.x);
}

Line Ray::GetLine(void) const {
  fvec2 const normal = GetNormal();
  float const dist   = m_root.Dot(normal);

  return Line(normal, dist);
}

Line Ray::GetLine(fvec2 const & facing) const {
  Line result = GetLine();

  result.SetFacing(facing);

  return result;
}

LineSegment Ray::GetLineSegment(float length) const {
  return LineSegment(m_root, m_root + length * m_dir);
}

LineSegment Ray::GetLineSegment(float length, fvec2 const & facing) const {
  LineSegment result = GetLineSegment(length);

  result.FaceTowards(facing);

  return result;
}

float Ray::DistanceTo(fvec2 const & point) const {
  fvec2 const diff = point - m_root;

  if (diff.Dot(m_dir) < 0.0f) {
    return diff.Length();
  }
  else {
    return std::abs(diff.Dot(GetNormal()));
  }
}

fvec2 Ray::ProjectPointOntoRay(fvec2 const & point) const {
  fvec2 const diff = point - m_root;
  float const dist = diff.Dot(m_dir);

  if (dist < 0.0f) {
    return m_root;
  }
  else {
    return dist * m_dir + m_root;
  }
}
fvec2 Ray::ProjectPointOntoLine(fvec2 const & point) const {
  fvec2 const diff = point - m_root;
  float const dist = diff.Dot(m_dir);

  return dist * m_dir + m_root;
}

