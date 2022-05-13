/*
 * Copywrite 2021 Dodge Lafnitzegger
 */

#include "Systems/Input/InputManager.h"

struct InputManager::Connector {
  Delegate<bool> insertButtonDelegate;
  Delegate<bool> numpadInsertButtonDelegate;
};

InputManager::InputManager(void) :
  InputManager(std::make_shared<AsciiWindow>())
{}

InputManager::InputManager(std::shared_ptr<IAsciiWindow> const & window) :
  InputManager(
    window,
    std::make_shared<ButtonManager>(),
    std::make_shared<MouseManager>(),
    std::make_shared<StateManager>(),
    std::make_shared<TextManager>()
  )
{}

InputManager::InputManager(
  std::shared_ptr<IAsciiWindow> const &   window,
  std::shared_ptr<IButtonManager> const & buttonManager,
  std::shared_ptr<IMouseManager> const &  mouseManager,
  std::shared_ptr<IStateManager> const &  stateManager,
  std::shared_ptr<ITextManager> const &   textManager

) :
  m_window(window),
  m_buttonManager(buttonManager),
  m_mouseManager(mouseManager),
  m_stateManager(stateManager),
  m_textManager(textManager)
{
  if (m_textManager) {
    m_textManager->SetManagers(buttonManager, stateManager);
  }

  m_connector = std::make_shared<InputManager::Connector>();
  if (m_buttonManager && m_stateManager) {
    m_connector->insertButtonDelegate = m_buttonManager->AddButtonEvent(
      AsciiButton::Insert,
      [this](bool isDown) {
        if (isDown) {
          bool const insertSet = m_stateManager->GetStateValue(AsciiState::Insert);
          m_stateManager->SetStateValue(AsciiState::Insert, !insertSet);
        }
      }
    );

    m_connector->numpadInsertButtonDelegate = m_buttonManager->AddButtonEvent(
      AsciiButton::NumPad0,
      [this](bool isDown) {
        if (isDown) {
          if (m_stateManager->GetStateValue(AsciiState::Shift) == m_stateManager->GetStateValue(AsciiState::NumLock)) {
            bool const insertSet = m_stateManager->GetStateValue(AsciiState::Insert);
            m_stateManager->SetStateValue(AsciiState::Insert, !insertSet);
          }
        }
      }
    );
  }
}

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
        if (m_stateManager) {
          m_stateManager->SetStateValue(
            event.stateEvent.state, event.stateEvent.isActive
          );
        }
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

std::shared_ptr<IStateManager> InputManager::GetStateManager(void) const {
  return m_stateManager;
}

std::shared_ptr<ITextManager> InputManager::GetTextManager(void) const {
  return m_textManager;
}
