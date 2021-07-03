/*
 * Copywrite 2021 Dodge Lafnitzegger
 */

#ifndef DCUTILITY_MATH_SHAPES2D_LINESEGMENT_H
#define DCUTILITY_MATH_SHAPES2D_LINESEGMENT_H

#include "Math/Vector.h"
#include "Math/Shapes2D/Line.h"

struct LineSegment {
  LineSegment(void) = default;
  LineSegment(fvec2 const & p0, fvec2 const & p1);

  LineSegment(LineSegment const &) = default;

  LineSegment & operator =(LineSegment const &) = default;

  bool operator ==(LineSegment const &) const = default;
  bool operator !=(LineSegment const &) const = default;

  float GetDistanceToPoint(fvec2 const & point) const;

  fvec2 ProjectPointOntoLineSegment(fvec2 const & point) const;
  fvec2 ProjectPointOntoLine(fvec2 const & point) const;
  float GetDeltaToProjection(fvec2 const & point) const;

  fvec2 GetNormal(void) const;

  float Length(void) const;
  float LengthSquared(void) const;

  Line GetLine(void) const;

  void FaceTowards(fvec2 const & target);
  void FaceAway(fvec2 const & target);

  fvec2 p0;
  fvec2 p1;
};

#endif // DCUTILITY_MATH_SHAPES2D_LINESEGMENT_H
