/*
 * Copywrite 2021 Dodge Lafnitzegger
 */

#include "Math/Shapes2D/Collision.h"
#include "gtest/gtest.h"

TEST(CollisionTest, IntersectingCircles_CheckCollision_Intersect) {
  Circle const circle0(fvec2(1.3f, 2.6f), 2.0f);
  Circle const circle1(fvec2(-2.4f, 3.1f), 4.0f);

  Intersection2D const intersection = Intersect(circle0, circle1);

  EXPECT_TRUE(intersection.intersects);
}

TEST(CollisionTest, ContainedCircles_CheckCollision_Intersect) {
  Circle const circle0(fvec2(-1.4f, -0.6f), 8.3f);
  Circle const circle1(fvec2(0.1f, -1.1f), 1.2f);

  Intersection2D const intersection = Intersect(circle0, circle1);

  EXPECT_TRUE(intersection.intersects);
}

TEST(CollisionTest, SeparatedCircles_CheckCollision_DoNotIntersect) {
  Circle const circle0(fvec2(10.4f, 2.6f), 2.1f);
  Circle const circle1(fvec2(-4.3f, 4.7f), 1.4f);

  Intersection2D const intersection = Intersect(circle0, circle1);

  EXPECT_FALSE(intersection.intersects);
}

TEST(CollisionTest, IntersectingCircles_CheckCollisionPoint_WithinBoth) {
  Circle const circle0(fvec2(2.1f, 4.6f), 10.1f);
  Circle const circle1(fvec2(-3.2f, -1.2f), 1.1f);

  Intersection2D const intersection = Intersect(circle0, circle1);

  ASSERT_TRUE(intersection.intersects);

  EXPECT_TRUE(circle0.Contains(intersection.point));
  EXPECT_TRUE(circle1.Contains(intersection.point));
}

TEST(CollisionTest, IntersectingCircleAndLine_CheckCollision_Intersect) {
  Circle const circle(fvec2(1.4f, -2.8f), 2.3f);
  Line const   line(fvec2(-0.6f, -0.8f), 0.5f);

  Intersection2D const intersection = Intersect(circle, line);

  EXPECT_TRUE(intersection.intersects);
}

TEST(CollisionTest, TouchingCircleAndLine_CheckCollision_Intersect) {
  Circle const circle(fvec2(8.2f, 1.6f), 7.2f);
  Line const   line(fvec2(1.0f, 0.0f), 1.0f);

  Intersection2D const intersection = Intersect(circle, line);

  EXPECT_TRUE(intersection.intersects);
}

TEST(CollisionTest, SeparatedCircleAndLine_CheckCollision_DoNotIntersect) {
  Circle const circle(fvec2(-2.1f, 1.7f), 0.3f);
  Line const   line(fvec2(0.2f, 0.5f), 3.1f);

  Intersection2D const intersection = Intersect(circle, line);

  EXPECT_FALSE(intersection.intersects);
}

TEST(CollisionTest, IntersectingCircleAndLine_CheckCollisionPoint_WithinBoth) {
  Circle const circle(fvec2(-0.3f, 2.8f), 1.5f);
  Line const   line(fvec2(0.1f, 0.7f), 2.4f);

  Intersection2D const intersection = Intersect(circle, line);

  ASSERT_TRUE(intersection.intersects);

  EXPECT_TRUE(circle.Contains(intersection.point));
  EXPECT_NEAR(line.DistanceTo(intersection.point), 0.0f, 0.0001f);
}

TEST(CollisionTest, IntersectingCircleAndLineSegment_CheckCollision_Intersect) {
  Circle const      circle(fvec2(1.3f, -4.1f), 2.4f);
  LineSegment const lineSegment(fvec2(6.4f, 0.5f), fvec2(-2.7f, -6.8f));

  DeltaIntersection2D const intersection = Intersect(circle, lineSegment);

  EXPECT_TRUE(intersection.intersects);
}

TEST(CollisionTest, CircleAndContainedLineSegment_CheckCollision_Intersect) {
  Circle const      circle(fvec2(0.5f, 2.3f), 10.2f);
  LineSegment const lineSegment(fvec2(-3.0f, -2.4f), fvec2(4.1f, -1.2f));

  DeltaIntersection2D const intersection = Intersect(circle, lineSegment);

  EXPECT_TRUE(intersection.intersects);
}

TEST(CollisionTest, TouchingCircleAndLineSegment_CheckCollision_Intersect) {
  Circle const      circle(fvec2(2.0f, -0.8f), 0.5f);
  LineSegment const lineSegment(fvec2(2.5f, 4.1f), fvec2(2.5f, -2.3f));

  DeltaIntersection2D const intersection = Intersect(circle, lineSegment);

  EXPECT_TRUE(intersection.intersects);
}

TEST(CollisionTest, TouchingCircleAndLineSegmentEndpoint_CheckCollision_Intersect) {
  Circle const      circle(fvec2(4.7f, 0.2f), 1.4f);
  LineSegment const lineSegment(fvec2(4.7f, 1.6f), fvec2(4.1f, 7.2f));

  DeltaIntersection2D const intersection = Intersect(circle, lineSegment);

  EXPECT_TRUE(intersection.intersects);
}

