/*
 * Copywrite 2021 Dodge Lafnitzegger
 */

#include "Widget/Widget.h"
#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "Systems/Input/InputManager.h"
#include "Systems/WidgetManager.h"

DEFINE_MockInputManager();
DEFINE_MockWidget();
DEFINE_MockWidgetManager();

using ::testing::Return;

TEST(WidgetTest, DefaultConstructedDefaultWidget_CheckParent_ParentIsNull) {
  Widget const widget;

  EXPECT_TRUE(widget.GetParent().expired());
}

TEST(WidgetTest, DefaultWidgetWithNoParent_CheckWidgetManager_ManagerIsNull) {
  Widget const widget;

  EXPECT_TRUE(widget.GetWidgetManager().expired());
}

TEST(WidgetTest, DefaultWidgetWithNoParent_CheckInputManager_ManagerIsNull) {
  Widget const widget;

  EXPECT_TRUE(widget.GetInputManager().expired());
}

TEST(WidgetTest, DefaultWidgetWithParent_CheckWidgetManager_ManagerIsParentsManager) {
  auto const parentWidgetManager = std::make_shared<MockWidgetManager>();

  auto const parent = std::make_shared<MockWidget>();
  EXPECT_CALL(*parent, GetWidgetManager())
    .WillOnce(Return(parentWidgetManager))
  ;

  auto const widget = std::make_shared<Widget>();

  parent->AddChild(fvec2(), ivec2(), widget);

  std::weak_ptr<IWidgetManager> const widgetManager = widget->GetWidgetManager();

  ASSERT_FALSE(widgetManager.expired());
  EXPECT_EQ(widgetManager.lock(), parentWidgetManager);
}

TEST(WidgetTest, DefaultWidgetWithParent_CheckInputManager_ManagerIsParentsManager) {
  auto const parentInputManager = std::make_shared<MockInputManager>();

  auto parent = std::make_shared<MockWidget>();
  EXPECT_CALL(*parent, GetInputManager())
    .WillOnce(Return(parentInputManager))
  ;

  auto const widget = std::make_shared<Widget>();

  parent->AddChild(fvec2(), ivec2(), widget);

  std::weak_ptr<IInputManager> const inputtManager = widget->GetInputManager();

  ASSERT_FALSE(inputtManager.expired());
  EXPECT_EQ(inputtManager.lock(), parentInputManager);
}

TEST(WidgetTest, WidgetWithChildren_CheckChildren_ChildrenExist) {
  auto const widget = std::make_shared<Widget>();

  widget->AddChild<Widget>(fvec2(), ivec2());
  widget->AddChild<Widget>(fvec2(), ivec2());
  widget->AddChild<Widget>(fvec2(), ivec2());

  ASSERT_EQ(widget->GetChildCount(), 3);

  EXPECT_NE(widget->GetChild(0), nullptr);
  EXPECT_NE(widget->GetChild(1), nullptr);
  EXPECT_NE(widget->GetChild(2), nullptr);
}

TEST(WidgetTest, WidgetWithChildren_CheckParentOfChildren_ParentIsWidget) {
  auto const widget = std::make_shared<Widget>();

  widget->AddChild<Widget>(fvec2(), ivec2());

  ASSERT_EQ(widget->GetChildCount(), 1);

  std::shared_ptr<Widget> const child = widget->GetChild(0);

  ASSERT_FALSE(child->GetParent().expired());

  EXPECT_EQ(child->GetParent().lock(), widget);
}

TEST(WidgetTest, WidgetWithNoChildren_CheckChildrenCount_NoChildren) {
  auto const widget = std::make_shared<Widget>();

  EXPECT_EQ(widget->GetChildCount(), 0);
}

TEST(WidgetTest, WidgetWithChildren_CheckChildrenCount_CountIsCorrect) {
  auto const widget = std::make_shared<Widget>();

  widget->AddChild<Widget>(fvec2(), ivec2());
  widget->AddChild<Widget>(fvec2(), ivec2());
  widget->AddChild<Widget>(fvec2(), ivec2());
  widget->AddChild<Widget>(fvec2(), ivec2());
  widget->AddChild<Widget>(fvec2(), ivec2());

  EXPECT_EQ(widget->GetChildCount(), 5);
}

