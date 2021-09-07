/*
 * Copywrite 2021 Dodge Lafnitzegger
 */

#include "Math/Shapes2D/LineSegment.h"
#include "gtest/gtest.h"

TEST(LineSegmentTest, CollapsedLineSegment_CheckDistanceToPoint_DistanceIsCorrect) {
  fvec2 const       pos(0.4f, -1.9f);
  LineSegment const lineSegment(pos, pos);

  EXPECT_FLOAT_EQ(lineSegment.DistanceTo(fvec2(3.4f, 2.1f)), 5.0f);
}

TEST(LineSegmentTest, CollapsedLineSegment_ProjectPointOntoSegment_ResultIsCollapsedPoint) {
  fvec2 const       pos(13.0f, 2.3f);
  LineSegment const lineSegment(pos, pos);

  EXPECT_EQ(lineSegment.ProjectPointOntoLineSegment(fvec2(4.1f, -10.5f)), pos);
}

TEST(LineSegmentTest, CollapsedLineSegment_ProjectPointOntoLine_ResultIsCollapsedPoint) {
  fvec2 const       pos(-2.1f, 8.7f);
  LineSegment const lineSegment(pos, pos);

  EXPECT_EQ(lineSegment.ProjectPointOntoLine(fvec2(12.2f, 1.1f)), pos);
}

TEST(LineSegmentTest, CollapsedLineSegment_CheckDeltaToProjection_ResultIs0) {
  fvec2 const       pos(-1.0f, 0.1f);
  LineSegment const lineSegment(pos, pos);

  EXPECT_EQ(lineSegment.GetDeltaToProjection(fvec2(-2.2f, 17.9f)), 0.0f);
}

TEST(LineSegmentTest, CollapsedLineSegment_CheckNormal_NormalIs0) {
  fvec2 const       pos(12.1f, 2.5f);
  LineSegment const lineSegment(pos, pos);

  EXPECT_EQ(lineSegment.GetNormal(), fvec2());
}

TEST(LineSegmentTest, CollapsedLineSegment_CheckLengthSquared_LengthSquaredIs0) {
  fvec2 const       pos(-8.3f, 0.0f);
  LineSegment const lineSegment(pos, pos);

  EXPECT_EQ(lineSegment.GetNormal(), fvec2());
}

TEST(LineSegmentTest, CollapsedLineSegment_ConvertToLine_LineGoesThroughPoint) {
  fvec2 const       pos(0.1f, -3.3f);
  LineSegment const lineSegment(pos, pos);
  Line const        line = lineSegment.GetLine();

  EXPECT_FLOAT_EQ(line.DistanceTo(pos), 0.0f);
}

TEST(LineSegmentTest, CollapsedLineSegment_FaceTowardsPoint_DoesNothing) {
  fvec2 const pos(-10.2f, -0.1f);
  LineSegment lineSegment(pos, pos);

  lineSegment.FaceTowards(fvec2());
  EXPECT_EQ(lineSegment.p0, pos);
  EXPECT_EQ(lineSegment.p1, pos);
}

TEST(LineSegmentTest, CollapsedLineSegment_FaceAwayFromPoint_DoesNothing) {
  fvec2 const pos(-7.4f, 4.9f);
  LineSegment lineSegment(pos, pos);

  lineSegment.FaceAway(fvec2());
  EXPECT_EQ(lineSegment.p0, pos);
  EXPECT_EQ(lineSegment.p1, pos);
}

TEST(LineSegmentTest, ValidLineSegment_ProjectPointOntoLineSegmentBeforeP0_DistanceToResultIs0) {
  LineSegment const lineSegment(fvec2(1.3f, 0.2f), fvec2(-3.7f, 5.0f));
  fvec2 const       target(2.3f, -1.0f);

  fvec2 const projected = lineSegment.ProjectPointOntoLineSegment(target);

  EXPECT_FLOAT_EQ(lineSegment.DistanceTo(projected), 0.0f);
}