TEST(CollisionTest, SeparateCircleAndLineSegmentWithNonIntersectingLine_CheckCollision_DoNotIntersect) {
  Circle const      circle(fvec2(7.2f, 3.1f), 2.0f);
  LineSegment const lineSegment(fvec2(0.5f, -1.6f), fvec2(0.7f, 4.8f));

  DeltaIntersection2D const intersection = Intersect(circle, lineSegment);

  EXPECT_FALSE(intersection.intersects);
}

TEST(CollisionTest, SeparateCircleAndLineSegmentWithIntersectingLine_CheckCollision_DoNotIntersect) {
  Circle const      circle(fvec2(-4.4f, 2.2f), 0.5f);
  LineSegment const lineSegment(fvec2(-5.7f, 1.6f), fvec2(-5.1f, 1.9f));

  DeltaIntersection2D const intersection = Intersect(circle, lineSegment);

  EXPECT_FALSE(intersection.intersects);
}

TEST(CollisionTest, IntersectingCircleAndLineSegment_CheckCollisionPoint_WithinBoth) {
  Circle const      circle(fvec2(6.4f, 12.1f), 4.3f);
  LineSegment const lineSegment(fvec2(2.1f, 3.1f), fvec2(6.7f, 9.8f));

  DeltaIntersection2D const intersection = Intersect(circle, lineSegment);

  ASSERT_TRUE(intersection.intersects);

  EXPECT_TRUE(circle.Contains(intersection.point));
  EXPECT_NEAR(lineSegment.DistanceTo(intersection.point), 0.0f, 0.0001f);
}

TEST(CollisionTest, CircleAndContainedLineSegment_CheckCollisionPoint_AtP0) {
  Circle const      circle(fvec2(-5.0f, -2.4f), 2.1f);
  LineSegment const lineSegment(fvec2(-4.1f, -1.3f), fvec2(-6.3f, -3.5f));

  DeltaIntersection2D const intersection = Intersect(circle, lineSegment);

  ASSERT_TRUE(intersection.intersects);

  EXPECT_EQ(intersection.point, lineSegment.p0);
}

TEST(CollisionTest, IntersectingCircleAndLineSegment_CheckCollisionPoint_ClosestToP0) {
  Circle const      circle(fvec2(-1.7f, 0.1f), 3.6f);
  LineSegment const lineSegment(fvec2(10.4f, -3.1f), fvec2(-2.5f, 2.4f));

  DeltaIntersection2D const intersection = Intersect(circle, lineSegment);

  ASSERT_TRUE(intersection.intersects);

  EXPECT_NEAR((circle.ClampToBoundary(intersection.point) - intersection.point).Length(), 0.0f, 0.0001f);
  EXPECT_FALSE(circle.Contains(lineSegment.GetDeltaPoint(intersection.delta - 0.0001f)));
}

TEST(CollisionTest, IntersectingCircleAndLineSegment_CheckCollisionDelta_DeltaIsCorrect) {
  Circle const      circle(fvec2(2.0f, -3.0f), 5.0f);
  LineSegment const lineSegment(fvec2(8.0f, 3.0f), fvec2(0.0f, -9.0f));

  DeltaIntersection2D const intersection = Intersect(circle, lineSegment);

  ASSERT_TRUE(intersection.intersects);

  EXPECT_FLOAT_EQ(intersection.delta, 0.25f);
}

TEST(CollisionTest, IntersectingCircleAndRay_CheckCollision_Intersect) {
  Circle const circle(fvec2(5.2f, 1.3f), 2.4f);
  Ray const    ray(fvec2(1.7f, 3.1f), fvec2(0.9f, -0.1f));

  DeltaIntersection2D const intersection = Intersect(circle, ray);

  EXPECT_TRUE(intersection.intersects);
}

TEST(CollisionTest, TouchingCircleAndRay_CheckCollision_Intersect) {
  Circle const circle(fvec2(-8.5, 5.3f), 7.5f);
  Ray const    ray(fvec2(-1.0f, -2.7f), fvec2(0.0f, 1.0f));

  DeltaIntersection2D const intersection = Intersect(circle, ray);

  EXPECT_TRUE(intersection.intersects);
}

TEST(CollisionTest, TouchingCircleAndRayRoot_CheckCollision_Intersect) {
  Circle const circle(fvec2(8.5f, 2.0f), 2.0f);
  Ray const    ray(fvec2(8.5f, 0.0f), fvec2(0.5f, -0.5f));

  DeltaIntersection2D const intersection = Intersect(circle, ray);

  EXPECT_TRUE(intersection.intersects);
}

TEST(CollisionTest, SeparateCircleAndRayWithNonIntersectingLine_CheckCollision_DoNotIntersect) {
  Circle const circle(fvec2(2.7f, -1.4f), 4.8f);
  Ray const    ray(fvec2(10.5f, -1.7f), fvec2(0.2f, -0.8f));

  DeltaIntersection2D const intersection = Intersect(circle, ray);

  EXPECT_FALSE(intersection.intersects);
}

TEST(CollisionTest, SeparateCircleAndRayWithIntersectingLine_CheckCollision_DoNotIntersect) {
  Circle const circle(fvec2(-8.3f, 5.1f), 5.2f);
  Ray const    ray(fvec2(0.0f, 4.4f), fvec2(0.9f, 0.1f));

  DeltaIntersection2D const intersection = Intersect(circle, ray);

  EXPECT_FALSE(intersection.intersects);
}

