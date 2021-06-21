/*
 * Copywrite 2021 Dodge Lafnitzegger
 */

#include "Systems/Input/ButtonManager.h"

Delegate<bool> ButtonManager::AddButtonEvent(
  AsciiButton button,
  DelegateFunc<bool> const & func
) {
  return m_buttonDelegators[button].AddDelegate(func);
}

Delegate<AsciiButton, bool> ButtonManager::AddGenericButtonEvent(
  DelegateFunc<AsciiButton, bool> const & func
) {
  return m_genericButtonDelegator.AddDelegate(func);
}

bool ButtonManager::GetButtonState(AsciiButton button) const {
  return m_buttonStates[int(button)];
}

void ButtonManager::SetButtonState(AsciiButton button, bool state) {
  if (GetButtonState(button) == state) {
    return;
  }

  m_buttonStates[int(button)] = state;

  if (m_buttonDelegators.count(button)) {
    if (m_buttonDelegators[button].IsEmpty()) {
      m_buttonDelegators.erase(button);
    }
    else {
      m_buttonDelegators[button].Trigger(state);
    }
  }

  m_genericButtonDelegator.Trigger(button, state);
}
