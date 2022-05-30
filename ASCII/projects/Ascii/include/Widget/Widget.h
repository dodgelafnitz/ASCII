/*
 * Copywrite 2021 Dodge Lafnitzegger
 */

#ifndef ASCII_WIDGET_WIDGET_H
#define ASCII_WIDGET_WIDGET_H

#include <memory>

#include "Containers/Grid.h"
#include "Widget/DrawParams.h"
#include "Window/Window.h"
#include "Systems/Input/TextManager.h"
#include "Systems/Input/InputManager.h"

class IWidgetManager;

class Widget : public std::enable_shared_from_this<Widget> {
public:
  static int const InvalidIndex = -1;

  virtual ~Widget(void) = default;

  virtual std::weak_ptr<IInputManager>  GetInputManager(void) const;
  virtual std::weak_ptr<IWidgetManager> GetWidgetManager(void) const;

  virtual void       OnDraw(DrawParams const & params) const {}
  virtual bool       HasChildModifiers(void) const           { return false; }
  virtual DrawParams GetChildModifiers(void) const           { return DrawParams(); }

  // Focussing Actions
  virtual bool OnPress(ivec2 const & subposition, AsciiButton button, int clickCount)                     { return false; }
  virtual bool OnHold(ivec2 const & subposition, AsciiButton button, int clickCount)                      { return false; }
  virtual bool OnDrag(ivec2 const & subposition, AsciiButton button, int clickCount, ivec2 const & delta) { return false; }

  // Targetted Actions
  virtual bool OnText(TextEvent const & textEvent) { return false; }

  // Focus Functions
  virtual void OnGainedFocus(void)          {}
  virtual void OnLostFocus(void)            {}
  virtual void OnDecendantGainedFocus(void) {}
  virtual void OnDecendantLostFocus(void)   {}

  // Unfocussed Actions
  virtual bool OnMouseEnter(ivec2 const & subposition, ivec2 const & delta) { return false; }
  virtual bool OnMouseLeave(ivec2 const & subposition, ivec2 const & delta) { return false; }
  virtual bool OnMouseMove(ivec2 const & subposition, ivec2 const & delta)  { return false; }
  virtual bool OnMouseDown(ivec2 const & subposition)                       { return false; }
  virtual bool OnMouseUp(ivec2 const & subposition)                         { return false; }
  virtual bool OnMouseHover(ivec2 const & subposition)                      { return false; }
  virtual bool OnMouseScroll(ivec2 const & subposition, int scroll)         { return false; }

  virtual bool DoesOcclude(ivec2 const & subposition) const { return false; }
  virtual bool DoesMaskChildren(void) const                 { return false; }
  virtual bool IsFocusable(void) const                      { return false; }

  virtual ivec2 GetSize(void) const              { return ivec2(0, 0); }
  virtual bool  CanSetSize(void) const           { return false; }
  virtual bool  OnTrySetSize(ivec2 const & size) { return false; }

  void Draw(DrawParams const & params) const;

  // Focussing Actions
  std::shared_ptr<Widget> Press(ivec2 const & subposition, AsciiButton button, int clickCount);
  std::shared_ptr<Widget> Hold(ivec2 const & subposition, AsciiButton button, int clickCount);
  std::shared_ptr<Widget> Drag(ivec2 const & subposition, AsciiButton button, int clickCount, ivec2 const & delta);

  // Targetted Actions
  std::shared_ptr<Widget> Text(TextEvent const & textEvent);

  // Focus Functions
  void GainFocus(void);

  // Unfocussed Actions
  std::shared_ptr<Widget> MouseEnter(ivec2 const & subposition, ivec2 const & delta);
  std::shared_ptr<Widget> MouseLeave(ivec2 const & subposition, ivec2 const & delta);
  std::shared_ptr<Widget> MouseMove(ivec2 const & subposition, ivec2 const & delta);
  std::shared_ptr<Widget> MouseDown(ivec2 const & subposition);
  std::shared_ptr<Widget> MouseUp(ivec2 const & subposition);
  std::shared_ptr<Widget> MouseHover(ivec2 const & subposition);
  std::shared_ptr<Widget> MouseScroll(ivec2 const & subposition, int scroll);

