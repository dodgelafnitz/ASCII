/*
 * Copywrite 2021 Dodge Lafnitzegger
 */

#include "Widget/Widget.h"

namespace {
  enum class WidgetActionType {
    Press,
    Hold,
    Drag,
    MouseEnter,
    MouseLeave,
    MouseMove,
    MouseDown,
    MouseUp,
    MouseHover,
    MouseScroll,

    Count
  };

  struct WidgetActionPayload {
    WidgetActionPayload(void) = default;

    WidgetActionPayload(WidgetActionType type) :
      type(type)
    {}

    WidgetActionPayload(WidgetActionType type, AsciiButton button, int clickCount) :
      type(type),
      button(button),
      integerData(clickCount)
    {}

    WidgetActionPayload(WidgetActionType type, AsciiButton button, int clickCount, ivec2 const & delta) :
      type(type),
      button(button),
      integerData(clickCount),
      delta(delta)
    {}

    WidgetActionPayload(WidgetActionType type, ivec2 const & delta) :
      type(type),
      delta(delta)
    {}

    WidgetActionPayload(WidgetActionType type, int scroll) :
      type(type),
      integerData(scroll)
    {}

    WidgetActionType  type        = WidgetActionType::Count;
    AsciiButton       button      = AsciiButton::Count;
    int               integerData = 0;
    ivec2             delta       = ivec2();
  };

  std::shared_ptr<Widget> WidgetActionHelper(std::shared_ptr<Widget> const & widget, ivec2 const & subposition, WidgetActionPayload const & payload) {
    if (!widget) {
      return nullptr;
    }

    for (int i = 0; i < widget->GetChildCount(); ++i) {
      std::shared_ptr<Widget> const child  = widget->GetChild(i);
      ivec2 const                   offset = widget->GetChildOffset(i) + child->GetControlledOrigin();

      if (subposition.x >= offset.x && subposition.y >= offset.y) {
        ivec2 const maxExtents       = offset + child->GetControlledSize();
        ivec2 const childSubposition = subposition - offset;

        if (subposition.x < maxExtents.x && subposition.y < maxExtents.y) {
          auto const handler = WidgetActionHelper(child, childSubposition, payload);

          if (handler) {
            return handler;
          }

          if (child->DoesOcclude(childSubposition)) {
            return nullptr;
          }
        }
      }
    }

    if (widget->GetSize().x > subposition.x && widget->GetSize().y > subposition.y) {
      bool handled = false;
      switch (payload.type) {
        case WidgetActionType::Press: {
          handled = widget->OnPress(subposition, payload.button, payload.integerData);
        } break;
        case WidgetActionType::Hold: {
          handled = widget->OnHold(subposition, payload.button, payload.integerData);
        } break;
        case WidgetActionType::Drag: {
          handled = widget->OnDrag(subposition, payload.button, payload.integerData, payload.delta);
        } break;
        case WidgetActionType::MouseEnter: {
          handled = widget->OnMouseEnter(subposition, payload.delta);
        } break;
        case WidgetActionType::MouseLeave: {
          handled = widget->OnMouseLeave(subposition, payload.delta);
        } break;
        case WidgetActionType::MouseMove: {
          handled = widget->OnMouseMove(subposition, payload.delta);
        } break;
        case WidgetActionType::MouseDown: {
          handled = widget->OnMouseDown(subposition);
        } break;
        case WidgetActionType::MouseUp: {
          handled = widget->OnMouseUp(subposition);
        } break;
        case WidgetActionType::MouseHover: {
          handled = widget->OnMouseHover(subposition);
        } break;
        case WidgetActionType::MouseScroll: {
          handled = widget->OnMouseScroll(subposition, payload.integerData);
        } break;
      }

      if (handled) {
        return widget;
      }
    }

    return nullptr;
  }
}

std::weak_ptr<IInputManager> Widget::GetInputManager(void) const {
  std::shared_ptr<Widget> const parent = m_parent.lock();
  if (parent) {
    return parent->GetInputManager();
  }
  else {
    return std::weak_ptr<IInputManager>();
  }
}

