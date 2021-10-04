/*
 * Copywrite 2021 Dodge Lafnitzegger
 */

#include "Containers/DynamicArray.h"
#include "gtest/gtest.h"

#include "Helpers/TestHelpers.h"

TEST(DynamicArrayTest, DefaultConstructed_CheckSize_SizeIs0) {
  DynamicArray<int, 17> const arr = DynamicArray<int, 17>();

  EXPECT_EQ(arr.Count(), 0);
}

TEST(DynamicArrayTest, DefaultConstructed_ElementsAdded_SizeIsCorrect) {
  DynamicArray<int, 9> arr;

  arr.Emplace(6);

  EXPECT_EQ(arr.Count(), 1);
}

TEST(DynamicArrayTest, DefaultConstructed_ElementsAdded_ValuesAreCorrect) {
  DynamicArray<int, 8> arr;

  arr.Emplace(3);

  ASSERT_EQ(arr.Count(), 1);
  EXPECT_EQ(arr[0], 3);
}

TEST(DynamicArrayTest, FilledArray_ElementsPopped_SizeIsCorrect) {
  DynamicArray<int, 4> arr;

  arr.Emplace(1);
  arr.Emplace(7);
  arr.Emplace(2);
  arr.Emplace(3);

  EXPECT_EQ(arr.Count(), 4);

  arr.PopBack();

  EXPECT_EQ(arr.Count(), 3);
}

TEST(DynamicArrayTest, FilledArray_ElementsPopped_ValuesAreCorrect) {
  DynamicArray<int, 50> arr;

  arr.Emplace(8);
  arr.Emplace(2);
  arr.Emplace(-1);
  arr.Emplace(0);

  EXPECT_EQ(arr.Count(), 4);

  arr.PopBack();

  ASSERT_EQ(arr.Count(), 3);
  EXPECT_EQ(arr[0], 8);
  EXPECT_EQ(arr[1], 2);
  EXPECT_EQ(arr[2], -1);
}

TEST(DynamicArrayTest, CopiedArray_CheckValues_ValuesAreSame) {
  DynamicArray<int, 10> arr0;
  DynamicArray<int, 10> arr1;

  arr1.Emplace(0);
  arr1.Emplace(10);
  arr1.Emplace(100);

  arr0 = arr1;

  ASSERT_EQ(arr0.Count(), 3);
  ASSERT_EQ(arr1.Count(), 3);

  EXPECT_EQ(arr0[0], arr1[0]);
  EXPECT_EQ(arr0[1], arr1[1]);
  EXPECT_EQ(arr0[2], arr1[2]);
}

TEST(DynamicArrayTest, MovedArray_CheckValues_ValuesAreMoved) {
  DynamicArray<int, 10> arr0;
  DynamicArray<int, 10> arr1;

  arr1.Emplace(0);
  arr1.Emplace(10);
  arr1.Emplace(100);

  arr0 = arr1;

  ASSERT_EQ(arr0.Count(), 3);
  ASSERT_EQ(arr1.Count(), 3);

  EXPECT_EQ(arr0[0], arr1[0]);
  EXPECT_EQ(arr0[1], arr1[1]);
  EXPECT_EQ(arr0[2], arr1[2]);
}

TEST(DynamicArrayTest, CreatedWithCount_CheckSize_SizeIsCorrect) {
  DynamicArray<char const *, 7> const arr(3);

  EXPECT_EQ(arr.Count(), 3);
}

TEST(DynamicArrayTest, CreatedWithCountAndValue_CheckValues_ValuesAreCorrect) {
  char const * str = "Hello";

  DynamicArray<char const *, 7> const arr(3, str);

  ASSERT_EQ(arr.Count(), 3);

  EXPECT_EQ(arr[0], str);
  EXPECT_EQ(arr[1], str);
  EXPECT_EQ(arr[2], str);
}

TEST(DynamicArrayTest, CreatedWithInitializerListBelowCapacity_CheckSize_SizeIsCorrect) {
  DynamicArray<int, 12> const arr = { 2, 6, -1 };

  EXPECT_EQ(arr.Count(), 3);
}

TEST(DynamicArrayTest, CreatedWithInitializerListBelowCapacity_CheckValues_ValuesAreCorrect) {
  DynamicArray<int, 6> const arr = { 5, -10, 201, 6 };

  ASSERT_EQ(arr.Count(), 4);

  EXPECT_EQ(arr[0], 5);
  EXPECT_EQ(arr[1], -10);
  EXPECT_EQ(arr[2], 201);
  EXPECT_EQ(arr[3], 6);
}

TEST(DynamicArrayTest, CreatedWithInitializerListAboveCapacity_CheckSize_SizeIsCapacity) {
  DynamicArray<int, 2> const arr = { 1, 3, 5, 9, 11 };

  ASSERT_EQ(arr.Count(), 2);
}

TEST(DynamicArrayTest, CreatedWithInitializerListAboveCapacity_CheckValues_ValuesAreCorrect) {
  DynamicArray<int, 3> const arr = { -1, -8, 15, 12, 1 };

  ASSERT_EQ(arr.Count(), 3);

  EXPECT_EQ(arr[0], -1);
  EXPECT_EQ(arr[1], -8);
  EXPECT_EQ(arr[2], 15);
}

