/*
 * Copywrite 2021 Dodge Lafnitzegger
 */

#ifndef DCUTILITY_CONTAINERS_DYNAMICARRAY_H
#define DCUTILITY_CONTAINERS_DYNAMICARRAY_H

#include <initializer_list>

template <typename T, int Capacity>
class DynamicArray {
public:
  DynamicArray(void) = default;

  DynamicArray(DynamicArray const & arr);

  DynamicArray(DynamicArray && arr);

  DynamicArray(int count, T const & value = T());

  DynamicArray(std::initializer_list<T> const & initializerList);

  ~DynamicArray(void);

  DynamicArray & operator =(DynamicArray const & arr);

  DynamicArray & operator =(DynamicArray && arr);

  T const & operator [](int index) const;

  T & operator [](int index);

  T const * Data(void) const;
  T * Data(void);

  int Count(void) const;

  bool Empty(void) const;

  void Clear(void);

  template <typename ... Params>
  void Emplace(Params &&... params);

  void PopBack(void) const;

  T const * begin(void) const;

  T * begin(void);

  T const * end(void) const;

  T * end(void);

private:
  int  m_count = 0;
  char m_data[Capacity * sizeof(T)];
};



#endif // DCUTILITY_CONTAINERS_DYNAMICARRAY_H
