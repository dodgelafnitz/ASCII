/*
 * Copywrite 2021 Dodge Lafnitzegger
 */

#ifndef DCUTILITY_MATH_VECTOR_H
#define DCUTILITY_MATH_VECTOR_H

#include <utility>

template <typename T, int Count>
class Vector {
public:
  static int const Size = Count;

  Vector(void) {
    for (int i = 0; i < Size; ++i) {
      m_values[i] = T();
    }
  }

  template <typename ... Params, typename Enable = std::enable_if_t<sizeof...(Params) == Count>>
  Vector(Params && ... values) {
    SetValues(0, std::forward<Params &&>(values)...);
  }

  T const & operator [](int index) const {
    return m_values[index];
  }

  T & operator [](int index) {
    return m_values[index];
  }

  bool operator ==(Vector const & vec) const {
    for (int i = 0; i < Size; ++i) {
      if ((*this)[i] != vec[i]) {
        return false;
      }
    }

    return true;
  }

  bool operator !=(Vector const & vec) const {
    for (int i = 0; i < Size; ++i) {
      if ((*this)[i] != vec[i]) {
        return true;
      }
    }

    return false;
  }

private:
  template <typename CurrentParam, typename ... Params>
  void SetValues(int index, CurrentParam && currentValue, Params && ... values) {
    m_values[index] = std::forward<CurrentParam &&>(currentValue);
    SetValues(index + 1, std::forward<Params &&>(values)...);
  }
  void SetValues(int index) {}

  T m_values[Count];
};

template <typename T>
class Vector<T, 1> {
public:
  static int const Size = 1;

  Vector(void) :
    x()
  {}

  Vector(T && x) :
    x(std::forward<T &&>(x))
  {}

  T const & operator [](int index) const {
    return x;
  }

  T & operator [](int index) {
    return x;
  }

  bool operator ==(Vector const & vec) const {
    for (int i = 0; i < Size; ++i) {
      if ((*this)[i] != vec[i]) {
        return false;
      }
    }

    return true;
  }

  bool operator !=(Vector const & vec) const {
    for (int i = 0; i < Size; ++i) {
      if ((*this)[i] != vec[i]) {
        return true;
      }
    }

    return false;
  }

  T x;
};

template <typename T>
class Vector<T, 2> {
public:
  static int const Size = 2;

  Vector(void) :
    x(),
    y()
  {}

  Vector(T && x, T && y) :
    x(std::forward<T &&>(x)),
    y(std::forward<T &&>(y))
  {}

  T const & operator [](int index) const {
    switch (index % 2) {
      case 0: return x;
      case 1: return y;
    }
    return x;
  }

  T & operator [](int index) {
    switch (index % 2) {
      case 0: return x;
      case 1: return y;
    }
    return x;
  }

  bool operator ==(Vector const & vec) const {
    for (int i = 0; i < Size; ++i) {
      if ((*this)[i] != vec[i]) {
        return false;
      }
    }

    return true;
  }

  bool operator !=(Vector const & vec) const {
    for (int i = 0; i < Size; ++i) {
      if ((*this)[i] != vec[i]) {
        return true;
      }
    }

    return false;
  }

  T x;
  T y;
};

template <typename T>
class Vector<T, 3> {
public:
  static int const Size = 3;

  Vector(void) :
    x(),
    y(),
    z()
  {}

  Vector(T && x, T && y, T && z) :
    x(std::forward<T &&>(x)),
    y(std::forward<T &&>(y)),
    z(std::forward<T &&>(z))
  {}

  T const & operator [](int index) const {
    switch (index % 3) {
      case 0: return x;
      case 1: return y;
      case 2: return z;
    }
    return x;
  }

  T & operator [](int index) {
    switch (index % 3) {
      case 0: return x;
      case 1: return y;
      case 2: return z;
    }
    return x;
  }

  bool operator ==(Vector const & vec) const {
    for (int i = 0; i < Size; ++i) {
      if ((*this)[i] != vec[i]) {
        return false;
      }
    }

    return true;
  }

  bool operator !=(Vector const & vec) const {
    for (int i = 0; i < Size; ++i) {
      if ((*this)[i] != vec[i]) {
        return true;
      }
    }

    return false;
  }

  T x;
  T y;
  T z;
};

template <typename T>
class Vector<T, 4> {
public:
  static int const Size = 4;

  Vector(void) :
    x(),
    y(),
    z(),
    w()
  {}

  Vector(T&& x, T && y, T && z, T && w) :
    x(std::forward<T &&>(x)),
    y(std::forward<T &&>(y)),
    z(std::forward<T &&>(z)),
    w(std::forward<T &&>(w))
  {}

  T const & operator [](int index) const {
    switch (index % 4) {
      case 0: return x;
      case 1: return y;
      case 2: return z;
      case 3: return w;
    }
    return x;
  }

  T & operator [](int index) {
    switch (index % 4) {
      case 0: return x;
      case 1: return y;
      case 2: return z;
      case 3: return w;
    }
    return x;
  }

  bool operator ==(Vector const & vec) const {
    for (int i = 0; i < Size; ++i) {
      if ((*this)[i] != vec[i]) {
        return false;
      }
    }

    return true;
  }

  bool operator !=(Vector const & vec) const {
    for (int i = 0; i < Size; ++i) {
      if ((*this)[i] != vec[i]) {
        return true;
      }
    }

    return false;
  }

  T x;
  T y;
  T z;
  T w;
};

template<int Count>
using ivec = Vector<int, Count>;

template<int Count>
using fvec = Vector<float, Count>;

template<int Count>
using uvec = Vector<unsigned int, Count>;

using ivec1 = Vector<int, 1>;
using ivec2 = Vector<int, 2>;
using ivec3 = Vector<int, 3>;
using ivec4 = Vector<int, 4>;

using uvec1 = Vector<unsigned int, 1>;
using uvec2 = Vector<unsigned int, 2>;
using uvec3 = Vector<unsigned int, 3>;
using uvec4 = Vector<unsigned int, 4>;

using fvec1 = Vector<float, 1>;
using fvec2 = Vector<float, 2>;
using fvec3 = Vector<float, 3>;
using fvec4 = Vector<float, 4>;

#endif // DCUTILITY_MATH_VECTOR_H
