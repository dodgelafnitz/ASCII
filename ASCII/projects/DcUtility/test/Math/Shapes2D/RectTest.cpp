/*
 * Copywrite 2021 Dodge Lafnitzegger
 */

#include "Math/Shapes2D/Rect.h"
#include "gtest/gtest.h"

TEST(RectTest, DefautConstructed_CheckDimensions_DimensionsAreNonNegative) {
  Rect const rect = Rect();

  EXPECT_GE(rect.GetDimensions().x, 0.0f);
  EXPECT_GE(rect.GetDimensions().y, 0.0f);
}

TEST(RectTest, ConstructedWithValues_CheckCenter_CenterIsCorrect) {
  fvec2 const center(1.0f, 3.4f);
  fvec2 const dimensions(0.5f, 0.8f);
  Rect const  rect(center, dimensions);

  EXPECT_EQ(rect.GetCenter(), center);
}

TEST(RectTest, ConstructedWithValues_CheckDimensions_DimensionsAreCorrect) {
  fvec2 const center(0.3f, 9.2f);
  fvec2 const dimensions(10.0f, 4.0f);
  Rect const  rect(center, dimensions);

  EXPECT_EQ(rect.GetDimensions(), dimensions);
}

TEST(RectTest, ConstructedWithNegativeDimensions_CheckDimensions_DimensionsAreCorrect) {
  fvec2 const center(0.3f, 9.2f);
  fvec2 const dimensions(-3.0f, -1.5f);
  Rect const  rect(center, dimensions);

  EXPECT_EQ(rect.GetDimensions(), -dimensions);
}

TEST(RectTest, AnyRect_SetCenter_CenterIsCorrect) {
  Rect rect;
  fvec2 const center(2.4f, 0.7f);

  rect.SetCenter(center);

  EXPECT_EQ(rect.GetCenter(), center);
}

TEST(RectTest, AnyRect_SetDimensionsToNonNegativeValues_DimensionsAreCorrect) {
  Rect rect;
  fvec2 const dimensions(1.3f, 8.2f);

  rect.SetDimensions(dimensions);

  EXPECT_EQ(rect.GetDimensions(), dimensions);
}

TEST(RectTest, AnyRect_SetDimensionsToNegativeValues_DimensionsAreNonNegative) {
  Rect rect;
  fvec2 const dimensions(-2.4f, -0.3f);

  rect.SetDimensions(dimensions);

  EXPECT_EQ(rect.GetDimensions(), -dimensions);
}

TEST(RectTest, AnyRect_CheckContainsContainedPoint_PointIsContained) {
  fvec2 const center(7.3f, -9.2f);
  fvec2 const dimensions(2.4f, 1.3f);
  Rect const  rect(center, dimensions);
  fvec2 const target(6.4f, -9.4f);

  EXPECT_TRUE(rect.Contains(target));
}

TEST(RectTest, AnyRect_CheckContainsExternalPoint_PointIsNotContained) {
  fvec2 const center(-3.5f, 3.6f);
  fvec2 const dimensions(0.5f, 0.8f);
  Rect const  rect(center, dimensions);
  fvec2 const target(-3.6f, 4.2f);

  EXPECT_FALSE(rect.Contains(target));
}

TEST(RectTest, AnyRect_CheckContainsBoundaryPoint_PointIsContained) {
  fvec2 const center(8.4f, -2.0f);
  fvec2 const dimensions(1.0f, 2.0f);
  Rect const  rect(center, dimensions);
  fvec2 const target(8.3f, -1.0f);

  EXPECT_TRUE(rect.Contains(target));
}

TEST(RectTest, AnyRect_CheckDistanceToContainedPoint_DistanceIs0) {
  fvec2 const center(-2.6f, 1.3f);
  fvec2 const dimensions(0.2f, 8.3f);
  Rect const  rect(center, dimensions);
  fvec2 const target(-2.6f, 5.1f);

  EXPECT_EQ(rect.DistanceTo(target), 0.0f);
}

TEST(RectTest, AnyRect_CheckDistanceToExternalPoint_DistanceIsCorrect) {
  fvec2 const center(0.0f, 1.5f);
  fvec2 const dimensions(1.3f, 5.4f);
  Rect const  rect(center, dimensions);
  fvec2 const target(0.4f, 8.2f);

  EXPECT_EQ(rect.DistanceTo(target), 4.0f);
}

TEST(RectTest, AnyRect_CheckDistanceToBoundaryPoint_DistanceIs0) {
  fvec2 const center(-10.2f, 4.3f);
  fvec2 const dimensions(8.0f, 2.6f);
  Rect const  rect(center, dimensions);
  fvec2 const target(-12.0f, 3.0f);

  EXPECT_NEAR(rect.DistanceTo(target), 0.0f, 0.0001f);
}

