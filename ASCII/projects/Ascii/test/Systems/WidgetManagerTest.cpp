/*
 * Copywrite 2022 Dodge Lafnitzegger
 */

#include "Systems/WidgetManager.h"
#include "gtest/gtest.h"
#include "gmock/gmock.h"

DEFINE_MockWidgetManager();

TEST(WidgetManagerTest, DefaultConstructed_CheckInputManager_NoInputManager) {
  WidgetManager widgetManager;

  EXPECT_EQ(widgetManager.GetInputManager(), nullptr);
}

TEST(WidgetManagerTest, ConstructedWithInputManager_CheckInputManager_SameInputManager) {
}

TEST(WidgetManagerTest, AnyWidgetManager_CheckRoot_RootExists) {
}

TEST(WidgetManagerTest, AnyWidgetManager_CheckRootsInputManager_CorrectInputManager) {
}

TEST(WidgetManagerTest, AnyWidgetManager_CheckRootsWidgetManager_CorrectWidgetManager) {
}