std::weak_ptr<IWidgetManager> Widget::GetWidgetManager(void) const {
  std::shared_ptr<Widget> const parent = m_parent.lock();
  if (parent) {
    return parent->GetWidgetManager();
  }
  else {
    return std::weak_ptr<IWidgetManager>();
  }
}

void Widget::Draw(DrawParams const & params) const {
  OnDraw(params);

  DrawParams childrenParams = params;

  if (HasChildModifiers()) {
    childrenParams = params.Constrain(GetChildModifiers());
  }

  for (int i = GetChildCount() - 1; i >= 0; --i) {
    std::shared_ptr<Widget> const & child = m_children[i].widget;

    ivec2 const offset           = GetChildOffset(i);
    DrawParams const childParams = childrenParams.Constrain(DrawParams(offset));

    child->Draw(childParams);
  }
}

std::shared_ptr<Widget> Widget::Press(ivec2 const & subposition, AsciiButton button, int clickCount) {
  return WidgetActionHelper(shared_from_this(), subposition, WidgetActionPayload(WidgetActionType::Press, button, clickCount));
}

std::shared_ptr<Widget> Widget::Hold(ivec2 const & subposition, AsciiButton button, int clickCount) {
  return WidgetActionHelper(shared_from_this(), subposition, WidgetActionPayload(WidgetActionType::Hold, button, clickCount));
}

std::shared_ptr<Widget> Widget::Drag(ivec2 const & subposition, AsciiButton button, int clickCount, ivec2 const & delta) {
  return WidgetActionHelper(shared_from_this(), subposition, WidgetActionPayload(WidgetActionType::Drag, button, clickCount, delta));
}

std::shared_ptr<Widget> Widget::Text(TextEvent const & textEvent) {
  return nullptr;
}

void Widget::GainFocus(void) {
}

std::shared_ptr<Widget> Widget::MouseEnter(ivec2 const & subposition, ivec2 const & delta) {
  return WidgetActionHelper(shared_from_this(), subposition, WidgetActionPayload(WidgetActionType::MouseEnter, delta));
}

std::shared_ptr<Widget> Widget::MouseLeave(ivec2 const & subposition, ivec2 const & delta) {
  return WidgetActionHelper(shared_from_this(), subposition, WidgetActionPayload(WidgetActionType::MouseLeave, delta));
}

std::shared_ptr<Widget> Widget::MouseMove(ivec2 const & subposition, ivec2 const & delta) {
  return WidgetActionHelper(shared_from_this(), subposition, WidgetActionPayload(WidgetActionType::MouseMove, delta));
}

std::shared_ptr<Widget> Widget::MouseDown(ivec2 const & subposition) {
  return WidgetActionHelper(shared_from_this(), subposition, WidgetActionPayload(WidgetActionType::MouseDown));
}

std::shared_ptr<Widget> Widget::MouseUp(ivec2 const & subposition) {
  return WidgetActionHelper(shared_from_this(), subposition, WidgetActionPayload(WidgetActionType::MouseUp));
}

std::shared_ptr<Widget> Widget::MouseHover(ivec2 const & subposition) {
  return WidgetActionHelper(shared_from_this(), subposition, WidgetActionPayload(WidgetActionType::MouseHover));
}

std::shared_ptr<Widget> Widget::MouseScroll(ivec2 const & subposition, int scroll) {
  return WidgetActionHelper(shared_from_this(), subposition, WidgetActionPayload(WidgetActionType::MouseScroll, scroll));
}

bool Widget::TrySetSize(ivec2 const & size) {
  return false;
}

ivec2 Widget::GetControlledOrigin(void) const {
  if (m_children.empty()) {
    return ivec2();
  }
  else {
    return m_controlledOffset;
  }
}

ivec2 Widget::GetControlledSize(void) const {
  if (m_children.empty()) {
    return GetSize();
  }
  else {
    return m_controlledSize;
  }
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
  return m_children[index].scaledOffset;
}

ivec2 Widget::GetChildConstantOffset(int index) const {
  return m_children[index].constantOffset;
}

