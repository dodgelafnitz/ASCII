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

TEST(UpdateManagerTest, FixedUpdateDelegateRegistered_Updated_DtIsFixedValue) {
  auto asciiWindow = std::make_shared<MockAsciiWindow>();
  EXPECT_CALL(*asciiWindow, GetRunMs())
    .WillOnce(Return(200))
    .WillOnce(Return(400))
    .WillOnce(Return(1600))
  ;

  UpdateManager updateManager(asciiWindow, 1.0f, 0.5f);
  updateManager.Update();

  float updateDt = 0.0f;
  Delegate<float> onUpdate = updateManager.AddOnFixedUpdate([&updateDt](float dt) {
    updateDt = dt;
  });

  updateManager.Update();

  EXPECT_EQ(updateDt, 1.0f);
}

TEST(UpdateManagerTest, DynamicUpdateDelegateRegistered_Updated_DtIsAtLeastMinValue) {
  auto asciiWindow = std::make_shared<MockAsciiWindow>();
  EXPECT_CALL(*asciiWindow, GetRunMs())
    .WillOnce(Return(200))
    .WillOnce(Return(400))
    .WillOnce(Return(600))
  ;

  UpdateManager updateManager(asciiWindow, 1.0f, 0.5f);
  updateManager.Update();

  float updateDt = 0.0f;
  Delegate<float, float> onUpdate = updateManager.AddOnDynamicUpdate([&updateDt](float dt, float) {
    updateDt = dt;
  });

  updateManager.Update();

  EXPECT_GE(updateDt, 0.5f);
}

TEST(UpdateManagerTest, BehindInFixedUpdates_Updated_MultipleFixedUpdates) {
  auto asciiWindow = std::make_shared<MockAsciiWindow>();
  EXPECT_CALL(*asciiWindow, GetRunMs())
    .WillOnce(Return(200))
    .WillOnce(Return(3000))
  ;

  UpdateManager updateManager(asciiWindow, 1.0f, 0.5f);

  int updates = 0;
  Delegate<float> onUpdate = updateManager.AddOnFixedUpdate([&updates](float) {
    ++updates;
  });

  updateManager.Update();

  EXPECT_EQ(updates, 3);
}

TEST(UpdateManagerTest, AheadInFixedUpdates_Updated_SkipFixedUpdate) {
  auto asciiWindow = std::make_shared<MockAsciiWindow>();
  EXPECT_CALL(*asciiWindow, GetRunMs())
    .WillOnce(Return(200))
    .WillOnce(Return(400))
    .WillOnce(Return(600))
  ;

  UpdateManager updateManager(asciiWindow, 1.0f, 0.5f);
  updateManager.Update();

  bool updated = false;
  Delegate<float> onUpdate = updateManager.AddOnFixedUpdate([&updated](float) {
    updated = true;
  });

  updateManager.Update();

  EXPECT_FALSE(updated);
}

TEST(UpdateManagerTest, AheadInDynamicUpdates_Updated_WaitForDynamicUpdate) {
  auto asciiWindow = std::make_shared<MockAsciiWindow>();
  EXPECT_CALL(*asciiWindow, GetRunMs())
    .WillOnce(Return(200))
    .WillOnce(Return(400))
    .WillOnce(Return(600))
  ;

  EXPECT_CALL(*asciiWindow, Sleep(Gt(280)));

  UpdateManager updateManager(asciiWindow, 1.0f, 0.5f);
  updateManager.Update();
  updateManager.Update();
}

TEST(UpdateManagerTest, DynamicUpdateDelegateRegistered_UpdateOnFixedUpdateFrame_ProgressIs0) {
  auto asciiWindow = std::make_shared<MockAsciiWindow>();
  EXPECT_CALL(*asciiWindow, GetRunMs())
    .WillOnce(Return(200))
    .WillOnce(Return(1200))
    .WillOnce(Return(2200))
  ;

  UpdateManager updateManager(asciiWindow, 1.0f, 0.5f);
  updateManager.Update();

  float progress = -1.0f;
  Delegate<float, float> onUpdate = updateManager.AddOnDynamicUpdate([&progress](float, float frameDelta) {
    progress = frameDelta;
  });

  updateManager.Update();

  EXPECT_EQ(progress, 0.0f);
}

TEST(UpdateManagerTest, DynamicUpdateDelegateRegistered_UpdateRightBeforeFixedUpdateFrame_ProgressIsAlmost1) {
  auto asciiWindow = std::make_shared<MockAsciiWindow>();
  EXPECT_CALL(*asciiWindow, GetRunMs())
    .WillOnce(Return(200))
    .WillOnce(Return(1200))
    .WillOnce(Return(2199))
  ;

  UpdateManager updateManager(asciiWindow, 1.0f, 0.5f);
  updateManager.Update();

  float progress = -1.0f;
  Delegate<float, float> onUpdate = updateManager.AddOnDynamicUpdate([&progress](float, float frameDelta) {
    progress = frameDelta;
  });

  updateManager.Update();

  EXPECT_GT(progress, 0.99f);
}

TEST(UpdateManagerTest, ImperfectUpdateTimes_ManyFixedUpdates_UpdateCountMatchesExpectedElapsedTime) {
  auto asciiWindow = std::make_shared<MockAsciiWindow>();
  EXPECT_CALL(*asciiWindow, GetRunMs())
    .WillOnce(Return(200))
    .WillOnce(Return(99999200))
  ;

  UpdateManager updateManager(asciiWindow, 0.51223f, 1.0f / 60.0f);

  int updates = 0;
  Delegate<float> onUpdate = updateManager.AddOnFixedUpdate([&updates](float) {
    ++updates;
  });

  updateManager.Update();

  int const expectedUpdates = int(float(99999200 - 200) / (0.51223f * 1000.0f)) + 1;

  EXPECT_EQ(updates, expectedUpdates);
}

TEST(UpdateManagerTest, DynamicUpdateDelegateRegistered_SleepForDynamicUpdate_ProgressIsBetween0And1) {
  auto asciiWindow = std::make_shared<MockAsciiWindow>();
  EXPECT_CALL(*asciiWindow, GetRunMs())
    .WillOnce(Return(200))
    .WillOnce(Return(400))
    .WillOnce(Return(600))
  ;

  UpdateManager updateManager(asciiWindow, 1.0f, 0.5f);
  updateManager.Update();

  float progress = -1.0f;
  Delegate<float, float> onUpdate = updateManager.AddOnDynamicUpdate([&progress](float, float frameDelta) {
    progress = frameDelta;
  });

  updateManager.Update();

  EXPECT_GE(progress, 0.0f);
  EXPECT_LT(progress, 1.0f);
}

TEST(UpdateManagerTest, SetFixedUpdateDt_CheckFixedDt_ValueIsCorrect) {
  auto asciiWindow = std::make_shared<MockAsciiWindow>();
  EXPECT_CALL(*asciiWindow, GetRunMs());

  UpdateManager updateManager(asciiWindow, 0.3f, 0.1f);

  updateManager.SetFixedUpdateDt(0.3f);

  EXPECT_EQ(updateManager.GetFixedUpdateDt(), 0.3f);
}

TEST(UpdateManagerTest, SetDynamicUpdateDt_CheckDynamicDt_ValueIsCorrect) {
  auto asciiWindow = std::make_shared<MockAsciiWindow>();
  EXPECT_CALL(*asciiWindow, GetRunMs());

  UpdateManager updateManager(asciiWindow, 0.3f, 0.1f);

  updateManager.SetDynamicUpdateDt(0.2f);

  EXPECT_EQ(updateManager.GetDynamicUpdateDt(), 0.2f);
}
