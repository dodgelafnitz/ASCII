/*
 * Copywrite 2021 Dodge Lafnitzegger
 */

#include "Math/Shapes2D/Collision.h"
#include "gtest/gtest.h"

TEST(CollisionTest, IntersectingCircles_CheckCollision_Intersect) {
}

TEST(CollisionTest, ContainedCircles_CheckCollision_Intersect) {
}

TEST(CollisionTest, SeparatedCircles_CheckCollision_DoNotIntersect) {
}

TEST(CollisionTest, IntersectingCircles_CheckCollisionPoint_WithinBoth) {
}

TEST(CollisionTest, IntersectingCircleAndLine_CheckCollision_Intersect) {
}

TEST(CollisionTest, TouchingCircleAndLine_CheckCollision_Intersect) {
}

TEST(CollisionTest, SeparatedCircleAndLine_CheckCollision_DoNotIntersect) {
}

TEST(CollisionTest, IntersectingCircleAndLine_CheckCollisionPoint_WithinBoth) {
}

TEST(CollisionTest, IntersectingCircleAndLineSegment_CheckCollision_Intersect) {
}

TEST(CollisionTest, CircleAndContainedLineSegment_CheckCollision_Intersect) {
}

TEST(CollisionTest, TouchingCircleAndLineSegment_CheckCollision_Intersect) {
}

TEST(CollisionTest, TouchingCircleAndLineSegmentEndpoint_CheckCollision_Intersect) {
}

TEST(CollisionTest, SeparateCircleAndLineSegmentWithNonIntersectingLine_CheckCollision_DoNotIntersect) {
}

TEST(CollisionTest, SeparateCircleAndLineSegmentWithIntersectingLine_CheckCollision_DoNotIntersect) {
}

TEST(CollisionTest, IntersectingCircleAndLineSegment_CheckCollisionPoint_WithinBoth) {
}

TEST(CollisionTest, CircleAndContainedLineSegment_CheckCollisionPoint_AtP0) {
}

TEST(CollisionTest, IntersectingCircleAndLineSegment_CheckCollisionPoint_ClosestToP0) {
}

TEST(CollisionTest, IntersectingCircleAndLineSegment_CheckCollisionDelta_DeltaIsCorrect) {
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
