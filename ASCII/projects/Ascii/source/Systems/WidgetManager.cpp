/*
 * Copywrite 2022 Dodge Lafnitzegger
 */

#include "Systems/WidgetManager.h"

#include "Systems/Input/InputManager.h"
#include "Widget/Widget.h"

namespace {
  class RootWidget : public Widget {
  public:
    RootWidget(void) = default;

    RootWidget(std::weak_ptr<IWidgetManager> const & manager) :
      m_manager(manager)
    {}

    virtual std::weak_ptr<IWidgetManager> GetWidgetManager(void) override {
      return m_manager;
    }

    virtual std::weak_ptr<IInputManager> GetInputManager(void) override {
      if (std::shared_ptr<IWidgetManager> const manager = m_manager.lock()) {
        return manager->GetInputManager();
      }

      return std::weak_ptr<IInputManager>();
    }

    void SetWidgetManager(std::weak_ptr<IWidgetManager> const & manager) {
      m_manager = manager;
    }

  private:
    std::weak_ptr<IWidgetManager> m_manager;
  };
}

std::shared_ptr<Widget const> IWidgetManager::GetRoot(void) const {
  return const_cast<IWidgetManager *>(this)->GetRoot();
}

std::shared_ptr<Widget const> IWidgetManager::GetHardFocus(void) const {
  return const_cast<IWidgetManager *>(this)->GetHardFocus();
}

std::shared_ptr<Widget const> IWidgetManager::GetSoftFocus(void) const {
  return const_cast<IWidgetManager *>(this)->GetSoftFocus();
}

WidgetManager::WidgetManager(void) :
  m_root(std::make_shared<RootWidget>())
{}

WidgetManager::WidgetManager(std::shared_ptr<IInputManager> const & inputManager) :
  WidgetManager()
{
  SetInputManager(inputManager);
}

std::shared_ptr<IInputManager> WidgetManager::GetInputManager(void) const {
  return m_inputManager;
}

void WidgetManager::SetInputManager(std::shared_ptr<IInputManager> const & inputManager) {
  if (m_inputManager == inputManager) {
    return;
  }

  m_inputManager = inputManager;
}

std::shared_ptr<Widget> WidgetManager::GetRoot(void) {
  if (m_root->GetWidgetManager().expired()) {
    std::static_pointer_cast<RootWidget>(m_root)->SetWidgetManager(weak_from_this());
  }

  return m_root;
}

std::shared_ptr<Widget> WidgetManager::GetHardFocus(void) {
  return nullptr;
}

std::shared_ptr<Widget> WidgetManager::GetSoftFocus(void) {
  return nullptr;
}

void WidgetManager::SetFocus(std::shared_ptr<Widget> const & focus) {
}

void WidgetManager::Update(float dt) {
}

void WidgetManager::Draw(Grid<AsciiCell, 2> & o_screen) const {
}
