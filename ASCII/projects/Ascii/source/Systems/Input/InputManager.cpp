/*
 * Copywrite 2021 Dodge Lafnitzegger
 */

#include "Systems/Input/InputManager.h"

InputManager::InputManager(void) :
  InputManager(std::make_shared<AsciiWindow>())
{}

InputManager::InputManager(std::shared_ptr<IAsciiWindow> const & window) :
  InputManager(
    window,
    std::make_shared<ButtonManager>(),
    std::make_shared<MouseManager>()
  )
{}

InputManager::InputManager(
  std::shared_ptr<IAsciiWindow> const &   window,
  std::shared_ptr<IButtonManager> const & buttonManager,
  std::shared_ptr<IMouseManager> const &  mouseManager
) :
  m_window(window),
  m_buttonManager(buttonManager),
  m_mouseManager(mouseManager)
{}

void InputManager::ProcessInput(void) {
  if (!m_window) {
    return;
  }

  std::vector<AsciiInputEvent> events = m_window->PollInput();

  for (auto const & event : events) {
    switch (event.type) {
      case AsciiInputType::Button: {
        if (m_buttonManager) {
          m_buttonManager->SetButtonState(
            event.buttonEvent.button,
            event.buttonEvent.isDown
          );
        }
      } break;

      case AsciiInputType::MousePosition: {
        if (m_mouseManager) {
          m_mouseManager->SetMousePosition(
            event.mousePositionEvent
          );
        }
      } break;

      case AsciiInputType::MouseScroll: {
        if (m_mouseManager) {
          m_mouseManager->ScrollMouse(
            event.mouseScrollEvent
          );
        }
      } break;

      case AsciiInputType::State: {
        // TODO
      } break;
    }
  }
}

std::shared_ptr<IAsciiWindow> InputManager::GetWindow(void) const {
  return m_window;
}

std::shared_ptr<IButtonManager> InputManager::GetButtonManager(void) const {
  return m_buttonManager;
}

std::shared_ptr<IMouseManager> InputManager::GetMouseManager(void) const {
  return m_mouseManager;
}
