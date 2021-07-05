/*
 * Copywrite 2021 Dodge Lafnitzegger
 */

#include "Math/Shapes2D/Ray.h"
#include "gtest/gtest.h"

TEST(RayTest, ConstructedWith0Direction_CheckDirection_DirectionIsValid) {
  Ray const ray;

  EXPECT_FLOAT_EQ(ray.GetDirection().Length(), 1.0f);
}

TEST(RayTest, ConstructedWithAbnormalDirection_CheckDirection_DirectionIsValid) {
  Ray const ray(fvec2(), fvec2(1.5f, 1.8f));

  EXPECT_FLOAT_EQ(ray.GetDirection().Length(), 1.0f);
}

TEST(RayTest, AnyRay_DirectionIsSetTo0_DirectionIsUnchanged) {
  fvec2 const dir = fvec2(-0.2f, 2.2f);
  Ray ray(fvec2(1.3f, 4.2f), dir);

  ray.SetDirection(fvec2(0.0f, 0.0f));

  EXPECT_EQ(ray.GetDirection(), dir);
}

TEST(RayTest, AnyRay_DirectionIsSetToAbnormalVector_DirectionIsValid) {
  Ray ray;

  ray.SetDirection(fvec2(0.1f, -0.3f));

  EXPECT_FLOAT_EQ(ray.GetDirection().Length(), 1.0f);
}

TEST(RayTest, AnyRay_DirectionIsSetToAbnormalVector_RootIsUnchanged) {
  FAIL();
}
TEST(RayTest, AnyRay_RootIsSet_RootIsCorrect) {
  fvec2 const root = fvec2(1.4f, -7.3f);
  Ray ray(fvec2(0.3f, -2.0f), fvec2(0.4f, 9.3f));

  ray.SetRoot(root);

  EXPECT_EQ(ray.GetRoot(), root);
}

TEST(RayTest, AnyRay_RootIsSet_DirectionIsUnchanged) {
  FAIL();
}

TEST(RayTest, AnyRay_DotNormalAndDirection_ResultIs0) {
  FAIL();
}

TEST(RayTest, AnyRay_ObtainedLineFromRay_NormalsAreSame) {
  FAIL();
}

TEST(RayTest, AnyRay_ObtainedLineFromRay_DistanceToRootIs0) {
  FAIL();
}

TEST(RayTest, AnyRay_ObtainedLineFromRayWithFacing_DistanceToRootIs0) {
  FAIL();
}

TEST(RayTest, AnyRay_ObtainedLineFromRayWithFacing_NormalFacesTarget) {
  FAIL();
}

TEST(RayTest, AnyRay_ObtainedLineSegmentFromRay_LengthIsCorrect) {
  FAIL();
}

TEST(RayTest, AnyRay_ObtainedLineSegmentFromRay_NormalsAreSame) {
  FAIL();
}

TEST(RayTest, AnyRay_ObtainedLineSegmentFromRay_RootIsP0OrP1) {
  FAIL();
}

TEST(RayTest, AnyRay_ObtainedNegativeLengthLineSegmentFromRay_LengthIsCorrect) {
  FAIL();
}

TEST(RayTest, AnyRay_ObtainedNegativeLengthLineSegmentFromRay_NormalsAreInverted) {
  FAIL();
}

TEST(RayTest, AnyRay_ObtainedNegativeLengthLineSegmentFromRay_RootIsP0OrP1) {
  FAIL();
}

TEST(RayTest, AnyRay_ObtainedLineSegmentFromRayWithFacing_LengthIsCorrect) {
  FAIL();
}

TEST(RayTest, AnyRay_ObtainedLineSegmentFromRayWithFacing_RootIsP0OrP1) {
  FAIL();
}

TEST(RayTest, AnyRay_ObtainedLineSegmentFromRayWithFacing_NormalFacesTarget) {
  FAIL();
}

TEST(RayTest, AnyRay_ObtainedNegativeLengthLineSegmentFromRayWithFacing_LengthIsCorrect) {
  FAIL();
}

TEST(RayTest, AnyRay_ObtainedNegativeLengthLineSegmentFromRayWithFacing_SegmentGoesInCorrectDirection) {
  FAIL();
}

TEST(RayTest, AnyRay_ObtainedNegativeLengthLineSegmentFromRayWithFacing_NormalFacesTarget) {
  FAIL();
}

TEST(RayTest, AnyRay_CheckDistanceToPointAlongRayPath_DistanceIsCorrect) {
  FAIL();
}

TEST(RayTest, AnyRay_CheckDistanceToPointBeforeRayPath_DistanceIsCorrect) {
  FAIL();
}

TEST(RayTest, AnyRay_ProjectPointOntoRayBeforeRayPath_ProjectionIsRoot) {
  FAIL();
}

TEST(RayTest, AnyRay_ProjectPointOntoRayAlongRayPath_ProjectionIsClosestPointToPoint) {
  FAIL();
}

TEST(RayTest, AnyRay_ProjectPointOntoLineBeforeRayPath_ProjectionIsAlnogDirectionFromRoot) {
  FAIL();
}

TEST(RayTest, AnyRay_ProjectPointOntoLineAlongRayPath_ProjectionIsAlnogDirectionFromRoot) {
  FAIL();
}

