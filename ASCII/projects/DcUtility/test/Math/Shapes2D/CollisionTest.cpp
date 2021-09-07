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

  EXPECT_TRUE(intersection.intersects);
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

  EXPECT_TRUE(intersection.intersects);
  EXPECT_TRUE(circle.Contains(intersection.point));
  EXPECT_NEAR(line.DistanceTo(intersection.point), 0.0f, 0.0001f);
}

TEST(CollisionTest, IntersectingCircleAndLineSegment_CheckCollision_Intersect) {
  Circle const      circle(fvec2(1.3f, -4.1f), 2.4f);
  LineSegment const lineSegment(fvec2(6.4f, 0.5f), fvec2(-2.7f, -6.8f));

  Intersection2D const intersection = Intersect(circle, lineSegment);

  EXPECT_TRUE(intersection.intersects);
}

TEST(CollisionTest, CircleAndContainedLineSegment_CheckCollision_Intersect) {
  Circle const      circle(fvec2(0.5f, 2.3f), 10.2f);
  LineSegment const lineSegment(fvec2(-3.0f, -2.4f), fvec2(4.1f, -1.2f));

  Intersection2D const intersection = Intersect(circle, lineSegment);

  EXPECT_TRUE(intersection.intersects);
}

TEST(CollisionTest, TouchingCircleAndLineSegment_CheckCollision_Intersect) {
  Circle const      circle(fvec2(2.0f, -0.8f), 0.5f);
  LineSegment const lineSegment(fvec2(2.5f, 4.1f), fvec2(2.5f, -2.3f));

  Intersection2D const intersection = Intersect(circle, lineSegment);

  EXPECT_TRUE(intersection.intersects);
}

TEST(CollisionTest, TouchingCircleAndLineSegmentEndpoint_CheckCollision_Intersect) {
  Circle const      circle(fvec2(4.7f, 0.2f), 1.4f);
  LineSegment const lineSegment(fvec2(4.7f, 1.6f), fvec2(4.1f, 7.2f));

  Intersection2D const intersection = Intersect(circle, lineSegment);

  EXPECT_TRUE(intersection.intersects);
}

TEST(CollisionTest, SeparateCircleAndLineSegmentWithNonIntersectingLine_CheckCollision_DoNotIntersect) {
  Circle const      circle(fvec2(7.2f, 3.1f), 2.0f);
  LineSegment const lineSegment(fvec2(0.5f, -1.6f), fvec2(0.7f, 4.8f));

  Intersection2D const intersection = Intersect(circle, lineSegment);

  EXPECT_FALSE(intersection.intersects);
}

TEST(CollisionTest, SeparateCircleAndLineSegmentWithIntersectingLine_CheckCollision_DoNotIntersect) {
  Circle const      circle(fvec2(-4.4f, 2.2f), 0.5f);
  LineSegment const lineSegment(fvec2(-5.7f, 1.6f), fvec2(-5.1f, 1.9f));

  Intersection2D const intersection = Intersect(circle, lineSegment);

  EXPECT_FALSE(intersection.intersects);
}

TEST(CollisionTest, IntersectingCircleAndLineSegment_CheckCollisionPoint_WithinBoth) {
  Circle const      circle(fvec2(6.4f, 12.1f), 4.3f);
  LineSegment const lineSegment(fvec2(2.1f, 3.1f), fvec2(6.7f, 9.8f));

  Intersection2D const intersection = Intersect(circle, lineSegment);

  EXPECT_TRUE(intersection.intersects);
  EXPECT_TRUE(circle.Contains(intersection.point));
  EXPECT_NEAR(lineSegment.DistanceTo(intersection.point), 0.0f, 0.0001f);
}

TEST(CollisionTest, CircleAndContainedLineSegment_CheckCollisionPoint_AtP0) {
  Circle const      circle(fvec2(-5.0f, -2.4f), 2.1f);
  LineSegment const lineSegment(fvec2(-4.1f, -1.3f), fvec2(-6.3f, -3.5f));

  Intersection2D const intersection = Intersect(circle, lineSegment);

  EXPECT_TRUE(intersection.intersects);
  EXPECT_EQ(intersection.point, lineSegment.p0);
}

TEST(CollisionTest, IntersectingCircleAndLineSegment_CheckCollisionPoint_ClosestToP0) {
  Circle const      circle(fvec2(-1.7f, 0.1f), 3.6f);
  LineSegment const lineSegment(fvec2(10.4f, -3.1f), fvec2(-2.5f, 2.4f));

  Intersection2D const intersection = Intersect(circle, lineSegment);

  EXPECT_TRUE(intersection.intersects);
  EXPECT_NEAR((circle.ClampToBoundary(intersection.point) - intersection.point).Length(), 0.0f, 0.0001f);
  EXPECT_FALSE(circle.Contains(lineSegment.GetDeltaPoint(intersection.delta - 0.0001f)));
}

TEST(CollisionTest, IntersectingCircleAndLineSegment_CheckCollisionDelta_DeltaIsCorrect) {
  Circle const      circle(fvec2(2.0f, -3.0f), 5.0f);
  LineSegment const lineSegment(fvec2(8.0f, 3.0f), fvec2(0.0f, -9.0f));

  Intersection2D const intersection = Intersect(circle, lineSegment);

  EXPECT_TRUE(intersection.intersects);
  EXPECT_FLOAT_EQ(intersection.delta, 0.25f);
}

TEST(CollisionTest, IntersectingCircleAndRay_CheckCollision_Intersect) {
}