  bool TrySetSize(ivec2 const & size);

  ivec2 GetControlledOrigin(void) const;
  ivec2 GetControlledSize(void) const;

  int                     GetChildCount(void) const;
  std::shared_ptr<Widget> GetChild(int index) const;
  int                     GetChildsIndex(std::shared_ptr<Widget> const & child) const;
  fvec2                   GetChildScaledOffset(int index) const;
  ivec2                   GetChildConstantOffset(int index) const;
  ivec2                   GetChildOffset(int index) const;

  ivec2                 GetPosition(void) const;
  std::weak_ptr<Widget> GetRoot(void) const;
  std::weak_ptr<Widget> GetParent(void) const;
  int                   GetIndex(void) const;

  void RemoveChild(int index);
  void SetChildScaledOffset(int index, fvec2 const & scaledOffset);
  void SetChildConstantOffset(int index, ivec2 const & constantOffset);

  int  AddChild(fvec2 const & scaledOffset, ivec2 const & constantOffset, std::shared_ptr<Widget> const & widget);
  void InsertChild(int childIndex, fvec2 const & scaledOffset, ivec2 const & constantOffset, std::shared_ptr<Widget> const & widget);

  template <typename T, typename ... Params>
  requires std::is_base_of_v<Widget, T>
    int AddChild(fvec2 const & scaledOffset, ivec2 const & constantOffset, Params && ... params) {
    return AddChild(scaledOffset, constantOffset, std::make_shared<T>(std::forward<Params &&>(params)...));
  }

  template <typename T, typename ... Params>
  requires std::is_base_of_v<Widget, T>
    void InsertChild(int childIndex, fvec2 const & scaledOffset, ivec2 const & constantOffset, Params && ... params) {
    InsertChild(childIndex, scaledOffset, constantOffset, std::make_shared<T>(std::forward<Params &&>(params)...));
  }

private:
  struct ChildData {
    ChildData(void) = default;
    ChildData(std::shared_ptr<Widget> const & widget, fvec2 const & scaledOffset, fvec2 const & constantOffset) :
      widget(widget),
      scaledOffset(scaledOffset),
      constantOffset(constantOffset)
    {}

    std::shared_ptr<Widget> widget;
    fvec2                   scaledOffset;
    ivec2                   constantOffset;
  };

  void OnChildResize(int childId);
  void SetParent(std::weak_ptr<Widget> const & widget, int childIndex);

  std::vector<ChildData> m_children;
  std::weak_ptr<Widget>  m_parent;
  int                    m_childIndex       = InvalidIndex;
  ivec2                  m_controlledOffset;
  ivec2                  m_controlledSize;
};