TEST(CollisionTest, IntersectingCircleAndRay_CheckCollisionPoint_WithinBoth) {
  Circle const circle(fvec2(0.5f, 2.7f), 1.1f);
  Ray const    ray(fvec2(8.2f, 2.3f), fvec2(-0.9f, 0.1f));

  DeltaIntersection2D const intersection = Intersect(circle, ray);

  ASSERT_TRUE(intersection.intersects);

  EXPECT_TRUE(circle.Contains(intersection.point));
  EXPECT_NEAR(ray.DistanceTo(intersection.point), 0.0f, 0.0001f);
}

TEST(CollisionTest, IntersectingCircleAndRay_CheckCollisionPoint_ClosestToRoot) {
  Circle const circle(fvec2(7.2f, -14.1f), 8.3f);
  Ray const    ray(fvec2(5.1f, 6.4f), fvec2(-0.1f, -0.9f));

  DeltaIntersection2D const intersection = Intersect(circle, ray);

  ASSERT_TRUE(intersection.intersects);

  EXPECT_NEAR((circle.ClampToBoundary(intersection.point) - intersection.point).Length(), 0.0f, 0.0001f);
  EXPECT_FALSE(circle.Contains(ray.GetRoot() + ray.GetDirection() * (intersection.delta - 0.0001f)));
}

TEST(CollisionTest, IntersectingCircleAndRay_CheckCollisionDelta_DeltaIsCorrect) {
  Circle const circle(fvec2(1.0f, -5.0f), 2.5f);
  Ray const    ray(fvec2(12.0f, -5.0f), fvec2(-1.0f, 0.0f));

  DeltaIntersection2D const intersection = Intersect(circle, ray);

  ASSERT_TRUE(intersection.intersects);

  EXPECT_FLOAT_EQ(intersection.delta, (ray.GetRoot() - intersection.point).Length());
  EXPECT_NEAR(ray.DistanceTo(intersection.point), 0.0f, 0.0001f);
}

TEST(CollisionTest, IntersectingCircleAndRect_CheckCollision_Intersect) {
  Circle const circle(fvec2(2.5f, -0.6f), 3.7f);
  Rect const   rect(fvec2(2.7f, 3.7f), fvec2(0.5f, 2.5f));

  Intersection2D const intersection = Intersect(circle, rect);

  EXPECT_TRUE(intersection.intersects);
}

TEST(CollisionTest, TouchingCircleAndRectAtEdge_CheckCollision_Intersect) {
  Circle const circle(fvec2(1.5, 3.7f), 1.5f);
  Rect const   rect(fvec2(4.0f, -2.4f), fvec2(2.0f, 42.5f));

  Intersection2D const intersection = Intersect(circle, rect);

  EXPECT_TRUE(intersection.intersects);
}

TEST(CollisionTest, TouchingCircleAndRectAtCorner_CheckCollision_Intersect) {
  Circle const circle(fvec2(-2.5f, -1.5f), 5.0f);
  Rect const   rect(fvec2(3.5f, 4.5f), fvec2(4.0f, 6.0f));

  Intersection2D const intersection = Intersect(circle, rect);

  EXPECT_TRUE(intersection.intersects);
}

TEST(CollisionTest, SeparateCircleAndRect_CheckCollision_DoNotIntersect) {
  Circle const circle(fvec2(8.3f, -2.2f), 9.1f);
  Rect const   rect(fvec2(-5.6f, -2.4f), fvec2(2.1f, 1.3f));

  Intersection2D const intersection = Intersect(circle, rect);

  EXPECT_FALSE(intersection.intersects);
}

TEST(CollisionTest, IntersectingCircleAndRect_CheckCollisionPoint_WithinBoth) {
  Circle const circle(fvec2(0.5f, 9.2f), 5.4f);
  Rect const   rect(fvec2(2.3f, 6.3f), fvec2(5.1f, 12.1f));

  Intersection2D const intersection = Intersect(circle, rect);

  ASSERT_TRUE(intersection.intersects);

  EXPECT_TRUE(circle.Contains(intersection.point));
  EXPECT_TRUE(rect.Contains(intersection.point));
}

TEST(CollisionTest, IntersectingLines_CheckCollision_Intersect) {
  Line const line0(fvec2(1.0f, 0.0f), 3.0f);
  Line const line1(fvec2(0.8f, 0.6f), 2.1f);

  Intersection2D const intersection = Intersect(line0, line1);

  EXPECT_TRUE(intersection.intersects);
}

TEST(CollisionTest, SeparateParallelLines_CheckCollision_DoNotIntersect) {
  Line const line0(fvec2(-0.6f, 0.8f), 1.4f);
  Line const line1(fvec2(0.6f, -0.8f), 0.3f);

  Intersection2D const intersection = Intersect(line0, line1);

  EXPECT_FALSE(intersection.intersects);
}

TEST(CollisionTest, IdenticalParallelLines_CheckCollision_Intersect) {
  Line const line(fvec2(0.2f, 0.5f), 1.3f);

  Intersection2D const intersection = Intersect(line, line);

  EXPECT_TRUE(intersection.intersects);
}

