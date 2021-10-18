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

  operator bool() const {
    return intersects;
  }

  bool  intersects = false;
  fvec2 point      = fvec2();
};

struct DeltaIntersection2D {
  DeltaIntersection2D(void) = default;
  DeltaIntersection2D(fvec2 const & point, float delta) :
    intersects(true),
    point(point),
    delta(delta)
  {}

  operator Intersection2D() const {
    return Intersection2D(point);
  }

  operator bool() const {
    return intersects;
  }

  bool  intersects = false;
  fvec2 point = fvec2();
  float delta = 0.0f;
};

Intersection2D      Intersect(Circle const & circle0, Circle const & circle1);
Intersection2D      Intersect(Circle const & circle,  Line const & line);
DeltaIntersection2D Intersect(Circle const & circle,  LineSegment const & lineSegment);
DeltaIntersection2D Intersect(Circle const & circle,  Ray const & ray);
Intersection2D      Intersect(Circle const & circle,  Rect const & rect);

Intersection2D      Intersect(Line const & line,  Circle const & circle);
Intersection2D      Intersect(Line const & line0, Line const & line1);
DeltaIntersection2D Intersect(Line const & line,  LineSegment const & lineSegment);
DeltaIntersection2D Intersect(Line const & line,  Ray const & ray);
Intersection2D      Intersect(Line const & line,  Rect const & rect);

DeltaIntersection2D Intersect(LineSegment const & lineSegment,  Circle const & circle);
DeltaIntersection2D Intersect(LineSegment const & lineSegment,  Line const & line);
DeltaIntersection2D Intersect(LineSegment const & lineSegment0, LineSegment const & lineSegment1);
DeltaIntersection2D Intersect(LineSegment const & lineSegment,  Ray const & ray);
DeltaIntersection2D Intersect(LineSegment const & lineSegment,  Rect const & rect);

DeltaIntersection2D Intersect(Ray const & ray,  Circle const & circle);
DeltaIntersection2D Intersect(Ray const & ray,  Line const & line);
DeltaIntersection2D Intersect(Ray const & ray,  LineSegment const & lineSegment);
DeltaIntersection2D Intersect(Ray const & ray0, Ray const & ray1);
DeltaIntersection2D Intersect(Ray const & ray,  Rect const & rect);

Intersection2D      Intersect(Rect const & rect,  Circle const & circle);
Intersection2D      Intersect(Rect const & rect,  Line const & line);
DeltaIntersection2D Intersect(Rect const & rect,  LineSegment const & lineSegment);
DeltaIntersection2D Intersect(Rect const & rect,  Ray const & ray);
Intersection2D      Intersect(Rect const & rect0, Rect const & rect1);

bool FastIntersect(Circle const & circle0, Circle const & circle1);
bool FastIntersect(Circle const & circle, Line const & line);
bool FastIntersect(Circle const & circle, LineSegment const & lineSegment);
bool FastIntersect(Circle const & circle, Ray const & ray);
bool FastIntersect(Circle const & circle, Rect const & rect);

bool FastIntersect(Line const & line, Circle const & circle);
bool FastIntersect(Line const & line0, Line const & line1);
bool FastIntersect(Line const & line, LineSegment const & lineSegment);
bool FastIntersect(Line const & line, Ray const & ray);
bool FastIntersect(Line const & line, Rect const & rect);

bool FastIntersect(LineSegment const & lineSegment, Circle const & circle);
bool FastIntersect(LineSegment const & lineSegment, Line const & line);
bool FastIntersect(LineSegment const & lineSegment0, LineSegment const & lineSegment1);
bool FastIntersect(LineSegment const & lineSegment, Ray const & ray);
bool FastIntersect(LineSegment const & lineSegment, Rect const & rect);

bool FastIntersect(Ray const & ray, Circle const & circle);
bool FastIntersect(Ray const & ray, Line const & line);
bool FastIntersect(Ray const & ray, LineSegment const & lineSegment);
bool FastIntersect(Ray const & ray0, Ray const & ray1);
bool FastIntersect(Ray const & ray, Rect const & rect);

bool FastIntersect(Rect const & rect, Circle const & circle);
bool FastIntersect(Rect const & rect, Line const & line);
bool FastIntersect(Rect const & rect, LineSegment const & lineSegment);
bool FastIntersect(Rect const & rect, Ray const & ray);
bool FastIntersect(Rect const & rect0, Rect const & rect1);

// Convex Hulls
// Polygons
// Arcs 
// Splines
// Cast Circles
// Cast Rects
// Cast Convex Hulls
// Cast Polygons

#endif // DCUTILITY_MATH_SHAPES2D_COLLISION_H
