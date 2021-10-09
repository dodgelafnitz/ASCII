/*
 * Copywrite 2021 Dodge Lafnitzegger
 */

#include "Math/Shapes2D/Collision.h"

#include "Containers/DynamicArray.h"

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
    fvec2 const farIntersect = rect.Clamp(circle.ClampToBoundary(intersect));

    fvec2 const halfSize    = rect.GetDimensions() * 0.5f;
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

    fvec2 minVals(std::numeric_limits<float>::max(), std::numeric_limits<float>::max());
    fvec2 maxVals(std::numeric_limits<float>::min(), std::numeric_limits<float>::min());

    for (int i = 0; i < sizeof(edges) / sizeof(*edges); ++i) {
      LineSegment const edge(corners[edges[i][0]], corners[edges[i][1]]);

      Intersection2D intersection = Intersect(circle, edge);

      if (intersection.intersects) {
        minVals.x = std::min(minVals.x, intersection.point.x);
        minVals.y = std::min(minVals.y, intersection.point.y);
        maxVals.x = std::max(maxVals.x, intersection.point.x);
        maxVals.y = std::max(maxVals.y, intersection.point.y);
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
        minVals.x = std::min(minVals.x, containedCircleExtents[i].x);
        minVals.y = std::min(minVals.y, containedCircleExtents[i].y);
        maxVals.x = std::max(maxVals.x, containedCircleExtents[i].x);
        maxVals.y = std::max(maxVals.y, containedCircleExtents[i].y);
      }
    }

    fvec2 const circleBased = (intersect + farIntersect) * 0.5f;
    fvec2 const areaBased   = (minVals + maxVals) * 0.5f;

    //float const delta = 0.25f;

    //fvec2 const reportedIntersection = circleBased * delta + areaBased * (1.0f - delta);

    // TODO: Fix to be a better approximation of center of weight for shape.
    return Intersection2D(areaBased);
  }
  else {
    return Intersection2D();
  }
}

Intersection2D Intersect(Line const & line, Circle const & circle) {
  return Intersect(circle, line);
}

Intersection2D Intersect(Line const & line0, Line const & line1) {
  fvec2 const normal0 = line0.GetNormal();
  fvec2 const normal1 = line1.GetNormal();

  fvec2 const root0 = line0.GetRoot();
  fvec2 const root1 = line1.GetRoot();

  float const len0 = line0.GetNormal().Length();
  float const len1 = line1.GetNormal().Length();

  if (normal0 == normal1 || normal0 == -normal1) {
    if (root0 == root1) {
      return Intersection2D(root0);
    }
    else {
      return Intersection2D();
    }
  }

  fvec2 const dir0      = fvec2(-normal0.y, normal0.x);
  fvec2 const diff      = root1 - root0;
  //float const alignment = dir0.Dot(normal1);

  //float const delta0 = (alignment == 0.0f) ? 0.0f : diff.Dot(normal1) / alignment;
  float const delta0 = diff.Dot(normal1) / dir0.Dot(normal1);

  return Intersection2D(root0 + dir0 * delta0);
}

// Todo: Optimize
Intersection2D Intersect(Line const & line, LineSegment const & lineSegment) {
  float const side0 = line.GetNormal().Dot(lineSegment.p0 - line.GetRoot());
  float const side1 = line.GetNormal().Dot(lineSegment.p1 - line.GetRoot());

  float const sideDiff = side0 * side1;

  if (sideDiff > 0.0f) {
    return Intersection2D();
  }
  else if (side0 == 0.0f) {
    return Intersection2D(lineSegment.p0, 0.0f);
  }
  else if (side1 == 0.0f) {
    return Intersection2D(lineSegment.p1, 1.0f);
  }

  Intersection2D const intersection = Intersect(line, lineSegment.GetLine());

  // This shouldn't happen
  if (!intersection.intersects) {
    return Intersection2D();
  }

  float const delta = lineSegment.GetDeltaToProjection(intersection.point);

  if (delta >= 0.0f && delta <= 1.0f) {
    return Intersection2D(intersection.point, delta);
  }
  else {
    return Intersection2D();
  }
}

