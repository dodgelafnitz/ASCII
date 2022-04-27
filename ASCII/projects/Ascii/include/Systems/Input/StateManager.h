/*
 * Copywrite 2021 Dodge Lafnitzegger
 */

#ifndef ASCII_SYSTEMS_INPUT_STATEMANAGER_H
#define ASCII_SYSTEMS_INPUT_STATEMANAGER_H

#include "General/Delagate.h"
#include "Window/Window.h"

class IStateManager {
public:
  virtual ~IStateManager(void) {}

  virtual Delegate<bool> AddStateEvent(
    AsciiState state,
    DelegateFunc<bool> const & func
  ) = 0;

  virtual Delegate<AsciiState, bool> AddGenericStateEvent(
    DelegateFunc<AsciiState, bool> const & func
  ) = 0;

  virtual bool GetStateValue(AsciiState state) const = 0;
  virtual void SetStateValue(AsciiState state, bool value) = 0;
};

class StateManager : public IStateManager {
public:
  virtual ~StateManager(void) override {}

  virtual Delegate<bool> AddStateEvent(
    AsciiState state,
    DelegateFunc<bool> const & func
  ) override;

  virtual Delegate<AsciiState, bool> AddGenericStateEvent(
    DelegateFunc<AsciiState, bool> const & func
  ) override;

  virtual bool GetStateValue(AsciiState state) const override;
  virtual void SetStateValue(AsciiState state, bool value) override;

private:
  std::unordered_map<AsciiState, Delegator<bool>> m_stateDelegators;
  Delegator<AsciiState, bool>                     m_genericStateDelegator;
  bool                                            m_stateValues[int(AsciiState::Count)] = { 0 };
};

#define DEFINE_MockStateManager()               \
class MockStateManager : public IStateManager { \
public:                                         \
  MOCK_METHOD(                                  \
    Delegate<bool>,                             \
    AddStateEvent,                              \
    (AsciiState, DelegateFunc<bool> const &),   \
    (override)                                  \
  );                                            \
  MOCK_METHOD(                                  \
    (Delegate<AsciiState, bool>),               \
    AddGenericStateEvent,                       \
    ((DelegateFunc<AsciiState, bool> const &)), \
    (override)                                  \
  );                                            \
  MOCK_METHOD(                                  \
    bool,                                       \
    GetStateValue,                              \
    (AsciiState),                               \
    (const, override)                           \
  );                                            \
  MOCK_METHOD(                                  \
    void,                                       \
    SetStateValue,                              \
    (AsciiState, bool),                         \
    (override)                                  \
  );                                            \
}


#endif // ASCII_SYSTEMS_INPUT_STATEMANAGER_H
