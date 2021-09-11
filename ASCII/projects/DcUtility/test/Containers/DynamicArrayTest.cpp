/*
 * Copywrite 2021 Dodge Lafnitzegger
 */

#include "Containers/DynamicArray.h"
#include "gtest/gtest.h"

TEST(DynamicArrayTest, DefaultConstructed_CheckSize_SizeIs0) {
}

TEST(DynamicArrayTest, DefaultConstructed_ElementsAdded_SizeIsCorrect) {
}

TEST(DynamicArrayTest, DefaultConstructed_ElementsAdded_ValuesAreCorrect) {
}

TEST(DynamicArrayTest, FilledArray_ElementsPopped_SizeIsCorrect) {
}

TEST(DynamicArrayTest, FilledArray_ElementsPopped_ValuesAreCorrect) {
}

TEST(DynamicArrayTest, CopiedArray_CheckValues_ValuesAreSame) {
}

TEST(DynamicArrayTest, MovedArray_CheckValues_ValuesAreMoved) {
}

TEST(DynamicArrayTest, CreatedWithCount_CheckSize_SizeIsCorrect) {
}

TEST(DynamicArrayTest, CreatedWithCountAndValue_CheckValues_ValuesAreCorrect) {
}

TEST(DynamicArrayTest, CreatedWithInitializerListBelowCapacity_CheckSize_SizeIsCorrect) {
}

TEST(DynamicArrayTest, CreatedWithInitializerListBelowCapacity_CheckValues_ValuesAreCorrect) {
}

TEST(DynamicArrayTest, CreatedWithInitializerListAboveCapacity_CheckSize_SizeIsCapacity) {
}

TEST(DynamicArrayTest, CreatedWithInitializerListAboveCapacity_CheckValues_ValuesAreCorrect) {
}

TEST(DynamicArrayTest, CreatedWithMemoryConsumingElements_DestructorCalled_NoMemoryLeak) {
}

TEST(DynamicArrayTest, AnyArray_CopyAssigned_SizeIsCorrect) {
}

TEST(DynamicArrayTest, AnyArray_CopyAssigned_ValuesAreCorrect) {
}

TEST(DynamicArrayTest, AnyArray_MoveAssigned_SizeIsCorrect) {
}

TEST(DynamicArrayTest, AnyArray_MoveAssigned_ValuesAreCorrect) {
}

TEST(DynamicArrayTest, AnyArray_MoveAssigned_OtherSizeIsUnchanged) {
}

TEST(DynamicArrayTest, AnyArray_MoveAssigned_OtherValuesAreMoved) {
}

TEST(DynamicArrayTest, AnyArray_DataViewed_DataPointsToFirstElement) {
}

TEST(DynamicArrayTest, AnyArray_Cleared_SizeIs0) {
}

TEST(DynamicArrayTest, ArrayWith0Elements_CheckEmpty_IsEmpty) {
}

TEST(DynamicArrayTest, ArrayWithElements_CheckEmpty_IsNotEmpty) {
}

TEST(DynamicArrayTest, ArrayWithMaxElements_CheckFull_IsFull) {
}

TEST(DynamicArrayTest, ArrayWithNotMaxElements_CheckFull_IsNotFull) {
}

TEST(DynamicArrayTest, AnyArray_Cleared_ValuesAreDestroyed) {
}

TEST(DynamicArrayTest, AnyArray_CheckRangeBegin_PointsToFirstElement) {
}

TEST(DynamicArrayTest, AnyArray_CheckRangeEnd_PointsToOnePastLastElement) {
}

TEST(DynamicArrayTest, EmptyArray_CheckRange_BeginMatchesEnd) {
}
