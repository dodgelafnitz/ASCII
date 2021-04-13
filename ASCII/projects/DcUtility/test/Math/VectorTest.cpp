/*
 * Copywrite 2021 Dodge Lafnitzegger
 */

#include "Math/Vector.h"
#include "gtest/gtest.h"

TEST(VectorTest, ConstructWithValues) {
  ivec<5> const vec(1, 5, 3, 4, 2);

  EXPECT_EQ(vec[0], 1);
  EXPECT_EQ(vec[1], 5);
  EXPECT_EQ(vec[2], 3);
  EXPECT_EQ(vec[3], 4);
  EXPECT_EQ(vec[4], 2);
}

TEST(VectorTest, SetValues) {
  fvec<2> vec(3.0f, 1.0);

  EXPECT_EQ(vec[0], 3.0f);
  EXPECT_EQ(vec[1], 1.0f);

  vec[0] = 2.0f;
  vec[1] = 4;

  EXPECT_EQ(vec[0], 2.0f);
  EXPECT_EQ(vec[1], 4.0f);
}

TEST(VectorTest, Vec1Access) {
  uvec1 const vec_0;

  EXPECT_EQ(&vec_0[0], &vec_0.x);

  uvec<1> const vec_1;

  EXPECT_EQ(&vec_1[0], &vec_1.x);
}

TEST(VectorTest, Vec2Access) {
  fvec2 const vec_0;

  EXPECT_EQ(&vec_0[0], &vec_0.x);
  EXPECT_EQ(&vec_0[1], &vec_0.y);

  fvec<2> const vec_1;

  EXPECT_EQ(&vec_1[0], &vec_1.x);
  EXPECT_EQ(&vec_1[1], &vec_1.y);
}

TEST(VectorTest, Vec3Access) {
  ivec3 const vec_0;

  EXPECT_EQ(&vec_0[0], &vec_0.x);
  EXPECT_EQ(&vec_0[1], &vec_0.y);
  EXPECT_EQ(&vec_0[2], &vec_0.z);

  fvec<3> const vec_1;

  EXPECT_EQ(&vec_1[0], &vec_1.x);
  EXPECT_EQ(&vec_1[1], &vec_1.y);
  EXPECT_EQ(&vec_1[2], &vec_1.z);
}

TEST(VectorTest, Vec4Access) {
  fvec4 const vec_0;

  EXPECT_EQ(&vec_0[0], &vec_0.x);
  EXPECT_EQ(&vec_0[1], &vec_0.y);
  EXPECT_EQ(&vec_0[2], &vec_0.z);
  EXPECT_EQ(&vec_0[3], &vec_0.w);

  uvec<4> const vec_1;

  EXPECT_EQ(&vec_1[0], &vec_1.x);
  EXPECT_EQ(&vec_1[1], &vec_1.y);
  EXPECT_EQ(&vec_1[2], &vec_1.z);
  EXPECT_EQ(&vec_1[3], &vec_1.w);
}

TEST(VectorTest, Vec1ConstructValues) {
  ivec1 const vec(3);

  EXPECT_EQ(vec.x, 3);
}

TEST(VectorTest, Vec2ConstructValues) {
  uvec2 const vec(3, 4);

  EXPECT_EQ(vec.x, 3);
  EXPECT_EQ(vec.y, 4);
}

TEST(VectorTest, Vec3ConstructValues) {
  ivec3 const vec(1, 0, 2);

  EXPECT_EQ(vec.x, 1);
  EXPECT_EQ(vec.y, 0);
  EXPECT_EQ(vec.z, 2);
}

TEST(VectorTest, Vec4ConstructValues) {
  fvec4 const vec(2.5f, -1.0f, 10.0, 3);

  EXPECT_EQ(vec.x, 2.5f);
  EXPECT_EQ(vec.y, -1.0f);
  EXPECT_EQ(vec.z, 10.0f);
  EXPECT_EQ(vec.w, 3.0f);
}
