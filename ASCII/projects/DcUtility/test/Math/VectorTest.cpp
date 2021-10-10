/*
 * Copywrite 2021 Dodge Lafnitzegger
 */

#include "Math/Vector.h"
#include "gtest/gtest.h"

#include <tuple>

#include "Helpers/TestHelpers.h"

TEST(VectorTest, CountIs1_DefaultConstructed_ElementIsDefaultConstructed) {
  Vector<DefaultConstructorCheck, 1> const vec;
  EXPECT_TRUE(vec.x.WasDefaultConstructed());
}

TEST(VectorTest, CountIs2_DefaultConstructed_ElementsAreDefaultConstructed) {
  Vector<DefaultConstructorCheck, 2> const vec;
  EXPECT_TRUE(vec.x.WasDefaultConstructed());
  EXPECT_TRUE(vec.y.WasDefaultConstructed());
}

TEST(VectorTest, CountIs3_DefaultConstructed_ElementsAreDefaultConstructed) {
  Vector<DefaultConstructorCheck, 3> const vec;
  EXPECT_TRUE(vec.x.WasDefaultConstructed());
  EXPECT_TRUE(vec.y.WasDefaultConstructed());
  EXPECT_TRUE(vec.z.WasDefaultConstructed());
}

TEST(VectorTest, CountIs4_DefaultConstructed_ElementsAreDefaultConstructed) {
  Vector<DefaultConstructorCheck, 4> const vec;
  EXPECT_TRUE(vec.x.WasDefaultConstructed());
  EXPECT_TRUE(vec.y.WasDefaultConstructed());
  EXPECT_TRUE(vec.z.WasDefaultConstructed());
  EXPECT_TRUE(vec.w.WasDefaultConstructed());
}

TEST(VectorTest, CountIs5_DefaultConstructed_ElementsAreDefaultConstructed) {
  Vector<DefaultConstructorCheck, 5> const vec;
  EXPECT_TRUE(vec[0].WasDefaultConstructed());
  EXPECT_TRUE(vec[1].WasDefaultConstructed());
  EXPECT_TRUE(vec[2].WasDefaultConstructed());
  EXPECT_TRUE(vec[3].WasDefaultConstructed());
  EXPECT_TRUE(vec[4].WasDefaultConstructed());
}

TEST(VectorTest, ConstAndCountIs1_CheckIndexAndNamedElement_ReferencesAreSame) {
  Vector<int, 1> const vec;
  EXPECT_EQ(&vec.x, &vec[0]);
}

TEST(VectorTest, ConstAndCountIs2_CheckIndexAndNamedElements_ReferencesAreSame) {
  Vector<int, 2> const vec;
  EXPECT_EQ(&vec.x, &vec[0]);
  EXPECT_EQ(&vec.y, &vec[1]);
}

TEST(VectorTest, ConstAndCountIs3_CheckIndexAndNamedElements_ReferencesAreSame) {
  Vector<int, 3> const vec;
  EXPECT_EQ(&vec.x, &vec[0]);
  EXPECT_EQ(&vec.y, &vec[1]);
  EXPECT_EQ(&vec.z, &vec[2]);
}

TEST(VectorTest, ConstAndCountIs4_CheckIndexAndNamedElements_ReferencesAreSame) {
  Vector<int, 4> const vec;
  EXPECT_EQ(&vec.x, &vec[0]);
  EXPECT_EQ(&vec.y, &vec[1]);
  EXPECT_EQ(&vec.z, &vec[2]);
  EXPECT_EQ(&vec.w, &vec[3]);
}

TEST(VectorTest, NonConstAndCountIs1_CheckIndexAndNamedElement_ReferencesAreSame) {
  Vector<int, 1> vec;
  EXPECT_EQ(&vec.x, &vec[0]);
}

TEST(VectorTest, NonConstAndCountIs2_CheckIndexAndNamedElements_ReferencesAreSame) {
  Vector<int, 2> vec;
  EXPECT_EQ(&vec.x, &vec[0]);
  EXPECT_EQ(&vec.y, &vec[1]);
}

TEST(VectorTest, NonConstAndCountIs3_CheckIndexAndNamedElements_ReferencesAreSame) {
  Vector<int, 3> vec;
  EXPECT_EQ(&vec.x, &vec[0]);
  EXPECT_EQ(&vec.y, &vec[1]);
  EXPECT_EQ(&vec.z, &vec[2]);
}

TEST(VectorTest, NonConstAndCountIs4_CheckIndexAndNamedElements_ReferencesAreSame) {
  Vector<int, 4> vec;
  EXPECT_EQ(&vec.x, &vec[0]);
  EXPECT_EQ(&vec.y, &vec[1]);
  EXPECT_EQ(&vec.z, &vec[2]);
  EXPECT_EQ(&vec.w, &vec[3]);
}

