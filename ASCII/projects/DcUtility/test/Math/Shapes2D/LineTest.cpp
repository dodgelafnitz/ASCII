/*
 * Copywrite 2021 Dodge Lafnitzegger
 */

#include "Math/Shapes2D/Line.h"
#include "gtest/gtest.h"

#include "Math/Shapes2D/LineSegment.h"
#include "Math/Shapes2D/Ray.h"

TEST(LineTest, DefaultConstructed_CheckProperties_RootIsOrigin) {
  Line const line;

  EXPECT_EQ(line.GetRoot(), fvec2());
}

TEST(LineTest, DefaultConstructed_CheckProperties_NormalIsNormalizedAndNon0) {
  Line const line;

  EXPECT_NE(line.GetNormal(), fvec2());
  EXPECT_FLOAT_EQ(line.GetNormal().LengthSquared(), 1.0f);
}

TEST(LineTest, RootIsSet_CheckRoot_DistanceToRootIs0) {
  Line line;

  line.SetRoot(fvec2(4.0f, 2.5f));
  EXPECT_FLOAT_EQ(line.DistanceTo(fvec2(4.0f, 2.5f)), 0.0f);
}

TEST(LineTest, RootIsSet_CheckNormal_NormalIsUnchanged) {
  Line line(fvec2(0.0f, -1.0f), 2.0f);

  line.SetRoot(fvec2(4.0f, 2.5f));
  EXPECT_EQ(line.GetNormal(), fvec2(0.0f, -1.0f));
}

TEST(LineTest, NormalIsSet_CheckNormal_NormalIsCorrectValueAndNormalized) {
  Line line(fvec2(0.0f, -1.0f), 2.0f);

  line.SetNormal(fvec2(3.0f, -4.0f));
  EXPECT_EQ(line.GetNormal(), fvec2(0.6f, -0.8f));
}

TEST(LineTest, NormalIsSetTo0_CheckNormal_NormalIsUnchanged) {
  Line line(fvec2(0.0f, -1.0f), 2.0f);

  line.SetNormal(fvec2());
  EXPECT_EQ(line.GetNormal(), fvec2(0.0f, -1.0f));
}

TEST(LineTest, FacingIsSet_CheckSignedDistanceToTarget_ResultIsNonNegative) {
  Line line(fvec2(0.5f, 0.5f), 3.0f);

  line.SetFacing(fvec2(0.0f, 0.0f));
  EXPECT_GE(line.SignedDistanceTo(fvec2(0.0f, 0.0f)), 0.0f);
}

TEST(LineTest, AnotherPointInFront_CheckSignedDistance_DistanceIsCorrect) {
  Line const line(fvec2(0.6f, 0.8f), 3.0f);

  EXPECT_FLOAT_EQ(line.SignedDistanceTo(fvec2(6.0f, 4.0f)), 3.8f);
}

TEST(LineTest, AnotherPointBehind_CheckSignedDistance_DistanceIsCorrect) {
  Line const line(fvec2(0.6f, -0.8f), 0.5f);

  EXPECT_FLOAT_EQ(line.SignedDistanceTo(fvec2(2.0f, 3.0f)), -1.7f);
}

TEST(LineTest, AnotherPointInFront_CheckUnsignedDistance_DistanceIsPositive) {
  Line const line(fvec2(0.6f, 0.8f), 3.0f);

  EXPECT_GT(line.DistanceTo(fvec2(1.0f, 12.0f)), 0.0f);
}

TEST(LineTest, AnotherPointBehind_CheckUnsignedDistance_DistanceIsPositive) {
  Line const line(fvec2(0.6f, 0.8f), 3.0f);

  EXPECT_GT(line.DistanceTo(fvec2(1.0f, -12.0f)), 0.0f);
}

TEST(LineTest, ProjectedPoint_CheckDistance_DistanceIs0) {
  Line const  line(fvec2(124.0f, 302.0f), 8.4f);
  fvec2 const point = line.ProjectPointOnto(fvec2(-13.0f, 27.0f));

  EXPECT_NEAR(line.DistanceTo(point), 0.0f, 0.0001f);
}

TEST(LineTest, ScaleIsSet_CheckScale_ScaleIsCorrectValue) {
  Line line(fvec2(0.0f, -1.0f), 2.0f);

  line.SetDistanceFromOrigin(1.0f);
  EXPECT_EQ(line.GetDistanceFromOrigin(), 1.0f);
}

TEST(LineTest, ScaleIsSet_CheckScale_NormalIsUnchanged) {
  Line line(fvec2(0.0f, -1.0f), 2.0f);

  line.SetDistanceFromOrigin(1.0f);
  EXPECT_EQ(line.GetNormal(), fvec2(0.0f, -1.0f));
}

TEST(LineTest, FacingIsSet_CheckRoot_RootIsUnchanged) {
  Line line(fvec2(0.0f, -1.0f), 2.0f);

  fvec2 const root = line.GetRoot();

  line.SetFacing(fvec2(0.0f, 0.0f));
  EXPECT_EQ(line.GetRoot(), root);
}