TEST(LineSegmentTest, ValidLineSegment_ProjectPointOntoLineSegmentAfterP1_DistanceToResultIs0) {
  LineSegment const lineSegment(fvec2(-2.0f, 10.4f), fvec2(0.0f, 11.3f));
  fvec2 const       target(1.1f, 12.0f);

  fvec2 const projected = lineSegment.ProjectPointOntoLineSegment(target);

  EXPECT_FLOAT_EQ(lineSegment.DistanceTo(projected), 0.0f);
}

TEST(LineSegmentTest, ValidLineSegment_ProjectPointOntoLineSegmentBetweenPoints_DistanceToResultIs0) {
  LineSegment const lineSegment(fvec2(-14.3f, 1.0f), fvec2(23.9f, -4.3f));
  fvec2 const       target(3.6f, -0.3f);

  fvec2 const projected = lineSegment.ProjectPointOntoLineSegment(target);

  EXPECT_FLOAT_EQ(lineSegment.DistanceTo(projected), 0.0f);
}

TEST(LineSegmentTest, ValidLineSegment_ProjectPointOntoLineSegmentBeforeP0_DeltaToResultIs0) {
  LineSegment const lineSegment(fvec2(0.3f, 8.4f), fvec2(-2.1f, 0.5f));
  fvec2 const       target(1.4f, 10.5f);

  fvec2 const projected = lineSegment.ProjectPointOntoLineSegment(target);

  EXPECT_FLOAT_EQ(lineSegment.GetDeltaToProjection(projected), 0.0f);
}

TEST(LineSegmentTest, ValidLineSegment_ProjectPointOntoLineSegmentAfterP1_DeltaToResultIsBetween0And1) {
  LineSegment const lineSegment(fvec2(-10.8f, 9.2f), fvec2(1.4f, -4.0f));
  fvec2 const       target(2.5f, -6.3f);

  fvec2 const projected = lineSegment.ProjectPointOntoLineSegment(target);

  EXPECT_FLOAT_EQ(lineSegment.GetDeltaToProjection(projected), 1.0f);
}

TEST(LineSegmentTest, ValidLineSegment_ProjectPointOntoLineSegmentBetweenPoints_DeltaToResultIs1) {
  LineSegment const lineSegment(fvec2(1.0f, 1.2f), fvec2(1.6f, 1.3f));
  fvec2 const       target(1.4f, 1.3f);

  fvec2 const projected = lineSegment.ProjectPointOntoLineSegment(target);

  EXPECT_GT(lineSegment.GetDeltaToProjection(projected), 0.0f);
  EXPECT_LT(lineSegment.GetDeltaToProjection(projected), 1.0f);
}

TEST(LineSegmentTest, ValidLineSegment_ProjectPointOntoLineBeforeP0_DistanceToResultIsDistanceToP0) {
  LineSegment const lineSegment(fvec2(0.1f, 2.8f), fvec2(-1.4f, -9.9f));
  fvec2 const       target(1.4f, 9.0f);

  fvec2 const projected = lineSegment.ProjectPointOntoLine(target);
  float const distance  = (projected - lineSegment.p0).Length();

  EXPECT_FLOAT_EQ(lineSegment.DistanceTo(projected), distance);
}

TEST(LineSegmentTest, ValidLineSegment_ProjectPointOntoLineAfterP1_DistanceToResultIsDistanceToP1) {
  LineSegment const lineSegment(fvec2(-5.3f, -1.1f), fvec2(4.2f, -0.3f));
  fvec2 const       target(6.3f, -0.1f);

  fvec2 const projected = lineSegment.ProjectPointOntoLine(target);
  float const distance = (projected - lineSegment.p1).Length();

  EXPECT_FLOAT_EQ(lineSegment.DistanceTo(projected), distance);
}

TEST(LineSegmentTest, ValidLineSegment_ProjectPointOntoLineBetweenPoints_DistanceToResultIs0) {
  LineSegment const lineSegment(fvec2(0.3f, 13.5f), fvec2(0.5f, -10.2f));
  fvec2 const       target(-3.9f, 0.7f);

  fvec2 const projected = lineSegment.ProjectPointOntoLine(target);

  EXPECT_FLOAT_EQ(lineSegment.DistanceTo(projected), 0.0f);
}