TEST(VectorTest, Vector_CheckSum_ResultIsSum) {
  Vector<OperatorCheck, 5> const vec("a", "b", "c", "d", "e");
  EXPECT_EQ(vec.Sum().GetValue(), "(((((0+a)+b)+c)+d)+e)");
}
TEST(VectorTest, Vector_CheckProduct_ResultIsProduct) {
  Vector<OperatorCheck, 5> const vec("a", "b", "c", "d", "e");
  EXPECT_EQ(vec.Product().GetValue(), "(((((1*a)*b)*c)*d)*e)");
}

TEST(VectorTest, TypesAreSame_Assign_ResultIsLeftHandSide) {
  Vector<int, 5>       vec0;
  Vector<int, 5> const vec1(1, 2, 3, 4, 5);

  vec0 = vec1;

  EXPECT_EQ(vec0[0], 1);
  EXPECT_EQ(vec0[1], 2);
  EXPECT_EQ(vec0[2], 3);
  EXPECT_EQ(vec0[3], 4);
  EXPECT_EQ(vec0[4], 5);
}
TEST(VectorTest, TypesAreDifferent_Assign_ResultIsLeftHandSide) {
  Vector<float, 5>     vec0;
  Vector<int, 5> const vec1(1, 2, 3, 4, 5);

  vec0 = vec1;

  EXPECT_EQ(vec0[0], 1.0f);
  EXPECT_EQ(vec0[1], 2.0f);
  EXPECT_EQ(vec0[2], 3.0f);
  EXPECT_EQ(vec0[3], 4.0f);
  EXPECT_EQ(vec0[4], 5.0f);
}

TEST(VectorTest, Addition_CheckValues_ValuesAreAdded) {
  Vector<OperatorCheck, 5> const vec0("a0", "b0", "c0", "d0", "e0");
  Vector<OperatorCheck, 5> const vec1("a1", "b1", "c1", "d1", "e1");

  Vector<OperatorCheck, 5> const vec2 = vec0 + vec1;

  EXPECT_EQ(vec2[0].GetValue(), "(a0+a1)");
  EXPECT_EQ(vec2[1].GetValue(), "(b0+b1)");
  EXPECT_EQ(vec2[2].GetValue(), "(c0+c1)");
  EXPECT_EQ(vec2[3].GetValue(), "(d0+d1)");
  EXPECT_EQ(vec2[4].GetValue(), "(e0+e1)");
}

TEST(VectorTest, Subtraction_CheckValues_ValuesAreSubtracted) {
  Vector<OperatorCheck, 5> const vec0("a0", "b0", "c0", "d0", "e0");
  Vector<OperatorCheck, 5> const vec1("a1", "b1", "c1", "d1", "e1");

  Vector<OperatorCheck, 5> const vec2 = vec0 - vec1;

  EXPECT_EQ(vec2[0].GetValue(), "(a0-a1)");
  EXPECT_EQ(vec2[1].GetValue(), "(b0-b1)");
  EXPECT_EQ(vec2[2].GetValue(), "(c0-c1)");
  EXPECT_EQ(vec2[3].GetValue(), "(d0-d1)");
  EXPECT_EQ(vec2[4].GetValue(), "(e0-e1)");
}

TEST(VectorTest, Multiplication_CheckValues_ValuesAreMutiplied) {
  Vector<OperatorCheck, 5> const vec0("a0", "b0", "c0", "d0", "e0");
  Vector<OperatorCheck, 5> const vec1("a1", "b1", "c1", "d1", "e1");

  Vector<OperatorCheck, 5> const vec2 = vec0 * vec1;

  EXPECT_EQ(vec2[0].GetValue(), "(a0*a1)");
  EXPECT_EQ(vec2[1].GetValue(), "(b0*b1)");
  EXPECT_EQ(vec2[2].GetValue(), "(c0*c1)");
  EXPECT_EQ(vec2[3].GetValue(), "(d0*d1)");
  EXPECT_EQ(vec2[4].GetValue(), "(e0*e1)");
}

TEST(VectorTest, Division_CheckValues_ValuesAreDivided) {
  Vector<OperatorCheck, 5> const vec0("a0", "b0", "c0", "d0", "e0");
  Vector<OperatorCheck, 5> const vec1("a1", "b1", "c1", "d1", "e1");

  Vector<OperatorCheck, 5> const vec2 = vec0 / vec1;

  EXPECT_EQ(vec2[0].GetValue(), "(a0/a1)");
  EXPECT_EQ(vec2[1].GetValue(), "(b0/b1)");
  EXPECT_EQ(vec2[2].GetValue(), "(c0/c1)");
  EXPECT_EQ(vec2[3].GetValue(), "(d0/d1)");
  EXPECT_EQ(vec2[4].GetValue(), "(e0/e1)");
}

