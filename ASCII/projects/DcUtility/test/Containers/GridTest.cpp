/*
 * Copywrite 2021 Dodge Lafnitzegger
 */

#include "Containers/Grid.h"
#include "gtest/gtest.h"

TEST(GridTest, SingleDimensionGrid) {
  Grid<int, 1> grid;

  EXPECT_TRUE(grid.Empty());
  EXPECT_EQ(grid.begin(), grid.end());
  EXPECT_EQ(grid.Count(), 0);

  grid.SetSize(5);

  EXPECT_FALSE(grid.Empty());
  EXPECT_NE(grid.begin(), grid.end());
  EXPECT_EQ(grid.Count(), 5);
}

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
