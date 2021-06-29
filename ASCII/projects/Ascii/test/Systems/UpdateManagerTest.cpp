/*
 * Copywrite 2021 Dodge Lafnitzegger
 */

#include "Systems/UpdateManager.h"
#include "gtest/gtest.h"
#include "gmock/gmock.h"

DEFINE_MockUpdateManager();

DEFINE_MockAsciiWindow();

using ::testing::Return;

TEST(UpdateMangerTest, FixedUpdateDelegateRegistered_Updated_DtIsFixedValue) {
  auto asciiWindow = std::make_shared<MockAsciiWindow>();
  EXPECT_CALL(*asciiWindow, GetRunMs())
    .WillOnce(Return(200))
    .WillOnce(Return(1400))
  ;

  UpdateManager updateManger(asciiWindow, 1.0f, 0.5f);

  float updateDt = 0.0f;
  Delegate<float> onUpdate = updateManger.AddOnFixedUpdate([&updateDt](float dt) {
    updateDt = dt;
  });

  updateManger.Update();

  EXPECT_EQ(updateDt, 1.0f);
}

TEST(UpdateMangerTest, DynamicUpdateDelegateRegistered_Updated_DtIsAtLeastMinValue) {
}

TEST(UpdateMangerTest, BehindInFixedUpdates_Updated_MultipleFixedUpdates) {
}

TEST(UpdateMangerTest, AheadInFixedUpdates_Updated_SkipFixedUpdate) {
}

TEST(UpdateMangerTest, AheadInDynamicUpdates_Updated_WaitForDynamicUpdate) {
}