TEST(CollisionTest, FlippedFacingIdenticalParallelLines_CheckCollision_Intersect) {
  Line const line0(fvec2(-1.0f, 0.0f), 2.0f);
  Line const line1(fvec2(1.0f, 0.0f), -2.0f);

  Intersection2D const intersection = Intersect(line0, line1);

  EXPECT_TRUE(intersection.intersects);
}

TEST(CollisionTest, IdenticalCorrectedParallelLines_CheckCollision_Intersect) {
  Line const line0(fvec2(1.3f, -1.0f), 2.0f);
  Line const line1(line0.GetNormal().Normal(), 2.0f);

  Intersection2D const intersection = Intersect(line0, line1);

  EXPECT_TRUE(intersection.intersects);
}

TEST(CollisionTest, IntersectingLines_CheckCollisionPoint_WithinBoth) {
  Line const line0(fvec2(0.2f, 4.3f), 10.1f);
  Line const line1(fvec2(-3.1f, 2.1f), -26.7f);

  Intersection2D const intersection = Intersect(line0, line1);

  ASSERT_TRUE(intersection.intersects);

  EXPECT_LT(line0.DistanceTo(intersection.point), 0.0001f);
  EXPECT_LT(line1.DistanceTo(intersection.point), 0.0001f);
}

TEST(CollisionTest, IdenticalParallelLines_CheckCollisionPoint_WithinBoth) {
  Line const line(fvec2(0.2f, 0.5f), 1.3f);

  Intersection2D const intersection = Intersect(line, line);

  ASSERT_TRUE(intersection.intersects);

  EXPECT_LT(line.DistanceTo(intersection.point), 0.0001f);
}

TEST(CollisionTest, IntersectingLineAndLineSegment_CheckCollision_Intersect) {
  Line const        line        = Line(fvec2(0.4f, 0.5f), 2.3f);
  LineSegment const lineSegment = LineSegment(fvec2(-3.1f, 2.1f), fvec2(8.3f, 2.7f));

  DeltaIntersection2D const intersection = Intersect(line, lineSegment);

  EXPECT_TRUE(intersection.intersects);
}

TEST(CollisionTest, LineAndContainedLineSegment_CheckCollision_Intersect) {
  Line const        line        = Line(fvec2(1.0f, 1.0f), 0.0f);
  LineSegment const lineSegment = LineSegment(fvec2(2.5f, -2.5f), fvec2(5.0f, -5.0f));

  DeltaIntersection2D const intersection = Intersect(line, lineSegment);

  EXPECT_TRUE(intersection.intersects);
}

TEST(CollisionTest, TouchingLineAndLineSegmentAtP0_CheckCollision_Intersect) {
  Line const        line        = Line(fvec2(3.0f, 4.0f), 5.0f);
  LineSegment const lineSegment = LineSegment(fvec2(3.0f, 4.0f), fvec2(5.7f, 12.3f));

  DeltaIntersection2D const intersection = Intersect(line, lineSegment);

  EXPECT_TRUE(intersection.intersects);
}

TEST(CollisionTest, TouchingLineAndLineSegmentAtP1_CheckCollision_Intersect) {
  Line const        line        = Line(fvec2(3.0f, -4.0f), 5.0f);
  LineSegment const lineSegment = LineSegment(fvec2(12.3f, -4.2f), fvec2(3.0f, 4.0f));

  DeltaIntersection2D const intersection = Intersect(line, lineSegment);

  EXPECT_TRUE(intersection.intersects);
}

TEST(CollisionTest, SeparateLineAndLineSegmentWithNonIntersectingLine_CheckCollision_DoNotIntersect) {
  Line const        line        = Line(fvec2(1.0f, 0.0f), 2.3f);
  LineSegment const lineSegment = LineSegment(fvec2(3.1f, -2.4f), fvec2(3.1f, -8.1f));

  DeltaIntersection2D const intersection = Intersect(line, lineSegment);

  EXPECT_FALSE(intersection.intersects);
}

TEST(CollisionTest, SeparateLineAndLineSegmentWithIntersectingLine_CheckCollision_DoNotIntersect) {
  Line const        line        = Line(fvec2(0.3f, 0.7f), 5.1f);
  LineSegment const lineSegment = LineSegment(fvec2(0.0f, 2.0f), fvec2(-1.5f, -3.6f));

  DeltaIntersection2D const intersection = Intersect(line, lineSegment);

  EXPECT_FALSE(intersection.intersects);
}

TEST(CollisionTest, IntersectingLineAndLineSegment_CheckCollisionPoint_WithinBoth) {
  Line const        line        = Line(fvec2(0.1f, -0.9f), 0.5f);
  LineSegment const lineSegment = LineSegment(fvec2(2.1f, 6.4f), fvec2(1.3f, -6.1f));

  DeltaIntersection2D const intersection = Intersect(line, lineSegment);

  ASSERT_TRUE(intersection.intersects);

  EXPECT_LT(line.DistanceTo(intersection.point), 0.0001f);
  EXPECT_LT(lineSegment.DistanceTo(intersection.point), 0.0001f);
}

TEST(CollisionTest, TouchingLineAndLineSegmentAtP0_CheckCollisionPoint_AtP0) {
  Line const        line        = Line(fvec2(0.0f, -1.0f), 3.5f);
  LineSegment const lineSegment = LineSegment(fvec2(4.1f, -3.5f), fvec2(7.4f, 2.5f));

  DeltaIntersection2D const intersection = Intersect(line, lineSegment);

  ASSERT_TRUE(intersection.intersects);

  EXPECT_EQ(intersection.point, lineSegment.p0);
}

