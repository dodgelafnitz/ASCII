/*
 * Copywrite 2021 Dodge Lafnitzegger
 */

#include "Containers/Grid.h"
#include "gtest/gtest.h"

TEST(GridTest, GetNextCoord) {
  ivec3 const coord0 = ivec3(0, 0, 0);
  ivec3 const coord1 = ivec3(0, 0, 1);
  ivec3 const coord2 = ivec3(0, 1, 0);
  ivec3 const coord3 = ivec3(0, 1, 1);
  ivec3 const coord4 = ivec3(1, 0, 0);
  ivec3 const coord5 = ivec3(1, 0, 1);
  ivec3 const coord6 = ivec3(1, 1, 0);
  ivec3 const coord7 = ivec3(1, 1, 1);
  ivec3 const max    = ivec3(2, 2, 2);

  ivec3 const res0 = Grid<float, 3>::GetNextCoord(coord0, max);
  ivec3 const res1 = Grid<float, 3>::GetNextCoord(coord1, max);
  ivec3 const res2 = Grid<float, 3>::GetNextCoord(coord2, max);
  ivec3 const res3 = Grid<float, 3>::GetNextCoord(coord3, max);
  ivec3 const res4 = Grid<float, 3>::GetNextCoord(coord4, max);
  ivec3 const res5 = Grid<float, 3>::GetNextCoord(coord5, max);
  ivec3 const res6 = Grid<float, 3>::GetNextCoord(coord6, max);
  ivec3 const res7 = Grid<float, 3>::GetNextCoord(coord7, max);

  EXPECT_EQ(res0, coord1);
  EXPECT_EQ(res1, coord2);
  EXPECT_EQ(res2, coord3);
  EXPECT_EQ(res3, coord4);
  EXPECT_EQ(res4, coord5);
  EXPECT_EQ(res5, coord6);
  EXPECT_EQ(res6, coord7);
  EXPECT_EQ(res7, max);
}

TEST(GridTest, SingleDimensionGrid_DefaultConstructed_IsEmpty) {
  Grid<int, 1> grid;

  EXPECT_TRUE(grid.Empty());
  EXPECT_EQ(grid.Count(), 0);
}

TEST(GridTest, SingleDimensionGrid_DefaultConstructed_BeginIsEnd) {
  Grid<int, 1> grid;

  EXPECT_EQ(grid.begin(), grid.end());
}

TEST(GridTest, SingleDimensionGrid_SizeSet_SizeIsCorrect) {
  Grid<int, 1> grid;

  grid.SetSize(5);

  EXPECT_EQ(grid.Count(), 5);
  EXPECT_EQ(grid.GetSize().x, 5);
  EXPECT_FALSE(grid.Empty());
}

TEST(GridTest, MultidimensionalGrid_SizeSetToLarger_ValuesAreCorrect) {
  Grid<int, 3> grid(ivec3(2, 2, 2), -1);
  grid[ivec3(0, 0, 0)] = 1;
  grid[ivec3(0, 0, 1)] = 2;
  grid[ivec3(0, 1, 0)] = 3;
  grid[ivec3(0, 1, 1)] = 4;
  grid[ivec3(1, 0, 0)] = 5;
  grid[ivec3(1, 0, 1)] = 6;
  grid[ivec3(1, 1, 0)] = 7;
  grid[ivec3(1, 1, 1)] = 8;

  grid.SetSize(ivec3(3, 3, 3));
  EXPECT_EQ(grid[ivec3(0, 0, 0)], 1);
  EXPECT_EQ(grid[ivec3(0, 0, 1)], 2);
  EXPECT_EQ(grid[ivec3(0, 0, 2)], 0);
  EXPECT_EQ(grid[ivec3(0, 1, 0)], 3);
  EXPECT_EQ(grid[ivec3(0, 1, 1)], 4);
  EXPECT_EQ(grid[ivec3(0, 1, 2)], 0);
  EXPECT_EQ(grid[ivec3(0, 2, 0)], 0);
  EXPECT_EQ(grid[ivec3(0, 2, 1)], 0);
  EXPECT_EQ(grid[ivec3(0, 2, 2)], 0);
  EXPECT_EQ(grid[ivec3(1, 0, 0)], 5);
  EXPECT_EQ(grid[ivec3(1, 0, 1)], 6);
  EXPECT_EQ(grid[ivec3(1, 0, 2)], 0);
  EXPECT_EQ(grid[ivec3(1, 1, 0)], 7);
  EXPECT_EQ(grid[ivec3(1, 1, 1)], 8);
  EXPECT_EQ(grid[ivec3(1, 1, 2)], 0);
  EXPECT_EQ(grid[ivec3(1, 2, 0)], 0);
  EXPECT_EQ(grid[ivec3(1, 2, 1)], 0);
  EXPECT_EQ(grid[ivec3(1, 2, 2)], 0);
  EXPECT_EQ(grid[ivec3(2, 0, 0)], 0);
  EXPECT_EQ(grid[ivec3(2, 0, 1)], 0);
  EXPECT_EQ(grid[ivec3(2, 0, 2)], 0);
  EXPECT_EQ(grid[ivec3(2, 1, 0)], 0);
  EXPECT_EQ(grid[ivec3(2, 1, 1)], 0);
  EXPECT_EQ(grid[ivec3(2, 1, 2)], 0);
  EXPECT_EQ(grid[ivec3(2, 2, 0)], 0);
  EXPECT_EQ(grid[ivec3(2, 2, 1)], 0);
  EXPECT_EQ(grid[ivec3(2, 2, 2)], 0);
}

