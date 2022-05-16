/*
 * Copywrite 2021 Dodge Lafnitzegger
 */

#include "Systems/Input/InputManager.h"
#include "gtest/gtest.h"
#include "gmock/gmock.h"

DEFINE_MockInputManager();

DEFINE_MockAsciiWindow();
DEFINE_MockButtonManager();
DEFINE_MockMouseManager();
DEFINE_MockStateManager();
DEFINE_MockTextManager();

using ::testing::Return;

class InputManagerTest : public ::testing::Test {
protected:
  static AsciiInputEvent AsciiButtonEvent(AsciiButton button, bool isDown) {
    AsciiInputEvent event;
    event.type = AsciiInputType::Button;
    event.buttonEvent.button = button;
    event.buttonEvent.isDown = isDown;

    return event;
  }

  static AsciiInputEvent AsciiMousePositionEvent(ivec2 const & position) {
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

  static AsciiInputEvent AsciiStateEvent(AsciiState state, bool value) {
    AsciiInputEvent event;
    event.type = AsciiInputType::State;
    event.stateEvent.state    = state;
    event.stateEvent.isActive = value;

    return event;
  }
};

TEST_F(InputManagerTest, ButtonEventRegistered_ButtonEventProcessed_EventTriggers) {
  std::vector<AsciiInputEvent> const events = {
    AsciiButtonEvent(AsciiButton::P, true),
  };

  auto asciiWindow = std::make_shared<MockAsciiWindow>();
  EXPECT_CALL(*asciiWindow, PollInput())
    .WillOnce(Return(events))
  ;

  std::shared_ptr<IMouseManager> mouseManager;
  std::shared_ptr<IStateManager> stateManager;
  std::shared_ptr<ITextManager>  textManager;

  auto buttonManager = std::make_shared<MockButtonManager>();
  EXPECT_CALL(*buttonManager, SetButtonState(AsciiButton::P, true))
    .Times(1)
  ;

  InputManager inputManager(
    asciiWindow,
    buttonManager,
    mouseManager,
    stateManager,
    textManager
  );

  inputManager.ProcessInput();
}

TEST_F(InputManagerTest, MousePositionEventRegistered_MousePositionEventProcessed_EventTriggers) {
  std::vector<AsciiInputEvent> const events = {
    AsciiMousePositionEvent(ivec2(14, 100)),
  };

  auto asciiWindow = std::make_shared<MockAsciiWindow>();
  EXPECT_CALL(*asciiWindow, PollInput())
    .WillOnce(Return(events))
  ;

  auto mouseManager = std::make_shared<MockMouseManager>();
  EXPECT_CALL(*mouseManager, SetMousePosition(ivec2(14, 100)))
    .Times(1)
  ;

  std::shared_ptr<IButtonManager> buttonManager;
  std::shared_ptr<IStateManager>  stateManager;
  std::shared_ptr<ITextManager>   textManager;

  InputManager inputManager(
    asciiWindow,
    buttonManager,
    mouseManager,
    stateManager,
    textManager
  );

  inputManager.ProcessInput();
}

TEST_F(InputManagerTest, MouseScrollEventRegistered_MouseScrollEventProcessed_EventTriggers) {
  std::vector<AsciiInputEvent> const events = {
    AsciiMouseScrollEvent(-4),
  };

  auto asciiWindow = std::make_shared<MockAsciiWindow>();
  EXPECT_CALL(*asciiWindow, PollInput())
    .WillOnce(Return(events))
  ;

  auto mouseManager = std::make_shared<MockMouseManager>();
  EXPECT_CALL(*mouseManager, ScrollMouse(-4))
    .Times(1)
  ;

  std::shared_ptr<IButtonManager> buttonManager;
  std::shared_ptr<IStateManager>  stateManager;
  std::shared_ptr<ITextManager>   textManager;

  InputManager inputManager(
    asciiWindow,
    buttonManager,
    mouseManager,
    stateManager,
    textManager
  );

  inputManager.ProcessInput();
}

TEST_F(InputManagerTest, StateEventRegistered_MouseScrollEventProcessed_EventTriggers) {
  std::vector<AsciiInputEvent> const events = {
    AsciiStateEvent(AsciiState::CapsLock, true),
  };

  auto asciiWindow = std::make_shared<MockAsciiWindow>();
  EXPECT_CALL(*asciiWindow, PollInput())
    .WillOnce(Return(events))
  ;

  auto stateManager = std::make_shared<MockStateManager>();
  EXPECT_CALL(*stateManager, SetStateValue(AsciiState::CapsLock, true))
    .Times(1)
  ;

  std::shared_ptr<IMouseManager>  mouseManager;
  std::shared_ptr<IButtonManager> buttonManager;
  std::shared_ptr<ITextManager>   textManager;

  InputManager inputManager(
    asciiWindow,
    buttonManager,
    mouseManager,
    stateManager,
    textManager
  );

  inputManager.ProcessInput();
}

TEST_F(InputManagerTest, AnyConstruction_ButtonAndStateManagersSet_TextManagerConnected) {
  auto textManager = std::make_shared<MockTextManager>();
  EXPECT_CALL(*textManager, SetButtonManager)
    .Times(1)
  ;
  EXPECT_CALL(*textManager, SetStateManager)
    .Times(1)
  ;

  std::shared_ptr<IAsciiWindow>   asciiWindow;
  std::shared_ptr<IButtonManager> buttonManager;
  std::shared_ptr<IStateManager>  stateManager;
  std::shared_ptr<IMouseManager>  mouseManager;

  InputManager inputManager(
    asciiWindow,
    buttonManager,
    mouseManager,
    stateManager,
    textManager
  );
}

TEST_F(InputManagerTest, NullWindow_ProcessEvents_DoesNothing) {
  InputManager inputManager(nullptr);

  inputManager.ProcessInput();
}

TEST_F(InputManagerTest, NullMouseManager_MousePositionEventProcessed_DoesNothing) {
  std::vector<AsciiInputEvent> const events = {
    AsciiMousePositionEvent(ivec2(30, 12)),
  };

  auto asciiWindow = std::make_shared<MockAsciiWindow>();
  EXPECT_CALL(*asciiWindow, PollInput())
    .WillOnce(Return(events))
  ;

  InputManager inputManager(asciiWindow, nullptr, nullptr, nullptr, nullptr);

  inputManager.ProcessInput();
}

TEST_F(InputManagerTest, NullMouseManager_MouseScrollEventProcessed_DoesNothing) {
  std::vector<AsciiInputEvent> const events = {
    AsciiMouseScrollEvent(-2),
  };

  auto asciiWindow = std::make_shared<MockAsciiWindow>();
  EXPECT_CALL(*asciiWindow, PollInput())
    .WillOnce(Return(events))
  ;

  InputManager inputManager(asciiWindow, nullptr, nullptr, nullptr, nullptr);

  inputManager.ProcessInput();
}

TEST_F(InputManagerTest, NullButtonManager_ButtonEventProcessed_DoesNothing) {
  std::vector<AsciiInputEvent> const events = {
    AsciiButtonEvent(AsciiButton::Space, false),
  };

  auto asciiWindow = std::make_shared<MockAsciiWindow>();
  EXPECT_CALL(*asciiWindow, PollInput())
    .WillOnce(Return(events))
  ;

  InputManager inputManager(asciiWindow, nullptr, nullptr, nullptr, nullptr);

  inputManager.ProcessInput();
}

TEST_F(InputManagerTest, NullButtonManager_StateEventProcessed_DoesNothing) {
  std::vector<AsciiInputEvent> const events = {
    AsciiStateEvent(AsciiState::Control, true),
  };

  auto asciiWindow = std::make_shared<MockAsciiWindow>();
  EXPECT_CALL(*asciiWindow, PollInput())
    .WillOnce(Return(events))
  ;

  InputManager inputManager(asciiWindow, nullptr, nullptr, nullptr, nullptr);

  inputManager.ProcessInput();
}
