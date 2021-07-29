/*
 * Copywrite 2021 Dodge Lafnitzegger
 */

#ifndef DCUTILITY_MATH_SHAPES2D_CIRCLE_H
#define DCUTILITY_MATH_SHAPES2D_CIRCLE_H

#include "Math/Vector.h"

class Circle {
public:
  Circle(void);
  Circle(fvec2 const & pos, float radius);

  fvec2 GetPosition(void) const;
  void  SetPosition(fvec2 const & position) const;

  float GetRadius(void) const;
  void  SetRadius(float radius) const;

  bool Contains(fvec2 const & target) const;

  float DistanceTo(fvec2 const & target) const;

  fvec2 Clamp(fvec2 const & target) const;
  fvec2 ClampToBoundary(fvec2 const & target) const;

private:
  fvec2 m_pos;
  float m_radius;
};

#endif // DCUTILITY_MATH_SHAPES2D_CIRCLE_H
