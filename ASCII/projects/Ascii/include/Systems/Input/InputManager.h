/*
 * Copywrite 2021 Dodge Lafnitzegger
 */

#ifndef ASCII_SYSTEMS_INPUT_INPUTMANAGER_H
#define ASCII_SYSTEMS_INPUT_INPUTMANAGER_H

#include "Systems/Input/ButtonManager.h"
#include "Systems/Input/MouseManager.h"

#include "Window/Window.h"

class IInputManager :
  public IButtonManager,
  public IMouseManager
{};

class InputManager : public IInputManager {
public:
  InputManager(void);
  InputManager(std::shared_ptr<IAsciiWindow> const & window);

  virtual ~InputManager(void) {}

  virtual Delegate<bool> AddButtonEvent(
    AsciiButton button,
    DelegateFunc<bool> const & func
  ) override;

  virtual Delegate<AsciiButton, bool> AddGenericButtonEvent(
    DelegateFunc<AsciiButton, bool> const & func
  ) override;

  virtual Delegate<ivec2> AddMousePositionEvent(
    DelegateFunc<ivec2> const & func
  ) override;

  virtual Delegate<ivec2> AddMouseDeltaEvent(
    DelegateFunc<ivec2> const & func
  ) override;

  virtual Delegate<int> AddMouseScrollEvent(
    DelegateFunc<int> const & func
  ) override;
  
  virtual bool  GetButtonState(AsciiButton button) const override;
  virtual ivec2 GetMousePosition(void) const override;

  void ProcessInput(void);

  std::shared_ptr<IAsciiWindow>   GetWindow(void) const;
  std::shared_ptr<IButtonManager> GetButtonManager(void) const;
  std::shared_ptr<IMouseManager>  GetMouseManager(void) const;

private:
  std::shared_ptr<IAsciiWindow>  m_window;
  std::shared_ptr<ButtonManager> m_buttonManager;
  std::shared_ptr<MouseManager>  m_mouseManager;
};

#define DEFINE_MockInputManager()                \
class MockInputManager : public IInputManager {  \
  MOCK_METHOD(                                   \
    Delegate<bool>,                              \
    AddButtonEvent,                              \
    (AsciiButton, DelegateFunc<bool> const &),   \
    (override)                                   \
  );                                             \
  MOCK_METHOD(                                   \
    (Delegate<AsciiButton, bool>),               \
    AddGenericButtonEvent,                       \
    ((DelegateFunc<AsciiButton, bool> const &)), \
    (override)                                   \
  );                                             \
  MOCK_METHOD(                                   \
    Delegate<ivec2>,                             \
    AddMousePositionEvent,                       \
    (DelegateFunc<ivec2> const &),               \
    (override)                                   \
  );                                             \
  MOCK_METHOD(                                   \
    (Delegate<ivec2>),                           \
    AddMouseDeltaEvent,                          \
    (DelegateFunc<ivec2> const &),               \
    (override)                                   \
  );                                             \
  MOCK_METHOD(                                   \
    (Delegate<int>),                             \
    AddMouseScrollEvent,                         \
    (DelegateFunc<int> const &),                 \
    (override)                                   \
  );                                             \
  MOCK_METHOD(                                   \
    (bool),                                      \
    GetButtonState,                              \
    (AsciiButton),                               \
    (const, override)                            \
  );                                             \
  MOCK_METHOD(                                   \
    ivec2,                                       \
    GetMousePosition,                            \
    (),                                          \
    (const, override)                            \
  );                                             \
}

#endif // ASCII_SYSTEMS_INPUT_INPUTMANAGER_H
