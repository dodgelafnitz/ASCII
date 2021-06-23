/*
 * Copywrite 2021 Dodge Lafnitzegger
 */

#ifndef ASCII_SYSTEMS_INPUT_MOUSEMANAGER_H
#define ASCII_SYSTEMS_INPUT_MOUSEMANAGER_H

#include "General/Delagate.h"
#include "Window/Window.h"

class IMouseManager {
public:
  virtual ~IMouseManager(void) {};

  virtual Delegate<ivec2> AddMousePositionEvent(
    DelegateFunc<ivec2> const & func
  ) = 0;

  virtual Delegate<ivec2> AddMouseDeltaEvent(
    DelegateFunc<ivec2> const & func
  ) = 0;

  virtual Delegate<int> AddMouseScrollEvent(
    DelegateFunc<int> const & func
  ) = 0;

  virtual ivec2 GetMousePosition(void) const = 0;
};

class MouseManager : public IMouseManager {
public:
  virtual ~MouseManager(void) {};

  virtual Delegate<ivec2> AddMousePositionEvent(
    DelegateFunc<ivec2> const & func
  ) override;

  virtual Delegate<ivec2> AddMouseDeltaEvent(
    DelegateFunc<ivec2> const & func
  ) override;

  virtual Delegate<int> AddMouseScrollEvent(
    DelegateFunc<int> const & func
  ) override;

  virtual ivec2 GetMousePosition(void) const override;
  void          SetMousePosition(ivec2 const & mousePosition);

  void ScrollMouse(int mouseScroll);

private:
  Delegator<ivec2> m_mousePositionDelegator;
  Delegator<ivec2> m_mouseDeltaDelegator;
  Delegator<int>   m_mouseScrollDelegator;
  ivec2            m_mousePos;
};

#define DEFINE_MockMouseManager()               \
class MockMouseManager : public IMouseManager { \
  MOCK_METHOD(                                  \
    Delegate<ivec2>,                            \
    AddMousePositionEvent,                      \
    (DelegateFunc<ivec2> const &),              \
    (override)                                  \
  );                                            \
  MOCK_METHOD(                                  \
    (Delegate<ivec2>),                          \
    AddMouseDeltaEvent,                         \
    (DelegateFunc<ivec2> const &),              \
    (override)                                  \
  );                                            \
  MOCK_METHOD(                                  \
    (Delegate<int>),                            \
    AddMouseScrollEvent,                        \
    (DelegateFunc<int> const &),                \
    (override)                                  \
  );                                            \
  MOCK_METHOD(                                  \
    ivec2,                                      \
    GetMousePosition,                           \
    (),                                         \
    (const, override)                           \
  );                                            \
}

#endif // ASCII_SYSTEMS_INPUT_MOUSEMANAGER_H
