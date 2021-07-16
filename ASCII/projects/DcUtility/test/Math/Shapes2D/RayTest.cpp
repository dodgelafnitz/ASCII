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
  fvec2 const dir = fvec2(-0.2f, 2.2f).Normalize();
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
  fvec2 const root = fvec2(0.1f, 8.2f);
  Ray ray(root, fvec2(-0.3f, -1.0f));

  ray.SetDirection(fvec2(1.5f, -2.7f));

  EXPECT_EQ(ray.GetRoot(), root);
}
TEST(RayTest, AnyRay_RootIsSet_RootIsCorrect) {
  fvec2 const root = fvec2(1.4f, -7.3f);
  Ray ray(fvec2(0.3f, -2.0f), fvec2(0.4f, 9.3f));

  ray.SetRoot(root);

  EXPECT_EQ(ray.GetRoot(), root);
}

TEST(RayTest, AnyRay_RootIsSet_DirectionIsUnchanged) {
  fvec2 const dir = fvec2(1.4f, -7.3f).Normalize();
  Ray ray(fvec2(-2.0f, 3.1f), dir);

  ray.SetRoot(fvec2(-2.5f, 1.3f));

  EXPECT_EQ(ray.GetDirection(), dir);
}

TEST(RayTest, AnyRay_DotNormalAndDirection_ResultIs0) {
  Ray const ray(fvec2(10.5f, -3.7f), fvec2(0.5f, -6.4f));

  EXPECT_FLOAT_EQ(ray.GetNormal().Dot(ray.GetDirection()), 0.0f);
}

TEST(RayTest, AnyRay_ObtainedLineFromRay_NormalsAreSame) {
  Ray const ray(fvec2(0.02f, 0.8f), fvec2(0.7f, -0.1f));

  fvec2 const lineNormal = ray.GetLine().GetNormal();
  fvec2 const rayNormal  = ray.GetNormal();

  EXPECT_FLOAT_EQ(lineNormal.x, rayNormal.x);
  EXPECT_FLOAT_EQ(lineNormal.y, rayNormal.y);
}

TEST(RayTest, AnyRay_ObtainedLineFromRay_DistanceToRootIs0) {
  Ray const ray(fvec2(-1.0f, -3.2f), fvec2(8.1f, -1.3f));

  EXPECT_FLOAT_EQ(ray.GetLine().DistanceTo(ray.GetRoot()), 0.0f);
}

TEST(RayTest, AnyRay_ObtainedLineFromRayWithFacing_DistanceToRootIs0) {
  Ray const ray(fvec2(3.6f, 0.2f), fvec2(4.0f, 3.0f));

  Line const line = ray.GetLine(fvec2());

  EXPECT_FLOAT_EQ(line.DistanceTo(ray.GetRoot()), 0.0f);
}

TEST(RayTest, AnyRay_ObtainedLineFromRayWithFacing_NormalFacesTarget) {
  Ray const   ray(fvec2(0.3f, 10.3f), fvec2(0.2f, 1.1f));
  fvec2 const target(1.0f, 3.0f);

  Line const  line = ray.GetLine(fvec2());
  fvec2 const diff = target - line.GetRoot();

  EXPECT_GT(diff.Dot(line.GetNormal()), 0.0f);
}

TEST(RayTest, AnyRay_ObtainedLineSegmentFromRay_LengthIsCorrect) {
  Ray const ray(fvec2(0.1f, -2.0f), fvec2(1.0f, 0.0f));

  LineSegment const seg = ray.GetLineSegment(1.4f);

  EXPECT_FLOAT_EQ(seg.Length(), 1.4f);
}

TEST(RayTest, AnyRay_ObtainedLineSegmentFromRay_NormalsAreSame) {
  Ray const ray(fvec2(-0.4f, 8.2f), fvec2(0.6f, 0.8f));

  fvec2 const segNormal = ray.GetLineSegment(12.8f).GetNormal();
  fvec2 const rayNormal = ray.GetNormal();

  EXPECT_FLOAT_EQ(segNormal.x, rayNormal.x);
  EXPECT_FLOAT_EQ(segNormal.y, rayNormal.y);
}

TEST(RayTest, AnyRay_ObtainedLineSegmentFromRay_RootIsP0OrP1) {
  fvec2 const root = fvec2(1.3f, 0.6f);
  Ray const   ray(root, fvec2(1.0f, 0.0f));

  LineSegment const seg = ray.GetLineSegment(2.4f);

  bool const rootIsEndpoint = root == seg.p0 || root == seg.p1;

  EXPECT_TRUE(rootIsEndpoint) <<
    "  root:   {" << root.x << ", " << root.y << "}" << std::endl <<
    "  seg.p0: {" << seg.p0.x << ", " << seg.p0.y << "}" << std::endl <<
    "  seg.p1: {" << seg.p1.x << ", " << seg.p1.y << "}" << std::endl
  ;
}

