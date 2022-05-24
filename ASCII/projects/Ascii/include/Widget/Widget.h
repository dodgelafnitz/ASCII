/*
 * Copywrite 2021 Dodge Lafnitzegger
 */

#ifndef ASCII_WIDGET_WIDGET_H
#define ASCII_WIDGET_WIDGET_H

#include <memory>

#include "Containers/Grid.h"
#include "Window/Window.h"
#include "Systems/Input/TextManager.h"

class IInputManager;
class IWidgetManager;

class Widget : public std::enable_shared_from_this<Widget> {
public:
  virtual ~Widget(void) = default;

  virtual std::weak_ptr<IInputManager>  GetInputManager(void) const;
  virtual std::weak_ptr<IWidgetManager> GetWidgetManager(void) const;

  virtual void OnDraw(Grid<AsciiCell, 2> & io_screen, ivec2 const & position) const {};

  virtual bool OnPress(ivec2 const & subposition, AsciiButton button, int clickCount)                     { return false; };
  virtual bool OnHold(ivec2 const & subposition, AsciiButton button, int clickCount)                      { return false; };
  virtual bool OnDrag(ivec2 const & subposition, AsciiButton button, int clickCount, ivec2 const & delta) { return false; };
  virtual bool OnText(TextEvent const & textEvent)                                                        { return false; };

  virtual void OnGainedFocus(void)      {}
  virtual void OnLostFocus(void)        {}
  virtual void OnChildGainedFocus(void) {}
  virtual void OnChildLostFocus(void)   {}

  virtual bool OnMouseEnter(ivec2 const & subposition, ivec2 const & delta) { return false; };
  virtual bool OnMouseLeave(ivec2 const & subposition, ivec2 const & delta) { return false; };
  virtual bool OnMouseHover(ivec2 const & subposition)                      { return false; };
  virtual bool OnMouseScroll(int scroll)                                    { return false; };

  virtual bool DoesOcclude(ivec2 const & subposition) const { return false; }
  virtual bool DoesMaskChildren(void) const                 { return false; };
  virtual bool IsFocusable(void) const                      { return false; };

  virtual ivec2 GetSize(void) const              { return ivec2(0, 0); }
  virtual bool  CanSetSize(void) const           { return false; };
  virtual bool  OnTrySetSize(ivec2 const & size) { return false; };

  void Draw(Grid<AsciiCell, 2> & io_screen, ivec2 const & position) const;

  bool Press(ivec2 const & subposition, AsciiButton button, int clickCount);
  bool Hold(ivec2 const & subposition, AsciiButton button, int clickCount);
  bool Drag(ivec2 const & subposition, AsciiButton button, int clickCount, ivec2 const & delta);
  bool Text(TextEvent const & textEvent);

  void GainFocus(void);

  void GainedFocus(void);
  void LostFocus(void);
  void ChildGainedFocus(void);
  void ChildLostFocus(void);

  bool MouseEnter(ivec2 const & subposition, ivec2 const & delta);
  bool MouseLeave(ivec2 const & subposition, ivec2 const & delta);
  bool MouseHover(ivec2 const & subposition);
  bool MouseScroll(int scroll);

  bool TrySetSize(ivec2 const & size);

  ivec2 GetControlledOrigin(void) const;
  ivec2 GetControlledSize(void) const;

  int                     GetChildCount(void) const;
  std::shared_ptr<Widget> GetChild(int index) const;
  int                     GetChildIndex(std::shared_ptr<Widget> const & child) const;
  fvec2                   GetChildScaledOffset(int index) const;
  ivec2                   GetChildConstantOffset(int index) const;
  ivec2                   GetChildOffset(int index) const;

  void RemoveChild(int index);
  void SetChildScaledOffset(int index, fvec2 scaledOffset);
  void SetChildConstantOffset(int index, ivec2 constantOffset);

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
    std::shared_ptr<Widget> widget;
    fvec2                   scaledOffset;
    ivec2                   constantOffset;
  };

  void OnChildResize(int childId);
  void SetParent(std::weak_ptr<Widget> const & widget, int childIndex);

  std::vector<ChildData> m_children;
  std::weak_ptr<Widget>  m_parent;
  int                    m_childIndex;
  ivec2                  m_controlledOffset;
  ivec2                  m_controlledSize;
};

#endif // ASCII_WIDGET_WIDGET_H