TEST(WidgetTest, WidgetWithChildren_CheckChildrenIndices_ChildrenIndicesAreCorrect) {
  auto const widget = std::make_shared<Widget>();

  auto const child0 = std::make_shared<Widget>();
  auto const child1 = std::make_shared<Widget>();
  auto const child2 = std::make_shared<Widget>();

  int const childIndex0 = widget->AddChild(fvec2(), ivec2(), child0);
  int const childIndex1 = widget->AddChild(fvec2(), ivec2(), child1);
  int const childIndex2 = widget->AddChild(fvec2(), ivec2(), child2);

  ASSERT_EQ(widget->GetChildCount(), 3);

  EXPECT_EQ(childIndex0, 0);
  EXPECT_EQ(childIndex1, 1);
  EXPECT_EQ(childIndex2, 2);

  EXPECT_EQ(widget->GetChildsIndex(child0), 0);
  EXPECT_EQ(widget->GetChildsIndex(child1), 1);
  EXPECT_EQ(widget->GetChildsIndex(child2), 2);
}

TEST(WidgetTest, WidgetWithChildren_CheckChildIndexFromChild_IndexIsCorrect) {
  auto const widget = std::make_shared<Widget>();

  auto const child0 = std::make_shared<Widget>();
  auto const child1 = std::make_shared<Widget>();

  int const childIndex0 = widget->AddChild(fvec2(), ivec2(), child0);
  int const childIndex1 = widget->AddChild(fvec2(), ivec2(), child1);

  ASSERT_EQ(widget->GetChildCount(), 2);

  EXPECT_EQ(childIndex0, 0);
  EXPECT_EQ(childIndex1, 1);

  EXPECT_EQ(child0->GetIndex(), 0);
  EXPECT_EQ(child1->GetIndex(), 1);
}

TEST(WidgetTest, WidgetWithChildren_CheckChildIndexFromNonChild_IndexIsInvalid) {
  auto const widget   = std::make_shared<Widget>();
  auto const nonChild = std::make_shared<Widget>();

  widget->AddChild<Widget>(fvec2(), ivec2());
  widget->AddChild<Widget>(fvec2(), ivec2());

  EXPECT_EQ(widget->GetChildsIndex(nonChild), Widget::InvalidIndex);
}

TEST(WidgetTest, WidgetWithChildren_CheckChildsScaledOffset_OffsetIsCorrect) {
  auto const  widget       = std::make_shared<Widget>();
  fvec2 const scaledOffset = fvec2(1.0f, 0.5f);

  widget->AddChild<Widget>(scaledOffset, ivec2());

  ASSERT_EQ(widget->GetChildCount(), 1);
  EXPECT_EQ(widget->GetChildScaledOffset(0), scaledOffset);
}

TEST(WidgetTest, WidgetWithChildren_SetChildsScaledOffset_OffsetIsCorrect) {
  auto const  widget       = std::make_shared<Widget>();
  fvec2 const scaledOffset = fvec2(1.0f, 0.5f);

  widget->AddChild<Widget>(fvec2(), ivec2());

  ASSERT_EQ(widget->GetChildCount(), 1);

  widget->SetChildScaledOffset(0, scaledOffset);

  EXPECT_EQ(widget->GetChildScaledOffset(0), scaledOffset);
}

TEST(WidgetTest, WidgetWithChildren_CheckChildsConstantOffset_OffsetIsCorrect) {
  auto const  widget         = std::make_shared<Widget>();
  ivec2 const constantOffset = ivec2(10, 20);

  widget->AddChild<Widget>(fvec2(), constantOffset);

  ASSERT_EQ(widget->GetChildCount(), 1);
  EXPECT_EQ(widget->GetChildConstantOffset(0), constantOffset);
}

TEST(WidgetTest, WidgetWithChildren_SetChildsConstantOffset_OffsetIsCorrect) {
  auto const  widget         = std::make_shared<Widget>();
  ivec2 const constantOffset = ivec2(10, 20);

  widget->AddChild<Widget>(fvec2(), ivec2());

  ASSERT_EQ(widget->GetChildCount(), 1);

  widget->SetChildConstantOffset(0, constantOffset);

  EXPECT_EQ(widget->GetChildConstantOffset(0), constantOffset);
}

