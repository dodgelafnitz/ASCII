/*
 * Copywrite 2021 Dodge Lafnitzegger
 */

#ifndef DCUTILITY_MATH_SHAPES2D_RECT_H
#define DCUTILITY_MATH_SHAPES2D_RECT_H

#include "Math/Vector.h"

class Rect {
public:
  Rect(void) = default;
  Rect(fvec2 const & center, fvec2 const & dimensions);

  fvec2 GetCenter(void) const;
  void  SetCenter(fvec2 const & center);

  fvec2 GetDimensions(void) const;
  void  SetDimensions(fvec2 const & dimensions);

  bool Contains(fvec2 const & target) const;

  float DistanceTo(fvec2 const & target) const;

  fvec2 Clamp(fvec2 const & target) const;
  fvec2 ClampToBoundary(fvec2 const & target) const;

private:
  fvec2 m_center;
  fvec2 m_dims;
};

#endif // DCUTILITY_MATH_SHAPES2D_RECT_H
