/*
 * Copywrite 2021 Dodge Lafnitzegger
 */

#include "Systems/Input/InputManager.h"

InputManager::InputManager(void) :
  InputManager(std::make_shared<AsciiWindow>())
{}

InputManager::InputManager(std::shared_ptr<IAsciiWindow> const & window) :
  m_window(window),
  m_buttonManager(std::make_shared<ButtonManager>()),
  m_mouseManager(std::make_shared<MouseManager>())
{}

Delegate<bool> InputManager::AddButtonEvent(
  AsciiButton button,
  DelegateFunc<bool> const & func
) {
  return m_buttonManager->AddButtonEvent(button, func);
}

Delegate<AsciiButton, bool> InputManager::AddGenericButtonEvent(
  DelegateFunc<AsciiButton, bool> const & func
) {
  return Delegate<AsciiButton, bool>();
}

Delegate<ivec2> InputManager::AddMousePositionEvent(
  DelegateFunc<ivec2> const & func
) {
  return Delegate<ivec2>();
}

Delegate<ivec2> InputManager::AddMouseDeltaEvent(
  DelegateFunc<ivec2> const & func
) {
  return Delegate<ivec2>();
}

Delegate<int> InputManager::AddMouseScrollEvent(
  DelegateFunc<int> const & func
) {
  return Delegate<int>();
}

bool InputManager::GetButtonState(AsciiButton button) const {
  return bool();
}

ivec2 InputManager::GetMousePosition(void) const {
  return ivec2();
}

void InputManager::ProcessInput(void) {
  std::vector<AsciiInputEvent> events = m_window->PollInput();

  for (auto const & event : events) {
    switch (event.type) {
      case AsciiInputType::Button: {
        m_buttonManager->SetButtonState(
          event.buttonEvent.button,
          event.buttonEvent.isDown
        );
      } break;

      case AsciiInputType::MousePosition: {
        m_mouseManager->SetMousePosition(
          event.mousePositionEvent
        );
      } break;

      case AsciiInputType::MouseScroll: {
        m_mouseManager->ScrollMouse(
          event.mouseScrollEvent
        );
      } break;

      case AsciiInputType::State: {
        // TODO
      } break;
    }
  }
}