TEST(WidgetTest, WidgetWithChildren_CheckChildsOffset_OffsetIsCorrect) {
  auto const widget = std::make_shared<MockWidget>();
  EXPECT_CALL(*widget, GetSize())
    .WillOnce(Return(ivec2(60, 60)))
  ;

  fvec2 const scaledOffset   = fvec2(0.75, -0.5f);
  ivec2 const constantOffset = ivec2(10, 20);

  widget->AddChild<Widget>(scaledOffset, constantOffset);

  EXPECT_EQ(widget->GetChildOffset(0), ivec2(55, -10));
}

TEST(WidgetTest, WidgetWithChildren_RemoveChild_ChildIsRemoved) {
  auto const widget = std::make_shared<Widget>();

  widget->AddChild<Widget>(fvec2(), ivec2());

  widget->RemoveChild(0);

  EXPECT_EQ(widget->GetChildCount(), 0);
}

TEST(WidgetTest, WidgetWithChildren_RemoveChild_ChildsParentIsNull) {
  auto const widget = std::make_shared<Widget>();
  auto const child  = std::make_shared<Widget>();

  widget->AddChild(fvec2(), ivec2(), child);

  widget->RemoveChild(0);

  EXPECT_EQ(widget->GetChildCount(), 0);
  EXPECT_TRUE(child->GetParent().expired());
}

TEST(WidgetTest, AnyWidgetAndWidgetWithParent_AddChild_ChildIsChildOfCorrectWidget) {
  auto const widget    = std::make_shared<Widget>();
  auto const oldParent = std::make_shared<Widget>();
  auto const child     = std::make_shared<Widget>();

  oldParent->AddChild(fvec2(), ivec2(), child);

  widget->AddChild(fvec2(), ivec2(), child);

  ASSERT_EQ(widget->GetChildCount(), 1);
  EXPECT_EQ(widget->GetChild(0), child);
  EXPECT_EQ(widget->GetChild(0)->GetParent().lock(), widget);
}

TEST(WidgetTest, AnyWidgetAndWidgetWithParent_AddChild_ChildIsNotChildOfOldParent) {
  auto const widget    = std::make_shared<Widget>();
  auto const oldParent = std::make_shared<Widget>();
  auto const child     = std::make_shared<Widget>();

  oldParent->AddChild(fvec2(), ivec2(), child);
  widget->AddChild(fvec2(), ivec2(), child);

  ASSERT_EQ(oldParent->GetChildCount(), 0);
}

TEST(WidgetTest, AnyWidgetAndWidgetWithParent_AddChild_ChildHasNewIndex) {
  auto const widget    = std::make_shared<Widget>();
  auto const oldParent = std::make_shared<Widget>();
  auto const child     = std::make_shared<Widget>();

  oldParent->AddChild<Widget>(fvec2(), ivec2());
  oldParent->AddChild<Widget>(fvec2(), ivec2());

  int const oldIndex = oldParent->AddChild(fvec2(), ivec2(), child);
  int const newIndex = widget->AddChild(fvec2(), ivec2(), child);

  EXPECT_NE(oldIndex, newIndex);
  EXPECT_EQ(newIndex, child->GetIndex());
}

TEST(WidgetTest, AnyWidget_AddChildByPointer_ChildIsChild) {
  auto const widget = std::make_shared<Widget>();
  auto const child  = std::make_shared<Widget>();

  widget->AddChild(fvec2(), ivec2(), child);

  ASSERT_EQ(widget->GetChildCount(), 1);
  EXPECT_EQ(widget->GetChild(0), child);
}

TEST(WidgetTest, AnyWidget_AddChildByPointer_ChildsParentIsCorrect) {
  auto const widget = std::make_shared<Widget>();
  auto const child  = std::make_shared<Widget>();

  widget->AddChild(fvec2(), ivec2(), child);

  EXPECT_EQ(child->GetParent().lock(), widget);
}

TEST(WidgetTest, AnyWidget_AddChildByPointer_ChildsIndexIsAtEndOfChildren) {
  auto const widget = std::make_shared<Widget>();
  auto const child  = std::make_shared<Widget>();

  widget->AddChild<Widget>(fvec2(), ivec2());
  widget->AddChild<Widget>(fvec2(), ivec2());
  widget->AddChild<Widget>(fvec2(), ivec2());
  widget->AddChild<Widget>(fvec2(), ivec2());

  widget->AddChild(fvec2(), ivec2(), child);

  EXPECT_EQ(child->GetIndex(), 4);
}

