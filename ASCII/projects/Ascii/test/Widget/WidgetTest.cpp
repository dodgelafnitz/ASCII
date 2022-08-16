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

using ::testing::AtLeast;
using ::testing::Return;
using ::testing::_;

namespace {
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
    .WillRepeatedly(Return(ivec2(60, 60)))
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

TEST(WidgetTest, AnyWidgetAndWidgetWithParentAndSilblings_AddChild_OldSilbingsHaveCorrectIndices) {
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

TEST(WidgetTest, AnyWidgetAndWidgetWithParent_InsertChild_ChildIsNotChildOfOldParent) {
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

TEST(WidgetTest, AnyWidgetAndWidgetWithParent_InsertChild_ChildHasNewIndex) {
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

TEST(WidgetTest, AnyWidgetAndWidgetWithParentAndSilblings_InsertChild_OldSilbingsHaveCorrectIndices) {
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
  auto const widget = std::make_shared<Widget>();

  widget->AddChild<UniqueWidget>(fvec2(), ivec2());

  EXPECT_TRUE(std::dynamic_pointer_cast<UniqueWidget>(widget->GetChild(0)));
}

TEST(WidgetTest, AnyWidget_AddChildInPlace_ChildsParentIsCorrect) {
  auto const widget = std::make_shared<Widget>();

  widget->AddChild<Widget>(fvec2(), ivec2());

  ASSERT_EQ(widget->GetChildCount(), 1);

  EXPECT_EQ(widget->GetChild(0)->GetParent().lock(), widget);
}

TEST(WidgetTest, AnyWidget_AddChildInPlace_ChildsIndexIsAtEndOfChildren) {
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

TEST(WidgetTest, AnyWidget_InsertChildByPointer_ChildIsChild) {
  auto const widget = std::make_shared<Widget>();
  auto const child  = std::make_shared<Widget>();

  widget->AddChild<Widget>(fvec2(), ivec2());
  widget->AddChild<Widget>(fvec2(), ivec2());
  widget->InsertChild(1, fvec2(), ivec2(), child);

  ASSERT_EQ(widget->GetChildCount(), 3);
  EXPECT_EQ(widget->GetChild(1), child);
}

TEST(WidgetTest, AnyWidget_InsertChildByPointer_ChildsParentIsCorrect) {
  auto const widget = std::make_shared<Widget>();
  auto const child  = std::make_shared<Widget>();

  widget->InsertChild(0, fvec2(), ivec2(), child);

  EXPECT_EQ(child->GetParent().lock(), widget);
}

TEST(WidgetTest, AnyWidget_InsertChildByPointer_ChildIsInCorrectLocation) {
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

TEST(WidgetTest, AnyWidget_InsertChildInPlace_ChildIsChild) {
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

TEST(WidgetTest, AnyWidget_InsertChildInPlace_ChildsParentIsCorrect) {
  auto const widget = std::make_shared<Widget>();

  widget->AddChild<Widget>(fvec2(), ivec2());
  widget->AddChild<Widget>(fvec2(), ivec2());

  ASSERT_EQ(widget->GetChildCount(), 2);

  widget->InsertChild<Widget>(1, fvec2(), ivec2());

  ASSERT_EQ(widget->GetChildCount(), 3);
  EXPECT_EQ(widget->GetChild(1)->GetParent().lock(), widget);
}

TEST(WidgetTest, AnyWidget_InsertChildInPlace_ChildIsInCorrectLocation) {
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

TEST(WidgetTest, WidgetWithChildren_InsertChild_ChildrenPreviouslyAtThatPointOrAfterHaveIncrementedIndices) {
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

TEST(WidgetTest, WidgetWithChildren_InsertChild_ChildrenPreviouslyAtThatPointOrAfterHaveIncrementedIndicesOnSelf) {
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

TEST(WidgetTest, AnyWidget_InsertChildAtSize_ChildIsAtEndOfChildren) {
  auto const widget = std::make_shared<Widget>();
  auto const child  = std::make_shared<Widget>();

  widget->AddChild<Widget>(fvec2(), ivec2());
  widget->AddChild<Widget>(fvec2(), ivec2());
  widget->AddChild<Widget>(fvec2(), ivec2());

  ASSERT_EQ(widget->GetChildCount(), 3);

  widget->InsertChild(3, fvec2(), ivec2(), child);

  EXPECT_EQ(child->GetIndex(), 3);
}

TEST(WidgetTest, AnyWidget_Draw_OnDrawIsCalled) {
  auto const widget = std::make_shared<MockWidget>();

  EXPECT_CALL(*widget, OnDraw(_));

  widget->Draw(DrawParams());
}

TEST(WidgetTest, AnyWidget_DrawWithModifiers_OnDrawIsCalledWithCorrectModifiers) {
  auto const widget = std::make_shared<MockWidget>();

  DrawParams const params(ivec2(1, 5), ivec2(-1, 6), ivec2(DrawParams::NoClamp, 10));

  EXPECT_CALL(*widget, OnDraw(params));

  widget->Draw(params);
}

TEST(WidgetTest, WidgetWithChildren_Draw_ChildrenAreDrawn) {
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

TEST(WidgetTest, WidgetWithChildren_Draw_ChildrenAreDrawnWithCorrectOffsets) {
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

TEST(WidgetTest, WidgetWithChildren_DrawWithChildrenDrawingToSameLocation_EarlierChildsCellIsDrawn) {
  Grid<AsciiCell, 2> screen(ivec2(1, 1));

  auto const widget = std::make_shared<Widget>();
  widget->AddChild<UniqueWidget>(fvec2(), ivec2(), DrawValue(), 0);
  widget->AddChild<UniqueWidget>(fvec2(), ivec2(), DrawValue(), 1);

  DrawParams const params(ivec2(), ivec2(), ivec2(DrawParams::NoClamp, DrawParams::NoClamp), nullptr, &screen);
  widget->Draw(params);

  EXPECT_EQ(screen[ivec2()].character, '0');
}

TEST(WidgetTest, WidgetWithChildren_DrawWithChildrenDrawingOverWidgetDrawnLocation_ChildsCellIsDrawn) {
  Grid<AsciiCell, 2> screen(ivec2(1, 1));

  auto const widget = std::make_shared<UniqueWidget>(DrawValue(), 0);
  widget->AddChild<UniqueWidget>(fvec2(), ivec2(), DrawValue(), 1);

  DrawParams const params(ivec2(), ivec2(), ivec2(DrawParams::NoClamp, DrawParams::NoClamp), nullptr, &screen);
  widget->Draw(params);

  EXPECT_EQ(screen[ivec2()].character, '1');
}

TEST(WidgetTest, WidgetWithChildrenAndChildModifiers_Draw_ChildrenAreDrawnWithModifiers) {
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

TEST(WidgetTest, WidgetWithChildrenAndChildModifiers_DrawWithModifiers_ChildrenAreDrawnWithStackedModifiers) {
  Grid<AsciiCell, 2> screen(ivec2(1, 1));

  DrawParams const modifiers0 = DrawParams(
    ivec2(), [](AsciiCell const & cell) {
      return AsciiCell(cell.character == '2' ? '3' : 'C', cell.foregroundColor, cell.backgroundColor);
    }
  );

  DrawParams const modifiers1 = DrawParams(
    ivec2(), [](AsciiCell const & cell) {
      return AsciiCell(cell.character == '1' ? '2' : 'B', cell.foregroundColor, cell.backgroundColor);
    }
  );

  DrawParams const modifiers2 = DrawParams(
    ivec2(), [](AsciiCell const & cell) {
      return AsciiCell(cell.character == '0' ? '1' : 'A', cell.foregroundColor, cell.backgroundColor);
    }
  );

  auto const diplayMapping = [](AsciiCell const & cell) {
    return AsciiCell(cell.character == '3' ? '!' : '?', cell.foregroundColor, cell.backgroundColor);
  };

  auto const widget = std::make_shared<UniqueWidget>(DrawModifiers(), modifiers0);
  auto const child0 = std::make_shared<UniqueWidget>(DrawModifiers(), modifiers1);
  auto const child1 = std::make_shared<UniqueWidget>(DrawModifiers(), modifiers2);
  auto const child2 = std::make_shared<UniqueWidget>(DrawValue(), 0);

  widget->AddChild(fvec2(), ivec2(), child0);
  child0->AddChild(fvec2(), ivec2(), child1);
  child1->AddChild(fvec2(), ivec2(), child2);

  DrawParams const params(ivec2(), ivec2(), ivec2(DrawParams::NoClamp, DrawParams::NoClamp), diplayMapping, &screen);
  widget->Draw(params);

  EXPECT_EQ(screen[ivec2(0, 0)].character, '!');
}

TEST(WidgetTest, AnyWidget_PressInWidget_OnPressIsCalled) {
  auto const widget = std::make_shared<MockWidget>();

  EXPECT_CALL(*widget, GetSize())
    .WillRepeatedly(Return(ivec2(1, 3)))
  ;

  ivec2       subposition = ivec2(0, 1);
  AsciiButton button      = AsciiButton::Grave;
  int         clickCount  = 4;

  EXPECT_CALL(*widget, OnPress(subposition, button, clickCount));

  widget->Press(subposition, button, clickCount);
}

TEST(WidgetTest, AnyWidget_PressWithinParentNotHandled_ParentOnPressIsCalled) {
  auto const widget = std::make_shared<MockWidget>();
  auto const parent = std::make_shared<MockWidget>();

  parent->AddChild(fvec2(), ivec2(2, 3), widget);

  EXPECT_CALL(*widget, GetSize())
    .WillRepeatedly(Return(ivec2(1, 3)))
  ;

  EXPECT_CALL(*parent, GetSize())
    .WillRepeatedly(Return(ivec2(6, 6)))
  ;

  ivec2       subposition = ivec2(0, 2);
  AsciiButton button      = AsciiButton::O;
  int         clickCount  = 2;

  EXPECT_CALL(*widget, OnPress(subposition, button, clickCount))
    .WillOnce(Return(false))
  ;

  EXPECT_CALL(*parent, OnPress(subposition + parent->GetChildOffset(widget->GetIndex()), button, clickCount));

  widget->Press(subposition, button, clickCount);
}

TEST(WidgetTest, AnyWidget_PressNotWithinParentNotHandled_ParentOnPressIsNotCalled) {
  auto const widget = std::make_shared<MockWidget>();
  auto const parent = std::make_shared<MockWidget>();

  parent->AddChild(fvec2(), ivec2(), widget);

  EXPECT_CALL(*widget, GetSize())
    .WillRepeatedly(Return(ivec2(5, 8)))
  ;

  EXPECT_CALL(*parent, GetSize())
    .WillRepeatedly(Return(ivec2(8, 4)))
  ;

  ivec2       subposition = ivec2(3, 7);
  AsciiButton button = AsciiButton::Y;
  int         clickCount = 1;

  EXPECT_CALL(*widget, OnPress(subposition, button, clickCount))
    .WillOnce(Return(false))
  ;

  EXPECT_CALL(*parent, OnPress(_, _, _))
    .Times(0)
  ;

  widget->Press(subposition, button, clickCount);
}

/*
TEST(WidgetTest, WidgetWithNoChildren_PressOutOfWidget_OnPressNotCalled) {
  auto const widget = std::make_shared<MockWidget>();

  EXPECT_CALL(*widget, GetSize())
    .WillRepeatedly(Return(ivec2(3, 4)))
  ;

  ivec2       subposition = ivec2(2, 6);
  AsciiButton button      = AsciiButton::Key7;
  int         clickCount  = 1;

  EXPECT_CALL(*widget, OnPress(_, _, _))
    .Times(0)
  ;

  widget->Press(subposition, button, clickCount);
}

TEST(WidgetTest, WidgetWithChildren_PressNotOverChildInWidget_OnPressIsCalled) {
  auto const widget = std::make_shared<MockWidget>();
  auto const child  = std::make_shared<MockWidget>();

  EXPECT_CALL(*widget, GetSize())
    .WillRepeatedly(Return(ivec2(4, 4)))
  ;

  EXPECT_CALL(*child, GetSize())
    .WillRepeatedly(Return(ivec2(2, 2)))
  ;

  widget->AddChild(fvec2(), ivec2(4, 0), child);

  ivec2       subposition = ivec2(1, 2);
  AsciiButton button      = AsciiButton::Mouse1;
  int         clickCount  = 1;

  EXPECT_CALL(*widget, OnPress(subposition, button, clickCount));

  widget->Press(subposition, button, clickCount);
}

TEST(WidgetTest, WidgetWithChildren_PressNotOverChildOutOfWidget_OnPressNotCalled) {
  auto const widget = std::make_shared<MockWidget>();
  auto const child  = std::make_shared<MockWidget>();

  EXPECT_CALL(*widget, GetSize())
    .WillRepeatedly(Return(ivec2(3, 5)))
  ;

  EXPECT_CALL(*child, GetSize())
    .WillRepeatedly(Return(ivec2(1, 1)))
  ;

  widget->AddChild(fvec2(), ivec2(8, 7), child);

  ivec2       subposition = ivec2(4, 4);
  AsciiButton button      = AsciiButton::Mouse3;
  int         clickCount  = 2;

  EXPECT_CALL(*widget, OnPress(_, _, _))
    .Times(0)
  ;

  widget->Press(subposition, button, clickCount);
}

TEST(WidgetTest, WidgetWithChildren_PressHandledByChild_OnPressNotCalled) {
  auto const widget = std::make_shared<MockWidget>();
  auto const child  = std::make_shared<MockWidget>();

  EXPECT_CALL(*widget, GetSize())
    .WillRepeatedly(Return(ivec2(6, 6)))
  ;

  EXPECT_CALL(*child, GetSize())
    .WillRepeatedly(Return(ivec2(6, 4)))
  ;

  widget->AddChild(fvec2(), ivec2(0, 2), child);

  ivec2       subposition = ivec2(1, 3);
  AsciiButton button      = AsciiButton::D;
  int         clickCount  = 2;

  EXPECT_CALL(*child, OnPress(ivec2(1, 1), button, clickCount))
    .WillOnce(Return(true))
  ;

  widget->Press(subposition, button, clickCount);
}

TEST(WidgetTest, AnyWidget_PressHandled_PressReturnsHandler) {
  auto const widget = std::make_shared<MockWidget>();

  EXPECT_CALL(*widget, GetSize())
    .WillRepeatedly(Return(ivec2(2, 2)))
  ;

  EXPECT_CALL(*widget, OnPress(_, _, _))
    .WillOnce(Return(true))
  ;

  std::shared_ptr<Widget> result = widget->Press(ivec2(), AsciiButton::D, 1);

  EXPECT_EQ(widget, result);
}

TEST(WidgetTest, AnyWidget_PressNotHandled_PressReturnsNull) {
  auto const widget = std::make_shared<MockWidget>();

  EXPECT_CALL(*widget, GetSize())
    .WillRepeatedly(Return(ivec2(1, 3)))
  ;

  EXPECT_CALL(*widget, OnPress(_, _, _))
    .WillOnce(Return(false))
  ;

  std::shared_ptr<Widget> result = widget->Press(ivec2(), AsciiButton::I, 8);

  EXPECT_EQ(result, nullptr);
}

TEST(WidgetTest, WidgetWithChildren_PressHandledByChild_PressReturnsHandler) {
  auto const widget = std::make_shared<MockWidget>();
  auto const child  = std::make_shared<MockWidget>();

  EXPECT_CALL(*widget, GetSize())
    .WillRepeatedly(Return(ivec2(3, 3)))
  ;

  EXPECT_CALL(*child, GetSize())
    .WillRepeatedly(Return(ivec2(2, 2)))
  ;

  EXPECT_CALL(*widget, OnPress(_, _, _))
    .Times(0)
  ;

  EXPECT_CALL(*child, OnPress(_, _, _))
    .WillOnce(Return(true))
  ;

  widget->AddChild(fvec2(), ivec2(), child);

  std::shared_ptr<Widget> result = widget->Press(ivec2(1, 1), AsciiButton::E, 1);

  EXPECT_EQ(result, child);
}

TEST(WidgetTest, WidgetWithChildren_PressOverChildWidget_ChildPressCalled) {
  auto const widget = std::make_shared<MockWidget>();
  auto const child  = std::make_shared<MockWidget>();

  EXPECT_CALL(*widget, GetSize())
    .WillRepeatedly(Return(ivec2(4, 4)))
  ;

  EXPECT_CALL(*child, GetSize())
    .WillRepeatedly(Return(ivec2(1, 5)))
  ;

  EXPECT_CALL(*widget, OnPress(_, _, _))
    .Times(0)
  ;

  EXPECT_CALL(*child, OnPress(_, _, _))
    .WillOnce(Return(true))
  ;

  widget->AddChild(fvec2(), ivec2(3, 2), child);

  std::shared_ptr<Widget> result = widget->Press(ivec2(3, 4), AsciiButton::N, 2);

  EXPECT_EQ(result, child);
}

TEST(WidgetTest, WidgetWithChildren_PressOverMultipleChildWidgetsWhereFirstHandles_CorrectChildOnPressCalled) {
  auto const widget = std::make_shared<MockWidget>();
  auto const child0 = std::make_shared<MockWidget>();
  auto const child1 = std::make_shared<MockWidget>();

  EXPECT_CALL(*widget, GetSize())
    .WillRepeatedly(Return(ivec2(4, 4)))
  ;

  EXPECT_CALL(*child0, GetSize())
    .WillRepeatedly(Return(ivec2(2, 2)))
  ;

  EXPECT_CALL(*child1, GetSize())
    .WillRepeatedly(Return(ivec2(3, 3)))
  ;

  widget->AddChild(fvec2(), ivec2(0, 0), child0);
  widget->AddChild(fvec2(), ivec2(1, 1), child1);

  EXPECT_CALL(*child0, OnPress(_, _, _))
    .WillOnce(Return(true))
  ;

  EXPECT_CALL(*child1, OnPress(_, _, _))
    .Times(0)
  ;

  widget->Press(ivec2(1, 1), AsciiButton::L, 1);
}

TEST(WidgetTest, WidgetWithChildren_PressOverMultipleChildWidgetsWhereFirstDoesNotHandle_SecondChildOnPressCalled) {
  auto const widget = std::make_shared<MockWidget>();
  auto const child0 = std::make_shared<MockWidget>();
  auto const child1 = std::make_shared<MockWidget>();

  EXPECT_CALL(*widget, GetSize())
    .WillRepeatedly(Return(ivec2(5, 2)))
  ;

  EXPECT_CALL(*child0, GetSize())
    .WillRepeatedly(Return(ivec2(3, 1)))
  ;

  EXPECT_CALL(*child1, GetSize())
    .WillRepeatedly(Return(ivec2(4, 1)))
  ;

  widget->AddChild(fvec2(), ivec2(1, 0), child0);
  widget->AddChild(fvec2(), ivec2(1, 0), child1);

  EXPECT_CALL(*child0, OnPress(_, _, _))
    .WillOnce(Return(false))
  ;

  EXPECT_CALL(*child1, OnPress(_, _, _))
    .WillOnce(Return(true))
  ;

  widget->Press(ivec2(2, 0), AsciiButton::R, 2);
}

TEST(WidgetTest, WidgetWithChildren_PressOverMultipleChildWidgetsWhereFirstDoesNotHandleButOccludes_SecondChildOnPressNotCalled) {
  auto const widget = std::make_shared<MockWidget>();
  auto const child0 = std::make_shared<MockWidget>();
  auto const child1 = std::make_shared<MockWidget>();

  EXPECT_CALL(*widget, GetSize())
    .WillRepeatedly(Return(ivec2(5, 2)))
  ;

  EXPECT_CALL(*child0, GetSize())
    .WillRepeatedly(Return(ivec2(3, 1)))
  ;

  EXPECT_CALL(*child1, GetSize())
    .WillRepeatedly(Return(ivec2(4, 1)))
  ;

  widget->AddChild(fvec2(), ivec2(1, 0), child0);
  widget->AddChild(fvec2(), ivec2(1, 0), child1);

  EXPECT_CALL(*child0, OnPress(_, _, _))
    .WillOnce(Return(false))
  ;

  EXPECT_CALL(*child0, DoesOcclude(ivec2(1, 0)))
    .WillRepeatedly(Return(true))
  ;

  EXPECT_CALL(*child1, OnPress(_, _, _))
    .Times(0)
  ;

  widget->Press(ivec2(2, 0), AsciiButton::R, 2);
}
*/

TEST(WidgetTest, WidgetWithNoParent_Hold_OnHoldIsCalled) {
  auto const widget = std::make_shared<MockWidget>();

  EXPECT_CALL(*widget, GetSize())
    .WillRepeatedly(Return(ivec2(4, 3)))
  ;

  ivec2       subposition = ivec2(1, 2);
  AsciiButton button      = AsciiButton::NumPadEnter;
  int         clickCount  = 1;

  EXPECT_CALL(*widget, OnHold(subposition, button, clickCount));

  widget->Hold(subposition, button, clickCount);
}

TEST(WidgetTest, WidgetWithNoParent_Drag_OnDragIsCalled) {
  auto const widget = std::make_shared<MockWidget>();

  EXPECT_CALL(*widget, GetSize())
    .WillRepeatedly(Return(ivec2(5, 1)))
  ;

  ivec2       subposition = ivec2(2, 0);
  AsciiButton button      = AsciiButton::Period;
  int         clickCount  = 4;
  ivec2       delta       = ivec2(-3, 1);

  EXPECT_CALL(*widget, OnDrag(subposition, button, clickCount, delta));

  widget->Drag(subposition, button, clickCount, delta);
}

TEST(WidgetTestFuture, MouseMoveTests) {
  FAIL();
}

TEST(WidgetTest, WidgetWithNoParent_MouseHover_OnMouseHoverIsCalled) {
  auto const widget = std::make_shared<MockWidget>();

  EXPECT_CALL(*widget, GetSize())
    .WillRepeatedly(Return(ivec2(4, 8)))
  ;

  ivec2 subposition = ivec2(2, 3);

  EXPECT_CALL(*widget, OnMouseHover(subposition));

  widget->MouseHover(subposition);
}

TEST(WidgetTest, WidgetWithNoParent_MouseScroll_OnMouseScrollIsCalled) {
  auto const widget = std::make_shared<MockWidget>();

  EXPECT_CALL(*widget, GetSize())
    .WillRepeatedly(Return(ivec2(3, 7)))
  ;

  ivec2 subposition = ivec2(1, 1);
  int   scroll      = 3;

  EXPECT_CALL(*widget, OnMouseScroll(subposition, scroll));

  widget->MouseScroll(subposition, scroll);
}

TEST(WidgetTest, WidgetWithNoParent_ButtonDown_OnButtonDownIsCalled) {
  auto const widget = std::make_shared<MockWidget>();

  EXPECT_CALL(*widget, GetSize())
    .WillRepeatedly(Return(ivec2(4, 3)))
  ;

  ivec2       subposition = ivec2(1, 2);
  AsciiButton button      = AsciiButton::NumPadDivide;

  EXPECT_CALL(*widget, OnButtonDown(subposition, button));

  widget->ButtonDown(subposition, button);
}

TEST(WidgetTest, WidgetWithNoParent_ButtonUp_OnButtonUpIsCalled) {
  auto const widget = std::make_shared<MockWidget>();

  EXPECT_CALL(*widget, GetSize())
    .WillRepeatedly(Return(ivec2(12, 30)))
  ;

  ivec2       subposition = ivec2(3, 19);
  AsciiButton button      = AsciiButton::Z;

  EXPECT_CALL(*widget, OnButtonUp(subposition, button));

  widget->ButtonUp(subposition, button);
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

TEST(WidgetTest, AnyWidget_SizeChanged_ControlledAreaIsCorrect) {
}

TEST(WidgetTest, WidgetWithParent_SizeChanged_AncestorsControlledAreasAreCorrect) {
}
