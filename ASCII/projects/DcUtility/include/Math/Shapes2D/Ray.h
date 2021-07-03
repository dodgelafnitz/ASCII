/*
 * Copywrite 2021 Dodge Lafnitzegger
 */

#ifndef DCUTILITY_MATH_SHAPES2D_RAY_H
#define DCUTILITY_MATH_SHAPES2D_RAY_H

#include "Math/Vector.h"
#include "Math/Shapes2D/Line.h"

class Ray {
public:
  Ray(void);
  Ray(fvec2 const & root, fvec2 const & normal);

  Ray(Ray const &) = default;

  Ray & operator =(Ray const &) = default;

  bool operator ==(Ray const &) const = default;
  bool operator !=(Ray const &) const = default;

  fvec2 GetRoot(void) const;
  void  SetRoot(fvec2 const & root);

  fvec2 GetNormal(void) const;
  void  SetNormal(fvec2 const & normal);

  Line GetLine(void) const;
  LineSegment GetLineSegment(float length) const;
  LineSegment GetLineSegment(float length, fvec2 const & facing) const;

  float DistanceTo(fvec2 const & point) const;

  fvec2 ProjectPointOntoRay(fvec2 const & point) const;
  fvec2 ProjectPointOntoLine(fvec2 const & point) const;

private:
  fvec2 m_root;
  fvec2 m_normal;
};

#endif // DCUTILITY_MATH_SHAPES2D_RAY_H
