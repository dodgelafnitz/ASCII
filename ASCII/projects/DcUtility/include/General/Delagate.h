/*
 * Copywrite 2021 Dodge Lafnitzegger
 */

#ifndef DCUTILITY_GENERAL_DELAGATE_H
#define DCUTILITY_GENERAL_DELAGATE_H

#include <functional>
#include <memory>

template <typename ... Params>
class Delegator;

template <typename ... Params>
class Delegate;

template <typename ... Params>
using DelegateFunc = std::function<void(Params const &...)>;

template <typename ... Params>
class Delegator {
public:
  using Functor  = DelegateFunc<Params...>;
  using Delegate = Delegate<Params...>;

  Delegator(void) :
    m_impl(std::make_shared<DelegatorImpl>())
  {}

  Delegator(Delegator const &) = delete;
  Delegator(Delegator &&)      = default;

  void Trigger(Params const &... values) {
    m_impl->Trigger(values...);
  }

  Delegate AddDelegate(Functor const & func) {
    return Delegate(m_impl, func);
  }

  bool IsEmpty(void) const {
    return m_impl->IsEmpty();
  }

private:
  friend class Delegate;

  class DelegatorImpl {
  public:
    void Trigger(Params const &... values) const {
      for (auto const & delegatePair : m_delagates) {
        delegatePair.second(values...);
      }
    }

    int AddDelegate(Functor const & func) {
      int const id    = m_nextId;
      m_delagates[id] = func;

      do {
        ++m_nextId;
      } while (m_delagates.count(m_nextId) != 0);

      return id;
    }

    void RemoveDelegate(int delegateId) {
      m_delagates.erase(delegateId);
    }

    bool IsEmpty(void) const {
      return m_delagates.empty();
    }

  private:
    std::unordered_map<int, Functor> m_delagates;
    int                              m_nextId = 0;
  };

  std::shared_ptr<DelegatorImpl> m_impl;
};

template <typename ... Params>
class Delegate {
public:
  Delegate(void)             = default;
  Delegate(Delegate const &) = delete;

  Delegate(Delegate && delegate) :
    m_delegator(delegate.m_delegator),
    m_id(delegate.m_id)
  {
    delegate.m_delegator.reset();
    delegate.m_id = 0;
  }

  ~Delegate(void) {
    Clear();
  }

  Delegate & operator =(Delegate const & delegate) = delete;

  Delegate & operator =(Delegate && delegate) {
    if (&delegate == this) {
      return *this;
    }

    m_delegator = delegate.m_delegator;
    m_id        = delegate.m_id;

    delegate.m_delegator.reset();
    delegate.m_id = 0;

    return *this;
  }

  void Clear(void) {
    std::shared_ptr<DelegatorImpl> delegator = m_delegator.lock();
    if (delegator) {
      delegator->RemoveDelegate(m_id);
    }

    m_delegator.reset();
    m_id = 0;
  }

private:
  friend class Delegator<Params...>;

  using Functor       = DelegateFunc<Params...>;
  using DelegatorImpl = typename Delegator<Params...>::DelegatorImpl;

  Delegate(std::shared_ptr<DelegatorImpl> const & delegator, Functor const & func) :
    m_delegator(delegator),
    m_id(delegator->AddDelegate(func))
  {}

  std::weak_ptr<DelegatorImpl> m_delegator;
  int                          m_id;
};

#endif // DCUTILITY_GENERAL_DELAGATE_H
