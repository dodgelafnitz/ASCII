/*
 * Copywrite 2021 Dodge Lafnitzegger
 */

#ifndef ASCII_SYSTEMS_INPUT_BUTTONMANAGER_H
#define ASCII_SYSTEMS_INPUT_BUTTONMANAGER_H

#include <unordered_map>

#include "General/Delagate.h"
#include "Window/Window.h"

class IButtonManager {
public:
  virtual ~IButtonManager(void) {};

  virtual Delegate<bool> AddButtonEvent(
    AsciiButton button,
    DelegateFunc<bool> const & func
  ) = 0;

  virtual Delegate<AsciiButton, bool> AddGenericButtonEvent(
    DelegateFunc<AsciiButton, bool> const & func
  ) = 0;

  virtual bool GetButtonState(AsciiButton button) const = 0;
};

class ButtonManager : public IButtonManager {
public:
  virtual ~ButtonManager(void) {};

  virtual Delegate<bool> AddButtonEvent(
    AsciiButton button,
    DelegateFunc<bool> const & func
  ) override;

  virtual Delegate<AsciiButton, bool> AddGenericButtonEvent(
    DelegateFunc<AsciiButton, bool> const & func
  ) override;

  virtual bool GetButtonState(AsciiButton button) const override;
  void         SetButtonState(AsciiButton button, bool state);

private:
  std::unordered_map<AsciiButton, Delegator<bool>> m_buttonDelegators;
  Delegator<AsciiButton, bool>                     m_genericButtonDelegator;
  bool                                             m_buttonStates[int(AsciiButton::Count)] = { 0 };
};

#define DEFINE_MockButtonManager()                \
class MockButtonManager : public IButtonManager { \
  MOCK_METHOD(                                    \
    Delegate<bool>,                               \
    AddButtonEvent,                               \
    (AsciiButton, DelegateFunc<bool> const &),    \
    (override)                                    \
  );                                              \
  MOCK_METHOD(                                    \
    (Delegate<AsciiButton, bool>),                \
    AddGenericButtonEvent,                        \
    ((DelegateFunc<AsciiButton, bool> const &)),  \
    (override)                                    \
  );                                              \
  MOCK_METHOD(                                    \
    (bool),                                       \
    GetButtonState,                               \
    (AsciiButton),                                \
    (const, override)                             \
  );                                              \
}

#endif // ASCII_SYSTEMS_INPUT_BUTTONMANAGER_H