TEST(CollisionTest, TouchingLineAndLineSegmentAtP1_CheckCollisionPoint_AtP1) {
  Line const        line        = Line(fvec2(1.0f, 0.0f), 2.0f);
  LineSegment const lineSegment = LineSegment(fvec2(6.1f, -9.6f), fvec2(2.0f, -0.3f));

  DeltaIntersection2D const intersection = Intersect(line, lineSegment);

  ASSERT_TRUE(intersection.intersects);

  EXPECT_EQ(intersection.point, lineSegment.p1);
}

TEST(CollisionTest, LineAndContainedLineSegment_CheckCollisionPoint_AtP0) {
  Line const        line        = Line(fvec2(0.0f, -1.0f), 0.5f);
  LineSegment const lineSegment = LineSegment(fvec2(2.2f, -0.5f), fvec2(3.2f, -0.5f));

  DeltaIntersection2D const intersection = Intersect(line, lineSegment);

  ASSERT_TRUE(intersection.intersects);

  EXPECT_EQ(intersection.point, lineSegment.p0);
}

TEST(CollisionTest, IntersectingLineAndLineSegment_CheckCollisionDelta_DeltaIsCorrect) {
  Line const        line        = Line(fvec2(0.5f, -1.0f), 0.0f);
  LineSegment const lineSegment = LineSegment(fvec2(5.0f, 1.0f), fvec2(3.0f, 3.0f));

  DeltaIntersection2D const intersection = Intersect(line, lineSegment);

  ASSERT_TRUE(intersection.intersects);

  EXPECT_FLOAT_EQ(intersection.delta, 0.5f);
}

TEST(CollisionTest, TouchingLineAndLineSegmentAtP0_CheckCollisionDelta_DeltaIs0) {
  Line const        line        = Line(fvec2(0.0f, 1.0f), 2.5f);
  LineSegment const lineSegment = LineSegment(fvec2(9.1f, 2.5f), fvec2(-4.2f, -0.6f));

  DeltaIntersection2D const intersection = Intersect(line, lineSegment);

  ASSERT_TRUE(intersection.intersects);

  EXPECT_FLOAT_EQ(intersection.delta, 0.0f);
}

TEST(CollisionTest, TouchingLineAndLineSegmentAtP1_CheckCollisionDelta_DeltaIs1) {
  Line const        line        = Line(fvec2(0.5f, 0.5f), 0.0f);
  LineSegment const lineSegment = LineSegment(fvec2(2.7f, 17.4f), fvec2(0.0f, 0.0f));

  DeltaIntersection2D const intersection = Intersect(line, lineSegment);

  ASSERT_TRUE(intersection.intersects);

  EXPECT_FLOAT_EQ(intersection.delta, 1.0f);
}

TEST(CollisionTest, LineAndContainedLineSegment_CheckCollisionDelta_DeltaIs0) {
  Line const        line        = Line(fvec2(1.0f, 0.0f), 4.5f);
  LineSegment const lineSegment = LineSegment(fvec2(4.5, -1.3f), fvec2(4.5, 10.3f));

  DeltaIntersection2D const intersection = Intersect(line, lineSegment);

  ASSERT_TRUE(intersection.intersects);

  EXPECT_EQ(intersection.delta, 0.0f);
}

TEST(CollisionTest, IntersectingLineAndRay_CheckCollision_Intersect) {
  Line const line = Line(fvec2(0.5f, 0.8f), 1.2f);
  Ray const  ray  = Ray(fvec2(5.3f, 8.1f), fvec2(-0.8f, 0.2f));

  DeltaIntersection2D const intersection = Intersect(line, ray);

  EXPECT_TRUE(intersection.intersects);
}

TEST(CollisionTest, LineAndContainedRay_CheckCollision_Intersect) {
  Line const line = Line(fvec2(-0.6f, 0.8f), 0.0f);
  Ray const  ray  = Ray(fvec2(4.0f, 3.0f), fvec2(0.8f, 0.6f));

  DeltaIntersection2D const intersection = Intersect(line, ray);

  EXPECT_TRUE(intersection.intersects);
}

TEST(CollisionTest, TouchingLineAndRayRoot_CheckCollision_Intersect) {
  Line const line = Line(fvec2(1.0f, 0.0f), 3.0f);
  Ray const  ray  = Ray(fvec2(3.0f, 7.3f), fvec2(0.7f, 0.7f));

  DeltaIntersection2D const intersection = Intersect(line, ray);

  EXPECT_TRUE(intersection.intersects);
}

TEST(CollisionTest, SeparateLineAndRayWithNonIntersectingLine_CheckCollision_DoNotIntersect) {
  Line const line = Line(fvec2(1.0f, 0.0f), -2.7f);
  Ray const  ray  = Ray(fvec2(4.2f, -1.1f), fvec2(0.0f, 1.0f));

  DeltaIntersection2D const intersection = Intersect(line, ray);

  EXPECT_FALSE(intersection.intersects);
}