TEST(WidgetTest, AnyWidget_AddChildInPlace_ChildIsChild) {
  class UniqueWidget : public Widget {
  public:
    UniqueWidget(bool & trigger) :
      m_trigger(trigger)
    {}

    virtual bool HasChildModifiers(void) const override { m_trigger = true; return false; }

  private:
    bool & m_trigger;
  };

  bool triggered = false;

  auto const widget = std::make_shared<Widget>();

  widget->AddChild<UniqueWidget>(fvec2(), ivec2(), triggered);

  widget->GetChild(0)->HasChildModifiers();

  EXPECT_TRUE(triggered);
}

TEST(WidgetTest, AnyWidget_AddChildInPlace_ChildsParentIsCorrect) {
}

TEST(WidgetTest, AnyWidget_AddChildInPlace_ChildsIndexIsAtEndOfChildren) {
}

TEST(WidgetTest, AnyWidget_InsertChildByPointer_ChildIsChild) {
}

TEST(WidgetTest, AnyWidget_InsertChildByPointer_ChildsParentIsCorrect) {
}

TEST(WidgetTest, AnyWidget_InsertChildByPointer_ChildIsInCorrectLocation) {
}

TEST(WidgetTest, AnyWidget_InsertChildInPlace_ChildIsChild) {
}

TEST(WidgetTest, AnyWidget_InsertChildInPlace_ChildsParentIsCorrect) {
}

TEST(WidgetTest, AnyWidget_InsertChildInPlace_ChildIsInCorrectLocation) {
}

TEST(WidgetTest, AnyWidget_Draw_OnDrawIsCalled) {
}

TEST(WidgetTest, AnyWidget_DrawWithModiifiers_OnDrawIsCalledWithCorrectModifiers) {
}

TEST(WidgetTest, WidgetWithChildren_Draw_ChildrenAreDrawn) {
}

TEST(WidgetTest, WidgetWithChildren_Draw_ChildrenAreDrawnWithCorrectOffsets) {
}

TEST(WidgetTest, WidgetWithChildren_DrawWithChildrenDrawingToSameLocation_EarlierChildsCellIsDrawn) {
}

TEST(WidgetTest, WidgetWithChildren_DrawWithChildrenDrawingOverWidgetDrawnLocation_ChildsCellIsDrawn) {
}

TEST(WidgetTest, WidgetWithChildrenAndChildModifiers_Draw_ChildrenAreDrawnWithModifiers) {
}

TEST(WidgetTest, WidgetWithChildrenAndChildModifiers_DrawWithModifiers_ChildrenAreDrawnWithStackedModifiers) {
}

TEST(WidgetTest, WidgetWithNoChildren_Press_OnPressIsCalled) {
}

TEST(WidgetTest, WidgetWithChildren_PressNotOverChild_OnPressIsCalled) {
}

TEST(WidgetTest, WidgetWithChildren_PressHandledByChild_OnPressNotCalled) {
}

TEST(WidgetTest, AnyWidget_PressHandled_PressReturnsHandler) {
}

TEST(WidgetTest, AnyWidget_PressNotHandled_PressReturnsNull) {
}

TEST(WidgetTest, WidgetWithChildren_PressHandledByChild_PressReturnsHandler) {
}

TEST(WidgetTest, WidgetWithChildren_PressOverChildWidget_ChildPressCalled) {
}

TEST(WidgetTest, WidgetWithChildren_PressOverMultipleChildWidgetsWhereFirstHandles_FirstChildOnPressCalled) {
}

TEST(WidgetTest, WidgetWithChildren_PressOverMultipleChildWidgetsWhereFirstHandles_SecondChildOnPressNotCalled) {
}

TEST(WidgetTest, WidgetWithChildren_PressOverMultipleChildWidgetsWhereFirstDoesNotHandle_SecondChildOnPressCalled) {
}

TEST(WidgetTest, WidgetWithChildren_PressOverMultipleChildWidgetsWhereFirstDoesNotHandleButOccludes_SecondChildOnPressNotCalled) {
}

TEST(WidgetTest, WidgetWithNoChildren_Hold_OnHoldIsCalled) {
}

TEST(WidgetTest, WidgetWithChildren_HoldNotOverChild_OnHoldIsCalled) {
}

TEST(WidgetTest, WidgetWithChildren_HoldHandledByChild_OnHoldNotCalled) {
}

TEST(WidgetTest, AnyWidget_HoldHandled_HoldReturnsHandler) {
}

