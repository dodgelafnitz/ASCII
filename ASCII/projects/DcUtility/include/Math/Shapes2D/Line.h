/*
 * Copywrite 2021 Dodge Lafnitzegger
 */

#ifndef DCUTILITY_MATH_SHAPES2D_LINE_H
#define DCUTILITY_MATH_SHAPES2D_LINE_H

#include "Math/Shapes2D/ForwardDeclarations.h"
#include "Math/Vector.h"

class Line {
public:
  static Line BuildFrom2Points(fvec2 const & point0, fvec2 const & point1);
  static Line BuildFromPointAndNormal(fvec2 const & point, fvec2 const & normal);
  static Line BuildFromLineSegment(LineSegment const & segment);
  static Line BuildFromRay(Ray const & ray);

  Line(void);
  Line(fvec2 const & normal, float distanceFromOrigin);

  fvec2 GetRoot(void) const;
  void  SetRoot(fvec2 const & root);

  fvec2 GetNormal(void) const;
  void  SetNormal(fvec2 const & normal);

  float GetDistanceFromOrigin(void) const;
  void  SetDistanceFromOrigin(float distance);

  void  SetFacing(fvec2 const & target);

  float DistanceTo(fvec2 const & point) const;
  float SignedDistanceTo(fvec2 const & point) const;
  fvec2 ProjectPointOnto(fvec2 const & point) const;

private:
  fvec2 m_normal;
  float m_dist;
};

#endif // DCUTILITY_MATH_SHAPES2D_LINE_H
