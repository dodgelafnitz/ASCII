/*
 * Copywrite 2021 Dodge Lafnitzegger
 */

#include "Systems/Input/InputManager.h"
#include "gtest/gtest.h"
#include "gmock/gmock.h"

DEFINE_MockInputManager();

DEFINE_MockAsciiWindow();

using ::testing::Return;

namespace {
}

class InputManagerTest : public ::testing::Test {
protected:
  static AsciiInputEvent AsciiButtonEvent(AsciiButton button, bool isDown) {
    AsciiInputEvent event;
    event.type = AsciiInputType::Button;
    event.buttonEvent.button = button;
    event.buttonEvent.isDown = isDown;

    return event;
  }

  static AsciiInputEvent AsciiMouseMoveEvent(ivec2 const & position) {
    AsciiInputEvent event;
    event.type = AsciiInputType::MousePosition;
    event.mousePositionEvent = position;

    return event;
  }

  static AsciiInputEvent AsciiMouseScrollEvent(int scroll) {
    AsciiInputEvent event;
    event.type = AsciiInputType::MouseScroll;
    event.mouseScrollEvent = scroll;

    return event;
  }

  static std::function<void(bool)> ButtonEventFunc(bool & o_state) {
    return [&o_state](bool isDown) {
      o_state = isDown;
    };
  }
};

TEST_F(InputManagerTest, ButtonEventRegistered_ButtonEventProcessed_EventTriggers) {
  std::vector<AsciiInputEvent> const events = {
    AsciiButtonEvent(AsciiButton::Q, true),
  };

  auto asciiWindow = std::make_shared<MockAsciiWindow>();
  EXPECT_CALL(*asciiWindow, PollInput())
    .WillOnce(Return(events))
  ;

  InputManager inputManager(asciiWindow);

  bool           isDown    = false;
  Delegate<bool> delegaete = inputManager.AddButtonEvent(
    AsciiButton::Q, ButtonEventFunc(isDown)
  );

  inputManager.ProcessInput();

  EXPECT_TRUE(isDown);
}
