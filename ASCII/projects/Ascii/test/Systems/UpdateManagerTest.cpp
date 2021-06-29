/*
 * Copywrite 2021 Dodge Lafnitzegger
 */

#include "Systems/UpdateManager.h"
#include "gtest/gtest.h"
#include "gmock/gmock.h"

DEFINE_MockUpdateManager();

DEFINE_MockAsciiWindow();

TEST(UpdateMangerTest, FixedUpdateDelegateRegistered_Updated_DtIsFixedValue) {
}

TEST(UpdateMangerTest, DynamicUpdateDelegateRegistered_Updated_DtIsAtLeastMinValue) {
}

TEST(UpdateMangerTest, BehindInFixedUpdates_Updated_MultipleFixedUpdates) {
}

TEST(UpdateMangerTest, AheadInDynamicUpdates_Updated_SkipDynamicUpdate) {
}
