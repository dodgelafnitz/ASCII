/*
 * Copywrite 2021 Dodge Lafnitzegger
 */

#include "Math/Shapes2D/Circle.h"
#include "gtest/gtest.h"

#include "Math/Shapes2D/Ray.h"

TEST(CircleTest, DefaultConstructed_CheckRadius_RadiusIsNonNegative) {
  Circle const circle = Circle();

  EXPECT_GE(circle.GetRadius(), 0.0f);
}

TEST(CircleTest, DefaultConstructed_CheckPosition_PositionIs0) {
  Circle const circle = Circle();

  EXPECT_EQ(circle.GetPosition(), fvec2());
}

TEST(CircleTest, ConstructedWithValues_CheckPosition_PositionIsCorrect) {
  fvec2 const  pos(1.4f, 2.3f);
  Circle const circle(pos, 9.2f);

  EXPECT_EQ(circle.GetPosition(), pos);
}

TEST(CircleTest, ConstructedWithValues_CheckRadius_RadiusIsCorrect) {
  float const  radius = 3.8f;
  Circle const circle(fvec2(-1.8f, 0.3f), radius);

  EXPECT_EQ(circle.GetRadius(), radius);
}

TEST(CircleTest, ConstructedWithNegativeRadius_CheckRadius_RadiusIsNonNegative) {
  float const  radius = -4.9f;
  Circle const circle(fvec2(10.4f, -4.7f), radius);

  EXPECT_EQ(circle.GetRadius(), -radius);
}

TEST(CircleTest, AnyCircle_PositionIsSet_PositionIsCorrect) {
  fvec2 const pos(9.3f, 5.4f);
  Circle      circle(fvec2(1.4f, -0.5f), 10.2f);

  circle.SetPosition(pos);

  EXPECT_EQ(circle.GetPosition(), pos);
}

TEST(CircleTest, AnyCircle_RadiusIsSetToPositive_RadiusIsCorrect) {
  float const radius = 2.2f;
  Circle      circle(fvec2(-4.0f, 6.3f), 2.4f);

  circle.SetRadius(radius);

  EXPECT_EQ(circle.GetRadius(), radius);
}

TEST(CircleTest, AnyCircle_RadiusIsSetToNegative_RadiusIsPositive) {
  float const radius = -5.9f;
  Circle      circle(fvec2(1.0f, 1.0f), 0.2f);

  circle.SetRadius(radius);

  EXPECT_EQ(circle.GetRadius(), -radius);
}

TEST(CircleTest, AnyCircle_RadiusIsSetTo0_RadiusIs0) {
  Circle circle(fvec2(-12.7f, -2.5f), 3.4f);

  circle.SetRadius(0.0f);

  EXPECT_EQ(circle.GetRadius(), 0.0f);
}

TEST(CircleTest, AnyCircle_CheckContainsContainedPoint_PointIsContained) {
  Circle const circle(fvec2(0.4f, 2.1f), 8.8f);
  fvec2 const  target(1.3f, 0.4f);

  EXPECT_TRUE(circle.Contains(target));
}

TEST(CircleTest, AnyCircle_CheckContainsExternalPoint_PointIsNotContained) {
  Circle const circle(fvec2(-2.6f, 1.3f), 4.2f);
  fvec2 const  target(-4.1f, -6.2f);

  EXPECT_FALSE(circle.Contains(target));
}

TEST(CircleTest, AnyCircle_CheckContainsBoundaryPoint_PointIsContained) {
  Circle const circle(fvec2(0.3f, 0.6f), 1.0f);
  fvec2 const  target(0.3f, 1.6f);

  EXPECT_TRUE(circle.Contains(target));
}

TEST(CircleTest, AnyCircle_CheckDistanceToContainedPoint_DistanceIs0) {
  Circle const circle(fvec2(1.7f, -4.2f), 4.2f);
  fvec2 const  target(1.2f, -2.1f);

  EXPECT_EQ(circle.DistanceTo(target), 0.0f);
}

TEST(CircleTest, AnyCircle_CheckDistanceToExternalPoint_DistanceIsCorrect) {
  Circle const circle(fvec2(0.2f, 0.5f), 2.3f);
  fvec2 const  target(0.2f, -5.3f);

  EXPECT_FLOAT_EQ(circle.DistanceTo(target), 3.5f);
}

TEST(CircleTest, AnyCircle_CheckDistanceToBoundaryPoint_DistanceIs0) {
  Circle const circle(fvec2(7.3f, -2.4f), 10.2f);
  fvec2 const  target(7.3f, 7.8f);

  EXPECT_NEAR(circle.DistanceTo(target), 0.0f, 0.0001f);
}

