/*
 * Copywrite 2022 Dodge Lafnitzegger
 */

#ifndef ASCII_SYSTEMS_WIDGETMANAGER_H
#define ASCII_SYSTEMS_WIDGETMANAGER_H

#include <memory>

#include "Containers/Grid.h"
#include "General/Delagate.h"

class IInputManager;

class AsciiButton;
class AsciiCell;
class TextEvent;
class Widget;

class IWidgetManager {
public:
  virtual ~IWidgetManager(void) = default;

  virtual std::shared_ptr<IInputManager> GetInputManager(void) const = 0;
  virtual void                           SetInputManager(std::shared_ptr<IInputManager> const & inputManager) = 0;

  virtual std::shared_ptr<Widget> GetRoot(void) const = 0;

  virtual std::shared_ptr<Widget> GetHardFocus(void) const = 0;
  virtual std::shared_ptr<Widget> GetSoftFocus(void) const = 0;

  virtual void Update(float dt) = 0;
  virtual void Draw(Grid<AsciiCell, 2> & o_screen) const = 0;
};

class WidgetManager : public IWidgetManager {
public:
  virtual ~WidgetManager(void) = default;

  virtual std::shared_ptr<IInputManager> GetInputManager(void) const override;
  virtual void                           SetInputManager(std::shared_ptr<IInputManager> const & inputManager) override;

  virtual std::shared_ptr<Widget> GetRoot(void) const override;

  virtual std::shared_ptr<Widget> GetHardFocus(void) const override;
  virtual std::shared_ptr<Widget> GetSoftFocus(void) const override;

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
    (const, override)                             \
  );                                              \
  MOCK_METHOD(                                    \
    std::shared_ptr<Widget>,                      \
    GetHardFocus,                                 \
    (),                                           \
    (const, override)                             \
  );                                              \
  MOCK_METHOD(                                    \
    std::shared_ptr<Widget>,                      \
    GetSoftFocus,                                 \
    (),                                           \
    (const, override)                             \
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
    (Grid<AsciiCell, 2> &),                       \
    (const, override)                             \
  );                                              \
}

#endif // ASCII_SYSTEMS_WIDGETMANAGER_H
