/*
 * Copywrite 2021 Dodge Lafnitzegger
 */

#ifndef ASCII_SYSTEMS_UPDATEMANAGER_H
#define ASCII_SYSTEMS_UPDATEMANAGER_H

#include "General/Delagate.h"
#include "Window/Window.h"

class IUpdateManager {
public:
  virtual ~IUpdateManager(void) {}

  virtual Delegate<float>        AddOnFixedUpdate(DelegateFunc<float> const & func) = 0;
  virtual Delegate<float, float> AddOnDynamicUpdate(DelegateFunc<float, float> const & func) = 0;

  virtual float GetFixedUpdateDt(void) const = 0;
  virtual void  SetFixedUpdateDt(float dt) = 0;

  virtual float GetDynamicUpdateDt(void) const = 0;
  virtual void  SetDynamicUpdateDt(float dt) = 0;

  virtual void Update(void) = 0;
};

class UpdateManager : public IUpdateManager {
public:
  UpdateManager(void);
  UpdateManager(std::shared_ptr<IAsciiWindow> const & window);
  UpdateManager(float fixedDt, float dynamicDt);
  UpdateManager(std::shared_ptr<IAsciiWindow> const & window, float fixedDt, float dynamicDt);
  virtual ~UpdateManager(void) {}

  virtual Delegate<float>        AddOnFixedUpdate(DelegateFunc<float> const & func) override;
  virtual Delegate<float, float> AddOnDynamicUpdate(DelegateFunc<float, float> const & func) override;

  virtual float GetFixedUpdateDt(void) const override;
  virtual void  SetFixedUpdateDt(float dt) override;

  virtual float GetDynamicUpdateDt(void) const override;
  virtual void  SetDynamicUpdateDt(float dt) override;

  virtual void Update(void) override;

private:
  std::shared_ptr<IAsciiWindow> m_window;

  Delegator<float>        m_fixedUpdateDelegator;
  Delegator<float, float> m_dynamicUpdateDelegator;

  float m_fixedUpdateDelta;
  float m_minDynamicUpdateDelta;

  int   m_nextFixedUpdateFrame;
  float m_fixedUpdateRemainder;

  int m_prevDynamicUpdateFrame;
  int m_nextDynamicUpdateFrame;
};

#define DEFINE_MockUpdateManager()                \
class MockUpdateManager : public IUpdateManager { \
  MOCK_METHOD(                                    \
    Delegate<float>,                              \
    AddOnFixedUpdate,                             \
    (DelegateFunc<float> const &),                \
    (override)                                    \
  );                                              \
  MOCK_METHOD(                                    \
    (Delegate<float, float>),                     \
    AddOnDynamicUpdate,                           \
    ((DelegateFunc<float, float> const &)),       \
    (override)                                    \
  );                                              \
  MOCK_METHOD(                                    \
    float,                                        \
    GetFixedUpdateDt,                             \
    (),                                           \
    (const, override)                             \
  );                                              \
  MOCK_METHOD(                                    \
    void,                                         \
    SetFixedUpdateDt,                             \
    (float),                                      \
    (override)                                    \
  );                                              \
  MOCK_METHOD(                                    \
    float,                                        \
    GetDynamicUpdateDt,                           \
    (),                                           \
    (const, override)                             \
  );                                              \
  MOCK_METHOD(                                    \
    void,                                         \
    SetDynamicUpdateDt,                           \
    (float),                                      \
    (override)                                    \
  );                                              \
}

#endif // ASCII_SYSTEMS_UPDATEMANAGER_H
