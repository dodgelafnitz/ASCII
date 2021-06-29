/*
 * Copywrite 2021 Dodge Lafnitzegger
 */

#include "Systems/UpdateManager.h"

#include <algorithm>

namespace {
  float const c_defaultFixedDt   = 1.0f / 10.0f;
  float const c_defaultDynamicDt = 1.0f / 60.0f;
}

UpdateManager::UpdateManager(void) :
  UpdateManager(std::make_shared<AsciiWindow>())
{}

UpdateManager::UpdateManager(std::shared_ptr<IAsciiWindow> const & window) :
  UpdateManager(window, c_defaultFixedDt, c_defaultDynamicDt)
{}

UpdateManager::UpdateManager(float fixedDt, float dynamicDt) :
  UpdateManager(std::make_shared<AsciiWindow>(), fixedDt, dynamicDt)
{}

UpdateManager::UpdateManager(
  std::shared_ptr<IAsciiWindow> const & window,
  float                                 fixedDt,
  float                                 dynamicDt
) :
  m_window(window),
  m_fixedUpdateDelta(fixedDt),
  m_minDynamicUpdateDelta(dynamicDt)
{
  int const currentTime = m_window->GetRunMs();

  m_nextFixedUpdateFrame   = currentTime;
  m_nextDynamicUpdateFrame = currentTime;
}

Delegate<float> UpdateManager::AddOnFixedUpdate(DelegateFunc<float> const& func) {
  return m_fixedUpdateDelegator.AddDelegate(func);
}

Delegate<float> UpdateManager::AddOnDynamicUpdate(DelegateFunc<float> const& func) {
  return Delegate<float>();
}

float UpdateManager::GetFixedUpdateDt(void) const {
  return float();
}

void UpdateManager::SetFixedUpdateDt(float dt) {
}

float UpdateManager::GetDynamicUpdateDt(void) const {
  return float();
}

void UpdateManager::SetDynamicUpdateDt(float dt) {
}

void UpdateManager::Update(void) {
  int const currentTime = m_window->GetRunMs();

  while (m_nextFixedUpdateFrame - currentTime <= 0) {
    m_fixedUpdateDelegator.Trigger(m_fixedUpdateDelta);

    m_nextFixedUpdateFrame += int(m_fixedUpdateDelta * 1000.0f);
  }

  int dynamicDt = m_nextDynamicUpdateFrame - currentTime;

  if (m_nextDynamicUpdateFrame - currentTime > 0) {
    m_window->Sleep(m_nextDynamicUpdateFrame - currentTime);
    m_nextDynamicUpdateFrame = currentTime + int(m_minDynamicUpdateDelta * 1000.0f);
  }
  else {
    m_nextDynamicUpdateFrame += int(m_minDynamicUpdateDelta * 1000.0f);
  }

  m_dynamicUpdateDelegator.Trigger(std::max(
    m_nextDynamicUpdateFrame,
    int(m_minDynamicUpdateDelta * 1000.0f)
  ));
}
