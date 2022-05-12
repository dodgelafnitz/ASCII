/*
 * Copywrite 2021 Dodge Lafnitzegger
 */

#ifndef ASCII_SYSTEMS_INPUT_INPUTMANAGER_H
#define ASCII_SYSTEMS_INPUT_INPUTMANAGER_H

#include "Systems/Input/ButtonManager.h"
#include "Systems/Input/MouseManager.h"
#include "Systems/Input/StateManager.h"
#include "Systems/Input/TextManager.h"
#include "Window/Window.h"

class IInputManager {
public:
  virtual ~IInputManager(void) {}

  virtual void ProcessInput(void) = 0;

  virtual std::shared_ptr<IAsciiWindow>   GetWindow(void) const        = 0;
  virtual std::shared_ptr<IButtonManager> GetButtonManager(void) const = 0;
  virtual std::shared_ptr<IMouseManager>  GetMouseManager(void) const  = 0;
  virtual std::shared_ptr<IStateManager>  GetStateManager(void) const = 0;
  virtual std::shared_ptr<ITextManager>   GetTextManager(void) const = 0;
};

class InputManager : public IInputManager {
public:
  InputManager(void);
  InputManager(std::shared_ptr<IAsciiWindow> const & window);
  InputManager(
    std::shared_ptr<IAsciiWindow> const &   window,
    std::shared_ptr<IButtonManager> const & buttonManager,
    std::shared_ptr<IMouseManager> const &  mouseManager,
    std::shared_ptr<IStateManager> const &  stateManager,
    std::shared_ptr<ITextManager> const &   textManager
  );

  virtual ~InputManager(void) {}

  virtual void ProcessInput(void) override;

  virtual std::shared_ptr<IAsciiWindow>   GetWindow(void) const override;
  virtual std::shared_ptr<IButtonManager> GetButtonManager(void) const override;
  virtual std::shared_ptr<IMouseManager>  GetMouseManager(void) const override;
  virtual std::shared_ptr<IStateManager>  GetStateManager(void) const override;
  virtual std::shared_ptr<ITextManager>   GetTextManager(void) const override;

private:
  std::shared_ptr<IAsciiWindow>   m_window;
  std::shared_ptr<IButtonManager> m_buttonManager;
  std::shared_ptr<IMouseManager>  m_mouseManager;
  std::shared_ptr<IStateManager>  m_stateManager;
  std::shared_ptr<ITextManager>   m_textManager;

  struct Connector;
  std::shared_ptr<Connector> m_connector;
};

#define DEFINE_MockInputManager()               \
class MockInputManager : public IInputManager { \
public:                                         \
  MOCK_METHOD(                                  \
    void,                                       \
    ProcessInput,                               \
    (),                                         \
    (override)                                  \
  );                                            \
  MOCK_METHOD(                                  \
    std::shared_ptr<IAsciiWindow>,              \
    GetWindow,                                  \
    (),                                         \
    (const, override)                           \
  );                                            \
  MOCK_METHOD(                                  \
    std::shared_ptr<IButtonManager>,            \
    GetButtonManager,                           \
    (),                                         \
    (const, override)                           \
  );                                            \
  MOCK_METHOD(                                  \
    std::shared_ptr<IMouseManager>,             \
    GetMouseManager,                            \
    (),                                         \
    (const, override)                           \
  );                                            \
  MOCK_METHOD(                                  \
    std::shared_ptr<IStateManager>,             \
    GetStateManager,                            \
    (),                                         \
    (const, override)                           \
  );                                            \
  MOCK_METHOD(                                  \
    std::shared_ptr<ITextManager>,              \
    GetTextManager,                             \
    (),                                         \
    (const, override)                           \
  );                                            \
}

#endif // ASCII_SYSTEMS_INPUT_INPUTMANAGER_H
