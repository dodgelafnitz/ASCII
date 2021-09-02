/*
 * Copywrite 2021 Dodge Lafnitzegger
 */

#include "Math/Shapes2D/Collision.h"

Intersection2D Intersect(Circle const & circle0, Circle const & circle1) {
  fvec2 const intersectionPoint = circle1.Clamp(circle0.GetPosition());

  if (circle0.Contains(intersectionPoint)) {
    return Intersection2D(intersectionPoint);
  }
  else {
    return Intersection2D();
  }
}

Intersection2D Intersect(Circle const & circle, Line const & line) {
  fvec2 const intersectionPoint = line.ProjectPointOnto(circle.GetPosition());

  if (circle.Contains(intersectionPoint)) {
    return Intersection2D(intersectionPoint);
  }
  else {
    return Intersection2D();
  }
}
