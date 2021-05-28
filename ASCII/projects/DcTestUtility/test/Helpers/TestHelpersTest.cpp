/*
 * Copywrite 2021 Dodge Lafnitzegger
 */

#include "Helpers/TestHelpers.h"
#include "gtest/gtest.h"

TEST(DefaultConstructorCheckTest, DefaultConstructed_CheckDefaultConstructed_True) {
  DefaultConstructorCheck check;
  EXPECT_TRUE(check.WasDefaultConstructed());
}

TEST(DefaultConstructorCheckTest, NotDefaultConstructed_CheckDefaultConstructed_False) {
  DefaultConstructorCheck check(1);
  EXPECT_FALSE(check.WasDefaultConstructed());
}

TEST(OperatorCheckTest,DefaultConstructed_CheckValue_ValueIsOpenCloseParens) {
  OperatorCheck const check;
  EXPECT_EQ(check.GetValue(), "()");
}

TEST(OperatorCheckTest, ConstructedWithString_CheckValue_ValueIsGivenString) {
  OperatorCheck const check("a");
  EXPECT_EQ(check.GetValue(), "a");
}

TEST(OperatorCheckTest, Assign_CheckValue_ValueIsFromOther) {
  OperatorCheck       check0;
  OperatorCheck const check1("a");

  check0 = check1;
  EXPECT_EQ(check0.GetValue(), "a");
}

TEST(OperatorCheckTest, AdditionAssign_CheckValue_ValueIsAddition) {
  OperatorCheck check("a");
  check += OperatorCheck("b");

  EXPECT_EQ(check.GetValue(), "(a+b)");
}

TEST(OperatorCheckTest, SubtractionAssign_CheckValue_ValueIsSubtraction) {
  OperatorCheck check("a");
  check -= OperatorCheck("b");

  EXPECT_EQ(check.GetValue(), "(a-b)");
}

TEST(OperatorCheckTest, MultiplicationAssign_CheckValue_ValueIsMultiplication) {
  OperatorCheck check("a");
  check *= OperatorCheck("b");

  EXPECT_EQ(check.GetValue(), "(a*b)");
}

TEST(OperatorCheckTest, DivisionAssign_CheckValue_ValueIsDivision) {
  OperatorCheck check("a");
  check /= OperatorCheck("b");

  EXPECT_EQ(check.GetValue(), "(a/b)");
}

TEST(OperatorCheckTest, ModulousAssign_CheckValue_ValueIsModulous) {
  OperatorCheck check("a");
  check %= OperatorCheck("b");

  EXPECT_EQ(check.GetValue(), "(a%b)");
}

TEST(OperatorCheckTest, AdditionAssignWith4Values_CheckValue_ValuesAreNested) {
  OperatorCheck check("a");
  check += OperatorCheck("b");
  check += OperatorCheck("c");
  check += OperatorCheck("d");

  EXPECT_EQ(check.GetValue(), "(((a+b)+c)+d)");
}

TEST(OperatorCheckTest, ConstructedWithInt_CheckValue_ValueIsString) {
  OperatorCheck check(14);
  EXPECT_EQ(check.GetValue(), "14");
}
