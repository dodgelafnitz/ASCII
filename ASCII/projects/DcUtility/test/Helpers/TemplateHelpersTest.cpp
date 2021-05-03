/*
 * Copywrite 2021 Dodge Lafnitzegger
 */

#include "Helpers/TemplateHelpers.h"
#include "gtest/gtest.h"

#include <functional>

TEST(TemplateRangeTest, BeginEqualEnd) {
  bool called = false;
  template_range<3, 3>([&called]<typename>() { called = true; });

  EXPECT_FALSE(called);
}

TEST(TemplateRangeTest, BeginLessEnd) {
  int lastAdd = -1;
  int total   = 0;
  template_range<0, 5>([&]<typename T>() {
    EXPECT_LT(lastAdd, T::value);

    lastAdd =  T::value;
    total   += T::value;
  });

  EXPECT_EQ(total, 10);
}

TEST(TemplateRangeTest, BeginGreaterEnd) {
  int lastAdd = 6;
  int total   = 0;
  template_range<5, 0>([&]<typename T>() {
    EXPECT_GT(lastAdd, T::value);

    lastAdd =  T::value;
    total   += T::value;
  });

  EXPECT_EQ(total, 15);
}
