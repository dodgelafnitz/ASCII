/*
 * Copywrite 2022 Dodge Lafnitzegger
 */

#include "Systems/WidgetManager.h"
#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "Systems/Input/InputManager.h"
#include "Widget/Widget.h"
#include "Window/Window.h"

DEFINE_MockAsciiWindow();
DEFINE_MockInputManager();
DEFINE_MockWidgetManager();

TEST(WidgetManagerTest, DefaultConstructed_CheckInputManager_NoInputManager) {
  auto const widgetManager = std::make_shared<WidgetManager>();

  EXPECT_EQ(widgetManager->GetInputManager(), nullptr);
}

TEST(WidgetManagerTest, ConstructedWithInputManager_CheckInputManager_SameInputManager) {
  auto const window        = std::make_shared<MockAsciiWindow>();
  auto const inputManager  = std::make_shared<InputManager>(window);
  auto const widgetManager = std::make_shared<WidgetManager>(inputManager);

  EXPECT_EQ(widgetManager->GetInputManager(), inputManager);
}

TEST(WidgetManagerTest, AnyWidgetManager_CheckRoot_RootExists) {
  auto const window        = std::make_shared<MockAsciiWindow>();
  auto const inputManager  = std::make_shared<InputManager>(window);
  auto const widgetManager = std::make_shared<WidgetManager>(inputManager);

  EXPECT_TRUE(widgetManager->GetRoot());
}

TEST(WidgetManagerTest, AnyWidgetManager_CheckRootsInputManager_CorrectInputManager) {
  auto const window        = std::make_shared<MockAsciiWindow>();
  auto const inputManager  = std::make_shared<InputManager>(window);
  auto const widgetManager = std::make_shared<WidgetManager>(inputManager);

  EXPECT_EQ(widgetManager->GetRoot()->GetInputManager().lock(), inputManager);
}

TEST(WidgetManagerTest, AnyWidgetManager_CheckRootsWidgetManager_CorrectWidgetManager) {
  auto const window        = std::make_shared<MockAsciiWindow>();
  auto const inputManager  = std::make_shared<InputManager>(window);
  auto const widgetManager = std::make_shared<WidgetManager>(inputManager);

  EXPECT_EQ(widgetManager->GetRoot()->GetWidgetManager().lock(), widgetManager);
}

TEST(WidgetManagerTest, WidgetManagerWithWidgets_MouseOverWidget_WidgetIsSoftFocus) {
  auto const window        = std::make_shared<MockAsciiWindow>();
  auto const inputManager  = std::make_shared<InputManager>(window);
  auto const widgetManager = std::make_shared<WidgetManager>(inputManager);

  FAIL();
}

TEST(WidgetManagerTest, AnyWidgetManager_OverNoChildWidget_RootIsSoftFocus) {
}

TEST(WidgetManagerTest, WidgetManagerWithWidgets_MouseOverWidgetWithinWidget_CorrectWidgetIsSoftFocus) {
}

TEST(WidgetManagerTest, WidgetManagerWithWidgets_MouseOverMaskedWidgetWithinWidget_CorrectWidgetIsSoftFocus) {
}

TEST(WidgetManagerTest, WidgetManagerWithWidgets_MouseOverOverlappingWidgetsWithinWidget_CorrectWidgetIsSoftFocus) {
}

TEST(WidgetManagerTest, WidgetManagerWithWidgets_MouseDownOnFocussableSoftFocus_WidgetBecomesHardFocus) {
}

TEST(WidgetManagerTest, WidgetManagerWithWidgets_MouseDownOnNonFocussableSoftFocusWithinFocussableWidget_FocussableWidgetBecomesHardFocus) {
}

TEST(WidgetManagerTest, WidgetManagerWithWidgets_MouseOffOfWindow_NoSoftFocus) {
}

TEST(WidgetManagerTest, WidgetManagerWithWidgets_WidgetPressed_PressedEventSentToHardFocus) {
}

TEST(WidgetManagerTest, WidgetManagerWithWidgets_WidgetMultiPressed_MultiPressedEventSentToHardFocus) {
}

TEST(WidgetManagerTest, WidgetManagerWithWidgets_WidgetHeld_HeldEventSentToHardFocus) {
}

TEST(WidgetManagerTest, WidgetManagerWithWidgets_WidgetMultiHeld_MultiHeldEventSentToHardFocus) {
}

TEST(WidgetManagerTest, WidgetManagerWithWidgets_WidgetDraggedImmediate_DraggedEventSentToHardFocus) {
}

TEST(WidgetManagerTest, WidgetManagerWithWidgets_WidgetMultiDraggedImmediate_MultiDraggedEventSentToHardFocus) {
}

TEST(WidgetManagerTest, WidgetManagerWithWidgets_WidgetDraggedFromHold_DraggedEventSentToHardFocus) {
}

TEST(WidgetManagerTest, WidgetManagerWithWidgets_WidgetMultiDraggedFromHold_MultiDraggedEventSentToHardFocus) {
}

TEST(WidgetManagerTest, WidgetManagerWithWidgets_TextEvent_TextEventSentToHardFocus) {
}

TEST(WidgetManagerTest, WidgetManagerWithWidgets_WidgetHovered_HoverEventSentToSoftFocus) {
}

TEST(WidgetManagerTest, WidgetManagerWithWidgets_WidgetScrolled_ScrollEventSentToSoftFocus) {
}

TEST(WidgetManagerTest, WidgetManagerWithWidgets_WidgetButtonDown_ButtonDownEventSentToSoftFocus) {
}

TEST(WidgetManagerTest, WidgetManagerWithWidgets_WidgetButtonUp_ButtonUpEventSentToSoftFocus) {
}

TEST(WidgetManagerTest, WidgetManagerWithWidgets_WidgetSendsEventToNonHandlingWidget_EventSentToParent) {
}

TEST(WidgetManagerTest, WidgetManagerWithWidgets_FocusIsSet_HardFocusChangedToWidget) {
}

TEST(WidgetManagerTest, WidgetManagerWithWidgets_WidgetManagerDraws_RootAndAllWidgetsDrawn) {
}