TEST(LineSegmentTest, ValidLineSegment_GetDeltaToPointBeforeP0_ResultIsNegative) {
  LineSegment const lineSegment(fvec2(2.3f, 0.1f), fvec2(13.4f, -0.3f));
  fvec2 const       target(-2.1f, 0.0f);

  EXPECT_LT(lineSegment.GetDeltaToProjection(target), 0.0f);
}

TEST(LineSegmentTest, ValidLineSegment_GetDeltaToPointAfterP1_ResultIsGreaterThan1) {
  LineSegment const lineSegment(fvec2(-1.6f, 8.3f), fvec2(0.3f, -2.2f));
  fvec2 const       target(0.5f, -2.9f);

  EXPECT_GT(lineSegment.GetDeltaToProjection(target), 1.0f);
}

TEST(LineSegmentTest, ValidLineSegment_GetDeltaToPointBetweenEndpoints_ResultIsBetween0And1) {
  LineSegment const lineSegment(fvec2(-10.4f, 2.2f), fvec2(8.6f, 3.0f));
  fvec2 const       target(1.6f, 3.7f);

  float const delta = lineSegment.GetDeltaToProjection(target);

  EXPECT_GT(delta, 0.0f);
  EXPECT_LT(delta, 1.0f);
}

TEST(LineSegmentTest, ValidLineSegment_GetDeltaToP0_ResultIs0) {
  LineSegment const lineSegment(fvec2(0.1f, -11.5f), fvec2(-12.2f, -5.3f));

  EXPECT_FLOAT_EQ(lineSegment.GetDeltaToProjection(lineSegment.p0), 0.0f);
}

TEST(LineSegmentTest, ValidLineSegment_GetDeltaToP1_ResultIs1) {
  LineSegment const lineSegment(fvec2(-3.2f, -1.1f), fvec2(14.6f, 8.9f));

  EXPECT_FLOAT_EQ(lineSegment.GetDeltaToProjection(lineSegment.p1), 1.0f);
}

TEST(LineSegmentTest, ValidLineSegment_GetPointAtNegativeDelta_ResultIsCorrect) {
  LineSegment const lineSegment(fvec2(2.0f, -4.0f), fvec2(3.0f, -5.0f));

  EXPECT_EQ(lineSegment.GetDeltaPoint(-1.0f), fvec2(1.0f, -3.0f));
}

TEST(LineSegmentTest, ValidLineSegment_GetPointAtDelta0_ResultIsP0) {
  LineSegment const lineSegment(fvec2(6.1f, 2.3f), fvec2(1.2f, -5.4f));

  EXPECT_EQ(lineSegment.GetDeltaPoint(0.0f), lineSegment.p0);
}

TEST(LineSegmentTest, ValidLineSegment_GetPointAtNormalDelta_ResultIsCorrect) {
  LineSegment const lineSegment(fvec2(2.5f, -0.5f), fvec2(4.5f, -1.5f));

  EXPECT_EQ(lineSegment.GetDeltaPoint(0.5f), fvec2(3.5f, -1.0f));
}

TEST(LineSegmentTest, ValidLineSegment_GetPointAtDelta1_ResultIsP1) {
  LineSegment const lineSegment(fvec2(2.7f, -1.3f), fvec2(-1.7f, 12.8f));

  EXPECT_EQ(lineSegment.GetDeltaPoint(1.0f), lineSegment.p1);
}

TEST(LineSegmentTest, ValidLineSegment_GetPointAtDeltaGreaterThan1_ResultIsCorrect) {
  LineSegment const lineSegment(fvec2(10.5f, 2.5f), fvec2(9.5f, 4.0f));

  EXPECT_EQ(lineSegment.GetDeltaPoint(2.0f), fvec2(8.5f, 5.5f));
}

TEST(LineSegmentTest, ValidLineSegment_ViewNormal_NormalHasLength1) {
  LineSegment const lineSegment(fvec2(0.0f, -1.1f), fvec2(-0.2f, -0.7f));

  EXPECT_FLOAT_EQ(lineSegment.GetNormal().Length(), 1.0f);
}

