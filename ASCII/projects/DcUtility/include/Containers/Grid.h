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
  Grid(ivec<Dimensions> const & size);
  Grid(ivec<Dimensions> const & size, T const & value);

  Grid(Grid const &) = default;
  Grid(Grid &&) = default;

  T const & operator [](ivec<Dimensions> const & location) const;
  T & operator [](ivec<Dimensions> const & location);

  T const * Data(void) const;
  T * Data(void);

  int Count(void) const;

  ivec<Dimensions> GetSize(void) const;
  void SetSize(ivec<Dimensions> const & size) const;

  bool Empty(void) const;

  void Clear(void);

  std::vector<T>::const_iterator begin(void) const;
  std::vector<T>::iterator begin(void);

  std::vector<T>::const_iterator end(void) const;
  std::vector<T>::iterator end(void);

private:
  std::vector<T>   m_data;
  ivec<Dimensions> m_size;
};

#endif // DCUTILITY_CONTAINERS_GRID_H