TEST(RayTest, AnyRay_ObtainedNegativeLengthLineSegmentFromRay_LengthIsCorrect) {
  Ray const ray(fvec2(10.6f, 4.6f), fvec2(-2.3f, -1.0f));

  LineSegment const seg = ray.GetLineSegment(-4.0f);

  EXPECT_FLOAT_EQ(seg.Length(), 4.0f);
}

TEST(RayTest, AnyRay_ObtainedNegativeLengthLineSegmentFromRay_NormalsAreInverted) {
  Ray const ray(fvec2(-4.0f, 8.7f), fvec2(-0.3f, 0.6f));

  LineSegment const seg = ray.GetLineSegment(-3.3f);

  fvec2 const segNormal = seg.GetNormal();
  fvec2 const rayNormal = ray.GetNormal();

  EXPECT_FLOAT_EQ(segNormal.x, -rayNormal.x);
  EXPECT_FLOAT_EQ(segNormal.y, -rayNormal.y);
}

TEST(RayTest, AnyRay_ObtainedNegativeLengthLineSegmentFromRay_RootIsP0OrP1) {
  fvec2 const root(0.5f, -12.2f);
  Ray const   ray(root, fvec2(1.0f, -4.3f));

  LineSegment const seg = ray.GetLineSegment(-6.3f);

  bool const rootIsEndpoint = root == seg.p0 || root == seg.p1;

  EXPECT_TRUE(rootIsEndpoint) <<
    "  root:   {" << root.x << ", " << root.y << "}" << std::endl <<
    "  seg.p0: {" << seg.p0.x << ", " << seg.p0.y << "}" << std::endl <<
    "  seg.p1: {" << seg.p1.x << ", " << seg.p1.y << "}" << std::endl
  ;
}

TEST(RayTest, AnyRay_ObtainedLineSegmentFromRayWithFacing_LengthIsCorrect) {
  Ray const ray(fvec2(-2.4f, 2.7f), fvec2(2.2f, -1.0f));

  LineSegment const seg = ray.GetLineSegment(12.0f, fvec2(-3.0f, -12.4f));

  EXPECT_FLOAT_EQ(seg.Length(), 12.0f);
}

TEST(RayTest, AnyRay_ObtainedLineSegmentFromRayWithFacing_RootIsP0OrP1) {
  fvec2 const root(3.4f, 20.8f);
  Ray const   ray(root, fvec2(0.1f, 0.8f));

  LineSegment const seg = ray.GetLineSegment(0.4f, fvec2(3.0f, 1.3f));

  bool const rootIsEndpoint = root == seg.p0 || root == seg.p1;

  EXPECT_TRUE(rootIsEndpoint) <<
    "  root:   {" << root.x << ", " << root.y << "}" << std::endl <<
    "  seg.p0: {" << seg.p0.x << ", " << seg.p0.y << "}" << std::endl <<
    "  seg.p1: {" << seg.p1.x << ", " << seg.p1.y << "}" << std::endl
  ;
}

TEST(RayTest, AnyRay_ObtainedLineSegmentFromRayWithFacing_NormalFacesTarget) {
  fvec2 const root(14.3f, -2.4f);
  Ray const   ray(root, fvec2(-0.8f, -0.6f));
  fvec2 const target(0.3f, -5.4f);

  LineSegment const seg = ray.GetLineSegment(3.1f, target);

  fvec2 const diff = target - root;

  EXPECT_GT(diff.Dot(seg.GetNormal()), 0.0f);
}

TEST(RayTest, AnyRay_ObtainedNegativeLengthLineSegmentFromRayWithFacing_LengthIsCorrect) {
  fvec2 const root(0.4f, 18.2f);
  Ray const   ray(root, fvec2(0.5f, 0.4f));
  fvec2 const target(0.7f, 2.3f);

  LineSegment const seg = ray.GetLineSegment(-8.0f, target);

  EXPECT_FLOAT_EQ(seg.Length(), 8.0f);
}

TEST(RayTest, AnyRay_ObtainedNegativeLengthLineSegmentFromRayWithFacing_SegmentGoesInCorrectDirection) {
  fvec2 const root(-3.7f, -4.2f);
  Ray const   ray(root, fvec2(1.0f, 0.0f));
  fvec2 const target(-0.4f, 6.7f);

  LineSegment const seg = ray.GetLineSegment(-4.2f, target);

  fvec2 const diff = (seg.p1 - root) + (seg.p0 - root);

  EXPECT_EQ(diff.Normal(), -ray.GetDirection());
}