TEST(GridTest, MultidimensionalGrid_SizeSetToSmaller_ValuesAreCorrect) {
  Grid<int, 3> grid(ivec3(3, 3, 3), -1);
  grid[ivec3(0, 0, 0)] = 1;
  grid[ivec3(0, 0, 1)] = 2;
  grid[ivec3(0, 0, 2)] = 3;
  grid[ivec3(0, 1, 0)] = 4;
  grid[ivec3(0, 1, 1)] = 5;
  grid[ivec3(0, 1, 2)] = 6;
  grid[ivec3(0, 2, 0)] = 7;
  grid[ivec3(0, 2, 1)] = 8;
  grid[ivec3(0, 2, 2)] = 9;
  grid[ivec3(1, 0, 0)] = 10;
  grid[ivec3(1, 0, 1)] = 11;
  grid[ivec3(1, 0, 2)] = 12;
  grid[ivec3(1, 1, 0)] = 13;
  grid[ivec3(1, 1, 1)] = 14;
  grid[ivec3(1, 1, 2)] = 15;
  grid[ivec3(1, 2, 0)] = 16;
  grid[ivec3(1, 2, 1)] = 17;
  grid[ivec3(1, 2, 2)] = 18;
  grid[ivec3(2, 0, 0)] = 19;
  grid[ivec3(2, 0, 1)] = 20;
  grid[ivec3(2, 0, 2)] = 21;
  grid[ivec3(2, 1, 0)] = 22;
  grid[ivec3(2, 1, 1)] = 23;
  grid[ivec3(2, 1, 2)] = 24;
  grid[ivec3(2, 2, 0)] = 25;
  grid[ivec3(2, 2, 1)] = 26;
  grid[ivec3(2, 2, 2)] = 27;

  grid.SetSize(ivec3(2, 2, 2));
  EXPECT_EQ(grid[ivec3(0, 0, 0)], 1);
  EXPECT_EQ(grid[ivec3(0, 0, 1)], 2);
  EXPECT_EQ(grid[ivec3(0, 1, 0)], 4);
  EXPECT_EQ(grid[ivec3(0, 1, 1)], 5);
  EXPECT_EQ(grid[ivec3(1, 0, 0)], 10);
  EXPECT_EQ(grid[ivec3(1, 0, 1)], 11);
  EXPECT_EQ(grid[ivec3(1, 1, 0)], 13);
  EXPECT_EQ(grid[ivec3(1, 1, 1)], 14);
}

TEST(GridTest, MultidimensionalGrid_SizeSetToDifferent_ValuesAreCorrect) {
  Grid<int, 3> grid(ivec3(3, 2, 2), -1);
  grid[ivec3(0, 0, 0)] = 1;
  grid[ivec3(0, 0, 1)] = 2;
  grid[ivec3(0, 1, 0)] = 3;
  grid[ivec3(0, 1, 1)] = 4;
  grid[ivec3(1, 0, 0)] = 5;
  grid[ivec3(1, 0, 1)] = 6;
  grid[ivec3(1, 1, 0)] = 7;
  grid[ivec3(1, 1, 1)] = 8;
  grid[ivec3(2, 0, 0)] = 9;
  grid[ivec3(2, 0, 1)] = 10;
  grid[ivec3(2, 1, 0)] = 11;
  grid[ivec3(2, 1, 1)] = 12;

  grid.SetSize(ivec3(2, 3, 2));
  EXPECT_EQ(grid[ivec3(0, 0, 0)], 1);
  EXPECT_EQ(grid[ivec3(0, 0, 1)], 2);
  EXPECT_EQ(grid[ivec3(0, 1, 0)], 3);
  EXPECT_EQ(grid[ivec3(0, 1, 1)], 4);
  EXPECT_EQ(grid[ivec3(0, 2, 0)], 0);
  EXPECT_EQ(grid[ivec3(0, 2, 1)], 0);
  EXPECT_EQ(grid[ivec3(1, 0, 0)], 5);
  EXPECT_EQ(grid[ivec3(1, 0, 1)], 6);
  EXPECT_EQ(grid[ivec3(1, 1, 0)], 7);
  EXPECT_EQ(grid[ivec3(1, 1, 1)], 8);
  EXPECT_EQ(grid[ivec3(1, 2, 0)], 0);
  EXPECT_EQ(grid[ivec3(1, 2, 1)], 0);
}

TEST(GridTest, MultidimensionalGrid_RangeBasedFor_IteratesOverAll) {
  Grid<int, 3> grid(ivec3(4, 4, 4));

  int count = 0;
  for (auto const & val : grid) {
    ++count;
  }

  EXPECT_EQ(count, grid.Count());
  EXPECT_EQ(count, 64);
}