TEST(WidgetTest, AnyWidget_HoldNotHandled_HoldReturnsNull) {
}

TEST(WidgetTest, WidgetWithChildren_HoldHandledByChild_HoldReturnsHandler) {
}

TEST(WidgetTest, WidgetWithChildren_HoldOverChildWidget_ChildHoldCalled) {
}

TEST(WidgetTest, WidgetWithChildren_HoldOverMultipleChildWidgetsWhereFirstHandles_FirstChildOnHoldCalled) {
}

TEST(WidgetTest, WidgetWithChildren_HoldOverMultipleChildWidgetsWhereFirstHandles_SecondChildOnHoldNotCalled) {
}

TEST(WidgetTest, WidgetWithChildren_HoldOverMultipleChildWidgetsWhereFirstDoesNotHandle_SecondChildOnHoldCalled) {
}

TEST(WidgetTest, WidgetWithChildren_HoldOverMultipleChildWidgetsWhereFirstDoesNotHandleButOccludes_SecondChildOnHoldNotCalled) {
}

TEST(WidgetTest, WidgetWithNoChildren_Drag_OnDragIsCalled) {
}

TEST(WidgetTest, WidgetWithChildren_DragNotOverChild_OnDragIsCalled) {
}

TEST(WidgetTest, WidgetWithChildren_DragHandledByChild_OnDragNotCalled) {
}

TEST(WidgetTest, AnyWidget_DragHandled_DragReturnsHandler) {
}

TEST(WidgetTest, AnyWidget_DragNotHandled_DragReturnsNull) {
}

TEST(WidgetTest, WidgetWithChildren_DragHandledByChild_DragReturnsHandler) {
}

TEST(WidgetTest, WidgetWithChildren_DragOverChildWidget_ChildDragCalled) {
}

TEST(WidgetTest, WidgetWithChildren_DragOverMultipleChildWidgetsWhereFirstHandles_FirstChildOnDragCalled) {
}

TEST(WidgetTest, WidgetWithChildren_DragOverMultipleChildWidgetsWhereFirstHandles_SecondChildOnDragNotCalled) {
}

TEST(WidgetTest, WidgetWithChildren_DragOverMultipleChildWidgetsWhereFirstDoesNotHandle_SecondChildOnDragCalled) {
}

TEST(WidgetTest, WidgetWithChildren_DragOverMultipleChildWidgetsWhereFirstDoesNotHandleButOccludes_SecondChildOnDragNotCalled) {
}

TEST(WidgetTest, AnyWidget_HandlesTextEvent_OnTextCalled) {
}

TEST(WidgetTest, AnyWidget_HandlesTextEvent_HandlerReturned) {
}

TEST(WidgetTest, WidgetWithParent_HandlesTextEvent_ParentTextNotCalled) {
}

TEST(WidgetTest, WidgetWithNoParent_DoesNotHandleTextEvent_NullReturned) {
}

TEST(WidgetTest, WidgetWithParent_DoesNotHandleTextEvent_ParentTextCalled) {
}

TEST(WidgetTest, WidgetWithManager_TriesToGainFocus_ManagerSetsFocusToWidget) {
}

TEST(WidgetTest, WidgetWithManager_TriesToGainFocus_OnFocusCalled) {
}

TEST(WidgetTest, WidgetWithNoManager_TriesToGainFocus_DoesNothing) {
}

TEST(WidgetTest, WidgetWithManagerWithPreviousFocus_TriesToGainFocus_PreviousFocussedLosesFocus) {
}

TEST(WidgetTest, FocussedWidget_TriesToGainFocus_NothingHappens) {
}

TEST(WidgetTest, AnyWidget_GainsFocus_HasFocus) {
}

TEST(WidgetTest, AnyWidget_LosesFocus_DoesNotHasFocus) {
}

TEST(WidgetTest, AnyWidget_GainsFocus_OnGainedFocusCalled) {
}

TEST(WidgetTest, WidgetWithAncestors_GainsFocus_AncestorsOnDecendantGainedFocusCalled) {
}

TEST(WidgetTest, AnyWidget_LosesFocus_OnLoseFocusCalled) {
}

TEST(WidgetTest, WidgetWithAncestors_LosesFocus_AncestorsOnDecendantLoseFocusCalled) {
}

TEST(WidgetTest, WidgetWithNoChildren_MouseEnter_OnMouseEnterIsCalled) {
}