TEST(CircleTest, AnyCircle_ClampContainedPoint_ResultIsTarget) {
  Circle const circle(fvec2(-2.4f, -15.3f), 2.1f);
  fvec2 const  target(-2.6f, -14.7f);

  EXPECT_EQ(circle.Clamp(target), target);
}

TEST(CircleTest, AnyCircle_ClampExternalPoint_ResultIsOnBoundary) {
  float const  radius = 4.9f;
  Circle const circle(fvec2(1.8f, -2.6f), radius);
  fvec2 const  target(8.4f, -10.1f);

  fvec2 const clamped = circle.Clamp(target);

  EXPECT_FLOAT_EQ((clamped - circle.GetPosition()).Length(), radius);
}

TEST(CircleTest, AnyCircle_ClampExternalPoint_ResultIsTowardsTarget) {
  Circle const circle(fvec2(-8.2f, 3.9f), 2.7f);
  fvec2 const  target(16.1f, 4.2f);

  fvec2 const clamped = circle.Clamp(target);
  fvec2 const diff    = target - circle.GetPosition();

  Ray const ray(circle.GetPosition(), diff);
  EXPECT_NEAR(ray.DistanceTo(clamped), 0.0f, 0.0001f);
}

TEST(CircleTest, AnyCircle_ClampBoundaryPoint_ResultIsTarget) {
  Circle const circle(fvec2(0.4f, 12.1f), 3.6f);
  fvec2 const  target(0.4f, 8.5f);

  fvec2 const clamped = circle.Clamp(target);
  fvec2 const diff    = target - circle.GetPosition();

  EXPECT_EQ(clamped, target);
}

TEST(CircleTest, AnyCircle_ClampContainedPointToBoundary_ResultIsOnBoundary) {
  float const  radius = 8.2f;
  Circle const circle(fvec2(-1.7f, -10.4f), radius);
  fvec2 const  target(-3.1f, -5.2f);

  fvec2 const clamped = circle.ClampToBoundary(target);

  EXPECT_FLOAT_EQ((clamped - circle.GetPosition()).Length(), radius);
}

TEST(CircleTest, AnyCircle_ClampContainedPointToBoundary_ResultIsTowardsTarget) {
  Circle const circle(fvec2(10.2f, 1.4f), 1.8f);
  fvec2 const  target(11.4f, 1.1f);

  fvec2 const clamped = circle.Clamp(target);
  fvec2 const diff    = target - circle.GetPosition();

  Ray const ray(circle.GetPosition(), diff);
  EXPECT_FLOAT_EQ(ray.DistanceTo(clamped), 0.0f);
}

TEST(CircleTest, AnyCircle_ClampCenterToBoundary_ResultIsOnBoundary) {
  float const  radius = 1.2f;
  fvec2 const  center(5.5f, 7.2f);
  Circle const circle(center, radius);

  fvec2 const clamped = circle.ClampToBoundary(center);

  EXPECT_FLOAT_EQ((clamped - circle.GetPosition()).Length(), radius);
}

TEST(CircleTest, AnyCircle_ClampExternalPointToBoundary_ResultIsOnBoundary) {
  float const  radius = 6.2f;
  Circle const circle(fvec2(-3.8f, 3.2f), radius);
  fvec2 const  target(-11.3f, -2.4f);

  fvec2 const clamped = circle.ClampToBoundary(target);

  EXPECT_FLOAT_EQ((clamped - circle.GetPosition()).Length(), radius);
}

TEST(CircleTest, AnyCircle_ClampExternalPointToBoundary_ResultIsTowardsTarget) {
  Circle const circle(fvec2(2.3f, -4.1f), 0.3f);
  fvec2 const  target(-2.4f, 0.9f);

  fvec2 const clamped = circle.Clamp(target);
  fvec2 const diff    = target - circle.GetPosition();

  Ray const ray(circle.GetPosition(), diff);
  EXPECT_NEAR(ray.DistanceTo(clamped), 0.0f, 0.0001f);
}

TEST(CircleTest, AnyCircle_ClampBoundaryPointToBoundary_ResultIsTarget) {
  Circle const circle(fvec2(10.3f, 2.6f), 4.3f);
  fvec2 const  target(6.0f, 2.6f);

  fvec2 const clamped = circle.Clamp(target);
  fvec2 const diff = target - circle.GetPosition();

  EXPECT_EQ(clamped, target);
}
