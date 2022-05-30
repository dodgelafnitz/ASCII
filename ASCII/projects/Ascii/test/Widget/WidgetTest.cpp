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
using ::testing::_;

class WidgetTest : public ::testing::Test {
protected:
  struct DrawValue {};
  struct DrawModifiers {};

  class UniqueWidget : public Widget {
  public:
    UniqueWidget(void) = default;
    UniqueWidget(DrawValue, int value) :
      m_drawValue(true),
      m_value(value)
    {}

    UniqueWidget(DrawModifiers, DrawParams const & modifiers) :
      m_drawModifiers(true),
      m_modifiers(modifiers)
    {}

    virtual void OnDraw(DrawParams const & params) const override {
      if (m_drawValue) {
        params.SetCell(ivec2(), AsciiCell('0' + m_value, 0, 3));
      }
    };

    virtual bool HasChildModifiers(void) const override {
      return m_drawModifiers;
    }

    virtual DrawParams GetChildModifiers(void) const override {
      return m_modifiers;
    }

    virtual ivec2 GetSize(void) const override {
      return m_size;
    };

  private:
    bool const m_drawValue     = false;
    bool const m_drawModifiers = false;

    int const        m_value     = 0;
    ivec2 const      m_size      = ivec2(8, 8);
    DrawParams const m_modifiers;
  };
};
TEST_F(WidgetTest, DefaultConstructedDefaultWidget_CheckParent_ParentIsNull) {
  Widget const widget;

  EXPECT_TRUE(widget.GetParent().expired());
}

TEST_F(WidgetTest, DefaultWidgetWithNoParent_CheckWidgetManager_ManagerIsNull) {
  Widget const widget;

  EXPECT_TRUE(widget.GetWidgetManager().expired());
}

TEST_F(WidgetTest, DefaultWidgetWithNoParent_CheckInputManager_ManagerIsNull) {
  Widget const widget;

  EXPECT_TRUE(widget.GetInputManager().expired());
}