TEST(VectorTest, Modulous_CheckValues_ValuesAreModuloed) {
  Vector<OperatorCheck, 5> const vec0("a0", "b0", "c0", "d0", "e0");
  Vector<OperatorCheck, 5> const vec1("a1", "b1", "c1", "d1", "e1");

  Vector<OperatorCheck, 5> const vec2 = vec0 % vec1;

  EXPECT_EQ(vec2[0].GetValue(), "(a0%a1)");
  EXPECT_EQ(vec2[1].GetValue(), "(b0%b1)");
  EXPECT_EQ(vec2[2].GetValue(), "(c0%c1)");
  EXPECT_EQ(vec2[3].GetValue(), "(d0%d1)");
  EXPECT_EQ(vec2[4].GetValue(), "(e0%e1)");
}

TEST(VectorTest, SameVectors_CheckEqual_ResultIsTrue) {
  Vector<int, 5> const vec0(1, 2, 3, 4, 5);
  Vector<int, 5> const vec1(1, 2, 3, 4, 5);

  ASSERT_TRUE(vec0 == vec1);
}

TEST(VectorTest, SameVectors_CheckNotEqual_ResultIsFalse) {
  Vector<int, 5> const vec0(1, 2, 3, 4, 5);
  Vector<int, 5> const vec1(1, 2, 3, 4, 5);

  ASSERT_FALSE(vec0 != vec1);
}

TEST(VectorTest, DifferentVectors_CheckEqual_ResultIsFalse) {
  Vector<int, 5> const vec0(1, 2, 3, 4, 5);
  Vector<int, 5> const vec1(0, 2, 3, 4, 5);

  ASSERT_FALSE(vec0 == vec1);
}

TEST(VectorTest, DifferentVectors_CheckNotEqual_ResultIsTrue) {
  Vector<int, 5> const vec0(1, 2, 3, 4, 5);
  Vector<int, 5> const vec1(0, 2, 3, 4, 5);

  ASSERT_TRUE(vec0 != vec1);
}

TEST(VectorTest, Min_CheckValues_ValuesAreMin) {
  Vector<float, 5> const vec0(0.0f, 1.0f, -2.0f, 0.5f, 4.5f);
  Vector<float, 5> const vec1(-3.0f, 2.0f, 1.0f, 0.5f, 3.5f);
  Vector<float, 5> const vec2(-3.0f, 1.0f, -2.0f, 0.5f, 3.5f);

  Vector<float, 5> const vec3 = vec0.Min(vec1);

  EXPECT_EQ(vec2, vec3);
}

TEST(VectorTest, DotProduct_CheckResult_ResultIsDotProduct) {
  Vector<OperatorCheck, 5> const vec0("a0", "b0", "c0", "d0", "e0");
  Vector<OperatorCheck, 5> const vec1("a1", "b1", "c1", "d1", "e1");

  std::string const res0 = "(((((0+(a0*a1))+(b0*b1))+(c0*c1))+(d0*d1))+(e0*e1))";
  std::string const res1 = "(((((0+(a1*a0))+(b1*b0))+(c1*c0))+(d1*d0))+(e1*e0))";

  ASSERT_EQ(vec0.Dot(vec1).GetValue(), res0);
  ASSERT_EQ(vec1.Dot(vec0).GetValue(), res1);
}

TEST(VectorTest, LengthSquared_CheckResult_ResultIsLengthSquared) {
  Vector<OperatorCheck, 5> const vec("a", "b", "c", "d", "e");

  ASSERT_EQ(vec.LengthSquared().GetValue(), vec.Dot(vec).GetValue());
}

TEST(VectorTest, Length_CheckResult_ResultIsLength) {
  Vector<float, 3> const vec(12.0f, 3.0f, 4.0f);

  ASSERT_EQ(vec.Length(), 13.0f);
}

TEST(VectorTest, NormalizedVector_CheckLength_LengthIs1) {
  Vector<float, 3> vec(2.7f, -4.3f, 10.1f);
  vec.Normalize();

  ASSERT_EQ(vec.Length(), 1.0f);
}

TEST(VectorTest, Normalized0Vector_CheckLength_LengthIs0) {
  Vector<float, 3> vec(0.0f, 0.0f, 0.0f);
  vec.Normalize();

  ASSERT_EQ(vec.Length(), 0.0f);
}

