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

TEST(DestructorCheckTest, ValidDestructorCheck_IsDestroyed_ValueIsSetToTrue) {
  bool value = false;

  {
    DestructorCheck check(&value);
    EXPECT_FALSE(value);
  }

  EXPECT_TRUE(value);
}

TEST(MoveCheckTest, DefaultConstructedCheck_CheckState_StateIsDefault) {
  MoveCheck const check;

  EXPECT_EQ(check.GetState(), MoveCheck::State::Default);
}

TEST(MoveCheckTest, CopyConstructedCheck_CheckState_StateIsCopied) {
  MoveCheck const check0;
  MoveCheck const check1(check0);

  EXPECT_EQ(check1.GetState(), MoveCheck::State::CopiedTo);
}

TEST(MoveCheckTest, CopyConstructedCheck_CheckStateOfSource_StateIsUnchanged) {
  MoveCheck const check0;
  MoveCheck const check1(check0);

  EXPECT_EQ(check0.GetState(), MoveCheck::State::Default);
}

TEST(MoveCheckTest, MoveConstructedCheck_CheckState_StateIsMovedTo) {
  MoveCheck       check0;
  MoveCheck const check1(std::move(check0));

  EXPECT_EQ(check1.GetState(), MoveCheck::State::MovedTo);
}

TEST(MoveCheckTest, MoveConstructedCheck_CheckStateOfSource_StateIsMovedFrom) {
  MoveCheck       check0;
  MoveCheck const check1(std::move(check0));

  EXPECT_EQ(check0.GetState(), MoveCheck::State::MovedFrom);
}

TEST(MoveCheckTest, AnyCheck_CopyAssignFromSelf_StateIsUnchanged) {
  MoveCheck check;

  check = check;

  EXPECT_EQ(check.GetState(), MoveCheck::State::Default);
}

TEST(MoveCheckTest, AnyCheck_CopyAssignFromOther_StateIsCopied) {
  MoveCheck const check0;
  MoveCheck       check1;

  check1 = check0;

  EXPECT_EQ(check1.GetState(), MoveCheck::State::CopiedTo);
}

TEST(MoveCheckTest, AnyCheck_CopyAssignFromOther_SourceStateIsUnchanged) {
  MoveCheck const check0;
  MoveCheck       check1;

  check1 = check0;

  EXPECT_EQ(check0.GetState(), MoveCheck::State::Default);
}

TEST(MoveCheckTest, AnyCheck_MoveAssignFromSelf_StateIsUnchanged) {
  MoveCheck check;

  check = std::move(check);

  EXPECT_EQ(check.GetState(), MoveCheck::State::Default);
}

TEST(MoveCheckTest, AnyCheck_MoveAssignFromOther_StateIsMovedTo) {
  MoveCheck check0;
  MoveCheck check1;

  check1 = std::move(check0);

  EXPECT_EQ(check1.GetState(), MoveCheck::State::MovedTo);
}

TEST(MoveCheckTest, AnyCheck_MoveAssignFromOther_SourceStateIsMovedFrom) {
  MoveCheck check0;
  MoveCheck check1;

  check1 = std::move(check0);

  EXPECT_EQ(check0.GetState(), MoveCheck::State::MovedFrom);
}