TEST(LineSegmentTest, ValidLineSegment_DotNormalWithEndpoints_ValuesAreSame) {
  LineSegment const lineSegment(fvec2(0.3f, 2.1f), fvec2(-0.1f, 1.8f));
  fvec2 const       normal = lineSegment.GetNormal();

  EXPECT_FLOAT_EQ(normal.Dot(lineSegment.p0), normal.Dot(lineSegment.p1));
}

TEST(LineSegmentTest, ValidLineSegment_GetLength_LengthIsDistanceBetweenPoints) {
  LineSegment const lineSegment(fvec2(10.4f, 12.3f), fvec2(4.2f, -8.5f));
  float const       length = (lineSegment.p0 - lineSegment.p1).Length();

  EXPECT_FLOAT_EQ(lineSegment.Length(), length);
}

TEST(LineSegmentTest, ValidLineSegment_GetLengthSquared_LengthIsSquaredDistanceBetweenPoints) {
  LineSegment const lineSegment(fvec2(0.1f, 0.5f), fvec2(0.6f, 19.0f));
  float const       lengthSquared = (lineSegment.p0 - lineSegment.p1).LengthSquared();

  EXPECT_FLOAT_EQ(lineSegment.LengthSquared(), lengthSquared);
}

TEST(LineSegmentTest, ValidLineSegment_GetLine_EndpointsAreOnLine) {
  LineSegment const lineSegment(fvec2(-5.4f, -0.2f), fvec2(-1.3f, 16.2f));
  Line const        line = lineSegment.GetLine();

  EXPECT_FLOAT_EQ(line.DistanceTo(lineSegment.p0), 0.0f);
  EXPECT_FLOAT_EQ(line.DistanceTo(lineSegment.p1), 0.0f);
}

TEST(LineSegmentTest, ValidLineSegment_GetLine_NormalsAreSame) {
  LineSegment const lineSegment(fvec2(0.1f, 2.0f), fvec2(-2.0f, -25.1f));
  Line const        line = lineSegment.GetLine();

  EXPECT_EQ(line.GetNormal(), lineSegment.GetNormal());
}

TEST(LineSegmentTest, ValidLineSegment_FaceTowardsPointOffLine_LinesSignedDistanceIsPositive) {
  LineSegment lineSegment(fvec2(1.3f, -7.2f), fvec2(0.5f, 6.1f));
  fvec2 const target(1.3f, 9.3f);

  Line const lineBefore = lineSegment.GetLine();
  EXPECT_LT(lineBefore.SignedDistanceTo(target), 0.0f);

  lineSegment.FaceTowards(target);
  Line const lineAfter = lineSegment.GetLine();
  EXPECT_GT(lineAfter.SignedDistanceTo(target), 0.0f);
}

TEST(LineSegmentTest, ValidLineSegment_FaceAwayFromPointOffLine_LinesSignedDistanceIsNegative) {
  LineSegment lineSegment(fvec2(10.3f, 0.3f), fvec2(-2.0f, 0.7f));
  fvec2 const target(0.3f, -2.8f);

  Line const lineBefore = lineSegment.GetLine();
  EXPECT_GT(lineBefore.SignedDistanceTo(target), 0.0f);

  lineSegment.FaceAway(target);
  Line const lineAfter = lineSegment.GetLine();
  EXPECT_LT(lineAfter.SignedDistanceTo(target), 0.0f);
}

TEST(LineSegmentTest, ValidLineSegment_FaceTowardsEndpoint_DoesNothing) {
  LineSegment const origLineSegment(fvec2(0.5f, 6.1f), fvec2(1.3f, -7.2f));

  LineSegment lineSegment = origLineSegment;
  lineSegment.FaceTowards(lineSegment.p0);
  EXPECT_EQ(origLineSegment, lineSegment);
}

TEST(LineSegmentTest, ValidLineSegment_FaceAwayFromEndpoint_DoesNothing) {
  LineSegment const origLineSegment(fvec2(-1.3f, 2.4f), fvec2(-22.2f, -0.3f));

  LineSegment lineSegment = origLineSegment;
  lineSegment.FaceAway(lineSegment.p1);
  EXPECT_EQ(origLineSegment, lineSegment);
}