TEST(RectTest, AnyRect_ClampContainedPoint_PointIsUnchanged) {
  fvec2 const center(2.3f, 8.1f);
  fvec2 const dimensions(2.1f, 0.5f);
  Rect const  rect(center, dimensions);
  fvec2 const target(2.6f, 8.2f);

  fvec2 const clamped = rect.Clamp(target);

  EXPECT_EQ(clamped, target);
}

TEST(RectTest, AnyRect_ClampAboveExternalPoint_ClampedPointIsCorrect) {
  fvec2 const center(-2.1f, 4.2f);
  fvec2 const dimensions(4.4f, 2.8f);
  Rect const  rect(center, dimensions);
  fvec2 const target(-0.3f, 7.3f);

  fvec2 const clamped = rect.Clamp(target);

  EXPECT_FLOAT_EQ(clamped.x, -0.3f);
  EXPECT_FLOAT_EQ(clamped.y, 5.6f);
}

TEST(RectTest, AnyRect_ClampAboveRightExternalPoint_ClampedPointIsCorrect) {
  fvec2 const center(6.3, -3.8f);
  fvec2 const dimensions(2.0f, 4.0f);
  Rect const  rect(center, dimensions);
  fvec2 const target(7.6f, -0.5f);

  fvec2 const clamped = rect.Clamp(target);

  EXPECT_FLOAT_EQ(clamped.x, 7.3f);
  EXPECT_FLOAT_EQ(clamped.y, -1.8f);
}

TEST(RectTest, AnyRect_ClampRightExternalPoint_ClampedPointIsCorrect) {
  fvec2 const center(0.3f, -2.1f);
  fvec2 const dimensions(1.4f, 2.6f);
  Rect const  rect(center, dimensions);
  fvec2 const target(2.7f, -3.2f);

  fvec2 const clamped = rect.Clamp(target);

  EXPECT_FLOAT_EQ(clamped.x, 1.0f);
  EXPECT_FLOAT_EQ(clamped.y, -3.2f);
}

TEST(RectTest, AnyRect_ClampBelowRightExternalPoint_ClampedPointIsCorrect) {
  fvec2 const center(0.0f, -6.2f);
  fvec2 const dimensions(5.4f, 2.4f);
  Rect const  rect(center, dimensions);
  fvec2 const target(4.3f, -8.2f);

  fvec2 const clamped = rect.Clamp(target);

  EXPECT_FLOAT_EQ(clamped.x, 2.7f);
  EXPECT_FLOAT_EQ(clamped.y, -7.4f);
}

TEST(RectTest, AnyRect_ClampBelowExternalPoint_ClampedPointIsCorrect) {
  fvec2 const center(1.4f, 2.7f);
  fvec2 const dimensions(8.0f, 2.8f);
  Rect const  rect(center, dimensions);
  fvec2 const target(4.3f, -2.1f);

  fvec2 const clamped = rect.Clamp(target);

  EXPECT_FLOAT_EQ(clamped.x, 4.3f);
  EXPECT_FLOAT_EQ(clamped.y, 1.3f);
}

TEST(RectTest, AnyRect_ClampBelowLeftExternalPoint_ClampedPointIsCorrect) {
  FAIL();
}

TEST(RectTest, AnyRect_ClampLeftExternalPoint_ClampedPointIsCorrect) {
  FAIL();
}

TEST(RectTest, AnyRect_ClampAboveLeftExternalPoint_ClampedPointIsCorrect) {
  FAIL();
}

TEST(RectTest, AnyRect_ClampUpperContainedPointToBoundary_ClampedPointIsCorrect) {
  FAIL();
}

TEST(RectTest, AnyRect_ClampRightContainedPointToBoundary_ClampedPointIsCorrect) {
  FAIL();
}

TEST(RectTest, AnyRect_ClampLowerContainedPointToBoundary_ClampedPointIsCorrect) {
  FAIL();
}

TEST(RectTest, AnyRect_ClampLeftContainedPointToBoundary_ClampedPointIsCorrect) {
  FAIL();
}

TEST(RectTest, AnyRect_ClampCenteredPointToBoundary_ClampedPointIsOnBoundary) {
  FAIL();
}

TEST(RectTest, AnyRect_ClampUpperRightContainedPointToBoundary_ClampedPointIsNotInCorner) {
  FAIL();
}

TEST(RectTest, AnyRect_ClampLowerRightContainedPointToBoundary_ClampedPointIsNotInCorner) {
  FAIL();
}

TEST(RectTest, AnyRect_ClampLowerLeftContainedPointToBoundary_ClampedPointIsNotInCorner) {
  FAIL();
}

TEST(RectTest, AnyRect_ClampUpperLeftContainedPointToBoundary_ClampedPointIsNotInCorner) {
  FAIL();
}
TEST(RectTest, AnyRect_ClampExternalPointToBoundary_ClampedPointIsCorrect) {
  FAIL();
}
