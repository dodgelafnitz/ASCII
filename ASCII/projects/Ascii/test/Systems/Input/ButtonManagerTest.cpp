/*
 * Copywrite 2021 Dodge Lafnitzegger
 */

#include "Systems/Input/ButtonManager.h"
#include "gtest/gtest.h"
#include "gmock/gmock.h"

DEFINE_MockButtonManager();

TEST(ButtonManagerTest, ButtonDelegateOwned_ButtonChanged_DelegateTriggered) {
  ButtonManager buttonManager;

  bool aState;

  Delegate<bool> onA = buttonManager.AddButtonEvent(AsciiButton::A, [&aState](bool isDown) {
    aState = isDown;
  });

  buttonManager.SetButtonState(AsciiButton::A, true);

  EXPECT_TRUE(aState);
}

TEST(ButtonManagerTest, GenericButtonDelegateOwned_ButtonChanged_DelegateTriggered) {
  ButtonManager buttonManager;

  AsciiButton button;
  bool        buttonState;

  Delegate<AsciiButton, bool> onA = buttonManager.AddGenericButtonEvent(
    [&button, &buttonState](AsciiButton triggeredButton, bool isDown) {
      button = triggeredButton;
      buttonState = isDown;
    }
  );

  buttonManager.SetButtonState(AsciiButton::B, true);

  EXPECT_EQ(button, AsciiButton::B);
  EXPECT_TRUE(buttonState);
}

TEST(ButtonManagerTest, ButtonIsSet_GetTheButton_ButtonIsSetValue) {
  ButtonManager buttonManager;

  buttonManager.SetButtonState(AsciiButton::Key0, true);

  EXPECT_TRUE(buttonManager.GetButtonState(AsciiButton::Key0));
}

TEST(ButtonManagerTest, DefaultConstructed_GetAnyButton_StateIsUp) {
  ButtonManager buttonManager;

  for (int i = 0; i < int(AsciiButton::Count); ++i) {
    EXPECT_FALSE(buttonManager.GetButtonState(AsciiButton(i)));
  }
}