TEST(CollisionTest, SeparateLineAndRayWithIntersectingLine_CheckCollision_DoNotIntersect) {
  Line const line = Line(fvec2(-0.5f, -0.9f), 8.4f);
  Ray const  ray  = Ray(fvec2(-1.3f, 2.1f), fvec2(0.6f, 0.8f));

  DeltaIntersection2D const intersection = Intersect(line, ray);

  EXPECT_FALSE(intersection.intersects);
}

TEST(CollisionTest, IntersectingLineAndRay_CheckCollisionPoint_WithinBoth) {
  Line const line = Line(fvec2(0.3f, -0.9f), 16.3f);
  Ray const  ray  = Ray(fvec2(0.0f, 10.1f), fvec2(0.2f, -0.7f));

  DeltaIntersection2D const intersection = Intersect(line, ray);

  ASSERT_TRUE(intersection.intersects);

  EXPECT_LT(line.DistanceTo(intersection.point), 0.0001f);
  EXPECT_LT(ray.DistanceTo(intersection.point), 0.0001f);
}

TEST(CollisionTest, LineAndContainedRay_CheckCollisionPoint_AtRoot) {
  Line const line = Line(fvec2(-0.2f, -0.8f), 0.0f);
  Ray const  ray  = Ray(fvec2(0.0f, 0.0f), fvec2(0.5f, 0.9f));

  DeltaIntersection2D const intersection = Intersect(line, ray);

  ASSERT_TRUE(intersection.intersects);

  EXPECT_EQ(intersection.point, ray.GetRoot());
}

TEST(CollisionTest, IntersectingLineAndRay_CheckCollisionDelta_DeltaIsCorrect) {
  Line const line = Line(fvec2(1.0f, 0.0f), 5.5f);
  Ray const  ray  = Ray(fvec2(-3.0f, 8.1f), fvec2(1.0f, 0.0f));

  DeltaIntersection2D const intersection = Intersect(line, ray);

  ASSERT_TRUE(intersection.intersects);

  EXPECT_EQ(intersection.delta, 8.5f);
}

TEST(CollisionTest, LineAndContainedRay_CheckCollisionDelta_DeltaIs0) {
  Line const line = Line(fvec2(-0.8f, 0.6f), 0.0f);
  Ray const  ray  = Ray(fvec2(3.0f, 4.0f), fvec2(0.6f, 0.8f));

  DeltaIntersection2D const intersection = Intersect(line, ray);

  ASSERT_TRUE(intersection.intersects);

  EXPECT_EQ(intersection.delta, 0.0f);
}

TEST(CollisionTest, IntersectingLineAndRect_CheckCollision_Intersect) {
  Line const line = Line(fvec2(1.0f, 0.0f), 5.1f);
  Rect const rect = Rect(fvec2(5.3, 6.2), fvec2(0.8f, 0.3f));

  Intersection2D const intersection = Intersect(line, rect);

  EXPECT_TRUE(intersection.intersects);
}

TEST(CollisionTest, TouchingLineAndRect_CheckCollision_Intersect) {
  Line const line = Line(fvec2(0.2f, -0.9f), 0.0f);
  Rect const rect = Rect(fvec2(-1.5f, 1.0f), fvec2(3.0f, 2.0f));

  Intersection2D const intersection = Intersect(line, rect);

  EXPECT_TRUE(intersection.intersects);
}

TEST(CollisionTest, SeparateLineAndRect_CheckCollision_DoNotIntersect) {
  Line const line = Line(fvec2(0.1f, 0.9f), 5.2f);
  Rect const rect = Rect(fvec2(12.3f, -4.1f), fvec2(2.1f, 5.2f));

  Intersection2D const intersection = Intersect(line, rect);

  EXPECT_FALSE(intersection.intersects);
}

TEST(CollisionTest, IntersectingLineAndRect_CheckCollisionPoint_WithinBoth) {
  Line const line = Line(fvec2(-0.7f, -0.8f), 2.3f);
  Rect const rect = Rect(fvec2(1.4f, -6.8f), fvec2(8.3f, 0.4f));

  Intersection2D const intersection = Intersect(line, rect);

  ASSERT_TRUE(intersection.intersects);

  EXPECT_LT(line.DistanceTo(intersection.point), 0.0001f);
  EXPECT_TRUE(rect.Contains(intersection.point));
}

TEST(CollisionTest, IntersectingLineSegments_CheckCollision_Intersect) {
  LineSegment const lineSegment0 = LineSegment(fvec2(0.1f, 4.3f), fvec2(-1.5f, -2.6f));
  LineSegment const lineSegment1 = LineSegment(fvec2(-3.1f, 1.2f), fvec2(8.1f, -1.2f));

  DeltaIntersection2D const intersection = Intersect(lineSegment0, lineSegment1);

  EXPECT_TRUE(intersection.intersects);
}

TEST(CollisionTest, OverlappingLineSegments_CheckCollision_Intersect) {
  LineSegment const lineSegment0 = LineSegment(fvec2(0.0f, 2.1f), fvec2(0.0f, 7.2f));
  LineSegment const lineSegment1 = LineSegment(fvec2(0.0f, 4.7f), fvec2(0.0f, 11.4f));

  DeltaIntersection2D const intersection = Intersect(lineSegment0, lineSegment1);

  EXPECT_TRUE(intersection.intersects);
}