#define DEFINE_MockWidget()                           \
class MockWidget : public Widget {                    \
public:                                               \
  MOCK_METHOD(                                        \
    std::weak_ptr<IInputManager>,                     \
    GetInputManager,                                  \
    (),                                               \
    (const, override)                                 \
  );                                                  \
  MOCK_METHOD(                                        \
    std::weak_ptr<IWidgetManager>,                    \
    GetWidgetManager,                                 \
    (),                                               \
    (const, override)                                 \
  );                                                  \
  MOCK_METHOD(                                        \
    void,                                             \
    OnDraw,                                           \
    (DrawParams const &),                             \
    (const, override)                                 \
  );                                                  \
  MOCK_METHOD(                                        \
    bool,                                             \
    HasChildModifiers,                                \
    (),                                               \
    (const, override)                                 \
  );                                                  \
  MOCK_METHOD(                                        \
    DrawParams,                                       \
    GetChildModifiers,                                \
    (),                                               \
    (const, override)                                 \
  );                                                  \
  MOCK_METHOD(                                        \
    bool,                                             \
    OnPress,                                          \
    (ivec2 const &, AsciiButton, int),                \
    (override)                                        \
  );                                                  \
  MOCK_METHOD(                                        \
    bool,                                             \
    OnHold,                                           \
    (ivec2 const &, AsciiButton, int),                \
    (override)                                        \
  );                                                  \
  MOCK_METHOD(                                        \
    bool,                                             \
    OnDrag,                                           \
    (ivec2 const &, AsciiButton, int, ivec2 const &), \
    (override)                                        \
  );                                                  \
  MOCK_METHOD(                                        \
    bool,                                             \
    OnText,                                           \
    (TextEvent const &),                              \
    (override)                                        \
  );                                                  \
  MOCK_METHOD(                                        \
    void,                                             \
    OnGainedFocus,                                    \
    (),                                               \
    (override)                                        \
  );                                                  \
  MOCK_METHOD(                                        \
    void,                                             \
    OnLostFocus,                                      \
    (),                                               \
    (override)                                        \
  );                                                  \
  MOCK_METHOD(                                        \
    void,                                             \
    OnDecendantGainedFocus,                           \
    (),                                               \
    (override)                                        \
  );                                                  \
  MOCK_METHOD(                                        \
    void,                                             \
    OnDecendantLostFocus,                             \
    (),                                               \
    (override)                                        \
  );                                                  \
  MOCK_METHOD(                                        \
    bool,                                             \
    OnMouseEnter,                                     \
    (ivec2 const &, ivec2 const &),                   \
    (override)                                        \
  );                                                  \
  MOCK_METHOD(                                        \
    bool,                                             \
    OnMouseLeave,                                     \
    (ivec2 const &, ivec2 const &),                   \
    (override)                                        \
  );                                                  \
  MOCK_METHOD(                                        \
    bool,                                             \
    OnMouseMove,                                      \
    (ivec2 const &, ivec2 const &),                   \
    (override)                                        \
  );                                                  \
  MOCK_METHOD(                                        \
    bool,                                             \
    OnMouseDown,                                      \
    (ivec2 const &),                                  \
    (override)                                        \
  );                                                  \
  MOCK_METHOD(                                        \
    bool,                                             \
    OnMouseUp,                                        \
    (ivec2 const &),                                  \
    (override)                                        \
  );                                                  \
  MOCK_METHOD(                                        \
    bool,                                             \
    OnMouseHover,                                     \
    (ivec2 const &),                                  \
    (override)                                        \
  );                                                  \
  MOCK_METHOD(                                        \
    bool,                                             \
    OnMouseScroll,                                    \
    (ivec2 const &, int),                             \
    (override)                                        \
  );                                                  \
  MOCK_METHOD(                                        \
    bool,                                             \
    DoesOcclude,                                      \
    (ivec2 const &),                                  \
    (const, override)                                 \
  );                                                  \
  MOCK_METHOD(                                        \
    bool,                                             \
    DoesMaskChildren,                                 \
    (),                                               \
    (const, override)                                 \
  );                                                  \
  MOCK_METHOD(                                        \
    bool,                                             \
    IsFocusable,                                      \
    (),                                               \
    (const, override)                                 \
  );                                                  \
  MOCK_METHOD(                                        \
    ivec2,                                            \
    GetSize,                                          \
    (),                                               \
    (const, override)                                 \
  );                                                  \
  MOCK_METHOD(                                        \
    bool,                                             \
    CanSetSize,                                       \
    (),                                               \
    (const, override)                                 \
  );                                                  \
  MOCK_METHOD(                                        \
    bool,                                             \
    OnTrySetSize,                                     \
    (ivec2 const &),                                  \
    (override)                                        \
  );                                                  \
}

#endif // ASCII_WIDGET_WIDGET_H
