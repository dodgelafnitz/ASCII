/*
 * Copywrite 2021 Dodge Lafnitzegger
 */

#include "Systems/Input/MouseManager.h"
#include "gtest/gtest.h"
#include "gmock/gmock.h"

DEFINE_MockMouseManager();

TEST(MouseManagerTest, MousePositionDelegateOwned_ChangeMousePosition_DelegateTriggered) {
  MouseManager mouseManager;

  ivec2 mousePos;

  Delegate<ivec2> const mousePosDelegate =
    mouseManager.AddMousePositionEvent([&mousePos](ivec2 const & newPos) {
      mousePos = newPos;
    })
  ;

  mouseManager.SetMousePosition(ivec2(4, 10));

  EXPECT_EQ(mousePos, ivec2(4, 10));
}

TEST(MouseManagerTest, MouseDeltaDelegateOwned_ChangeMousePosition_DelegateTriggered) {
  MouseManager mouseManager;

  mouseManager.SetMousePosition(ivec2(7, 3));

  ivec2 mouseDelta;

  Delegate<ivec2> const mouseDeltaDelegate =
    mouseManager.AddMouseDeltaEvent([&mouseDelta](ivec2 const & delta) {
      mouseDelta = delta;
    })
  ;

  mouseManager.SetMousePosition(ivec2(6, 18));

  EXPECT_EQ(mouseDelta, ivec2(-1, 15));
}

TEST(MouseManagerTest, MouseScrollDelegateOwned_ChangeMouseScroll_DelegateTriggered) {
  MouseManager mouseManager;

  int mouseScroll;

  Delegate<int> const mouseScrollDelegate =
    mouseManager.AddMouseScrollEvent([&mouseScroll](int const & scroll) {
      mouseScroll = scroll;
    })
  ;

  mouseManager.ScrollMouse(-15);

  EXPECT_EQ(mouseScroll, -15);
}

TEST(MouseManagerTest, MouseMoved_GetPosition_PositionIsCorrect) {
  MouseManager mouseManager;

  mouseManager.SetMousePosition(ivec2(12, 67));

  EXPECT_EQ(mouseManager.GetMousePosition(), ivec2(12, 67));
}
