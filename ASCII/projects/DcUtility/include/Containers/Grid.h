/*
 * Copywrite 2021 Dodge Lafnitzegger
 */

#ifndef DCUTILITY_CONTAINERS_GRID_H
#define DCUTILITY_CONTAINERS_GRID_H

#include <vector>

#include "Math/Vector.h"

template <typename T, int Dimensions>
class Grid {
public:
  Grid(void) = default;
  Grid(uvec<Dimensions> const & size) :
    m_data(size.Product()),
    m_size(size)
  {}

  Grid(ivec<Dimensions> const & size, T const & value) :
    m_data(size.Product(), value),
    m_size(size)
  {}

  Grid(Grid const &) = default;
  Grid(Grid &&) = default;

  T const & operator [](ivec<Dimensions> const & location) const;
  T & operator [](ivec<Dimensions> const & location);

  T const * Data(void) const;
  T * Data(void);

  int Count(void) const {
    return m_size.Product();
  }

  ivec<Dimensions> GetSize(void) const;
  void SetSize(ivec<Dimensions> const & size) const;

  bool Empty(void) const {
    for (int i = 0; i < Dimensions; ++i) {
      if (m_size[i] != 0) {
        return false;
      }
    }
    return true;
  }

  void Clear(void);

  std::vector<T>::const_iterator begin(void) const {
    return m_data.begin();
  }

  std::vector<T>::iterator begin(void) {
    return m_data.begin();
  }

  std::vector<T>::const_iterator end(void) const {
    return m_data.begin();
  }

  std::vector<T>::iterator end(void) {
    return m_data.begin();
  }

private:
  std::vector<T>   m_data;
  uvec<Dimensions> m_size;
};

#endif // DCUTILITY_CONTAINERS_GRID_H