TEST(CollisionTest, TouchingLineSegments_CheckCollision_Intersect) {
  LineSegment const lineSegment0 = LineSegment(fvec2(1.5f, 3.0f), fvec2(5.5f, 5.0f));
  LineSegment const lineSegment1 = LineSegment(fvec2(3.5f, 4.0f), fvec2(8.0f, -10.0f));

  DeltaIntersection2D const intersection = Intersect(lineSegment0, lineSegment1);

  EXPECT_TRUE(intersection.intersects);
}

TEST(CollisionTest, SeparateLineSegments_CheckCollision_DoNotIntersect) {
  LineSegment const lineSegment0 = LineSegment(fvec2(8.2f, -1.3f), fvec2(3.6f, -2.1f));
  LineSegment const lineSegment1 = LineSegment(fvec2(-5.1f, 2.6f), fvec2(9.2f, 0.3f));

  DeltaIntersection2D const intersection = Intersect(lineSegment0, lineSegment1);

  EXPECT_FALSE(intersection.intersects);
}

TEST(CollisionTest, IntersectingLineSegments_CheckCollisionPoint_WithinBoth) {
  LineSegment const lineSegment0 = LineSegment(fvec2(0.5f, 5.7f), fvec2(-6.1f, -10.1f));
  LineSegment const lineSegment1 = LineSegment(fvec2(-3.1f, 3.2f), fvec2(5.0f, -8.3f));

  DeltaIntersection2D const intersection = Intersect(lineSegment0, lineSegment1);

  ASSERT_TRUE(intersection.intersects);

  EXPECT_LT(lineSegment0.DistanceTo(intersection.point), 0.0001f);
  EXPECT_LT(lineSegment1.DistanceTo(intersection.point), 0.0001f);
}

TEST(CollisionTest, IntersectingLineSegments_CheckCollisionDelta_DeltaIsCorrectForLineSegment0) {
  LineSegment const lineSegment0 = LineSegment(fvec2(0.0f, 0.0f), fvec2(8.0f, 4.0f));
  LineSegment const lineSegment1 = LineSegment(fvec2(3.0f, 5.0f), fvec2(1.0f, -3.0f));

  DeltaIntersection2D const intersection = Intersect(lineSegment0, lineSegment1);

  ASSERT_TRUE(intersection.intersects);

  EXPECT_FLOAT_EQ(intersection.delta, 0.25f);
}

TEST(CollisionTest, IntersectingLineSegmentAndRay_CheckCollision_Intersect) {
  LineSegment const lineSegment = LineSegment(fvec2(0.0f, 0.0f), fvec2(8.0f, 4.0f));
  Ray const         ray         = Ray(fvec2(4.1f, -1.8f), fvec2(0.3f, 0.9f));

  DeltaIntersection2D const intersection = Intersect(lineSegment, ray);

  EXPECT_TRUE(intersection.intersects);
}

TEST(CollisionTest, TouchingLineSegmentAndRayOnLineSegmentPoint_CheckCollision_Intersect) {
  LineSegment const lineSegment = LineSegment(fvec2(2.0f, 4.0f), fvec2(9.2f, -3.1f));
  Ray const         ray         = Ray(fvec2(2.0f, -1.8f), fvec2(0.0f, 1.0f));

  DeltaIntersection2D const intersection = Intersect(lineSegment, ray);

  EXPECT_TRUE(intersection.intersects);
}

TEST(CollisionTest, TouchingLineSegmentAndRayOnRayRoot_CheckCollision_Intersect) {
  LineSegment const lineSegment = LineSegment(fvec2(0.0f, 3.1f), fvec2(0.0f, -4.8f));
  Ray const         ray         = Ray(fvec2(0.0f, 0.0f), fvec2(0.4f, 0.8f));

  DeltaIntersection2D const intersection = Intersect(lineSegment, ray);

  EXPECT_TRUE(intersection.intersects);
}

TEST(CollisionTest, ContainedLineSegmentAndRay_CheckCollision_Intersect) {
  LineSegment const lineSegment = LineSegment(fvec2(8.0f, 4.5f), fvec2(10.0f, 6.5f));
  Ray const         ray         = Ray(fvec2(1.0f, -2.5f), fvec2(1.0f, 1.0f));

  DeltaIntersection2D const intersection = Intersect(lineSegment, ray);

  EXPECT_TRUE(intersection.intersects);
}

TEST(CollisionTest, LineSegmentAndContainedRayRoot_CheckCollision_Intersect) {
  LineSegment const lineSegment = LineSegment(fvec2(0.0f, 2.3f), fvec2(0.0f, -8.1f));
  Ray const         ray         = Ray(fvec2(0.0f, -2.1f), fvec2(0.0f, 1.0f));

  DeltaIntersection2D const intersection = Intersect(lineSegment, ray);

  EXPECT_TRUE(intersection.intersects);
}

TEST(CollisionTest, SeparateLineSegmentAndRay_CheckCollision_DoNotIntersect) {
}

TEST(CollisionTest, IntersectingLineSegmentAndRay_CheckCollisionPoint_WithinBoth) {
}

TEST(CollisionTest, TouchingLineSegmentAndRayOnLineSegmentP0_CheckCollisionPoint_IsP0) {
}

TEST(CollisionTest, TouchingLineSegmentAndRayOnLineSegmentP1_CheckCollisionPoint_IsP1) {
}

TEST(CollisionTest, TouchingLineSegmentAndRayOnRayRoot_CheckCollisionPoint_IsRoot) {
}