TEST_F(WidgetTest, DefaultWidgetWithParent_CheckWidgetManager_ManagerIsParentsManager) {
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

TEST_F(WidgetTest, DefaultWidgetWithParent_CheckInputManager_ManagerIsParentsManager) {
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

TEST_F(WidgetTest, WidgetWithChildren_CheckChildren_ChildrenExist) {
  auto const widget = std::make_shared<Widget>();

  widget->AddChild<Widget>(fvec2(), ivec2());
  widget->AddChild<Widget>(fvec2(), ivec2());
  widget->AddChild<Widget>(fvec2(), ivec2());

  ASSERT_EQ(widget->GetChildCount(), 3);

  EXPECT_NE(widget->GetChild(0), nullptr);
  EXPECT_NE(widget->GetChild(1), nullptr);
  EXPECT_NE(widget->GetChild(2), nullptr);
}

TEST_F(WidgetTest, WidgetWithChildren_CheckParentOfChildren_ParentIsWidget) {
  auto const widget = std::make_shared<Widget>();

  widget->AddChild<Widget>(fvec2(), ivec2());

  ASSERT_EQ(widget->GetChildCount(), 1);

  std::shared_ptr<Widget> const child = widget->GetChild(0);

  ASSERT_FALSE(child->GetParent().expired());

  EXPECT_EQ(child->GetParent().lock(), widget);
}

TEST_F(WidgetTest, WidgetWithNoChildren_CheckChildrenCount_NoChildren) {
  auto const widget = std::make_shared<Widget>();

  EXPECT_EQ(widget->GetChildCount(), 0);
}

TEST_F(WidgetTest, WidgetWithChildren_CheckChildrenCount_CountIsCorrect) {
  auto const widget = std::make_shared<Widget>();

  widget->AddChild<Widget>(fvec2(), ivec2());
  widget->AddChild<Widget>(fvec2(), ivec2());
  widget->AddChild<Widget>(fvec2(), ivec2());
  widget->AddChild<Widget>(fvec2(), ivec2());
  widget->AddChild<Widget>(fvec2(), ivec2());

  EXPECT_EQ(widget->GetChildCount(), 5);
}

TEST_F(WidgetTest, WidgetWithChildren_CheckChildrenIndices_ChildrenIndicesAreCorrect) {
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

TEST_F(WidgetTest, WidgetWithChildren_CheckChildIndexFromChild_IndexIsCorrect) {
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

TEST_F(WidgetTest, WidgetWithChildren_CheckChildIndexFromNonChild_IndexIsInvalid) {
  auto const widget   = std::make_shared<Widget>();
  auto const nonChild = std::make_shared<Widget>();

  widget->AddChild<Widget>(fvec2(), ivec2());
  widget->AddChild<Widget>(fvec2(), ivec2());

  EXPECT_EQ(widget->GetChildsIndex(nonChild), Widget::InvalidIndex);
}

TEST_F(WidgetTest, WidgetWithChildren_CheckChildsScaledOffset_OffsetIsCorrect) {
  auto const  widget       = std::make_shared<Widget>();
  fvec2 const scaledOffset = fvec2(1.0f, 0.5f);

  widget->AddChild<Widget>(scaledOffset, ivec2());

  ASSERT_EQ(widget->GetChildCount(), 1);
  EXPECT_EQ(widget->GetChildScaledOffset(0), scaledOffset);
}

TEST_F(WidgetTest, WidgetWithChildren_SetChildsScaledOffset_OffsetIsCorrect) {
  auto const  widget       = std::make_shared<Widget>();
  fvec2 const scaledOffset = fvec2(1.0f, 0.5f);

  widget->AddChild<Widget>(fvec2(), ivec2());

  ASSERT_EQ(widget->GetChildCount(), 1);

  widget->SetChildScaledOffset(0, scaledOffset);

  EXPECT_EQ(widget->GetChildScaledOffset(0), scaledOffset);
}

TEST_F(WidgetTest, WidgetWithChildren_CheckChildsConstantOffset_OffsetIsCorrect) {
  auto const  widget         = std::make_shared<Widget>();
  ivec2 const constantOffset = ivec2(10, 20);

  widget->AddChild<Widget>(fvec2(), constantOffset);

  ASSERT_EQ(widget->GetChildCount(), 1);
  EXPECT_EQ(widget->GetChildConstantOffset(0), constantOffset);
}

TEST_F(WidgetTest, WidgetWithChildren_SetChildsConstantOffset_OffsetIsCorrect) {
  auto const  widget         = std::make_shared<Widget>();
  ivec2 const constantOffset = ivec2(10, 20);

  widget->AddChild<Widget>(fvec2(), ivec2());

  ASSERT_EQ(widget->GetChildCount(), 1);

  widget->SetChildConstantOffset(0, constantOffset);

  EXPECT_EQ(widget->GetChildConstantOffset(0), constantOffset);
}

TEST_F(WidgetTest, WidgetWithChildren_CheckChildsOffset_OffsetIsCorrect) {
  auto const widget = std::make_shared<MockWidget>();
  EXPECT_CALL(*widget, GetSize())
    .WillOnce(Return(ivec2(60, 60)))
  ;

  fvec2 const scaledOffset   = fvec2(0.75, -0.5f);
  ivec2 const constantOffset = ivec2(10, 20);

  widget->AddChild<Widget>(scaledOffset, constantOffset);

  EXPECT_EQ(widget->GetChildOffset(0), ivec2(55, -10));
}

TEST_F(WidgetTest, WidgetWithChildren_RemoveChild_ChildIsRemoved) {
  auto const widget = std::make_shared<Widget>();

  widget->AddChild<Widget>(fvec2(), ivec2());

  widget->RemoveChild(0);

  EXPECT_EQ(widget->GetChildCount(), 0);
}

TEST_F(WidgetTest, WidgetWithChildren_RemoveChild_ChildsParentIsNull) {
  auto const widget = std::make_shared<Widget>();
  auto const child  = std::make_shared<Widget>();

  widget->AddChild(fvec2(), ivec2(), child);

  widget->RemoveChild(0);

  EXPECT_EQ(widget->GetChildCount(), 0);
  EXPECT_TRUE(child->GetParent().expired());
}

TEST_F(WidgetTest, AnyWidgetAndWidgetWithParent_AddChild_ChildIsChildOfCorrectWidget) {
  auto const widget    = std::make_shared<Widget>();
  auto const oldParent = std::make_shared<Widget>();
  auto const child     = std::make_shared<Widget>();

  oldParent->AddChild(fvec2(), ivec2(), child);

  widget->AddChild(fvec2(), ivec2(), child);

  ASSERT_EQ(widget->GetChildCount(), 1);
  EXPECT_EQ(widget->GetChild(0), child);
  EXPECT_EQ(widget->GetChild(0)->GetParent().lock(), widget);
}

TEST_F(WidgetTest, AnyWidgetAndWidgetWithParent_AddChild_ChildIsNotChildOfOldParent) {
  auto const widget    = std::make_shared<Widget>();
  auto const oldParent = std::make_shared<Widget>();
  auto const child     = std::make_shared<Widget>();

  oldParent->AddChild(fvec2(), ivec2(), child);
  widget->AddChild(fvec2(), ivec2(), child);

  ASSERT_EQ(oldParent->GetChildCount(), 0);
}

TEST_F(WidgetTest, AnyWidgetAndWidgetWithParent_AddChild_ChildHasNewIndex) {
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

TEST_F(WidgetTest, AnyWidgetAndWidgetWithParentAndSilblings_AddChild_OldSilbingsHaveCorrectIndices) {
  auto const widget    = std::make_shared<Widget>();
  auto const oldParent = std::make_shared<Widget>();
  auto const child     = std::make_shared<Widget>();

  oldParent->AddChild<Widget>(fvec2(), ivec2());
  oldParent->AddChild<Widget>(fvec2(), ivec2());

  oldParent->AddChild(fvec2(), ivec2(), child);

  oldParent->AddChild<Widget>(fvec2(), ivec2());
  oldParent->AddChild<Widget>(fvec2(), ivec2());

  widget->AddChild(fvec2(), ivec2(), child);

  ASSERT_EQ(oldParent->GetChildCount(), 4);
  EXPECT_EQ(oldParent->GetChild(0)->GetIndex(), 0);
  EXPECT_EQ(oldParent->GetChild(1)->GetIndex(), 1);
  EXPECT_EQ(oldParent->GetChild(2)->GetIndex(), 2);
  EXPECT_EQ(oldParent->GetChild(3)->GetIndex(), 3);
}

TEST_F(WidgetTest, AnyWidgetAndWidgetWithParent_InsertChild_ChildIsNotChildOfOldParent) {
  auto const widget    = std::make_shared<Widget>();
  auto const oldParent = std::make_shared<Widget>();
  auto const child     = std::make_shared<Widget>();

  oldParent->AddChild(fvec2(), ivec2(), child);

  widget->AddChild<Widget>(fvec2(), ivec2());
  widget->AddChild<Widget>(fvec2(), ivec2());
  widget->AddChild<Widget>(fvec2(), ivec2());

  ASSERT_EQ(widget->GetChildCount(), 3);

  widget->InsertChild(1, fvec2(), ivec2(), child);

  EXPECT_EQ(oldParent->GetChildCount(), 0);
}

TEST_F(WidgetTest, AnyWidgetAndWidgetWithParent_InsertChild_ChildHasNewIndex) {
  auto const widget    = std::make_shared<Widget>();
  auto const oldParent = std::make_shared<Widget>();
  auto const child     = std::make_shared<Widget>();

  oldParent->AddChild<Widget>(fvec2(), ivec2());
  oldParent->AddChild<Widget>(fvec2(), ivec2());

  widget->AddChild<Widget>(fvec2(), ivec2());
  widget->AddChild<Widget>(fvec2(), ivec2());

  ASSERT_EQ(widget->GetChildCount(), 2);

  int const oldIndex = oldParent->AddChild(fvec2(), ivec2(), child);
  widget->InsertChild(1, fvec2(), ivec2(), child);

  EXPECT_NE(oldIndex, 1);
  EXPECT_EQ(child->GetIndex(), 1);
}

TEST_F(WidgetTest, AnyWidgetAndWidgetWithParentAndSilblings_InsertChild_OldSilbingsHaveCorrectIndices) {
  auto const widget    = std::make_shared<Widget>();
  auto const oldParent = std::make_shared<Widget>();
  auto const child     = std::make_shared<Widget>();

  oldParent->AddChild<Widget>(fvec2(), ivec2());
  oldParent->AddChild<Widget>(fvec2(), ivec2());

  oldParent->AddChild(fvec2(), ivec2(), child);

  oldParent->AddChild<Widget>(fvec2(), ivec2());
  oldParent->AddChild<Widget>(fvec2(), ivec2());

  widget->AddChild<Widget>(fvec2(), ivec2());
  widget->AddChild<Widget>(fvec2(), ivec2());
  widget->AddChild<Widget>(fvec2(), ivec2());
  widget->AddChild<Widget>(fvec2(), ivec2());

  ASSERT_EQ(widget->GetChildCount(), 4);

  widget->InsertChild(2, fvec2(), ivec2(), child);

  ASSERT_EQ(oldParent->GetChildCount(), 4);
  EXPECT_EQ(oldParent->GetChild(0)->GetIndex(), 0);
  EXPECT_EQ(oldParent->GetChild(1)->GetIndex(), 1);
  EXPECT_EQ(oldParent->GetChild(2)->GetIndex(), 2);
  EXPECT_EQ(oldParent->GetChild(3)->GetIndex(), 3);
}

TEST_F(WidgetTest, AnyWidget_AddChildByPointer_ChildIsChild) {
  auto const widget = std::make_shared<Widget>();
  auto const child  = std::make_shared<Widget>();

  widget->AddChild(fvec2(), ivec2(), child);

  ASSERT_EQ(widget->GetChildCount(), 1);
  EXPECT_EQ(widget->GetChild(0), child);
}

TEST_F(WidgetTest, AnyWidget_AddChildByPointer_ChildsParentIsCorrect) {
  auto const widget = std::make_shared<Widget>();
  auto const child  = std::make_shared<Widget>();

  widget->AddChild(fvec2(), ivec2(), child);

  EXPECT_EQ(child->GetParent().lock(), widget);
}

TEST_F(WidgetTest, AnyWidget_AddChildByPointer_ChildsIndexIsAtEndOfChildren) {
  auto const widget = std::make_shared<Widget>();
  auto const child  = std::make_shared<Widget>();

  widget->AddChild<Widget>(fvec2(), ivec2());
  widget->AddChild<Widget>(fvec2(), ivec2());
  widget->AddChild<Widget>(fvec2(), ivec2());
  widget->AddChild<Widget>(fvec2(), ivec2());

  widget->AddChild(fvec2(), ivec2(), child);

  EXPECT_EQ(child->GetIndex(), 4);
}

TEST_F(WidgetTest, AnyWidget_AddChildInPlace_ChildIsChild) {
  auto const widget = std::make_shared<Widget>();

  widget->AddChild<UniqueWidget>(fvec2(), ivec2());

  EXPECT_TRUE(std::dynamic_pointer_cast<UniqueWidget>(widget->GetChild(0)));
}

TEST_F(WidgetTest, AnyWidget_AddChildInPlace_ChildsParentIsCorrect) {
  auto const widget = std::make_shared<Widget>();

  widget->AddChild<Widget>(fvec2(), ivec2());

  ASSERT_EQ(widget->GetChildCount(), 1);

  EXPECT_EQ(widget->GetChild(0)->GetParent().lock(), widget);
}

TEST_F(WidgetTest, AnyWidget_AddChildInPlace_ChildsIndexIsAtEndOfChildren) {
  auto const widget = std::make_shared<Widget>();

  widget->AddChild<Widget>(fvec2(), ivec2());
  widget->AddChild<Widget>(fvec2(), ivec2());
  widget->AddChild<Widget>(fvec2(), ivec2());
  widget->AddChild<Widget>(fvec2(), ivec2());

  int const index = widget->AddChild<Widget>(fvec2(), ivec2());

  ASSERT_EQ(widget->GetChildCount(), 5);

  EXPECT_EQ(index, widget->GetChild(index)->GetIndex());
  EXPECT_EQ(index, widget->GetChildCount() - 1);
}

TEST_F(WidgetTest, AnyWidget_InsertChildByPointer_ChildIsChild) {
  auto const widget = std::make_shared<Widget>();
  auto const child  = std::make_shared<Widget>();

  widget->AddChild<Widget>(fvec2(), ivec2());
  widget->AddChild<Widget>(fvec2(), ivec2());
  widget->InsertChild(1, fvec2(), ivec2(), child);

  ASSERT_EQ(widget->GetChildCount(), 3);
  EXPECT_EQ(widget->GetChild(1), child);
}

TEST_F(WidgetTest, AnyWidget_InsertChildByPointer_ChildsParentIsCorrect) {
  auto const widget = std::make_shared<Widget>();
  auto const child  = std::make_shared<Widget>();

  widget->InsertChild(0, fvec2(), ivec2(), child);

  EXPECT_EQ(child->GetParent().lock(), widget);
}

TEST_F(WidgetTest, AnyWidget_InsertChildByPointer_ChildIsInCorrectLocation) {
  auto const widget = std::make_shared<Widget>();
  auto const child  = std::make_shared<Widget>();

  widget->AddChild<Widget>(fvec2(), ivec2());
  widget->AddChild<Widget>(fvec2(), ivec2());
  widget->AddChild<Widget>(fvec2(), ivec2());

  ASSERT_EQ(widget->GetChildCount(), 3);

  widget->InsertChild(1, fvec2(), ivec2(), child);

  ASSERT_EQ(widget->GetChildCount(), 4);
  EXPECT_EQ(widget->GetChild(1), child);
}

TEST_F(WidgetTest, AnyWidget_InsertChildInPlace_ChildIsChild) {
  auto const widget = std::make_shared<Widget>();

  widget->AddChild<Widget>(fvec2(), ivec2());
  widget->AddChild<Widget>(fvec2(), ivec2());
  widget->AddChild<Widget>(fvec2(), ivec2());
  widget->AddChild<Widget>(fvec2(), ivec2());

  ASSERT_EQ(widget->GetChildCount(), 4);

  widget->InsertChild<UniqueWidget>(2, fvec2(), ivec2());

  ASSERT_EQ(widget->GetChildCount(), 5);
  EXPECT_TRUE(std::dynamic_pointer_cast<UniqueWidget>(widget->GetChild(2)));
}

TEST_F(WidgetTest, AnyWidget_InsertChildInPlace_ChildsParentIsCorrect) {
  auto const widget = std::make_shared<Widget>();

  widget->AddChild<Widget>(fvec2(), ivec2());
  widget->AddChild<Widget>(fvec2(), ivec2());

  ASSERT_EQ(widget->GetChildCount(), 2);

  widget->InsertChild<Widget>(1, fvec2(), ivec2());

  ASSERT_EQ(widget->GetChildCount(), 3);
  EXPECT_EQ(widget->GetChild(1)->GetParent().lock(), widget);
}

TEST_F(WidgetTest, AnyWidget_InsertChildInPlace_ChildIsInCorrectLocation) {
  auto const widget = std::make_shared<Widget>();

  widget->AddChild<Widget>(fvec2(), ivec2());
  widget->AddChild<Widget>(fvec2(), ivec2());
  widget->AddChild<Widget>(fvec2(), ivec2());
  widget->AddChild<Widget>(fvec2(), ivec2());

  ASSERT_EQ(widget->GetChildCount(), 4);

  widget->InsertChild<UniqueWidget>(2, fvec2(), ivec2());

  ASSERT_EQ(widget->GetChildCount(), 5);
  ASSERT_TRUE(std::dynamic_pointer_cast<UniqueWidget>(widget->GetChild(2)));
  EXPECT_EQ(std::dynamic_pointer_cast<UniqueWidget>(widget->GetChild(2))->GetIndex(), 2);
}

TEST_F(WidgetTest, WidgetWithChildren_InsertChild_ChildrenPreviouslyAtThatPointOrAfterHaveIncrementedIndices) {
  auto const widget   = std::make_shared<Widget>();
  auto const child0   = std::make_shared<Widget>();
  auto const child1   = std::make_shared<Widget>();
  auto const child2   = std::make_shared<Widget>();
  auto const child3   = std::make_shared<Widget>();
  auto const child4   = std::make_shared<Widget>();
  auto const inserted = std::make_shared<Widget>();

  widget->AddChild(fvec2(), ivec2(), child0);
  widget->AddChild(fvec2(), ivec2(), child1);
  widget->AddChild(fvec2(), ivec2(), child2);
  widget->AddChild(fvec2(), ivec2(), child3);
  widget->AddChild(fvec2(), ivec2(), child4);

  ASSERT_EQ(widget->GetChildCount(), 5);

  widget->InsertChild(2, fvec2(), ivec2(), inserted);

  ASSERT_EQ(widget->GetChildCount(), 6);

  EXPECT_EQ(widget->GetChildsIndex(child0), 0);
  EXPECT_EQ(widget->GetChildsIndex(child1), 1);
  EXPECT_EQ(widget->GetChildsIndex(child2), 3);
  EXPECT_EQ(widget->GetChildsIndex(child3), 4);
  EXPECT_EQ(widget->GetChildsIndex(child4), 5);
  EXPECT_EQ(widget->GetChildsIndex(inserted), 2);
}

TEST_F(WidgetTest, WidgetWithChildren_InsertChild_ChildrenPreviouslyAtThatPointOrAfterHaveIncrementedIndicesOnSelf) {
  auto const widget   = std::make_shared<Widget>();
  auto const child0   = std::make_shared<Widget>();
  auto const child1   = std::make_shared<Widget>();
  auto const child2   = std::make_shared<Widget>();
  auto const inserted = std::make_shared<Widget>();

  widget->AddChild(fvec2(), ivec2(), child0);
  widget->AddChild(fvec2(), ivec2(), child1);
  widget->AddChild(fvec2(), ivec2(), child2);

  ASSERT_EQ(widget->GetChildCount(), 3);

  widget->InsertChild(1, fvec2(), ivec2(), inserted);

  ASSERT_EQ(widget->GetChildCount(), 4);

  EXPECT_EQ(child0->GetIndex(), 0);
  EXPECT_EQ(child1->GetIndex(), 2);
  EXPECT_EQ(child2->GetIndex(), 3);
  EXPECT_EQ(inserted->GetIndex(), 1);
}

TEST_F(WidgetTest, AnyWidget_InsertChildAtSize_ChildIsAtEndOfChildren) {
  auto const widget = std::make_shared<Widget>();
  auto const child  = std::make_shared<Widget>();

  widget->AddChild<Widget>(fvec2(), ivec2());
  widget->AddChild<Widget>(fvec2(), ivec2());
  widget->AddChild<Widget>(fvec2(), ivec2());

  ASSERT_EQ(widget->GetChildCount(), 3);

  widget->InsertChild(3, fvec2(), ivec2(), child);

  EXPECT_EQ(child->GetIndex(), 3);
}

TEST_F(WidgetTest, AnyWidget_Draw_OnDrawIsCalled) {
  auto const widget = std::make_shared<MockWidget>();

  EXPECT_CALL(*widget, OnDraw(_));

  widget->Draw(DrawParams());
}

TEST_F(WidgetTest, AnyWidget_DrawWithModifiers_OnDrawIsCalledWithCorrectModifiers) {
  auto const widget = std::make_shared<MockWidget>();

  DrawParams const params(ivec2(1, 5), ivec2(-1, 6), ivec2(DrawParams::NoClamp, 10));

  EXPECT_CALL(*widget, OnDraw(params));

  widget->Draw(params);
}

TEST_F(WidgetTest, WidgetWithChildren_Draw_ChildrenAreDrawn) {
  auto const widget = std::make_shared<MockWidget>();
  auto const child0 = std::make_shared<MockWidget>();
  auto const child1 = std::make_shared<MockWidget>();

  DrawParams const params(ivec2(1, 5), ivec2(-1, 6), ivec2(DrawParams::NoClamp, 10));

  widget->AddChild(fvec2(), ivec2(), child0);
  widget->AddChild(fvec2(), ivec2(), child1);

  EXPECT_CALL(*widget, OnDraw(params));
  EXPECT_CALL(*child0, OnDraw(_));
  EXPECT_CALL(*child1, OnDraw(_));

  widget->Draw(params);
}

TEST_F(WidgetTest, WidgetWithChildren_Draw_ChildrenAreDrawnWithCorrectOffsets) {
  Grid<AsciiCell, 2> screen(ivec2(12, 12));

  auto const widget = std::make_shared<UniqueWidget>(DrawValue(), 0);

  DrawParams const params(ivec2(1, 5), ivec2(), ivec2(DrawParams::NoClamp, DrawParams::NoClamp), nullptr, &screen);

  widget->AddChild<UniqueWidget>(fvec2(0.5f, 0.0f), ivec2(0, 3), DrawValue(), 1);
  widget->AddChild<UniqueWidget>(fvec2(0.25f, 0.75f), ivec2(-1, -1), DrawValue(), 2);

  widget->GetChild(1)->AddChild<UniqueWidget>(fvec2(0.5f, -.25f), ivec2(5, -2), DrawValue(), 3);

  widget->Draw(params);

  EXPECT_EQ(screen[ivec2(1, 5)].character, '0');
  EXPECT_EQ(screen[ivec2(5, 8)].character, '1');
  EXPECT_EQ(screen[ivec2(2, 10)].character, '2');
  EXPECT_EQ(screen[ivec2(11, 6)].character, '3');
}

TEST_F(WidgetTest, WidgetWithChildren_DrawWithChildrenDrawingToSameLocation_EarlierChildsCellIsDrawn) {
  Grid<AsciiCell, 2> screen(ivec2(1, 1));

  auto const widget = std::make_shared<Widget>();
  widget->AddChild<UniqueWidget>(fvec2(), ivec2(), DrawValue(), 0);
  widget->AddChild<UniqueWidget>(fvec2(), ivec2(), DrawValue(), 1);

  DrawParams const params(ivec2(), ivec2(), ivec2(DrawParams::NoClamp, DrawParams::NoClamp), nullptr, &screen);
  widget->Draw(params);

  EXPECT_EQ(screen[ivec2()].character, '0');
}

TEST_F(WidgetTest, WidgetWithChildren_DrawWithChildrenDrawingOverWidgetDrawnLocation_ChildsCellIsDrawn) {
  Grid<AsciiCell, 2> screen(ivec2(1, 1));

  auto const widget = std::make_shared<UniqueWidget>(DrawValue(), 0);
  widget->AddChild<UniqueWidget>(fvec2(), ivec2(), DrawValue(), 1);

  DrawParams const params(ivec2(), ivec2(), ivec2(DrawParams::NoClamp, DrawParams::NoClamp), nullptr, &screen);
  widget->Draw(params);

  EXPECT_EQ(screen[ivec2()].character, '1');
}

TEST_F(WidgetTest, WidgetWithChildrenAndChildModifiers_Draw_ChildrenAreDrawnWithModifiers) {
  Grid<AsciiCell, 2> screen(ivec2(3, 1));

  DrawParams const modifiers = DrawParams(
    ivec2(), [](AsciiCell const & cell) {
      return AsciiCell(cell.character + ('A' - '0'), cell.foregroundColor, cell.backgroundColor);
    }
  );

  auto const widget = std::make_shared<UniqueWidget>(DrawModifiers(), modifiers);
  widget->AddChild<UniqueWidget>(fvec2(), ivec2(0, 0), DrawValue(), 0);
  widget->AddChild<UniqueWidget>(fvec2(), ivec2(1, 0), DrawValue(), 1);
  widget->AddChild<UniqueWidget>(fvec2(), ivec2(2, 0), DrawValue(), 2);

  DrawParams const params(ivec2(), ivec2(), ivec2(DrawParams::NoClamp, DrawParams::NoClamp), nullptr, &screen);
  widget->Draw(params);

  EXPECT_EQ(screen[ivec2(0, 0)].character, 'A');
  EXPECT_EQ(screen[ivec2(1, 0)].character, 'B');
  EXPECT_EQ(screen[ivec2(2, 0)].character, 'C');
}

TEST_F(WidgetTest, WidgetWithChildrenAndChildModifiers_DrawWithModifiers_ChildrenAreDrawnWithStackedModifiers) {
}

TEST_F(WidgetTest, WidgetWithNoChildren_Press_OnPressIsCalled) {
}

TEST_F(WidgetTest, WidgetWithChildren_PressNotOverChild_OnPressIsCalled) {
}

TEST_F(WidgetTest, WidgetWithChildren_PressHandledByChild_OnPressNotCalled) {
}

TEST_F(WidgetTest, AnyWidget_PressHandled_PressReturnsHandler) {
}

TEST_F(WidgetTest, AnyWidget_PressNotHandled_PressReturnsNull) {
}

TEST_F(WidgetTest, WidgetWithChildren_PressHandledByChild_PressReturnsHandler) {
}

TEST_F(WidgetTest, WidgetWithChildren_PressOverChildWidget_ChildPressCalled) {
}

TEST_F(WidgetTest, WidgetWithChildren_PressOverMultipleChildWidgetsWhereFirstHandles_FirstChildOnPressCalled) {
}

TEST_F(WidgetTest, WidgetWithChildren_PressOverMultipleChildWidgetsWhereFirstHandles_SecondChildOnPressNotCalled) {
}

TEST_F(WidgetTest, WidgetWithChildren_PressOverMultipleChildWidgetsWhereFirstDoesNotHandle_SecondChildOnPressCalled) {
}

TEST_F(WidgetTest, WidgetWithChildren_PressOverMultipleChildWidgetsWhereFirstDoesNotHandleButOccludes_SecondChildOnPressNotCalled) {
}

TEST_F(WidgetTest, WidgetWithNoChildren_Hold_OnHoldIsCalled) {
}

TEST_F(WidgetTest, WidgetWithChildren_HoldNotOverChild_OnHoldIsCalled) {
}

TEST_F(WidgetTest, WidgetWithChildren_HoldHandledByChild_OnHoldNotCalled) {
}

TEST_F(WidgetTest, AnyWidget_HoldHandled_HoldReturnsHandler) {
}

TEST_F(WidgetTest, AnyWidget_HoldNotHandled_HoldReturnsNull) {
}

TEST_F(WidgetTest, WidgetWithChildren_HoldHandledByChild_HoldReturnsHandler) {
}

TEST_F(WidgetTest, WidgetWithChildren_HoldOverChildWidget_ChildHoldCalled) {
}

TEST_F(WidgetTest, WidgetWithChildren_HoldOverMultipleChildWidgetsWhereFirstHandles_FirstChildOnHoldCalled) {
}

TEST_F(WidgetTest, WidgetWithChildren_HoldOverMultipleChildWidgetsWhereFirstHandles_SecondChildOnHoldNotCalled) {
}

TEST_F(WidgetTest, WidgetWithChildren_HoldOverMultipleChildWidgetsWhereFirstDoesNotHandle_SecondChildOnHoldCalled) {
}

TEST_F(WidgetTest, WidgetWithChildren_HoldOverMultipleChildWidgetsWhereFirstDoesNotHandleButOccludes_SecondChildOnHoldNotCalled) {
}

TEST_F(WidgetTest, WidgetWithNoChildren_Drag_OnDragIsCalled) {
}

TEST_F(WidgetTest, WidgetWithChildren_DragNotOverChild_OnDragIsCalled) {
}

TEST_F(WidgetTest, WidgetWithChildren_DragHandledByChild_OnDragNotCalled) {
}

TEST_F(WidgetTest, AnyWidget_DragHandled_DragReturnsHandler) {
}

TEST_F(WidgetTest, AnyWidget_DragNotHandled_DragReturnsNull) {
}

TEST_F(WidgetTest, WidgetWithChildren_DragHandledByChild_DragReturnsHandler) {
}

TEST_F(WidgetTest, WidgetWithChildren_DragOverChildWidget_ChildDragCalled) {
}

TEST_F(WidgetTest, WidgetWithChildren_DragOverMultipleChildWidgetsWhereFirstHandles_FirstChildOnDragCalled) {
}

TEST_F(WidgetTest, WidgetWithChildren_DragOverMultipleChildWidgetsWhereFirstHandles_SecondChildOnDragNotCalled) {
}

TEST_F(WidgetTest, WidgetWithChildren_DragOverMultipleChildWidgetsWhereFirstDoesNotHandle_SecondChildOnDragCalled) {
}

TEST_F(WidgetTest, WidgetWithChildren_DragOverMultipleChildWidgetsWhereFirstDoesNotHandleButOccludes_SecondChildOnDragNotCalled) {
}

TEST_F(WidgetTest, AnyWidget_HandlesTextEvent_OnTextCalled) {
}

TEST_F(WidgetTest, AnyWidget_HandlesTextEvent_HandlerReturned) {
}

TEST_F(WidgetTest, WidgetWithParent_HandlesTextEvent_ParentTextNotCalled) {
}

TEST_F(WidgetTest, WidgetWithNoParent_DoesNotHandleTextEvent_NullReturned) {
}

TEST_F(WidgetTest, WidgetWithParent_DoesNotHandleTextEvent_ParentTextCalled) {
}

TEST_F(WidgetTest, WidgetWithManager_TriesToGainFocus_ManagerSetsFocusToWidget) {
}

TEST_F(WidgetTest, WidgetWithManager_TriesToGainFocus_OnFocusCalled) {
}

TEST_F(WidgetTest, WidgetWithNoManager_TriesToGainFocus_DoesNothing) {
}

TEST_F(WidgetTest, WidgetWithManagerWithPreviousFocus_TriesToGainFocus_PreviousFocussedLosesFocus) {
}

TEST_F(WidgetTest, FocussedWidget_TriesToGainFocus_NothingHappens) {
}

TEST_F(WidgetTest, AnyWidget_GainsFocus_HasFocus) {
}

TEST_F(WidgetTest, AnyWidget_LosesFocus_DoesNotHasFocus) {
}

TEST_F(WidgetTest, AnyWidget_GainsFocus_OnGainedFocusCalled) {
}

TEST_F(WidgetTest, WidgetWithAncestors_GainsFocus_AncestorsOnDecendantGainedFocusCalled) {
}

TEST_F(WidgetTest, AnyWidget_LosesFocus_OnLoseFocusCalled) {
}

TEST_F(WidgetTest, WidgetWithAncestors_LosesFocus_AncestorsOnDecendantLoseFocusCalled) {
}

TEST_F(WidgetTest, WidgetWithNoChildren_MouseEnter_OnMouseEnterIsCalled) {
}

TEST_F(WidgetTest, WidgetWithChildren_MouseEnterNotOverChild_OnMouseEnterIsCalled) {
}

TEST_F(WidgetTest, WidgetWithChildren_MouseEnterHandledByChild_OnMouseEnterNotCalled) {
}

TEST_F(WidgetTest, AnyWidget_MouseEnterHandled_MouseEnterReturnsHandler) {
}

TEST_F(WidgetTest, AnyWidget_MouseEnterNotHandled_MouseEnterReturnsNull) {
}

TEST_F(WidgetTest, WidgetWithChildren_MouseEnterHandledByChild_MouseEnterReturnsHandler) {
}

TEST_F(WidgetTest, WidgetWithChildren_MouseEnterOverChildWidget_ChildMouseEnterCalled) {
}

TEST_F(WidgetTest, WidgetWithChildren_MouseEnterOverMultipleChildWidgetsWhereFirstHandles_FirstChildOnMouseEnterCalled) {
}

TEST_F(WidgetTest, WidgetWithChildren_MouseEnterOverMultipleChildWidgetsWhereFirstHandles_SecondChildOnMouseEnterNotCalled) {
}

TEST_F(WidgetTest, WidgetWithChildren_MouseEnterOverMultipleChildWidgetsWhereFirstDoesNotHandle_SecondChildOnMouseEnterCalled) {
}

TEST_F(WidgetTest, WidgetWithChildren_MouseEnterOverMultipleChildWidgetsWhereFirstDoesNotHandleButOccludes_SecondChildOnMouseEnterNotCalled) {
}

TEST_F(WidgetTest, WidgetWithNoChildren_MouseLeave_OnMouseLeaveIsCalled) {
}

TEST_F(WidgetTest, WidgetWithChildren_MouseLeaveNotOverChild_OnMouseLeaveIsCalled) {
}

TEST_F(WidgetTest, WidgetWithChildren_MouseLeaveHandledByChild_OnMouseLeaveNotCalled) {
}

TEST_F(WidgetTest, AnyWidget_MouseLeaveHandled_MouseLeaveReturnsHandler) {
}

TEST_F(WidgetTest, AnyWidget_MouseLeaveNotHandled_MouseLeaveReturnsNull) {
}

TEST_F(WidgetTest, WidgetWithChildren_MouseLeaveHandledByChild_MouseLeaveReturnsHandler) {
}

TEST_F(WidgetTest, WidgetWithChildren_MouseLeaveOverChildWidget_ChildMouseLeaveCalled) {
}

TEST_F(WidgetTest, WidgetWithChildren_MouseLeaveOverMultipleChildWidgetsWhereFirstHandles_FirstChildOnMouseLeaveCalled) {
}

TEST_F(WidgetTest, WidgetWithChildren_MouseLeaveOverMultipleChildWidgetsWhereFirstHandles_SecondChildOnMouseLeaveNotCalled) {
}

TEST_F(WidgetTest, WidgetWithChildren_MouseLeaveOverMultipleChildWidgetsWhereFirstDoesNotHandle_SecondChildOnMouseLeaveCalled) {
}

TEST_F(WidgetTest, WidgetWithChildren_MouseLeaveOverMultipleChildWidgetsWhereFirstDoesNotHandleButOccludes_SecondChildOnMouseLeaveNotCalled) {
}

TEST_F(WidgetTest, WidgetWithNoChildren_MouseMove_OnMouseMoveIsCalled) {
}

TEST_F(WidgetTest, WidgetWithChildren_MouseMoveNotOverChild_OnMouseMoveIsCalled) {
}

TEST_F(WidgetTest, WidgetWithChildren_MouseMoveHandledByChild_OnMouseMoveNotCalled) {
}

TEST_F(WidgetTest, AnyWidget_MouseMoveHandled_MouseMoveReturnsHandler) {
}

TEST_F(WidgetTest, AnyWidget_MouseMoveNotHandled_MouseMoveReturnsNull) {
}

TEST_F(WidgetTest, WidgetWithChildren_MouseMoveHandledByChild_MouseMoveReturnsHandler) {
}

TEST_F(WidgetTest, WidgetWithChildren_MouseMoveOverChildWidget_ChildMouseMoveCalled) {
}

TEST_F(WidgetTest, WidgetWithChildren_MouseMoveOverMultipleChildWidgetsWhereFirstHandles_FirstChildOnMouseMoveCalled) {
}

TEST_F(WidgetTest, WidgetWithChildren_MouseMoveOverMultipleChildWidgetsWhereFirstHandles_SecondChildOnMouseMoveNotCalled) {
}

TEST_F(WidgetTest, WidgetWithChildren_MouseMoveOverMultipleChildWidgetsWhereFirstDoesNotHandle_SecondChildOnMouseMoveCalled) {
}

TEST_F(WidgetTest, WidgetWithChildren_MouseMoveOverMultipleChildWidgetsWhereFirstDoesNotHandleButOccludes_SecondChildOnMouseMoveNotCalled) {
}

TEST_F(WidgetTest, WidgetWithNoChildren_MouseDown_OnMouseDownIsCalled) {
}

TEST_F(WidgetTest, WidgetWithChildren_MouseDownNotOverChild_OnMouseDownIsCalled) {
}

TEST_F(WidgetTest, WidgetWithChildren_MouseDownHandledByChild_OnMouseDownNotCalled) {
}

TEST_F(WidgetTest, AnyWidget_MouseDownHandled_MouseDownReturnsHandler) {
}

TEST_F(WidgetTest, AnyWidget_MouseDownNotHandled_MouseDownReturnsNull) {
}

TEST_F(WidgetTest, WidgetWithChildren_MouseDownHandledByChild_MouseDownReturnsHandler) {
}

TEST_F(WidgetTest, WidgetWithChildren_MouseDownOverChildWidget_ChildMouseDownCalled) {
}

TEST_F(WidgetTest, WidgetWithChildren_MouseDownOverMultipleChildWidgetsWhereFirstHandles_FirstChildOnMouseDownCalled) {
}

TEST_F(WidgetTest, WidgetWithChildren_MouseDownOverMultipleChildWidgetsWhereFirstHandles_SecondChildOnMouseDownNotCalled) {
}

TEST_F(WidgetTest, WidgetWithChildren_MouseDownOverMultipleChildWidgetsWhereFirstDoesNotHandle_SecondChildOnMouseDownCalled) {
}

TEST_F(WidgetTest, WidgetWithChildren_MouseDownOverMultipleChildWidgetsWhereFirstDoesNotHandleButOccludes_SecondChildOnMouseDownNotCalled) {
}

TEST_F(WidgetTest, WidgetWithNoChildren_MouseUp_OnMouseUpIsCalled) {
}

TEST_F(WidgetTest, WidgetWithChildren_MouseUpNotOverChild_OnMouseUpIsCalled) {
}

TEST_F(WidgetTest, WidgetWithChildren_MouseUpHandledByChild_OnMouseUpNotCalled) {
}

TEST_F(WidgetTest, AnyWidget_MouseUpHandled_MouseUpReturnsHandler) {
}

TEST_F(WidgetTest, AnyWidget_MouseUpNotHandled_MouseUpReturnsNull) {
}

TEST_F(WidgetTest, WidgetWithChildren_MouseUpHandledByChild_MouseUpReturnsHandler) {
}

TEST_F(WidgetTest, WidgetWithChildren_MouseUpOverChildWidget_ChildMouseUpCalled) {
}

TEST_F(WidgetTest, WidgetWithChildren_MouseUpOverMultipleChildWidgetsWhereFirstHandles_FirstChildOnMouseUpCalled) {
}

TEST_F(WidgetTest, WidgetWithChildren_MouseUpOverMultipleChildWidgetsWhereFirstHandles_SecondChildOnMouseUpNotCalled) {
}

TEST_F(WidgetTest, WidgetWithChildren_MouseUpOverMultipleChildWidgetsWhereFirstDoesNotHandle_SecondChildOnMouseUpCalled) {
}

TEST_F(WidgetTest, WidgetWithChildren_MouseUpOverMultipleChildWidgetsWhereFirstDoesNotHandleButOccludes_SecondChildOnMouseUpNotCalled) {
}

TEST_F(WidgetTest, WidgetWithNoChildren_MouseHover_OnMouseHoverIsCalled) {
}

TEST_F(WidgetTest, WidgetWithChildren_MouseHoverNotOverChild_OnMouseHoverIsCalled) {
}

TEST_F(WidgetTest, WidgetWithChildren_MouseHoverHandledByChild_OnMouseHoverNotCalled) {
}

TEST_F(WidgetTest, AnyWidget_MouseHoverHandled_MouseHoverReturnsHandler) {
}

TEST_F(WidgetTest, AnyWidget_MouseHoverNotHandled_MouseHoverReturnsNull) {
}

TEST_F(WidgetTest, WidgetWithChildren_MouseHoverHandledByChild_MouseHoverReturnsHandler) {
}

TEST_F(WidgetTest, WidgetWithChildren_MouseHoverOverChildWidget_ChildMouseHoverCalled) {
}

TEST_F(WidgetTest, WidgetWithChildren_MouseHoverOverMultipleChildWidgetsWhereFirstHandles_FirstChildOnMouseHoverCalled) {
}

TEST_F(WidgetTest, WidgetWithChildren_MouseHoverOverMultipleChildWidgetsWhereFirstHandles_SecondChildOnMouseHoverNotCalled) {
}

TEST_F(WidgetTest, WidgetWithChildren_MouseHoverOverMultipleChildWidgetsWhereFirstDoesNotHandle_SecondChildOnMouseHoverCalled) {
}

TEST_F(WidgetTest, WidgetWithChildren_MouseHoverOverMultipleChildWidgetsWhereFirstDoesNotHandleButOccludes_SecondChildOnMouseHoverNotCalled) {
}

TEST_F(WidgetTest, WidgetWithNoChildren_MouseScroll_OnMouseScrollIsCalled) {
}

TEST_F(WidgetTest, WidgetWithChildren_MouseScrollNotOverChild_OnMouseScrollIsCalled) {
}

TEST_F(WidgetTest, WidgetWithChildren_MouseScrollHandledByChild_OnMouseScrollNotCalled) {
}

TEST_F(WidgetTest, AnyWidget_MouseScrollHandled_MouseScrollReturnsHandler) {
}

TEST_F(WidgetTest, AnyWidget_MouseScrollNotHandled_MouseScrollReturnsNull) {
}

TEST_F(WidgetTest, WidgetWithChildren_MouseScrollHandledByChild_MouseScrollReturnsHandler) {
}

TEST_F(WidgetTest, WidgetWithChildren_MouseScrollOverChildWidget_ChildMouseScrollCalled) {
}

TEST_F(WidgetTest, WidgetWithChildren_MouseScrollOverMultipleChildWidgetsWhereFirstHandles_FirstChildOnMouseScrollCalled) {
}

TEST_F(WidgetTest, WidgetWithChildren_MouseScrollOverMultipleChildWidgetsWhereFirstHandles_SecondChildOnMouseScrollNotCalled) {
}

TEST_F(WidgetTest, WidgetWithChildren_MouseScrollOverMultipleChildWidgetsWhereFirstDoesNotHandle_SecondChildOnMouseScrollCalled) {
}

TEST_F(WidgetTest, WidgetWithChildren_MouseScrollOverMultipleChildWidgetsWhereFirstDoesNotHandleButOccludes_SecondChildOnMouseScrollNotCalled) {
}

TEST_F(WidgetTest, NonResizableWidget_TryToSetSize_ReturnsFalseAndDoesNotSetSize) {
}

TEST_F(WidgetTest, ResizableWidget_TryToSetSize_CallsOnSetSize) {
}

TEST_F(WidgetTest, ResizableWidget_TriesAndSucceedsToSetSize_ReturnsTrue) {
}

TEST_F(WidgetTest, ResizableWidget_TriesAndFailsToSetSize_ReturnsFalse) {
}

TEST_F(WidgetTest, ResizableWidget_TriesToSetSizeToCurrentSize_DoesNothing) {
}

TEST_F(WidgetTest, ResizableWidget_TriesToSetSizeToCurrentSize_ReturnsTrue) {
}

TEST_F(WidgetTest, WidgetWithNoChildren_CheckControlledOrigin_OriginIs0) {
}

TEST_F(WidgetTest, WidgetWithNoChildren_CheckControlledSize_SizeIsSizeOfWidget) {
}

TEST_F(WidgetTest, WidgetWithContainedChildren_CheckControlledOrigin_OriginIs0) {
}

TEST_F(WidgetTest, WidgetWithContainedChildren_CheckControlledSize_SizeIsSizeOfWidget) {
}

TEST_F(WidgetTest, WidgetWithMaskedUncontainedChildren_CheckControlledOrigin_OriginIs0) {
}

TEST_F(WidgetTest, WidgetWithMaskedUncontainedChildren_CheckControlledSize_SizeIsSizeOfWidget) {
}

TEST_F(WidgetTest, WidgetWithUncontainedChildren_CheckControlledOrigin_OriginRelativeLeastOfAll) {
}

TEST_F(WidgetTest, WidgetWithUncontainedChildren_CheckControlledSize_SizeIsDiffBetwenMinAndMaxExtents) {
}

TEST_F(WidgetTest, AnyWidget_SizeChanged_ControlledAreaIsCorrect) {
}

TEST_F(WidgetTest, WidgetWithParent_SizeChanged_AncestorsControlledAreasAreCorrect) {
}
