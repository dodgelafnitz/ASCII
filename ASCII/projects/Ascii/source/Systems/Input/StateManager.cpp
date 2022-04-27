/*
 * Copywrite 2021 Dodge Lafnitzegger
 */

#include "Systems/Input/StateManager.h"

Delegate<bool> StateManager::AddStateEvent(
  AsciiState state,
  DelegateFunc<bool> const & func
) {
  return m_stateDelegators[state].AddDelegate(func);
}

Delegate<AsciiState, bool> StateManager::AddGenericStateEvent(
  DelegateFunc<AsciiState, bool> const & func
) {
  return m_genericStateDelegator.AddDelegate(func);
}

bool StateManager::GetStateValue(AsciiState state) const {
  return m_stateValues[int(state)];
}

void StateManager::SetStateValue(AsciiState state, bool value) {
  if (m_stateValues[int(state)] == value) {
    return;
  }

  m_stateValues[int(state)] = value;

  if (m_stateDelegators.count(state) != 0) {
    if (m_stateDelegators[state].IsEmpty()) {
      m_stateDelegators.erase(state);
    }
    else {
      m_stateDelegators[state].Trigger(value);
    }
  }

  m_genericStateDelegator.Trigger(state, value);
}
