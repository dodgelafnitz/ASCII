/*
 * Copywrite 2021 Dodge Lafnitzegger
 */

#include "Systems/Input/MouseManager.h"

Delegate<ivec2> MouseManager::AddMousePositionEvent(
  DelegateFunc<ivec2> const & func
) {
  return m_mousePositionDelegator.AddDelegate(func);
}

Delegate<ivec2> MouseManager::AddMouseDeltaEvent(
  DelegateFunc<ivec2> const & func
) {
  return m_mouseDeltaDelegator.AddDelegate(func);
}

Delegate<int> MouseManager::AddMouseScrollEvent(
  DelegateFunc<int> const & func
) {
  return m_mouseScrollDelegator.AddDelegate(func);
}

ivec2 MouseManager::GetMousePosition(void) const {
  return m_mousePos;
}

void MouseManager::SetMousePosition(ivec2 const & mousePosition) {
  ivec2 const mouseDelta = mousePosition - m_mousePos;

  m_mousePos = mousePosition;

  m_mousePositionDelegator.Trigger(m_mousePos);
  m_mouseDeltaDelegator.Trigger(mouseDelta);
}

void MouseManager::ScrollMouse(int mouseScroll) {
  m_mouseScrollDelegator.Trigger(mouseScroll);
}
