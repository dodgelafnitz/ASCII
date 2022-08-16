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
  static ivec<Dimensions> GetNextCoord(
    ivec<Dimensions> const & current,
    ivec<Dimensions> const & max
  ) {
    ivec<Dimensions> result = current;

    for (int i = 0; i < Dimensions; ++i) {
      if (++result[i] != max[i]) {
        return result;
      }

      result[i] = 0;
    }

    return max;
  }

  Grid(void) = default;
  Grid(ivec<Dimensions> const & size) :
    m_data(size.Product()),
    m_size(size)
  {
    EvaluateIndexer();
  }

  Grid(ivec<Dimensions> const & size, T const & value) :
    m_data(size.Product(), value),
    m_size(size)
  {
    EvaluateIndexer();
  }

  Grid(Grid const &) = default;
  Grid(Grid &&) = default;

  Grid & operator =(Grid const &) = default;
  Grid & operator =(Grid &&) = default;

  T const & operator [](ivec<Dimensions> const & location) const {
    int const index = location.Dot(m_indexer);
    return m_data[index];
  }

  T & operator [](ivec<Dimensions> const & location) {
    int const index = location.Dot(m_indexer);
    return m_data[index];
  }

  T const * Data(void) const {
    return m_data.data();
  }

  T * Data(void) {
    return m_data.data();
  }

  int Count(void) const {
    return m_size.Product();
  }

  ivec<Dimensions> GetSize(void) const {
    return m_size;
  }

  void SetSize(ivec<Dimensions> const & size) {
    ivec<Dimensions> const minBounds = m_size.Min(size);

    Grid newGrid(size);
    for (ivec<Dimensions> i; i != minBounds; i = GetNextCoord(i, minBounds)) {
      newGrid[i] = (*this)[i];
    }
    *this = std::move(newGrid);

    EvaluateIndexer();
  }

  bool Empty(void) const {
    for (int i = 0; i < Dimensions; ++i) {
      if (m_size[i] != 0) {
        return false;
      }
    }
    return true;
  }

  void Clear(void) {
    SetSize(ivec<Dimensions>());
  }

  std::vector<T>::const_iterator begin(void) const {
    return m_data.begin();
  }

  std::vector<T>::iterator begin(void) {
    return m_data.begin();
  }

  std::vector<T>::const_iterator end(void) const {
    return m_data.end();
  }

  std::vector<T>::iterator end(void) {
    return m_data.end();
  }

private:
  void EvaluateIndexer(void) {
    m_indexer[0] = 1;
    for (int i = 1; i < Dimensions; ++i) {
      m_indexer[i] = m_size[i - 1] * m_indexer[i - 1];
    }
  }

  std::vector<T>   m_data;
  ivec<Dimensions> m_size;
  ivec<Dimensions> m_indexer;
};

#endif // DCUTILITY_CONTAINERS_GRID_H