// Todo: Optimize
Intersection2D Intersect(Line const & line, Ray const & ray) {
  float const rootSide    = line.GetNormal().Dot(ray.GetRoot() - line.GetRoot());
  float const headingSide = line.GetNormal().Dot(ray.GetDirection());

  float const sideDiff = rootSide * headingSide;

  if (sideDiff > 0.0f) {
    return Intersection2D();
  }
  else if (rootSide == 0.0f) {
    return Intersection2D(ray.GetRoot());
  }
  else if (headingSide == 0.0f) {
    return Intersection2D();
  }

  Intersection2D const intersection = Intersect(line, ray.GetLine());

  // This shouldn't happen
  if (!intersection.intersects) {
    return Intersection2D();
  }

  float const delta = ray.GetDirection().Dot(intersection.point - ray.GetRoot());

  if (delta >= 0.0f) {
    return Intersection2D(intersection.point, delta);
  }
  else {
    return Intersection2D();
  }
}

Intersection2D Intersect(Line const & line, Rect const & rect) {
  fvec2 const root = line.GetRoot();

  fvec2 const center      = rect.GetCenter();
  fvec2 const halfSize    = rect.GetDimensions() * 0.5f;
  fvec2 const offHalfSize = fvec2(-halfSize.x, halfSize.y);

  fvec2 const corners[4] = {
    center + halfSize,
    center + offHalfSize,
    center - halfSize,
    center - offHalfSize,
  };

  LineSegment const edges[4] = {
    LineSegment(corners[0], corners[1]),
    LineSegment(corners[1], corners[2]),
    LineSegment(corners[2], corners[3]),
    LineSegment(corners[3], corners[0]),
  };

  DynamicArray<fvec2, 2> intersections;

  for (int i = 0; i < 4; ++i) {
    Intersection2D intersect = Intersect(line, edges[i]);

    if (intersect.intersects) {
      if (intersections.Empty()) {
        intersections.Emplace(intersect.point);
      }
      else {
        if (intersections[0] != intersect.point) {
          intersections.Emplace(intersect.point);
          break;
        }
      }
    }
  }

  switch (intersections.Count()) {
    default:
    case 0:
      return Intersection2D();
    case 1:
      return Intersection2D(intersections[0]);
    case 2:
      return Intersection2D((intersections[0] + intersections[1]) * 0.5f);
  }
}

Intersection2D Intersect(LineSegment const & lineSegment, Circle const & circle) {
  return Intersect(circle, lineSegment);
}

Intersection2D Intersect(LineSegment const & lineSegment, Line const & line) {
  return Intersect(line, lineSegment);
}

Intersection2D Intersect(LineSegment const & lineSegment0, LineSegment const & lineSegment1) {
  Intersection2D const intersection = Intersect(lineSegment0.GetLine(), lineSegment1.GetLine());

  if (!intersection.intersects) {
    return Intersection2D();
  }

  float const delta0 = lineSegment0.GetDeltaToProjection(intersection.point);
  float const delta1 = lineSegment1.GetDeltaToProjection(intersection.point);

  if (delta0 >= 0.0f && delta0 <= 1.0f && delta1 >= 0.0f && delta1 <= 1.0f) {
    return Intersection2D(intersection.point, delta0);
  }
  else {
    return Intersection2D();
  }
}

Intersection2D Intersect(LineSegment const & lineSegment, Ray const & ray) {
  Intersection2D const intersection = Intersect(lineSegment.GetLine(), ray.GetLine());

  if (!intersection.intersects) {
    return Intersection2D();
  }

  float const lineSegmentDelta = lineSegment.GetDeltaToProjection(intersection.point);
  float const rayDelta         = ray.GetDirection().Dot(intersection.point - ray.GetRoot());

  if (lineSegmentDelta >= 0.0f && lineSegmentDelta <= 1.0f && rayDelta >= 0.0f) {
    return Intersection2D(intersection.point, lineSegmentDelta);
  }
  else {
    return Intersection2D();
  }
}
