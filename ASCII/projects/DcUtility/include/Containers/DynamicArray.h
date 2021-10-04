/*
 * Copywrite 2021 Dodge Lafnitzegger
 */

#ifndef DCUTILITY_CONTAINERS_DYNAMICARRAY_H
#define DCUTILITY_CONTAINERS_DYNAMICARRAY_H

#include <initializer_list>
#include <utility>

template <typename T, int Capacity>
class DynamicArray {
public:
  DynamicArray(void) = default;

  DynamicArray(DynamicArray const & arr) {
    for (T const & val : arr) {
      Emplace(val);
    }
  }

  DynamicArray(DynamicArray && arr) {
    for (T & val : arr) {
      Emplace(std::move(val));
    }
  }

  DynamicArray(int count, T const & value = T()) {
    for (int i = 0; i < count; ++i) {
      Emplace(value);
    }
  }

  DynamicArray(std::initializer_list<T> const & initializerList) {
    for (T const & val : initializerList) {
      Emplace(val);
    }
  }

  ~DynamicArray(void) {
    Clear();
  }

  DynamicArray & operator =(DynamicArray const & arr) {
    if (&arr == this) {
      return *this;
    }

    Clear();

    for (T const & val : arr) {
      Emplace(val);
    }

    return *this;
  }

  DynamicArray & operator =(DynamicArray && arr) {
    if (&arr == this) {
      return *this;
    }

    Clear();

    for (T & val : arr) {
      Emplace(std::move(val));
    }

    return *this;
  }

  T const & operator [](int index) const {
    return Data()[index];
  }

  T & operator [](int index) {
    return Data()[index];
  }

  T const * Data(void) const {
    return reinterpret_cast<T const *>(m_data);
  }

  T * Data(void) {
    return reinterpret_cast<T *>(m_data);
  }

  int Count(void) const {
    return m_count;
  }

  bool Empty(void) const {
    return m_count == 0;
  }

  bool Full(void) const {
    return m_count == Capacity;
  }

  void Clear(void) {
    while (!Empty()) {
      PopBack();
    }
  }

  template <typename ... Params>
  void Emplace(Params &&... params) {
    if (Full()) {
      return;
    }

    new (end()) T(std::forward<Params>(params)...);

    ++m_count;
  }

  void PopBack(void) {
    if (Empty()) {
      return;
    }

    --m_count;

    end()->~T();
  }

  T const * begin(void) const {
    return Data();
  }

  T * begin(void) {
    return Data();
  }

  T const * end(void) const {
    return Data() + m_count;
  }

  T * end(void) {
    return Data() + m_count;
  }

private:
  int  m_count = 0;
  char m_data[Capacity * sizeof(T)];
};

#endif // DCUTILITY_CONTAINERS_DYNAMICARRAY_H