ivec2 Widget::GetChildOffset(int index) const {
  ChildData const & child = m_children[index];
  return child.scaledOffset * GetSize() + child.constantOffset;
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
    std::shared_ptr<Widget> const & child = m_children[i].widget;

    --child->m_childIndex;
  }

  std::shared_ptr<Widget> const & child = m_children[index].widget;
  child->SetParent(std::weak_ptr<Widget>(), InvalidIndex);

  m_children.erase(m_children.begin() + index);
}

void Widget::SetChildScaledOffset(int index, fvec2 const & scaledOffset) {
  if (index == InvalidIndex) {
    return;
  }

  m_children[index].scaledOffset = scaledOffset;
}

void Widget::SetChildConstantOffset(int index, ivec2 const & constantOffset) {
  if (index == InvalidIndex) {
    return;
  }

  m_children[index].constantOffset = constantOffset;
}

int Widget::AddChild(fvec2 const & scaledOffset, ivec2 const & constantOffset, std::shared_ptr<Widget> const & widget) {
  InsertChild(GetChildCount(), scaledOffset, constantOffset, widget);

  if (widget->GetParent().lock().get() == this) {
    return widget->GetIndex();
  }
  else {
    return InvalidIndex;
  }
}

void Widget::InsertChild(int childIndex, fvec2 const & scaledOffset, ivec2 const & constantOffset, std::shared_ptr<Widget> const & widget) {
  if (childIndex > GetChildCount() || childIndex < 0) {
    return;
  }

  if (std::shared_ptr<Widget> const oldParent = widget->GetParent().lock()) {
    if (oldParent == shared_from_this()) {
      return;
    }
    else {
      oldParent->RemoveChild(widget->GetIndex());
    }
  }

  if (m_children.empty()) {
    m_controlledOffset = ivec2();
    m_controlledSize   = GetSize();
  }

  m_children.insert(m_children.begin() + childIndex, ChildData(widget, scaledOffset, constantOffset));

  for (int i = childIndex; i < GetChildCount(); ++i) {
    m_children[i].widget->SetParent(weak_from_this(), i);
  }

  bool const notifyParent = true;
  OnChildResize(childIndex, notifyParent);
}

void Widget::OnChildResize(int childId, bool notifyParent) {
  if (childId >= m_children.size()) {
    return;
  }

  std::shared_ptr<Widget> const child = GetChild(childId);

  AppendControlledArea(GetChildOffset(childId) + child->GetControlledOrigin(), child->GetControlledSize(), notifyParent);
}

void Widget::AppendControlledArea(ivec2 const & offset, ivec2 const & size, bool notifyParent) {
  ivec2 const newMin = offset;
  ivec2 const newMax = offset + size;

  ivec2 const oldMin = m_controlledOffset;
  ivec2 const oldMax = m_controlledOffset + m_controlledSize;

  ivec2 const min = oldMin.Min(newMin);
  ivec2 const max = oldMax.Max(newMax);

  if (min != oldMin || max != oldMax) {
    m_controlledOffset = min;
    m_controlledSize   = max - min;

    if (notifyParent) {
      if (std::shared_ptr<Widget> const parent = GetParent().lock()) {

        bool const notifyParent = true;
        parent->OnChildResize(GetIndex(), notifyParent);
      }
    }
  }
  else if (notifyParent) {
    ResetControlledArea();
  }
}

void Widget::ResetControlledArea(void) {
  ivec2 const oldOffset = m_controlledOffset;
  ivec2 const oldSize   = m_controlledSize;

  m_controlledOffset = ivec2();
  m_controlledSize   = GetSize();

  for (int i = 0; i < GetChildCount(); ++i) {
    bool const notifyParent = false;
    OnChildResize(i, notifyParent);
  }

  if (oldOffset != m_controlledOffset || oldSize != m_controlledSize) {
    if (std::shared_ptr<Widget> const parent = GetParent().lock()) {

      bool const notifyParent = true;
      parent->OnChildResize(GetIndex(), notifyParent);
    }
  }
}

void Widget::SetParent(std::weak_ptr<Widget> const & widget, int childIndex) {
  m_parent     = widget;
  m_childIndex = childIndex;
}
