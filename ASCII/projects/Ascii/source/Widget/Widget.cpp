/*
 * Copywrite 2021 Dodge Lafnitzegger
 */

#include "Widget/Widget.h"

std::weak_ptr<IInputManager> Widget::GetInputManager(void) const {
  std::shared_ptr<Widget> parent = m_parent.lock();
  if (parent) {
    return parent->GetInputManager();
  }
  else {
    return std::weak_ptr<IInputManager>();
  }
}

std::weak_ptr<IWidgetManager> Widget::GetWidgetManager(void) const {
  std::shared_ptr<Widget> parent = m_parent.lock();
  if (parent) {
    return parent->GetWidgetManager();
  }
  else {
    return std::weak_ptr<IWidgetManager>();
  }
}

void Widget::Draw(Grid<AsciiCell, 2> & io_screen, DrawParams const & params) const {
}

std::shared_ptr<Widget> Widget::Press(ivec2 const & subposition, AsciiButton button, int clickCount) {
  return nullptr;
}

std::shared_ptr<Widget> Widget::Hold(ivec2 const & subposition, AsciiButton button, int clickCount) {
  return nullptr;
}

std::shared_ptr<Widget> Widget::Drag(ivec2 const & subposition, AsciiButton button, int clickCount, ivec2 const & delta) {
  return nullptr;
}

std::shared_ptr<Widget> Widget::Text(TextEvent const & textEvent) {
  return nullptr;
}

void Widget::GainFocus(void) {
}

std::shared_ptr<Widget> Widget::MouseEnter(ivec2 const & subposition, ivec2 const & delta) {
  return nullptr;
}

std::shared_ptr<Widget> Widget::MouseLeave(ivec2 const & subposition, ivec2 const & delta) {
  return nullptr;
}

std::shared_ptr<Widget> Widget::MouseMove(ivec2 const & subposition, ivec2 const & delta) {
  return nullptr;
}
std::shared_ptr<Widget> Widget::MouseDown(ivec2 const & subposition) {
  return nullptr;
}

std::shared_ptr<Widget> Widget::MouseUp(ivec2 const & subposition) {
  return nullptr;
}

std::shared_ptr<Widget> Widget::MouseHover(ivec2 const & subposition) {
  return nullptr;
}

std::shared_ptr<Widget> Widget::MouseScroll(ivec2 const & subposition, int scroll) {
  return nullptr;
}

bool Widget::TrySetSize(ivec2 const & size) {
  return false;
}

ivec2 Widget::GetControlledOrigin(void) const {
  return ivec2();
}

ivec2 Widget::GetControlledSize(void) const {
  return ivec2();
}

int Widget::GetChildCount(void) const {
  return m_children.size();
}

std::shared_ptr<Widget> Widget::GetChild(int index) const {
  if (index == InvalidIndex) {
    return nullptr;
  }
  else {
    return m_children[index].widget;
  }
}

int Widget::GetChildsIndex(std::shared_ptr<Widget> const & child) const {
  if (child->GetParent().lock() == shared_from_this()) {
    return child->GetIndex();
  }
  else {
    return InvalidIndex;
  }
}

fvec2 Widget::GetChildScaledOffset(int index) const {
  return fvec2();
}

ivec2 Widget::GetChildConstantOffset(int index) const {
  return ivec2();
}

ivec2 Widget::GetChildOffset(int index) const {
  return ivec2();
}

std::weak_ptr<Widget> Widget::GetParent(void) const {
  return m_parent;
}

int Widget::GetIndex(void) const {
  return m_childIndex;
}

void Widget::RemoveChild(int index) {
  if (index == InvalidIndex) {
    return;
  }

  for (int i = index + 1; i < m_children.size(); ++i) {
    std::shared_ptr<Widget> & child = m_children[i].widget;

    --child->m_childIndex;
  }

  m_children.erase(m_children.begin() + index);
}

void Widget::SetChildScaledOffset(int index, fvec2 scaledOffset) {
}

void Widget::SetChildConstantOffset(int index, ivec2 constantOffset) {
}

int Widget::AddChild(fvec2 const & scaledOffset, ivec2 const & constantOffset, std::shared_ptr<Widget> const & widget) {
  if (std::shared_ptr<Widget> const oldParent = widget->GetParent().lock()) {
    if (oldParent == shared_from_this()) {
      return widget->GetIndex();
    }
    else {
      oldParent->RemoveChild(widget->GetIndex());
    }
  }

  int const index = m_children.size();

  m_children.emplace_back(widget, scaledOffset, constantOffset);
  widget->SetParent(weak_from_this(), index);

  return index;
}

void Widget::InsertChild(int childIndex, fvec2 const & scaledOffset, ivec2 const & constantOffset, std::shared_ptr<Widget> const & widget) {
}

void Widget::OnChildResize(int childId) {
}

void Widget::SetParent(std::weak_ptr<Widget> const & widget, int childIndex) {
  m_parent     = widget;
  m_childIndex = childIndex;
}
