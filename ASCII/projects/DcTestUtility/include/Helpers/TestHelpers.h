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
  OperatorCheck(char value) :
    m_value(1, value)
  {}

  std::string const & GetValue(void) {
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

#endif // DCTESTUTILITY_HELPERS_TESTHELPERS_H
