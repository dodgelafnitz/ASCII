/*
 * Copywrite 2021 Dodge Lafnitzegger
 */

#include "Systems/UpdateManager.h"

#include <algorithm>

namespace {
  float const c_defaultFixedDt   = 1.0f / 10.0f;
  float const c_defaultDynamicDt = 1.0f / 60.0f;
}

UpdateManager::UpdateManager(std::shared_ptr<IAsciiWindow> const & window) :
  UpdateManager(window, c_defaultFixedDt, c_defaultDynamicDt)
{}

UpdateManager::UpdateManager(
  std::shared_ptr<IAsciiWindow> const & window,
  float                                 fixedDt,
  float                                 dynamicDt
) :
  m_window(window),
  m_fixedUpdateDelta(fixedDt),
  m_minDynamicUpdateDelta(dynamicDt),
  m_fixedUpdateRemainder(0.0f)
{
  int const currentTime = m_window->GetRunMs();

  m_nextFixedUpdateFrame   = currentTime;
  m_prevDynamicUpdateFrame = currentTime - int(dynamicDt * 1000.0f);
  m_nextDynamicUpdateFrame = currentTime;
}

Delegate<float> UpdateManager::AddOnFixedUpdate(DelegateFunc<float> const& func) {
  return m_fixedUpdateDelegator.AddDelegate(func);
}

Delegate<float, float> UpdateManager::AddOnDynamicUpdate(DelegateFunc<float, float> const& func) {
  return m_dynamicUpdateDelegator.AddDelegate(func);
}

float UpdateManager::GetFixedUpdateDt(void) const {
  return m_fixedUpdateDelta;
}

void UpdateManager::SetFixedUpdateDt(float dt) {
  m_fixedUpdateDelta = dt;
}

float UpdateManager::GetDynamicUpdateDt(void) const {
  return m_minDynamicUpdateDelta;
}

void UpdateManager::SetDynamicUpdateDt(float dt) {
  m_minDynamicUpdateDelta = dt;
}

void UpdateManager::Update(void) {
  int const   currentTime              = m_window->GetRunMs();
  int const   fixedUpdateMs            = int(m_fixedUpdateDelta * 1000.0f);
  float const fixedUpdateRemainderPart = float(m_fixedUpdateDelta * 1000.0 - double(fixedUpdateMs));

  while (m_nextFixedUpdateFrame - currentTime <= 0) {
    m_fixedUpdateDelegator.Trigger(m_fixedUpdateDelta);

    m_nextFixedUpdateFrame += int(m_fixedUpdateDelta * 1000.0f);

    m_fixedUpdateRemainder += fixedUpdateRemainderPart;
    while (m_fixedUpdateRemainder > 1.0f) {
      m_nextFixedUpdateFrame += 1;
      m_fixedUpdateRemainder -= 1.0f;
    }
  }

  int const prevFixedUpdateFrame = m_nextFixedUpdateFrame - fixedUpdateMs;

  float dynamicDt = 0.0f;
  float progress  = 0.0f;

  if (m_nextDynamicUpdateFrame - currentTime > 0) {
    m_window->Sleep(m_nextDynamicUpdateFrame - currentTime);

    m_nextDynamicUpdateFrame = currentTime + int(m_minDynamicUpdateDelta * 1000.0f);

    dynamicDt = m_minDynamicUpdateDelta;
    progress  = float(m_nextDynamicUpdateFrame - prevFixedUpdateFrame) / float(fixedUpdateMs);
  }
  else {
    dynamicDt = float(currentTime - m_prevDynamicUpdateFrame) / 1000.0f;
    progress  = float(currentTime - prevFixedUpdateFrame) / float(fixedUpdateMs);

    m_nextDynamicUpdateFrame = currentTime + int(m_minDynamicUpdateDelta * 1000.0f);
  }

  m_prevDynamicUpdateFrame = currentTime;

  m_dynamicUpdateDelegator.Trigger(dynamicDt, progress);
}
