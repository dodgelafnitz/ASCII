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
      float const xDiff        = std::sqrt(circle.GetRadius() * circle.GetRadius() - yDiffSquared);

      fvec2 const towardsP0 = (lineSegment.p0 - lineSegment.p1).Normalize();

      fvec2 const collisionPoint = closestPoint + towardsP0 * xDiff;
      float const delta          = lineSegment.GetDeltaToProjection(collisionPoint);

      if (delta >= 0.0f && delta <= 1.0f) {
        return Intersection2D(collisionPoint, delta);
      }
    }
  }

  return Intersection2D();
}

Intersection2D Intersect(Circle const & circle, Ray const & ray) {
  if (circle.Contains(ray.GetRoot())) {
    return Intersection2D(ray.GetRoot(), 0.0f);
  }
  else {
    fvec2 const closestPoint = ray.ProjectPointOntoLine(circle.GetPosition());

    if (circle.Contains(closestPoint)) {
      float const yDiffSquared = (closestPoint - circle.GetPosition()).LengthSquared();
      float const xDiff        = std::sqrt(circle.GetRadius() * circle.GetRadius() - yDiffSquared);

      fvec2 const collisionPoint   = closestPoint - ray.GetDirection() * xDiff;
      fvec2 const towardsCollision = collisionPoint - ray.GetRoot();
      float const delta            = towardsCollision.Length();
      bool const  collisionInFront = towardsCollision.Dot(ray.GetDirection()) > 0.0f;

      if (collisionInFront && delta >= 0.0f) {
        return Intersection2D(collisionPoint, delta);
      }
    }
  }

  return Intersection2D();
}

Intersection2D Intersect(Circle const & circle, Rect const & rect) {
  fvec2 const intersect = rect.Clamp(circle.GetPosition());

  if (circle.Contains(intersect)) {
    fvec2 totalIntersect;
    int intersectCount = 0;

    fvec2 const halfSize = rect.GetDimensions() * 0.5f;
    fvec2 const offHalfSize = fvec2(halfSize.x, -halfSize.y);

    fvec2 const corners[] = {
      rect.GetCenter() + halfSize,
      rect.GetCenter() + offHalfSize,
      rect.GetCenter() - halfSize,
      rect.GetCenter() - offHalfSize,
    };

    int const edges[][2] = {
      { 0, 1 },
      { 1, 2 },
      { 2, 3 },
      { 3, 0 },
      { 0, 3 },
      { 3, 2 },
      { 2, 1 },
      { 1, 0 },
    };

    for (int i = 0; i < sizeof(edges) / sizeof(*edges); ++i) {
      LineSegment const edge(corners[edges[i][0]], corners[edges[i][1]]);

      Intersection2D intersection = Intersect(circle, edge);

      if (intersection.intersects) {
        totalIntersect += intersection.point;
        ++intersectCount;
      }
    }

    fvec2 const containedCircleExtents[] = {
      circle.GetPosition() + fvec2(circle.GetRadius(),  0.0f),
      circle.GetPosition() + fvec2(-circle.GetRadius(), 0.0f),
      circle.GetPosition() + fvec2(0.0f,                circle.GetRadius()),
      circle.GetPosition() + fvec2(0.0f,                -circle.GetRadius()),
    };

    for (int i = 0; i < sizeof(containedCircleExtents) / sizeof(*containedCircleExtents); ++i) {
      if (rect.Contains(containedCircleExtents[i])) {
        totalIntersect += 2.0f * containedCircleExtents[i];
        intersectCount += 2;
      }
    }

    if (intersectCount == 0) {
      return Intersection2D(intersect);
    }
    else {
      totalIntersect /= float(intersectCount);

      return Intersection2D(totalIntersect);
    }
  }
  else {
    return Intersection2D();
  }
}