TEST(DynamicArrayTest, CreatedWithMemoryConsumingElements_DestructorCalled_NoMemoryLeak) {
  bool destructed[5] = { 0 };

  {
    DynamicArray<DestructorCheck, 8> arr;

    arr.Emplace(destructed + 0);
    arr.Emplace(destructed + 1);
    arr.Emplace(destructed + 2);
    arr.Emplace(destructed + 3);
    arr.Emplace(destructed + 4);
  }

  EXPECT_TRUE(destructed[0]);
  EXPECT_TRUE(destructed[1]);
  EXPECT_TRUE(destructed[2]);
  EXPECT_TRUE(destructed[3]);
  EXPECT_TRUE(destructed[4]);
}

TEST(DynamicArrayTest, AnyArray_CopyAssigned_SizeIsCorrect) {
  DynamicArray<int, 3>       arr0;
  DynamicArray<int, 3> const arr1 = { 7, 4, -1 };

  arr0 = arr1;

  EXPECT_EQ(arr0.Count(), 3);
}

TEST(DynamicArrayTest, AnyArray_CopyAssigned_ValuesAreCorrect) {
  DynamicArray<int, 9>       arr0;
  DynamicArray<int, 9> const arr1 = { 6, 2, 1, 0, 6 };

  arr0 = arr1;

  ASSERT_EQ(arr0.Count(), 5);

  EXPECT_EQ(arr0[0], 6);
  EXPECT_EQ(arr0[1], 2);
  EXPECT_EQ(arr0[2], 1);
  EXPECT_EQ(arr0[3], 0);
  EXPECT_EQ(arr0[4], 6);
}

TEST(DynamicArrayTest, AnyArray_MoveAssigned_SizeIsCorrect) {
  DynamicArray<int, 2> arr0;
  DynamicArray<int, 2> arr1 = { 8, 3 };

  arr0 = std::move(arr1);

  EXPECT_EQ(arr0.Count(), 2);
}

TEST(DynamicArrayTest, AnyArray_MoveAssigned_ValuesAreCorrect) {
  DynamicArray<int, 4> arr0;
  DynamicArray<int, 4> arr1 = { 2, 1, -7 };

  arr0 = std::move(arr1);

  ASSERT_EQ(arr0.Count(), 3);

  EXPECT_EQ(arr0[0], 2);
  EXPECT_EQ(arr0[1], 1);
  EXPECT_EQ(arr0[2], -7);
}

TEST(DynamicArrayTest, AnyArray_MoveAssigned_OtherSizeIsUnchanged) {
  DynamicArray<int, 7> arr0;
  DynamicArray<int, 7> arr1 = { 1, 4, 8, 2 };

  arr0 = std::move(arr1);

  EXPECT_EQ(arr1.Count(), 4);
}

TEST(DynamicArrayTest, AnyArray_MoveAssigned_OtherValuesAreMoved) {
  DynamicArray<MoveCheck, 12> arr0;
  DynamicArray<MoveCheck, 12> arr1(5);

  arr0 = std::move(arr1);

  ASSERT_EQ(arr1.Count(), 5);

  EXPECT_EQ(arr1[0].GetState(), MoveCheck::State::MovedFrom);
  EXPECT_EQ(arr1[1].GetState(), MoveCheck::State::MovedFrom);
  EXPECT_EQ(arr1[2].GetState(), MoveCheck::State::MovedFrom);
  EXPECT_EQ(arr1[3].GetState(), MoveCheck::State::MovedFrom);
  EXPECT_EQ(arr1[4].GetState(), MoveCheck::State::MovedFrom);
}

TEST(DynamicArrayTest, AnyArray_DataViewed_DataPointsToFirstElement) {
  DynamicArray<int, 5> const arr = { 1, -4, 2 };

  ASSERT_GE(arr.Count(), 1);

  EXPECT_EQ(arr.Data(), &arr[0]);
}

TEST(DynamicArrayTest, AnyArray_Cleared_SizeIs0) {
  DynamicArray<int, 8> arr = { 0, 0, 0, 0, 0 };

  arr.Clear();

  EXPECT_EQ(arr.Count(), 0);
}

TEST(DynamicArrayTest, ArrayWith0Elements_CheckEmpty_IsEmpty) {
  DynamicArray<int, 14> const arr = DynamicArray<int, 14>();

  EXPECT_TRUE(arr.Empty());
}

TEST(DynamicArrayTest, ArrayWithElements_CheckEmpty_IsNotEmpty) {
  DynamicArray<int, 3> const arr = { 1 };

  EXPECT_FALSE(arr.Empty());
}

TEST(DynamicArrayTest, ArrayWithMaxElements_CheckFull_IsFull) {
  DynamicArray<int, 5> const arr(5);

  EXPECT_TRUE(arr.Full());
}

TEST(DynamicArrayTest, ArrayWithNotMaxElements_CheckFull_IsNotFull) {
  DynamicArray<int, 5> const arr(2);

  EXPECT_FALSE(arr.Full());
}

TEST(DynamicArrayTest, AnyArray_CheckRangeBegin_PointsToFirstElement) {
  DynamicArray<int, 7> const arr(3);

  EXPECT_EQ(std::begin(arr), arr.Data());
}

TEST(DynamicArrayTest, AnyArray_CheckRangeEnd_PointsToOnePastLastElement) {
  DynamicArray<int, 12> const arr(6);

  EXPECT_EQ(std::end(arr), arr.Data() + arr.Count());
}

TEST(DynamicArrayTest, EmptyArray_CheckRange_BeginMatchesEnd) {
  DynamicArray<int, 12> const arr = DynamicArray<int, 12>();

  EXPECT_EQ(arr.begin(), arr.end());
}