TEST(WidgetTest, WidgetWithChildren_MouseEnterNotOverChild_OnMouseEnterIsCalled) {
}

TEST(WidgetTest, WidgetWithChildren_MouseEnterHandledByChild_OnMouseEnterNotCalled) {
}

TEST(WidgetTest, AnyWidget_MouseEnterHandled_MouseEnterReturnsHandler) {
}

TEST(WidgetTest, AnyWidget_MouseEnterNotHandled_MouseEnterReturnsNull) {
}

TEST(WidgetTest, WidgetWithChildren_MouseEnterHandledByChild_MouseEnterReturnsHandler) {
}

TEST(WidgetTest, WidgetWithChildren_MouseEnterOverChildWidget_ChildMouseEnterCalled) {
}

TEST(WidgetTest, WidgetWithChildren_MouseEnterOverMultipleChildWidgetsWhereFirstHandles_FirstChildOnMouseEnterCalled) {
}

TEST(WidgetTest, WidgetWithChildren_MouseEnterOverMultipleChildWidgetsWhereFirstHandles_SecondChildOnMouseEnterNotCalled) {
}

TEST(WidgetTest, WidgetWithChildren_MouseEnterOverMultipleChildWidgetsWhereFirstDoesNotHandle_SecondChildOnMouseEnterCalled) {
}

TEST(WidgetTest, WidgetWithChildren_MouseEnterOverMultipleChildWidgetsWhereFirstDoesNotHandleButOccludes_SecondChildOnMouseEnterNotCalled) {
}

TEST(WidgetTest, WidgetWithNoChildren_MouseLeave_OnMouseLeaveIsCalled) {
}

TEST(WidgetTest, WidgetWithChildren_MouseLeaveNotOverChild_OnMouseLeaveIsCalled) {
}

TEST(WidgetTest, WidgetWithChildren_MouseLeaveHandledByChild_OnMouseLeaveNotCalled) {
}

TEST(WidgetTest, AnyWidget_MouseLeaveHandled_MouseLeaveReturnsHandler) {
}

TEST(WidgetTest, AnyWidget_MouseLeaveNotHandled_MouseLeaveReturnsNull) {
}

TEST(WidgetTest, WidgetWithChildren_MouseLeaveHandledByChild_MouseLeaveReturnsHandler) {
}

TEST(WidgetTest, WidgetWithChildren_MouseLeaveOverChildWidget_ChildMouseLeaveCalled) {
}

TEST(WidgetTest, WidgetWithChildren_MouseLeaveOverMultipleChildWidgetsWhereFirstHandles_FirstChildOnMouseLeaveCalled) {
}

TEST(WidgetTest, WidgetWithChildren_MouseLeaveOverMultipleChildWidgetsWhereFirstHandles_SecondChildOnMouseLeaveNotCalled) {
}

TEST(WidgetTest, WidgetWithChildren_MouseLeaveOverMultipleChildWidgetsWhereFirstDoesNotHandle_SecondChildOnMouseLeaveCalled) {
}

TEST(WidgetTest, WidgetWithChildren_MouseLeaveOverMultipleChildWidgetsWhereFirstDoesNotHandleButOccludes_SecondChildOnMouseLeaveNotCalled) {
}

TEST(WidgetTest, WidgetWithNoChildren_MouseMove_OnMouseMoveIsCalled) {
}

TEST(WidgetTest, WidgetWithChildren_MouseMoveNotOverChild_OnMouseMoveIsCalled) {
}

TEST(WidgetTest, WidgetWithChildren_MouseMoveHandledByChild_OnMouseMoveNotCalled) {
}

TEST(WidgetTest, AnyWidget_MouseMoveHandled_MouseMoveReturnsHandler) {
}

TEST(WidgetTest, AnyWidget_MouseMoveNotHandled_MouseMoveReturnsNull) {
}

TEST(WidgetTest, WidgetWithChildren_MouseMoveHandledByChild_MouseMoveReturnsHandler) {
}

TEST(WidgetTest, WidgetWithChildren_MouseMoveOverChildWidget_ChildMouseMoveCalled) {
}

TEST(WidgetTest, WidgetWithChildren_MouseMoveOverMultipleChildWidgetsWhereFirstHandles_FirstChildOnMouseMoveCalled) {
}

