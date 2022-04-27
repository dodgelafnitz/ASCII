/*
 * Copywrite 2021 Dodge Lafnitzegger
 */

#include "Systems/Input/StateManager.h"
#include "gtest/gtest.h"

#include "gmock/gmock.h"

DEFINE_MockStateManager();

TEST(StateManagerTest, ConstructedStateManager_CheckStates_StatesAreUnset)
{
  StateManager const stateManager;

  for (int state = 0; state != int(AsciiState::Count); ++state) {
    EXPECT_FALSE(stateManager.GetStateValue(AsciiState(state)));
  }
}

TEST(StateManagerTest, AnyStateManager_SetState_StateIsSet)
{
  StateManager stateManager;

  stateManager.SetStateValue(AsciiState::Shift, true);

  EXPECT_TRUE(stateManager.GetStateValue(AsciiState::Shift));
}

TEST(StateManagerTest, StateDelegateOwned_SetStateToOther_DelegateTriggered)
{
  StateManager stateManager;

  bool triggered = false;
  Delegate<bool> onCaps = stateManager.AddStateEvent(AsciiState::CapsLock, [&](bool) {
    triggered = true;
  });

  stateManager.SetStateValue(AsciiState::CapsLock, true);

  EXPECT_TRUE(triggered);
}

TEST(StateManagerTest, StateDelegateOwned_SetStateToSame_DelegateNotTriggered)
{
  StateManager stateManager;

  bool triggered = false;
  Delegate<bool> onCaps = stateManager.AddStateEvent(AsciiState::CapsLock, [&](bool) {
    triggered = true;
  });

  stateManager.SetStateValue(AsciiState::CapsLock, false);

  EXPECT_FALSE(triggered);
}

TEST(StateManagerTest, GenericDelegateOwned_SetStateToOther_DelegateTriggered)
{
  StateManager stateManager;

  bool triggered = false;
  Delegate<AsciiState, bool> onAny = stateManager.AddGenericStateEvent([&](AsciiState, bool) {
    triggered = true;
  });

  stateManager.SetStateValue(AsciiState::CapsLock, true);

  EXPECT_TRUE(triggered);
}

TEST(StateManagerTest, GenericDelegateOwned_SetStateToSame_DelegateNotTriggered)
{
  StateManager stateManager;

  bool triggered = false;
  Delegate<AsciiState, bool> onAny = stateManager.AddGenericStateEvent([&](AsciiState, bool) {
    triggered = true;
  });

  stateManager.SetStateValue(AsciiState::CapsLock, false);

  EXPECT_FALSE(triggered);
}
