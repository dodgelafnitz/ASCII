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
  fvec2 const center(5.2f, 0.2f);
  fvec2 const dimensions(2.0f, 3.6f);
  Rect const  rect(center, dimensions);
  fvec2 const target(4.0f, -2.4f);

  fvec2 const clamped = rect.Clamp(target);

  EXPECT_FLOAT_EQ(clamped.x, 4.2f);
  EXPECT_FLOAT_EQ(clamped.y, -1.6f);
}

TEST(RectTest, AnyRect_ClampLeftExternalPoint_ClampedPointIsCorrect) {
  fvec2 const center(-1.2f, 0.8f);
  fvec2 const dimensions(1.4f, 2.0f);
  Rect const  rect(center, dimensions);
  fvec2 const target(-2.0f, 1.2f);

  fvec2 const clamped = rect.Clamp(target);

  EXPECT_FLOAT_EQ(clamped.x, -1.9f);
  EXPECT_FLOAT_EQ(clamped.y, 1.2f);
}

TEST(RectTest, AnyRect_ClampAboveLeftExternalPoint_ClampedPointIsCorrect) {
  fvec2 const center(0.1f, 10.2f);
  fvec2 const dimensions(8.4f, 3.8f);
  Rect const  rect(center, dimensions);
  fvec2 const target(-5.3f, 14.4f);

  fvec2 const clamped = rect.Clamp(target);

  EXPECT_FLOAT_EQ(clamped.x, -4.1f);
  EXPECT_FLOAT_EQ(clamped.y, 12.1f);
}

TEST(RectTest, AnyRect_ClampUpperContainedPointToBoundary_ClampedPointIsCorrect) {
  fvec2 const center(-2.3f, -5.1f);
  fvec2 const dimensions(1.4f, 2.8f);
  Rect const  rect(center, dimensions);
  fvec2 const target(-2.2f, -4.0f);

  fvec2 const clamped = rect.ClampToBoundary(target);

  EXPECT_FLOAT_EQ(clamped.x, -2.2f);
  EXPECT_FLOAT_EQ(clamped.y, -3.7);
}

TEST(RectTest, AnyRect_ClampRightContainedPointToBoundary_ClampedPointIsCorrect) {
  fvec2 const center(10.0f, 2.5f);
  fvec2 const dimensions(6.4f, 4.2f);
  Rect const  rect(center, dimensions);
  fvec2 const target(12.6f, 2.7f);

  fvec2 const clamped = rect.ClampToBoundary(target);

  EXPECT_FLOAT_EQ(clamped.x, 13.2f);
  EXPECT_FLOAT_EQ(clamped.y, 2.7f);
}

TEST(RectTest, AnyRect_ClampLowerContainedPointToBoundary_ClampedPointIsCorrect) {
  fvec2 const center(-3.1f, -6.6f);
  fvec2 const dimensions(2.4f, 0.8f);
  Rect const  rect(center, dimensions);
  fvec2 const target(-3.3f, -6.8f);

  fvec2 const clamped = rect.ClampToBoundary(target);

  EXPECT_FLOAT_EQ(clamped.x, -3.3f);
  EXPECT_FLOAT_EQ(clamped.y, -7.0f);
}

TEST(RectTest, AnyRect_ClampLeftContainedPointToBoundary_ClampedPointIsCorrect) {
  fvec2 const center(8.4f, 9.3f);
  fvec2 const dimensions(4.2f, 3.6f);
  Rect const  rect(center, dimensions);
  fvec2 const target(6.7f, 9.0f);

  fvec2 const clamped = rect.ClampToBoundary(target);

  EXPECT_FLOAT_EQ(clamped.x, 6.3f);
  EXPECT_FLOAT_EQ(clamped.y, 9.0f);
}

TEST(RectTest, AnyRect_ClampCenteredPointToBoundary_ClampedPointIsOnBoundary) {
  fvec2 const center(0.0f, 0.0f);
  fvec2 const dimensions(2.0f, 2.0f);
  Rect const  rect(center, dimensions);
  fvec2 const target(0.0f, 0.0f);

  fvec2 const clamped = rect.ClampToBoundary(target);

  bool const onLeftBoundary   = clamped.x == -1.0f;
  bool const onRightBoundary  = clamped.x == 1.0f;
  bool const onTopBoundary    = clamped.y == 1.0f;
  bool const onBottomBoundary = clamped.y == -1.0f;

  bool const onBoundary =
    onLeftBoundary ||
    onRightBoundary ||
    onTopBoundary ||
    onBottomBoundary
  ;

  EXPECT_TRUE(onBoundary) <<
    "  clamped: {" << clamped.x << ", " << clamped.y << "}" << std::endl <<
    "  x or y should be -1.0f or 1.0f." << std::endl
  ;
}

TEST(RectTest, AnyRect_ClampUpperRightContainedPointToBoundary_ClampedPointIsNotInCorner) {
  fvec2 const center(0.0f, 0.0f);
  fvec2 const dimensions(2.0f, 2.0f);
  Rect const  rect(center, dimensions);
  fvec2 const target(0.5f, 0.5f);

  fvec2 const clamped = rect.ClampToBoundary(target);
  fvec2 const corner(1.0f, 1.0f);

  EXPECT_NE(clamped, corner);
}

TEST(RectTest, AnyRect_ClampLowerRightContainedPointToBoundary_ClampedPointIsNotInCorner) {
  fvec2 const center(0.0f, 0.0f);
  fvec2 const dimensions(2.0f, 2.0f);
  Rect const  rect(center, dimensions);
  fvec2 const target(0.5f, -0.5f);

  fvec2 const clamped = rect.ClampToBoundary(target);
  fvec2 const corner(1.0f, -1.0f);

  EXPECT_NE(clamped, corner);
}

TEST(RectTest, AnyRect_ClampLowerLeftContainedPointToBoundary_ClampedPointIsNotInCorner) {
  fvec2 const center(0.0f, 0.0f);
  fvec2 const dimensions(2.0f, 2.0f);
  Rect const  rect(center, dimensions);
  fvec2 const target(-0.5f, -0.5f);

  fvec2 const clamped = rect.ClampToBoundary(target);
  fvec2 const corner(-1.0f, -1.0f);

  EXPECT_NE(clamped, corner);
}

TEST(RectTest, AnyRect_ClampUpperLeftContainedPointToBoundary_ClampedPointIsNotInCorner) {
  fvec2 const center(0.0f, 0.0f);
  fvec2 const dimensions(2.0f, 2.0f);
  Rect const  rect(center, dimensions);
  fvec2 const target(-0.5f, 0.5f);

  fvec2 const clamped = rect.ClampToBoundary(target);
  fvec2 const corner(-1.0f, 1.0f);

  EXPECT_NE(clamped, corner);
}

TEST(RectTest, AnyRect_ClampExternalPointToBoundary_ClampedPointIsCorrect) {
  fvec2 const center(-4.6f, 2.7f);
  fvec2 const dimensions(8.2f, 3.1f);
  Rect const  rect(center, dimensions);
  fvec2 const target(14.2f, 6.2f);

  fvec2 const boundaryClamped = rect.ClampToBoundary(target);
  fvec2 const clamped         = rect.Clamp(target);

  EXPECT_EQ(clamped, boundaryClamped);
}
