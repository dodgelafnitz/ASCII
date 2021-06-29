/*
 * Copywrite 2021 Dodge Lafnitzegger
 */

#ifndef ASCII_SYSTEMS_UPDATEMANAGER_H
#define ASCII_SYSTEMS_UPDATEMANAGER_H

#include "General/Delagate.h"
#include "Window/Window.h"

class IUpdateManger {
public:
  virtual ~IUpdateManger(void) {}

  virtual Delegate<float> AddOnFixedUpdate(DelegateFunc<float> const & func);
  virtual Delegate<float> AddOnDynamicUpdate(DelegateFunc<float> const & func);

  virtual float GetFixedUpdateDt(void) const;
  virtual void  SetFixedUpdateDt(float dt);

  virtual float GetDynamicUpdateDt(void) const;
  virtual void  SetDynamicUpdateDt(float dt);
};

class UpdateManger : public IUpdateManger {
public:
  UpdateManger(void);
  UpdateManger(float fixedDt, float dynamicDt);
  virtual ~UpdateManger(void) {}

  virtual Delegate<float> AddOnFixedUpdate(DelegateFunc<float> const & func) override;
  virtual Delegate<float> AddOnDynamicUpdate(DelegateFunc<float> const & func) override;

  virtual float GetFixedUpdateDt(void) const override;
  virtual void  SetFixedUpdateDt(float dt) override;

  virtual float GetDynamicUpdateDt(void) const override;
  virtual void  SetDynamicUpdateDt(float dt) override;

private:
  std::shared_ptr<IAsciiWindow> m_window;

  Delegator<float> m_fixedUpdateDelegator;
  Delegator<float> m_dynamicUpdateDelegator;

  float m_fixedUpdateDelta;
  float m_minDynamicUpdateDelta;

  int m_nextFixedUpdateFrame;
  int m_nextDynamicUpdateFrame;
};

#define DEFINE_MockUpdateManager()               \
class MockUpdateManager : public IUpdateManger { \
  MOCK_METHOD(                                   \
    Delegate<float>,                             \
    AddOnFixedUpdate,                            \
    (DelegateFunc<float> const &),               \
    (override)                                   \
  );                                             \
  MOCK_METHOD(                                   \
    Delegate<float>,                             \
    AddOnDynamicUpdate,                          \
    (DelegateFunc<float> const &),               \
    (override)                                   \
  );                                             \
  MOCK_METHOD(                                   \
    float,                                       \
    GetFixedUpdateDt,                            \
    (),                                          \
    (const, override)                            \
  );                                             \
  MOCK_METHOD(                                   \
    void,                                        \
    SetFixedUpdateDt,                            \
    (float),                                     \
    (override)                                   \
  );                                             \
  MOCK_METHOD(                                   \
    float,                                       \
    GetDynamicUpdateDt,                          \
    (),                                          \
    (const, override)                            \
  );                                             \
  MOCK_METHOD(                                   \
    void,                                        \
    SetDynamicUpdateDt,                          \
    (float),                                     \
    (override)                                   \
  );                                             \
}

#endif // ASCII_SYSTEMS_UPDATEMANAGER_H
