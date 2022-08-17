/*
 * Copywrite 2022 Dodge Lafnitzegger
 */

#include "Systems/WidgetManager.h"

std::shared_ptr<IInputManager> WidgetManager::GetInputManager(void) const {
  return nullptr;
}

void WidgetManager::SetInputManager(std::shared_ptr<IInputManager> const & inputManager) {
}

std::shared_ptr<Widget> WidgetManager::GetRoot(void) const {
  return nullptr;
}

std::shared_ptr<Widget> WidgetManager::GetHardFocus(void) const {
  return nullptr;
}

std::shared_ptr<Widget> WidgetManager::GetSoftFocus(void) const {
  return nullptr;
}

void WidgetManager::SetFocus(std::shared_ptr<Widget> const & focus) {
}

void WidgetManager::Update(float dt) {
}

void WidgetManager::Draw(Grid<AsciiCell, 2> & o_screen) const {
}
