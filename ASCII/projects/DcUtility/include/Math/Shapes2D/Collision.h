/*
 * Copywrite 2021 Dodge Lafnitzegger
 */

#ifndef DCUTILITY_MATH_SHAPES2D_COLLISION_H
#define DCUTILITY_MATH_SHAPES2D_COLLISION_H

#include "Math/Shapes2D/Circle.h"
#include "Math/Shapes2D/Line.h"
#include "Math/Shapes2D/LineSegment.h"
#include "Math/Shapes2D/Ray.h"
#include "Math/Shapes2D/Rect.h"

struct Intersection2D {
  Intersection2D(void) = default;
  Intersection2D(fvec2 const & point) :
    intersects(true),
    point(point)
  {}
  Intersection2D(fvec2 const & point, float delta) :
    intersects(true),
    hasDelta(true),
    point(point),
    delta(delta)
  {}

  bool  intersects = false;
  bool  hasDelta   = false;
  fvec2 point      = fvec2();
  float delta      = 0.0f;
};

Intersection2D Intersect(Circle const & circle0, Circle const & circle1);
Intersection2D Intersect(Circle const & circle,  Line const & line);
Intersection2D Intersect(Circle const & circle,  LineSegment const & lineSegment);
Intersection2D Intersect(Circle const & circle,  Ray const & ray);
Intersection2D Intersect(Circle const & circle,  Rect const & rect);

Intersection2D Intersect(Line const & line,  Circle const & circle);
Intersection2D Intersect(Line const & line0, Line const & line1);
Intersection2D Intersect(Line const & line,  LineSegment const & lineSegment);
Intersection2D Intersect(Line const & line,  Ray const & ray);
Intersection2D Intersect(Line const & line,  Rect const & rect);

Intersection2D Intersect(LineSegment const & lineSegment,  Circle const & circle);
Intersection2D Intersect(LineSegment const & lineSegment,  Line const & line);
Intersection2D Intersect(LineSegment const & lineSegment0, LineSegment const & lineSegment1);
Intersection2D Intersect(LineSegment const & lineSegment,  Ray const & ray);
Intersection2D Intersect(LineSegment const & lineSegment,  Rect const & rect);

Intersection2D Intersect(Ray const & ray,  Circle const & circle);
Intersection2D Intersect(Ray const & ray,  Line const & line);
Intersection2D Intersect(Ray const & ray,  LineSegment const & lineSegment);
Intersection2D Intersect(Ray const & ray0, Ray const & ray1);
Intersection2D Intersect(Ray const & ray,  Rect const & rect);

Intersection2D Intersect(Rect const & rect,  Circle const & circle);
Intersection2D Intersect(Rect const & rect,  Line const & line);
Intersection2D Intersect(Rect const & rect,  LineSegment const & lineSegment);
Intersection2D Intersect(Rect const & rect,  Ray const & ray);
Intersection2D Intersect(Rect const & rect0, Rect const & rect1);

// Convex Hulls
// Polygons
// Arcs 
// Splines
// Cast Circles
// Cast Rects
// Cast Convex Hulls
// Cast Polygons

#endif // DCUTILITY_MATH_SHAPES2D_COLLISION_H
