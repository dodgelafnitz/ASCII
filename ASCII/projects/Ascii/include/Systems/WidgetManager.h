/*
 * Copywrite 2022 Dodge Lafnitzegger
 */

#ifndef ASCII_SYSTEMS_WIDGETMANAGER_H
#define ASCII_SYSTEMS_WIDGETMANAGER_H

#include <memory>

#include "Containers/Grid.h"
#include "General/Delagate.h"
#include "Window/Window.h"
#include "Systems/Input/TextManager.h"

class IInputManager;
class Widget;

class IWidgetManager {
public:
  virtual ~IWidgetManager(void) = default;

  virtual std::shared_ptr<IInputManager> GetInputManager(void) const = 0;
  virtual void                           SetInputManager(std::shared_ptr<IInputManager> const & inputManager) = 0;

  virtual std::shared_ptr<Widget> GetRoot(void) = 0;
  std::shared_ptr<Widget const>   GetRoot(void) const;

  virtual std::shared_ptr<Widget> GetHardFocus(void) = 0;
  std::shared_ptr<Widget const>   GetHardFocus(void) const;
  virtual std::shared_ptr<Widget> GetSoftFocus(void) = 0;
  std::shared_ptr<Widget const>   GetSoftFocus(void) const;

  virtual void SetFocus(std::shared_ptr<Widget> const & focus) = 0;

  virtual void Update(float dt) = 0;
  virtual void Draw(Grid<AsciiCell, 2> & o_screen) const = 0;
};

class WidgetManager : public IWidgetManager, public std::enable_shared_from_this<WidgetManager> {
public:
  WidgetManager(void);
  WidgetManager(std::shared_ptr<IInputManager> const & inputManager);

  virtual ~WidgetManager(void) override = default;

  virtual std::shared_ptr<IInputManager> GetInputManager(void) const override;
  virtual void                           SetInputManager(std::shared_ptr<IInputManager> const & inputManager) override;

  virtual std::shared_ptr<Widget> GetRoot(void) override;

  virtual std::shared_ptr<Widget> GetHardFocus(void) override;
  virtual std::shared_ptr<Widget> GetSoftFocus(void) override;

  virtual void SetFocus(std::shared_ptr<Widget> const & focus) override;

  virtual void Update(float dt) override;
  virtual void Draw(Grid<AsciiCell, 2> & o_screen) const override;

private:
  Delegate<AsciiButton, bool> m_buttonDelegate;
  Delegate<ivec2>             m_mousePositionDelegate;
  Delegate<int>               m_mouseScrollDelegate;
  Delegate<TextEvent>         m_textEventDelegate;

  std::shared_ptr<IInputManager> m_inputManager;
  std::shared_ptr<Widget>        m_root;
  std::shared_ptr<Widget>        m_hardFocus;
  std::shared_ptr<Widget>        m_softFocus;
};

#define DEFINE_MockWidgetManager()                \
class MockWidgetManager : public IWidgetManager { \
public:                                           \
  MOCK_METHOD(                                    \
    std::shared_ptr<IInputManager>,               \
    GetInputManager,                              \
    (),                                           \
    (const, override)                             \
  );                                              \
  MOCK_METHOD(                                    \
    void,                                         \
    SetInputManager,                              \
    (std::shared_ptr<IInputManager> const &),     \
    (override)                                    \
  );                                              \
  MOCK_METHOD(                                    \
    std::shared_ptr<Widget>,                      \
    GetRoot,                                      \
    (),                                           \
    (override)                                    \
  );                                              \
  MOCK_METHOD(                                    \
    std::shared_ptr<Widget>,                      \
    GetHardFocus,                                 \
    (),                                           \
    (override)                                    \
  );                                              \
  MOCK_METHOD(                                    \
    std::shared_ptr<Widget>,                      \
    GetSoftFocus,                                 \
    (),                                           \
    (override)                                    \
  );                                              \
  MOCK_METHOD(                                    \
    void,                                         \
    SetFocus,                                     \
    (std::shared_ptr<Widget> const &),            \
    (override)                                    \
  );                                              \
  MOCK_METHOD(                                    \
    void,                                         \
    Update,                                       \
    (float),                                      \
    (override)                                    \
  );                                              \
  MOCK_METHOD(                                    \
    void,                                         \
    Draw,                                         \
    ((Grid<AsciiCell, 2> &)),                     \
    (const, override)                             \
  );                                              \
}

#endif // ASCII_SYSTEMS_WIDGETMANAGER_H
