/*
 * Copywrite 2021 Dodge Lafnitzegger
 */

#ifndef DCTESTUTILITY_HELPERS_TESTHELPERS_H
#define DCTESTUTILITY_HELPERS_TESTHELPERS_H

#include <string>

class DefaultConstructorCheck {
public:
  DefaultConstructorCheck(void) :
    m_defaultConstructed(true)
  {}

  template <typename ... Params>
  DefaultConstructorCheck(Params && ...) :
    m_defaultConstructed(false)
  {}

  bool WasDefaultConstructed(void) const {
    return m_defaultConstructed;
  }

private:
  bool m_defaultConstructed;
};

class OperatorCheck {
public:
  OperatorCheck(void) :
    m_value("()")
  {}

  OperatorCheck(std::string const & value) :
    m_value(value)
  {}

  OperatorCheck(int value) :
    m_value(std::to_string(value))
  {}

  std::string const & GetValue(void) const {
    return m_value;
  }

  OperatorCheck & operator += (OperatorCheck const & check) {
    BinaryOp('+', m_value, check.m_value);

    return *this;
  }

  OperatorCheck & operator -= (OperatorCheck const & check) {
    BinaryOp('-', m_value, check.m_value);

    return *this;
  }

  OperatorCheck & operator *= (OperatorCheck const & check) {
    BinaryOp('*', m_value, check.m_value);

    return *this;
  }

  OperatorCheck & operator /= (OperatorCheck const & check) {
    BinaryOp('/', m_value, check.m_value);

    return *this;
  }

  OperatorCheck & operator %= (OperatorCheck const & check) {
    BinaryOp('%', m_value, check.m_value);

    return *this;
  }

private:
  void BinaryOp(char op, std::string const & lVal, std::string const & rVal) {
    m_value = std::string("(") + lVal + op + rVal + ")";
  }

  std::string m_value;
};

class DestructorCheck {
public:
  DestructorCheck(void) = default;
  DestructorCheck(bool * destructed) :
    m_destructed(destructed)
  {}

  DestructorCheck(DestructorCheck const &) = default;

  DestructorCheck(DestructorCheck && check) :
    m_destructed(check.m_destructed)
  {
    check.m_destructed = nullptr;
  }

  ~DestructorCheck(void) {
    if (m_destructed) {
      *m_destructed = true;
    }
  }

private:
  bool * m_destructed = nullptr;
};

class MoveCheck {
public:
  enum class State {
    Default,
    MovedTo,
    CopiedTo,
    MovedFrom,
  };

  MoveCheck(void) = default;

  MoveCheck(MoveCheck const & check) :
    m_state(State::CopiedTo)
  {}

  MoveCheck(MoveCheck && check) :
    m_state(State::MovedTo)
  {
    check.m_state = State::MovedFrom;
  }

  MoveCheck & operator =(MoveCheck const & check) {
    if (&check != this) {
      m_state = State::CopiedTo;
    }

    return *this;
  }

  MoveCheck & operator =(MoveCheck && check) {
    if (&check != this) {
      m_state       = State::MovedTo;
      check.m_state = State::MovedFrom;
    }

    return *this;
  }

  State GetState(void) const {
    return m_state;
  }

private:
  State m_state = State::Default;
};

#endif // DCTESTUTILITY_HELPERS_TESTHELPERS_H