TEST(CollisionTest, ContainedLineSegmentAndRay_CheckCollisionPoint_IsP0) {
}

TEST(CollisionTest, LineSegmentAndContainedRayRootTowardsP0_CheckCollisionPoint_IsP0) {
}

TEST(CollisionTest, LineSegmentAndContainedRayRootTowardsP1_CheckCollisionPoint_IsRoot) {
}

TEST(CollisionTest, IntersectingLineSegmentAndRay_CheckCollisionDelta_DeltaIsCorrect) {
}

TEST(CollisionTest, TouchingLineSegmentAndRayOnLineSegmentP0_CheckCollisionDelta_DeltaIs0) {
}

TEST(CollisionTest, TouchingLineSegmentAndRayOnLineSegmentP1_CheckCollisionDelta_DeltaIs1) {
}

TEST(CollisionTest, TouchingLineSegmentAndRayOnRayRoot_CheckCollisionDelta_DeltaIsCorrect) {
}

TEST(CollisionTest, ContainedLineSegmentAndRay_CheckCollisionDelta_DeltaIs0) {
}

TEST(CollisionTest, LineSegmentAndContainedRayRootTowardsP0_CheckCollisionDelta_DeltaIs0) {
}

TEST(CollisionTest, LineSegmentAndContainedRayRootTowardsP1_CheckCollisionDelta_DeltaIsCorrectForLineSegment) {
}

TEST(CollisionTest, RayAndContainedLineSegment_CheckCollisionPoint_IsClosestPointOnSegmentToRoot) {
}

TEST(CollisionTest, ContainedRayRootAndLineSegment_CheckCollisionPoint_IsRoot) {
}

TEST(CollisionTest, IntersectingRayAndLineSegment_CheckCollisionDelta_DeltaIsCorrect) {
}

TEST(CollisionTest, TouchingRayAndLineSegmentOnLineSegmentEndpoint_CheckCollisionDelta_DeltaIsCorrect) {
}

TEST(CollisionTest, TouchingAndRayLineSegmentOnRayRoot_CheckCollisionDelta_DeltaIs0) {
}

TEST(CollisionTest, RayAndContainedLineSegment_CheckCollisionDelta_DeltaIsCorrect) {
}

TEST(CollisionTest, ContainedRayRootAndLineSegment_CheckCollisionDelta_DeltaIs0) {
}

TEST(CollisionTest, IntersectingRays_CheckCollision_Intersect) {
}

TEST(CollisionTest, TouchingRays_CheckCollision_Intersect) {
}

TEST(CollisionTest, ContainedRays_CheckCollision_Intersect) {
}

TEST(CollisionTest, ParallelSeparateRays_CheckCollision_DoNotIntersect) {
}

TEST(CollisionTest, SeparateRays_CheckCollision_DoNotIntersect) {
}

TEST(CollisionTest, IntersectingRays_CheckCollisionPoint_WithinBoth) {
}

TEST(CollisionTest, TouchingRays_CheckCollisionPoint_WithinBoth) {
}

TEST(CollisionTest, ContainedRays_CheckCollisionPoint_AtContainedRoot) {
}

TEST(CollisionTest, ContainedRaysInOppositeDirections_CheckCollisionPoint_AtRay0Root) {
}

TEST(CollisionTest, IntersectingRays_CheckCollisionDelta_DeltaIsCorrectForRay0) {
}

TEST(CollisionTest, TouchingRaysAtRay0Root_CheckCollisionDelta_DeltaIs0) {
}

TEST(CollisionTest, TouchingRaysAtRay1Root_CheckCollisionDelta_DeltaIsCorrectForRay0) {
}

TEST(CollisionTest, ContainedRaysWithRay0RootContained_CheckCollisionDelta_DeltaIs0) {
}

TEST(CollisionTest, ContainedRaysWithRay1RootContained_CheckCollisionDelta_DeltaIsCorrectForRay0) {
}

TEST(CollisionTest, ContainedRaysInOppositeDirections_CheckCollisionDelta_DeltaIs0) {
}

TEST(CollisionTest, IntersectingRayAndRect_CheckCollision_Intersect) {
}

TEST(CollisionTest, TouchingRayAndRectAtRectCorner_CheckCollision_Intersect) {
}

TEST(CollisionTest, TouchingRayAndRectAtRayRoot_CheckCollision_Intersect) {
}

TEST(CollisionTest, SeparateRayAndRect_CheckCollision_DoNotIntersect) {
}

TEST(CollisionTest, IntersectingRayAndRect_CheckCollisionPoint_WithinBoth) {
}

TEST(CollisionTest, ContainedRayAndRect_CheckCollisionPoint_AtRoot) {
}

TEST(CollisionTest, IntersectingRayAndRect_CheckCollisionDelta_DeltaIsCorrect) {
}

TEST(CollisionTest, ContainedRayAndRect_CheckCollisionDelta_DeltaIs0) {
}

TEST(CollisionTest, IntersectingRects_CheckCollision_Intersect) {
}

TEST(CollisionTest, TouchingRects_CheckCollision_Intersect) {
}

TEST(CollisionTest, SeparateRects_CheckCollision_DoNotIntersect) {
}

TEST(CollisionTest, IntersectingRects_CheckCollisionPoint_WithinBoth) {
}
