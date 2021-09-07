/*
 * Copywrite 2021 Dodge Lafnitzegger
 */

#include "Math/Shapes2D/LineSegment.h"

LineSegment::LineSegment(fvec2 const & p0, fvec2 const & p1) :
  p0(p0),
  p1(p1)
{}

float LineSegment::DistanceTo(fvec2 const & point) const {
  return (ProjectPointOntoLineSegment(point) - point).Length();
}

fvec2 LineSegment::ProjectPointOntoLineSegment(fvec2 const & point) const {
  float const delta = GetDeltaToProjection(point);

  if (delta <= 0.0f) {
    return p0;
  }
  else if (delta >= 1.0f) {
    return p1;
  }
  else {
    return p0 * (1.0f - delta) + p1 * delta;
  }
}

fvec2 LineSegment::ProjectPointOntoLine(fvec2 const & point) const {
  float const delta = GetDeltaToProjection(point);

  return p0 * (1.0f - delta) + p1 * delta;
}

float LineSegment::GetDeltaToProjection(fvec2 const & point) const {
  fvec2 const diff    = p1 - p0;
  fvec2 const towards = diff.Normal();
  float const length  = (point - p0).Dot(towards);

  return length == 0.0f ? 0.0f : length / diff.Length();
}

fvec2 LineSegment::GetDeltaPoint(float delta) const {
  return (1.0f - delta) * p0 + delta * p1;
}

fvec2 LineSegment::GetNormal(void) const {
  fvec2 const diff = p1 - p0;
  return fvec2(-diff.y, diff.x).Normalize();
}

float LineSegment::Length(void) const {
  fvec2 const diff = p1 - p0;
  return fvec2(-diff.y, diff.x).Length();
}

float LineSegment::LengthSquared(void) const {
  fvec2 const diff = p1 - p0;
  return fvec2(-diff.y, diff.x).LengthSquared();
}

Line LineSegment::GetLine(void) const {
  fvec2 const normal = GetNormal();

  if (normal.LengthSquared() == 0.0f) {
    return Line::BuildFromPointAndNormal(p0, normal);
  }
  else {
    return Line(normal, p0.Dot(normal));
  }
}

void LineSegment::FaceTowards(fvec2 const & target) {
  if (target == p0 || target == p1) {
    return;
  }

  fvec2 const normal = GetNormal();
  if ((target - p0).Dot(normal) < 0.0f) {
    std::swap(p0, p1);
  }
}

void LineSegment::FaceAway(fvec2 const & target) {
  if (target == p0 || target == p1) {
    return;
  }

  fvec2 const normal = GetNormal();
  if ((target - p0).Dot(normal) > 0.0f) {
    std::swap(p0, p1);
  }
}
