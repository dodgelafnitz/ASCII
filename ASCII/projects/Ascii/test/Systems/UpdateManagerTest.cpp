/*
 * Copywrite 2021 Dodge Lafnitzegger
 */

#include "Systems/UpdateManager.h"
#include "gtest/gtest.h"
#include "gmock/gmock.h"

DEFINE_MockUpdateManager();

DEFINE_MockAsciiWindow();

using ::testing::Return;
using ::testing::Gt;

TEST(UpdateMangerTest, FixedUpdateDelegateRegistered_Updated_DtIsFixedValue) {
  auto asciiWindow = std::make_shared<MockAsciiWindow>();
  EXPECT_CALL(*asciiWindow, GetRunMs())
    .WillOnce(Return(200))
    .WillOnce(Return(400))
    .WillOnce(Return(1600))
  ;

  UpdateManager updateManger(asciiWindow, 1.0f, 0.5f);
  updateManger.Update();

  float updateDt = 0.0f;
  Delegate<float> onUpdate = updateManger.AddOnFixedUpdate([&updateDt](float dt) {
    updateDt = dt;
  });

  updateManger.Update();

  EXPECT_EQ(updateDt, 1.0f);
}

TEST(UpdateMangerTest, DynamicUpdateDelegateRegistered_Updated_DtIsAtLeastMinValue) {
  auto asciiWindow = std::make_shared<MockAsciiWindow>();
  EXPECT_CALL(*asciiWindow, GetRunMs())
    .WillOnce(Return(200))
    .WillOnce(Return(400))
    .WillOnce(Return(600))
  ;

  UpdateManager updateManger(asciiWindow, 1.0f, 0.5f);
  updateManger.Update();

  float updateDt = 0.0f;
  Delegate<float, float> onUpdate = updateManger.AddOnDynamicUpdate([&updateDt](float dt, float) {
    updateDt = dt;
  });

  updateManger.Update();

  EXPECT_GE(updateDt, 0.5f);
}

TEST(UpdateMangerTest, BehindInFixedUpdates_Updated_MultipleFixedUpdates) {
  auto asciiWindow = std::make_shared<MockAsciiWindow>();
  EXPECT_CALL(*asciiWindow, GetRunMs())
    .WillOnce(Return(200))
    .WillOnce(Return(3000))
  ;

  UpdateManager updateManger(asciiWindow, 1.0f, 0.5f);

  int updates = 0;
  Delegate<float> onUpdate = updateManger.AddOnFixedUpdate([&updates](float) {
    ++updates;
  });

  updateManger.Update();

  EXPECT_EQ(updates, 3);
}

TEST(UpdateMangerTest, AheadInFixedUpdates_Updated_SkipFixedUpdate) {
  auto asciiWindow = std::make_shared<MockAsciiWindow>();
  EXPECT_CALL(*asciiWindow, GetRunMs())
    .WillOnce(Return(200))
    .WillOnce(Return(400))
    .WillOnce(Return(600))
  ;

  UpdateManager updateManger(asciiWindow, 1.0f, 0.5f);
  updateManger.Update();

  bool updated = false;
  Delegate<float> onUpdate = updateManger.AddOnFixedUpdate([&updated](float) {
    updated = true;
  });

  updateManger.Update();

  EXPECT_FALSE(updated);
}

TEST(UpdateMangerTest, AheadInDynamicUpdates_Updated_WaitForDynamicUpdate) {
  auto asciiWindow = std::make_shared<MockAsciiWindow>();
  EXPECT_CALL(*asciiWindow, GetRunMs())
    .WillOnce(Return(200))
    .WillOnce(Return(400))
    .WillOnce(Return(600))
  ;

  EXPECT_CALL(*asciiWindow, Sleep(Gt(280)));

  UpdateManager updateManger(asciiWindow, 1.0f, 0.5f);
  updateManger.Update();
  updateManger.Update();
}

TEST(UpdateMangerTest, DynamicUpdateDelegateRegistered_UpdateOnFixedUpdateFrame_ProgressIs0) {
  auto asciiWindow = std::make_shared<MockAsciiWindow>();
  EXPECT_CALL(*asciiWindow, GetRunMs())
    .WillOnce(Return(200))
    .WillOnce(Return(1200))
    .WillOnce(Return(2200))
  ;

  UpdateManager updateManger(asciiWindow, 1.0f, 0.5f);
  updateManger.Update();

  float progress = -1.0f;
  Delegate<float, float> onUpdate = updateManger.AddOnDynamicUpdate([&progress](float, float frameDelta) {
    progress = frameDelta;
  });

  updateManger.Update();

  EXPECT_EQ(progress, 0.0f);
}

TEST(UpdateMangerTest, DynamicUpdateDelegateRegistered_UpdateRightBeforeFixedUpdateFrame_ProgressIsAlmost1) {
  auto asciiWindow = std::make_shared<MockAsciiWindow>();
  EXPECT_CALL(*asciiWindow, GetRunMs())
    .WillOnce(Return(200))
    .WillOnce(Return(1200))
    .WillOnce(Return(2199))
  ;

  UpdateManager updateManger(asciiWindow, 1.0f, 0.5f);
  updateManger.Update();

  float progress = -1.0f;
  Delegate<float, float> onUpdate = updateManger.AddOnDynamicUpdate([&progress](float, float frameDelta) {
    progress = frameDelta;
  });

  updateManger.Update();

  EXPECT_GT(progress, 0.99f);
}

TEST(UpdateMangerTest, ImperfectUpdateTimes_ManyFixedUpdates_UpdateCountMatchesExpectedElapsedTime) {
  auto asciiWindow = std::make_shared<MockAsciiWindow>();
  EXPECT_CALL(*asciiWindow, GetRunMs())
    .WillOnce(Return(200))
    .WillOnce(Return(99999200))
  ;

  UpdateManager updateManger(asciiWindow, 0.51223f, 1.0f / 60.0f);

  int updates = 0;
  Delegate<float> onUpdate = updateManger.AddOnFixedUpdate([&updates](float) {
    ++updates;
  });

  updateManger.Update();

  int const expectedUpdates = int(float(99999200 - 200) / (0.51223f * 1000.0f)) + 1;

  EXPECT_EQ(updates, expectedUpdates);
}
