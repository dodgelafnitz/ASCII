/*
 * Copywrite 2021 Dodge Lafnitzegger
 */

#include "Math/Shapes2D/Collision.h"

Intersection2D Intersect(Circle const & circle0, Circle const & circle1) {
  fvec2 const intersectionPoint0 = circle1.Clamp(circle0.GetPosition());
  fvec2 const intersectionPoint1 = circle0.Clamp(circle1.GetPosition());

  fvec2 const intersectionPoint = (intersectionPoint0 + intersectionPoint1) * 0.5f;

  if (circle0.Contains(intersectionPoint) && circle1.Contains(intersectionPoint)) {
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

Intersection2D Intersect(Circle const & circle, LineSegment const & lineSegment) {
  if (circle.Contains(lineSegment.p0)) {
    return Intersection2D(lineSegment.p0, 0.0f);
  }
  else {
    fvec2 const closestPoint = lineSegment.ProjectPointOntoLine(circle.GetPosition());

    if (circle.Contains(closestPoint)) {
      float const yDiffSquared = (closestPoint - circle.GetPosition()).LengthSquared();
      float const xDiff = std::sqrt(circle.GetRadius() * circle.GetRadius() - yDiffSquared);

      fvec2 const towardsP0 = (lineSegment.p0 - lineSegment.p1).Normalize();

      fvec2 const collisionPoint = closestPoint + towardsP0 * xDiff;
      float const delta = lineSegment.GetDeltaToProjection(collisionPoint);

      if (delta >= 0.0f && delta <= 1.0f) {
        return Intersection2D(collisionPoint, delta);
      }
    }
  }

  return Intersection2D();
}
