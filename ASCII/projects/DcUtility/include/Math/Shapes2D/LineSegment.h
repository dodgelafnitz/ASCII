/*
 * Copywrite 2021 Dodge Lafnitzegger
 */

#ifndef DCUTILITY_MATH_SHAPES2D_LINESEGMENT_H
#define DCUTILITY_MATH_SHAPES2D_LINESEGMENT_H

#include "Math/Vector.h"
#include "Math/Shapes2D/Line.h"

struct LineSegment {
  LineSegment(void);
  LineSegment(fvec2 const & p0, fvec2 const & p1);

  float GetDistanceToPoint(fvec2 const & point) const;

  fvec2 ProjectPointOntoLineSegment(fvec2 const & point) const;
  fvec2 ProjectPointOntoLine(fvec2 const & point) const;
  float GetDeltaToProjection(fvec2 const & point) const;

  fvec2 GetNormal(void) const;

  float GetLength(void) const;
  float GetLengthSquared(void) const;

  Line GetLine(void) const;

  void FaceTowrads(fvec2 const & target);
  void FaceAway(fvec2 const & target);

  fvec2 p0;
  fvec2 p1;
};

#endif // DCUTILITY_MATH_SHAPES2D_LINESEGMENT_H