TEST(RayTest, AnyRay_ObtainedNegativeLengthLineSegmentFromRayWithFacing_NormalFacesTarget) {
  fvec2 const root(-2.5f, 12.2f);
  Ray const   ray(root, fvec2(-0.1f, -0.8f));
  fvec2 const target(10.2f, 0.1f);

  LineSegment const seg = ray.GetLineSegment(-8.2f, target);

  fvec2 const diff = target - root;

  EXPECT_GT(diff.Dot(seg.GetNormal()), 0.0f);
}

TEST(RayTest, AnyRay_CheckDistanceToPointAlongRayPath_DistanceIsCorrect) {
  Ray const   ray(fvec2(1.0f, 0.0f), fvec2(-0.6f, -0.8f));
  fvec2 const target(1.0f, -3.0f);

  EXPECT_FLOAT_EQ(ray.DistanceTo(target), 1.8f);
}

TEST(RayTest, AnyRay_CheckDistanceToPointBeforeRayPath_DistanceIsCorrect) {
  Ray const   ray(fvec2(2.0f, -4.0f), fvec2(0.8f, -0.6f));
  fvec2 const target(-2.0f, -1.0f);

  EXPECT_FLOAT_EQ(ray.DistanceTo(target), 5.0f);
}

TEST(RayTest, AnyRay_ProjectPointOntoRayBeforeRayPath_ProjectionIsRoot) {
  Ray const   ray(fvec2(1.2f, 2.7f), fvec2(-0.2f, -0.1f));
  fvec2 const target(1.7f, 4.9f);

  EXPECT_EQ(ray.ProjectPointOntoRay(target), ray.GetRoot());
}

TEST(RayTest, AnyRay_ProjectPointOntoRayAlongRayPath_ProjectionIsClosestPointToPoint) {
  Ray const   ray(fvec2(1.5f,-4.5f), fvec2(1.0f, 1.0f));
  fvec2 const target(4.0f, 8.0f);

  EXPECT_EQ(ray.ProjectPointOntoRay(target), fvec2(9.0f, 3.0f));
}

TEST(RayTest, AnyRay_ProjectPointOntoLineBeforeRayPath_ProjectionIsAlongDirectionFromRoot) {
  Ray const   ray(fvec2(5.0f, 2.0f), fvec2(0.6f, -0.8f));
  fvec2 const target(2.0f, 8.0f);

  fvec2 const projected = ray.ProjectPointOntoLine(target);
  fvec2 const diff      = ray.GetRoot() - projected;
  float const dist      = diff.Length() * (diff.Dot(ray.GetDirection()) > 0.0f ? 1.0f : -1.0f);

  EXPECT_EQ(projected + dist * ray.GetDirection(), ray.GetRoot());
}

TEST(RayTest, AnyRay_ProjectPointOntoLineAlongRayPath_ProjectionIsAlongDirectionFromRoot) {
  Ray const   ray(fvec2(-2.7f, -8.4f), fvec2(-0.1f, 0.9f));
  fvec2 const target(-5.3f, 9.2f);

  fvec2 const projected = ray.ProjectPointOntoLine(target);
  fvec2 const diff = ray.GetRoot() - projected;
  float const dist = diff.Length() * (diff.Dot(ray.GetDirection()) > 0.0f ? 1.0f : -1.0f);

  fvec2 const expectedRoot = projected + dist * ray.GetDirection();

  EXPECT_FLOAT_EQ(expectedRoot.x, ray.GetRoot().x);
  EXPECT_FLOAT_EQ(expectedRoot.y, ray.GetRoot().y);
}

TEST(RayTest, AnyRay_ProjectPointOntoLineBeforeRayPath_ProjectionIsClosestPointOnLineToTarget) {
  Ray const   ray(fvec2(0.3f, 15.3f), fvec2(-0.5f, 0.2f));
  fvec2 const target(12.6f, 18.0f);

  fvec2 const projected = ray.ProjectPointOntoLine(target);

  EXPECT_FLOAT_EQ((target - projected).Length(), ray.GetLine().DistanceTo(target));
}

TEST(RayTest, AnyRay_ProjectPointOntoLineAlongRayPath_ProjectionIsClosestPointOnLineToTarget) {
  Ray const   ray(fvec2(-1.4f, -2.5), fvec2(1.2f, 0.3f));
  fvec2 const target(9.2f, 6.7f);

  fvec2 const projected = ray.ProjectPointOntoLine(target);

  EXPECT_FLOAT_EQ((target - projected).Length(), ray.GetLine().DistanceTo(target));
}