TEST(WidgetTest, WidgetWithChildren_MouseMoveOverMultipleChildWidgetsWhereFirstHandles_SecondChildOnMouseMoveNotCalled) {
}

TEST(WidgetTest, WidgetWithChildren_MouseMoveOverMultipleChildWidgetsWhereFirstDoesNotHandle_SecondChildOnMouseMoveCalled) {
}

TEST(WidgetTest, WidgetWithChildren_MouseMoveOverMultipleChildWidgetsWhereFirstDoesNotHandleButOccludes_SecondChildOnMouseMoveNotCalled) {
}

TEST(WidgetTest, WidgetWithNoChildren_MouseDown_OnMouseDownIsCalled) {
}

TEST(WidgetTest, WidgetWithChildren_MouseDownNotOverChild_OnMouseDownIsCalled) {
}

TEST(WidgetTest, WidgetWithChildren_MouseDownHandledByChild_OnMouseDownNotCalled) {
}

TEST(WidgetTest, AnyWidget_MouseDownHandled_MouseDownReturnsHandler) {
}

TEST(WidgetTest, AnyWidget_MouseDownNotHandled_MouseDownReturnsNull) {
}

TEST(WidgetTest, WidgetWithChildren_MouseDownHandledByChild_MouseDownReturnsHandler) {
}

TEST(WidgetTest, WidgetWithChildren_MouseDownOverChildWidget_ChildMouseDownCalled) {
}

TEST(WidgetTest, WidgetWithChildren_MouseDownOverMultipleChildWidgetsWhereFirstHandles_FirstChildOnMouseDownCalled) {
}

TEST(WidgetTest, WidgetWithChildren_MouseDownOverMultipleChildWidgetsWhereFirstHandles_SecondChildOnMouseDownNotCalled) {
}

TEST(WidgetTest, WidgetWithChildren_MouseDownOverMultipleChildWidgetsWhereFirstDoesNotHandle_SecondChildOnMouseDownCalled) {
}

TEST(WidgetTest, WidgetWithChildren_MouseDownOverMultipleChildWidgetsWhereFirstDoesNotHandleButOccludes_SecondChildOnMouseDownNotCalled) {
}

TEST(WidgetTest, WidgetWithNoChildren_MouseUp_OnMouseUpIsCalled) {
}

TEST(WidgetTest, WidgetWithChildren_MouseUpNotOverChild_OnMouseUpIsCalled) {
}

TEST(WidgetTest, WidgetWithChildren_MouseUpHandledByChild_OnMouseUpNotCalled) {
}

TEST(WidgetTest, AnyWidget_MouseUpHandled_MouseUpReturnsHandler) {
}

TEST(WidgetTest, AnyWidget_MouseUpNotHandled_MouseUpReturnsNull) {
}

TEST(WidgetTest, WidgetWithChildren_MouseUpHandledByChild_MouseUpReturnsHandler) {
}

TEST(WidgetTest, WidgetWithChildren_MouseUpOverChildWidget_ChildMouseUpCalled) {
}

TEST(WidgetTest, WidgetWithChildren_MouseUpOverMultipleChildWidgetsWhereFirstHandles_FirstChildOnMouseUpCalled) {
}

TEST(WidgetTest, WidgetWithChildren_MouseUpOverMultipleChildWidgetsWhereFirstHandles_SecondChildOnMouseUpNotCalled) {
}

TEST(WidgetTest, WidgetWithChildren_MouseUpOverMultipleChildWidgetsWhereFirstDoesNotHandle_SecondChildOnMouseUpCalled) {
}

TEST(WidgetTest, WidgetWithChildren_MouseUpOverMultipleChildWidgetsWhereFirstDoesNotHandleButOccludes_SecondChildOnMouseUpNotCalled) {
}

TEST(WidgetTest, WidgetWithNoChildren_MouseHover_OnMouseHoverIsCalled) {
}

TEST(WidgetTest, WidgetWithChildren_MouseHoverNotOverChild_OnMouseHoverIsCalled) {
}

TEST(WidgetTest, WidgetWithChildren_MouseHoverHandledByChild_OnMouseHoverNotCalled) {
}

TEST(WidgetTest, AnyWidget_MouseHoverHandled_MouseHoverReturnsHandler) {
}

TEST(WidgetTest, AnyWidget_MouseHoverNotHandled_MouseHoverReturnsNull) {
}

TEST(WidgetTest, WidgetWithChildren_MouseHoverHandledByChild_MouseHoverReturnsHandler) {
}

