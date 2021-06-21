/*
 * Copywrite 2021 Dodge Lafnitzegger
 */

#include "General/Delagate.h"
#include "gtest/gtest.h"

TEST(DelegatorTest, DelegatorWithOneDelegate_Triggered_DelegateFunctionTriggers) {
  Delegator<int> delegator;

  int val = 0;

  Delegate<int> delegate =
    delegator.AddDelegate([&val](int const & newVal) {
      val = newVal;
    })
  ;

  delegator.Trigger(1);

  EXPECT_EQ(val, 1);
}

TEST(DelegatorTest, DelegatorWithDestructedDelegate_Triggered_DelegateDoesNotTrigger) {
  Delegator<int> delegator;

  int val = 0;

  {
    Delegate<int> delegate =
      delegator.AddDelegate([&val](int const & newVal) {
      val = newVal;
      })
      ;
  }

  delegator.Trigger(1);

  EXPECT_EQ(val, 0);
}

TEST(DelegatorTest, MovedDelegate_Triggered_TriggeresExacltyOnce) {
  Delegator<> delegator;

  int val = 0;

  Delegate<> delegate0;

  {
    Delegate<> delegate1 =
      delegator.AddDelegate([&val]() {
        ++val;
      })
    ;

    delegate0 = std::move(delegate1);
  }

  delegator.Trigger();

  EXPECT_EQ(val, 1);
}

TEST(DelegatorTest, MultipleDelegates_Triggered_TriggerForEachDelegate) {
  Delegator<> delegator;

  int val = 0;

  auto DelFunc = [&val]() {
    ++val;
  };

  Delegate<> delegate0 = delegator.AddDelegate(DelFunc);
  Delegate<> delegate1 = delegator.AddDelegate(DelFunc);
  Delegate<> delegate2 = delegator.AddDelegate(DelFunc);
  Delegate<> delegate3 = delegator.AddDelegate(DelFunc);
  Delegate<> delegate4 = delegator.AddDelegate(DelFunc);

  delegator.Trigger();

  EXPECT_EQ(val, 5);
}

TEST(DelegatorTest, RemovedDelegator_DelegateCleared_DoesNotCrash) {
  Delegate<int> delegate;

  {
    Delegator<int> delegator;

    delegate = delegator.AddDelegate([](int const &) {});
  }

  ASSERT_NO_THROW(delegate.Clear());
}

TEST(DelegatorTest, NoDeligates_CheckIsEmpty_IsEmpty) {
  Delegator<> delegator;

  ASSERT_TRUE(delegator.IsEmpty());
}

TEST(DelegatorTest, MultipleDeligates_CheckIsEmpty_IsNotEmpty) {
  Delegator<> delegator;

  auto DelFunc = [](){};

  Delegate<> delegate0 = delegator.AddDelegate(DelFunc);
  Delegate<> delegate1 = delegator.AddDelegate(DelFunc);
  Delegate<> delegate2 = delegator.AddDelegate(DelFunc);
  Delegate<> delegate3 = delegator.AddDelegate(DelFunc);
  Delegate<> delegate4 = delegator.AddDelegate(DelFunc);

  ASSERT_FALSE(delegator.IsEmpty());
}