TEST(CollisionTest, TouchingCircleAndRay_CheckCollision_Intersect) {
}

TEST(CollisionTest, TouchingCircleAndRayRoot_CheckCollision_Intersect) {
}

TEST(CollisionTest, SeparateCircleAndRayWithNonIntersectingLine_CheckCollision_DoNotIntersect) {
}

TEST(CollisionTest, SeparateCircleAndRayWithIntersectingLine_CheckCollision_DoNotIntersect) {
}

TEST(CollisionTest, IntersectingCircleAndRay_CheckCollisionPoint_WithinBoth) {
}

TEST(CollisionTest, IntersectingCircleAndRay_CheckCollisionPoint_ClosestToRoot) {
}

TEST(CollisionTest, IntersectingCircleAndRay_CheckCollisionDelta_DeltaIsCorrect) {
}

TEST(CollisionTest, IntersectingCircleAndRect_CheckCollision_Intersect) {
}

TEST(CollisionTest, TouchingCircleAndRectAtEdge_CheckCollision_Intersect) {
}

TEST(CollisionTest, TouchingCircleAndRectAtCorner_CheckCollision_Intersect) {
}

TEST(CollisionTest, SeparateCircleAndRect_CheckCollision_DoNotIntersect) {
}

TEST(CollisionTest, IntersectingCircleAndRect_CheckCollisionPoint_WithinBoth) {
}

TEST(CollisionTest, IntersectingLines_CheckCollision_Intersect) {
}

TEST(CollisionTest, SeparateParallelLines_CheckCollision_DoNotIntersect) {
}

TEST(CollisionTest, IdenticalParallelLines_CheckCollision_Intersect) {
}

TEST(CollisionTest, FlippedFacingIdenticalParallelLines_CheckCollision_Intersect) {
}

TEST(CollisionTest, IntersectingLines_CheckCollisionPoint_WithinBoth) {
}

TEST(CollisionTest, IdenticalParallelLines_CheckCollisionPoint_WithinBoth) {
}

TEST(CollisionTest, IntersectingLineAndLineSegment_CheckCollision_Intersect) {
}

TEST(CollisionTest, LineAndContainedLineSegment_CheckCollision_Intersect) {
}

TEST(CollisionTest, TouchingLineAndLineSegment_CheckCollision_Intersect) {
}

TEST(CollisionTest, SeparateLineAndLineSegmentWithNonIntersectingLine_CheckCollision_DoNotIntersect) {
}

TEST(CollisionTest, SeparateLineAndLineSegmentWithIntersectingLine_CheckCollision_DoNotIntersect) {
}

TEST(CollisionTest, IntersectingLineAndLineSegment_CheckCollisionPoint_WithinBoth) {
}

TEST(CollisionTest, LineAndContainedLineSegment_CheckCollisionPoint_AtP0) {
}

TEST(CollisionTest, IntersectingLineAndLineSegment_CheckCollisionDelta_DeltaIsCorrect) {
}

TEST(CollisionTest, LineAndContainedLineSegment_CheckCollisionDelta_DeltaIs0) {
}


TEST(CollisionTest, IntersectingLineAndRay_CheckCollision_Intersect) {
}

TEST(CollisionTest, LineAndContainedRay_CheckCollision_Intersect) {
}

TEST(CollisionTest, TouchingLineAndRayRoot_CheckCollision_Intersect) {
}

TEST(CollisionTest, SeparateLineAndRayWithNonIntersectingLine_CheckCollision_DoNotIntersect) {
}

TEST(CollisionTest, SeparateLineAndRayWithIntersectingLine_CheckCollision_DoNotIntersect) {
}

TEST(CollisionTest, IntersectingLineAndRay_CheckCollisionPoint_WithinBoth) {
}

TEST(CollisionTest, LineAndContainedRay_CheckCollisionPoint_AtRoot) {
}

TEST(CollisionTest, IntersectingLineAndRay_CheckCollisionDelta_DeltaIsCorrect) {
}

TEST(CollisionTest, LineAndContainedRay_CheckCollisionDelta_DeltaIs0) {
}

TEST(CollisionTest, IntersectingLineAndRect_CheckCollision_Intersect) {
}

TEST(CollisionTest, TouchingLineAndRect_CheckCollision_Intersect) {
}

TEST(CollisionTest, SeparateLineAndRect_CheckCollision_DoNotIntersect) {
}

TEST(CollisionTest, IntersectingLineAndRect_CheckCollisionPoint_WithinBoth) {
}

TEST(CollisionTest, IntersectingLineSegments_CheckCollision_Intersect) {
}

TEST(CollisionTest, TouchingLineSegments_CheckCollision_Intersect) {
}

TEST(CollisionTest, SeparateLineSegments_CheckCollision_DoNotIntersect) {
}

TEST(CollisionTest, IntersectingLineSegments_CheckCollisionPoint_WithinBoth) {
}

TEST(CollisionTest, IntersectingLineSegments_CheckCollisionDelta_DeltaIsCorrectForLineSegment0) {
}

TEST(CollisionTest, IntersectingLineSegmentAndRay_CheckCollision_Intersect) {
}

TEST(CollisionTest, TouchingLineSegmentAndRayOnLineSegmentPoint_CheckCollision_Intersect) {
}

TEST(CollisionTest, TouchingLineSegmentAndRayOnRayRoot_CheckCollision_Intersect) {
}

TEST(CollisionTest, ContainedLineSegmentAndRay_CheckCollision_Intersect) {
}

TEST(CollisionTest, LineSegmentAndContainedRayRoot_CheckCollision_Intersect) {
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