TEST(WidgetTest, WidgetWithChildren_MouseHoverOverChildWidget_ChildMouseHoverCalled) {
}

TEST(WidgetTest, WidgetWithChildren_MouseHoverOverMultipleChildWidgetsWhereFirstHandles_FirstChildOnMouseHoverCalled) {
}

TEST(WidgetTest, WidgetWithChildren_MouseHoverOverMultipleChildWidgetsWhereFirstHandles_SecondChildOnMouseHoverNotCalled) {
}

TEST(WidgetTest, WidgetWithChildren_MouseHoverOverMultipleChildWidgetsWhereFirstDoesNotHandle_SecondChildOnMouseHoverCalled) {
}

TEST(WidgetTest, WidgetWithChildren_MouseHoverOverMultipleChildWidgetsWhereFirstDoesNotHandleButOccludes_SecondChildOnMouseHoverNotCalled) {
}

TEST(WidgetTest, WidgetWithNoChildren_MouseScroll_OnMouseScrollIsCalled) {
}

TEST(WidgetTest, WidgetWithChildren_MouseScrollNotOverChild_OnMouseScrollIsCalled) {
}

TEST(WidgetTest, WidgetWithChildren_MouseScrollHandledByChild_OnMouseScrollNotCalled) {
}

TEST(WidgetTest, AnyWidget_MouseScrollHandled_MouseScrollReturnsHandler) {
}

TEST(WidgetTest, AnyWidget_MouseScrollNotHandled_MouseScrollReturnsNull) {
}

TEST(WidgetTest, WidgetWithChildren_MouseScrollHandledByChild_MouseScrollReturnsHandler) {
}

TEST(WidgetTest, WidgetWithChildren_MouseScrollOverChildWidget_ChildMouseScrollCalled) {
}

TEST(WidgetTest, WidgetWithChildren_MouseScrollOverMultipleChildWidgetsWhereFirstHandles_FirstChildOnMouseScrollCalled) {
}

TEST(WidgetTest, WidgetWithChildren_MouseScrollOverMultipleChildWidgetsWhereFirstHandles_SecondChildOnMouseScrollNotCalled) {
}

TEST(WidgetTest, WidgetWithChildren_MouseScrollOverMultipleChildWidgetsWhereFirstDoesNotHandle_SecondChildOnMouseScrollCalled) {
}

TEST(WidgetTest, WidgetWithChildren_MouseScrollOverMultipleChildWidgetsWhereFirstDoesNotHandleButOccludes_SecondChildOnMouseScrollNotCalled) {
}

TEST(WidgetTest, NonResizableWidget_TryToSetSize_ReturnsFalseAndDoesNotSetSize) {
}

TEST(WidgetTest, ResizableWidget_TryToSetSize_CallsOnSetSize) {
}

TEST(WidgetTest, ResizableWidget_TriesAndSucceedsToSetSize_ReturnsTrue) {
}

TEST(WidgetTest, ResizableWidget_TriesAndFailsToSetSize_ReturnsFalse) {
}

TEST(WidgetTest, ResizableWidget_TriesToSetSizeToCurrentSize_DoesNothing) {
}

TEST(WidgetTest, ResizableWidget_TriesToSetSizeToCurrentSize_ReturnsTrue) {
}

TEST(WidgetTest, WidgetWithNoChildren_CheckControlledOrigin_OriginIs0) {
}

TEST(WidgetTest, WidgetWithNoChildren_CheckControlledSize_SizeIsSizeOfWidget) {
}

TEST(WidgetTest, WidgetWithContainedChildren_CheckControlledOrigin_OriginIs0) {
}

TEST(WidgetTest, WidgetWithContainedChildren_CheckControlledSize_SizeIsSizeOfWidget) {
}

TEST(WidgetTest, WidgetWithMaskedUncontainedChildren_CheckControlledOrigin_OriginIs0) {
}

TEST(WidgetTest, WidgetWithMaskedUncontainedChildren_CheckControlledSize_SizeIsSizeOfWidget) {
}

TEST(WidgetTest, WidgetWithUncontainedChildren_CheckControlledOrigin_OriginRelativeLeastOfAll) {
}

TEST(WidgetTest, WidgetWithUncontainedChildren_CheckControlledSize_SizeIsDiffBetwenMinAndMaxExtents) {
}
