/*
 * Copywrite 2022 Dodge Lafnitzegger
 */

#include "Widget/DrawParams.h"
#include "gtest/gtest.h"

TEST(DrawParamsTest, AnyDrawParams_DrawAttemptOutsideOfGrid_DoesNothing) {
  ivec2 const clampZoneOffset = ivec2(-50, -50);
  ivec2 const clampZoneSize   = ivec2(100, 100);

  Grid<AsciiCell, 2> grid(ivec2(4, 4));

  DrawParams params(ivec2(), clampZoneOffset, clampZoneSize, nullptr, &grid);

  params.SetCell(ivec2(-2, 2), AsciiCell('w', 3, 9));

  for (AsciiCell const & cell : grid) {
    EXPECT_EQ(cell.character, ' ');
    EXPECT_EQ(cell.foregroundColor, 0);
    EXPECT_EQ(cell.backgroundColor, 0);
  }
}

TEST(DrawParamsTest, AnyDrawParams_DrawAttemptOutsideOfClampedArea_DoesNothing) {
  ivec2 const clampZoneOffset = ivec2(1, 1);
  ivec2 const clampZoneSize   = ivec2(2, 2);

  Grid<AsciiCell, 2> grid(ivec2(4, 4));

  DrawParams params(ivec2(), clampZoneOffset, clampZoneSize, nullptr, &grid);


  params.SetCell(ivec2(3, 1), AsciiCell('q', 1, 2));

  for (AsciiCell const & cell : grid) {
    EXPECT_EQ(cell.character, ' ');
    EXPECT_EQ(cell.foregroundColor, 0);
    EXPECT_EQ(cell.backgroundColor, 0);
  }
}

TEST(DrawParamsTest, AnyDrawParams_ForcedDrawAttemptOutsideOfClampedArea_Draws) {
  ivec2 const clampZoneOffset = ivec2(1, 1);
  ivec2 const clampZoneSize   = ivec2(2, 2);

  Grid<AsciiCell, 2> grid(ivec2(4, 4));

  DrawParams params(ivec2(), clampZoneOffset, clampZoneSize, nullptr, &grid);

  params.SetCell(ivec2(3, 3), AsciiCell('a', 1, 1), DrawParams::IgnoreClamp);

  EXPECT_EQ(grid[ivec2(3, 3)].character, 'a');
  EXPECT_EQ(grid[ivec2(3, 3)].foregroundColor, 1);
  EXPECT_EQ(grid[ivec2(3, 3)].backgroundColor, 1);
}

TEST(DrawParamsTest, AnyDrawParams_ForcedDrawAttemptOutsideOfGrid_DoesNothing) {
  ivec2 const clampZoneOffset = ivec2(2, 2);
  ivec2 const clampZoneSize   = ivec2(6, 3);

  Grid<AsciiCell, 2> grid(ivec2(5, 5));

  DrawParams params(ivec2(), clampZoneOffset, clampZoneSize, nullptr, &grid);

  params.SetCell(ivec2(6, 3), AsciiCell('1', 0, 7), DrawParams::IgnoreClamp);

  for (AsciiCell const & cell : grid) {
    EXPECT_EQ(cell.character, ' ');
    EXPECT_EQ(cell.foregroundColor, 0);
    EXPECT_EQ(cell.backgroundColor, 0);
  }
}

TEST(DrawParamsTest, DrawParamsWithNegativeClampSizeX_DrawAttempt_Draws) {
  ivec2 const clampZoneOffset = ivec2(2, 2);
  ivec2 const clampZoneSize   = ivec2(-3, 3);

  Grid<AsciiCell, 2> grid(ivec2(5, 5));

  DrawParams params(ivec2(), clampZoneOffset, clampZoneSize, nullptr, &grid);

  params.SetCell(ivec2(1, 3), AsciiCell('0', 3, 5));

  EXPECT_EQ(grid[ivec2(1, 3)].character, '0');
  EXPECT_EQ(grid[ivec2(1, 3)].foregroundColor, 3);
  EXPECT_EQ(grid[ivec2(1, 3)].backgroundColor, 5);
}

TEST(DrawParamsTest, DrawParamsWithNegativeClampSizeY_DrawAttempt_Draws) {
  ivec2 const clampZoneOffset = ivec2(0, 4);
  ivec2 const clampZoneSize   = ivec2(3, -10);

  Grid<AsciiCell, 2> grid(ivec2(5, 5));

  DrawParams params(ivec2(), clampZoneOffset, clampZoneSize, nullptr, &grid);

  params.SetCell(ivec2(2, 1), AsciiCell('.', 7, 3));

  EXPECT_EQ(grid[ivec2(2, 1)].character, '.');
  EXPECT_EQ(grid[ivec2(2, 1)].foregroundColor, 7);
  EXPECT_EQ(grid[ivec2(2, 1)].backgroundColor, 3);
}

TEST(DrawParamsTest, DrawParamsWith0ClampSizeX_DrawAttempt_DoesNothing) {
  ivec2 const clampZoneOffset = ivec2(0, 4);
  ivec2 const clampZoneSize   = ivec2(0, 1);

  Grid<AsciiCell, 2> grid(ivec2(5, 5));

  DrawParams params(ivec2(), clampZoneOffset, clampZoneSize, nullptr, &grid);

  params.SetCell(ivec2(0, 4), AsciiCell(';', 2, 0));

  for (AsciiCell const & cell : grid) {
    EXPECT_EQ(cell.character, ' ');
    EXPECT_EQ(cell.foregroundColor, 0);
    EXPECT_EQ(cell.backgroundColor, 0);
  }
}

TEST(DrawParamsTest, DrawParamsWith0ClampSizeY_DrawAttempt_DoesNothing) {
  ivec2 const clampZoneOffset = ivec2(1, 1);
  ivec2 const clampZoneSize   = ivec2(2, 0);

  Grid<AsciiCell, 2> grid(ivec2(3, 3));

  DrawParams params(ivec2(), clampZoneOffset, clampZoneSize, nullptr, &grid);

  params.SetCell(ivec2(2, 1), AsciiCell('X', 7, 3));

  for (AsciiCell const & cell : grid) {
    EXPECT_EQ(cell.character, ' ');
    EXPECT_EQ(cell.foregroundColor, 0);
    EXPECT_EQ(cell.backgroundColor, 0);
  }
}

TEST(DrawParamsTest, DrawParamsWithNegativeClampSizeX_ForcedDrawAttempt_Draws) {
  ivec2 const clampZoneOffset = ivec2(5, 5);
  ivec2 const clampZoneSize   = ivec2(-3, 2);

  Grid<AsciiCell, 2> grid(ivec2(10, 10));

  DrawParams params(ivec2(), clampZoneOffset, clampZoneSize, nullptr, &grid);

  params.SetCell(ivec2(4, 7), AsciiCell('X', 1, 6), DrawParams::IgnoreClamp);

  EXPECT_EQ(grid[ivec2(4, 7)].character, 'X');
  EXPECT_EQ(grid[ivec2(4, 7)].foregroundColor, 1);
  EXPECT_EQ(grid[ivec2(4, 7)].backgroundColor, 6);
}

TEST(DrawParamsTest, DrawParamsWithNegativeClampSizeY_ForcedDrawAttempt_Draws) {
  ivec2 const clampZoneOffset = ivec2(5, 5);
  ivec2 const clampZoneSize   = ivec2(3, -6);

  Grid<AsciiCell, 2> grid(ivec2(10, 10));

  DrawParams params(ivec2(), clampZoneOffset, clampZoneSize, nullptr, &grid);

  params.SetCell(ivec2(6, 2), AsciiCell('-', 7, 4), DrawParams::IgnoreClamp);

  EXPECT_EQ(grid[ivec2(6, 2)].character, '-');
  EXPECT_EQ(grid[ivec2(6, 2)].foregroundColor, 7);
  EXPECT_EQ(grid[ivec2(6, 2)].backgroundColor, 4);
}

TEST(DrawParamsTest, DrawParamsWithOffsetSet_DrawAttempt_DrawsCorrectly) {
  ivec2 const clampZoneOffset = ivec2(0, 0);
  ivec2 const clampZoneSize   = ivec2(10, 10);
  ivec2 const drawOffset      = ivec2(3, 3);

  Grid<AsciiCell, 2> grid(ivec2(10, 10));

  DrawParams params(drawOffset, clampZoneOffset, clampZoneSize, nullptr, &grid);

  params.SetCell(ivec2(1, 3), AsciiCell('U', 3, 1));

  EXPECT_EQ(grid[ivec2(4, 6)].character, 'U');
  EXPECT_EQ(grid[ivec2(4, 6)].foregroundColor, 3);
  EXPECT_EQ(grid[ivec2(4, 6)].backgroundColor, 1);
}

TEST(DrawParamsTest, DrawParamsWithOffsetSet_DrawAttemptOffsetOutOfClampedArea_DoesNothing) {
  ivec2 const clampZoneOffset = ivec2(1, 1);
  ivec2 const clampZoneSize   = ivec2(4, 4);
  ivec2 const drawOffset      = ivec2(2, 3);

  Grid<AsciiCell, 2> grid(ivec2(4, 7));

  DrawParams params(drawOffset, clampZoneOffset, clampZoneSize, nullptr, &grid);

  params.SetCell(ivec2(1, 3), AsciiCell('C', 1, 1));

  for (AsciiCell const & cell : grid) {
    EXPECT_EQ(cell.character, ' ');
    EXPECT_EQ(cell.foregroundColor, 0);
    EXPECT_EQ(cell.backgroundColor, 0);
  }
}

TEST(DrawParamsTest, DrawParamsWithOffsetSet_DrawAttemptOffsetIntoClampedArea_DrawsCorrectly) {
  ivec2 const clampZoneOffset = ivec2(1, 1);
  ivec2 const clampZoneSize   = ivec2(4, 4);
  ivec2 const drawOffset      = ivec2(2, -3);

  Grid<AsciiCell, 2> grid(ivec2(4, 7));

  DrawParams params(drawOffset, clampZoneOffset, clampZoneSize, nullptr, &grid);

  params.SetCell(ivec2(1, 6), AsciiCell('R', 0, 5));

  EXPECT_EQ(grid[ivec2(3, 3)].character, 'R');
  EXPECT_EQ(grid[ivec2(3, 3)].foregroundColor, 0);
  EXPECT_EQ(grid[ivec2(3, 3)].backgroundColor, 5);
}

TEST(DrawParamsTest, DrawParamsWithDisplayMapping_DrawAttempt_DrawsMappedCell) {
  ivec2 const clampZoneOffset = ivec2(0, 0);
  ivec2 const clampZoneSize   = ivec2(2, 2);
  auto const  displayMapping  = [](AsciiCell const & input) {
    char const capitalized = (input.character >= 'a' && input.character <= 'z') ? input.character + 'A' - 'a' : input.character;
    return AsciiCell(capitalized, input.backgroundColor, input.foregroundColor);
  };

  Grid<AsciiCell, 2> grid(ivec2(3, 3));

  DrawParams params(ivec2(), clampZoneOffset, clampZoneSize, displayMapping, &grid);

  params.SetCell(ivec2(1, 1), AsciiCell('h', 3, 1));

  EXPECT_EQ(grid[ivec2(1, 1)].character, 'H');
  EXPECT_EQ(grid[ivec2(1, 1)].foregroundColor, 1);
  EXPECT_EQ(grid[ivec2(1, 1)].backgroundColor, 3);
}

TEST(DrawParamsTest, DrawParamsWithDisplayMapping_ForcedCharacterDrawAttempt_DrawsMappedCellWithForcedCharacter) {
  ivec2 const clampZoneOffset = ivec2(0, 0);
  ivec2 const clampZoneSize   = ivec2(2, 2);
  auto const  displayMapping  = [](AsciiCell const & input) {
    char const capitalized = (input.character >= 'a' && input.character <= 'z') ? input.character + 'A' - 'a' : input.character;
    return AsciiCell(capitalized, input.backgroundColor, input.foregroundColor);
  };

  Grid<AsciiCell, 2> grid(ivec2(3, 3));

  DrawParams params(ivec2(), clampZoneOffset, clampZoneSize, displayMapping, &grid);

  params.SetCell(ivec2(1, 1), AsciiCell('h', 3, 1), DrawParams::ForceCharacter);

  EXPECT_EQ(grid[ivec2(1, 1)].character, 'h');
  EXPECT_EQ(grid[ivec2(1, 1)].foregroundColor, 1);
  EXPECT_EQ(grid[ivec2(1, 1)].backgroundColor, 3);
}

TEST(DrawParamsTest, DrawParamsWithDisplayMapping_ForcedForegroundDrawAttempt_DrawsMappedCellWithForcedForeground) {
  ivec2 const clampZoneOffset = ivec2(0, 0);
  ivec2 const clampZoneSize   = ivec2(2, 2);
  auto const  displayMapping  = [](AsciiCell const & input) {
    char const capitalized = (input.character >= 'a' && input.character <= 'z') ? input.character + 'A' - 'a' : input.character;
    return AsciiCell(capitalized, input.backgroundColor, input.foregroundColor);
  };

  Grid<AsciiCell, 2> grid(ivec2(3, 3));

  DrawParams params(ivec2(), clampZoneOffset, clampZoneSize, displayMapping, &grid);

  params.SetCell(ivec2(1, 1), AsciiCell('h', 3, 1), DrawParams::ForceForeground);

  EXPECT_EQ(grid[ivec2(1, 1)].character, 'H');
  EXPECT_EQ(grid[ivec2(1, 1)].foregroundColor, 3);
  EXPECT_EQ(grid[ivec2(1, 1)].backgroundColor, 3);
}

TEST(DrawParamsTest, DrawParamsWithDisplayMapping_ForcedBackgrounddDrawAttempt_DrawsMappedCellWithForcedBackground) {
  ivec2 const clampZoneOffset = ivec2(0, 0);
  ivec2 const clampZoneSize   = ivec2(2, 2);
  auto const  displayMapping  = [](AsciiCell const & input) {
    char const capitalized = (input.character >= 'a' && input.character <= 'z') ? input.character + 'A' - 'a' : input.character;
    return AsciiCell(capitalized, input.backgroundColor, input.foregroundColor);
  };

  Grid<AsciiCell, 2> grid(ivec2(3, 3));

  DrawParams params(ivec2(), clampZoneOffset, clampZoneSize, displayMapping, &grid);

  params.SetCell(ivec2(1, 1), AsciiCell('h', 3, 1), DrawParams::ForceBackground);

  EXPECT_EQ(grid[ivec2(1, 1)].character, 'H');
  EXPECT_EQ(grid[ivec2(1, 1)].foregroundColor, 1);
  EXPECT_EQ(grid[ivec2(1, 1)].backgroundColor, 1);
}

TEST(DrawParamsTest, AnyDrawParams_Modify_ModifiesCell) {
  Grid<AsciiCell, 2> grid(ivec2(3, 3));
  DrawParams         params(ivec2(), ivec2(), ivec2(DrawParams::NoClamp, DrawParams::NoClamp), nullptr, &grid);

  AsciiCell const startCell('A', 4, 2);

  ivec2 const location = ivec2(1, 1);

  params.SetCell(location, startCell);

  ASSERT_EQ(grid[location], startCell);

  params.ModifyCell(location, [](AsciiCell const & cell) {
    return AsciiCell('Q', cell.foregroundColor, cell.backgroundColor);
  });

  EXPECT_EQ(grid[location].character, 'Q');
  EXPECT_EQ(grid[location].foregroundColor, 4);
  EXPECT_EQ(grid[location].backgroundColor, 2);
}

TEST(DrawParamsTest, DrawParamsWithDisplayMapping_Modify_MapsModifiedCell) {
  Grid<AsciiCell, 2> grid(ivec2(5, 4));

  auto const displayMapping = [](AsciiCell const & input) {
    char const capitalized = (input.character >= 'a' && input.character <= 'z') ? input.character + 'A' - 'a' : input.character;
    return AsciiCell(capitalized, input.foregroundColor, input.backgroundColor);
  };

  DrawParams params(ivec2(), ivec2(), ivec2(DrawParams::NoClamp, DrawParams::NoClamp), displayMapping, &grid);

  AsciiCell const startCell('S', 1, 3);

  ivec2 const location = ivec2(2, 1);

  params.SetCell(location, startCell);

  ASSERT_EQ(grid[location], startCell);

  params.ModifyCell(location, [](AsciiCell const & cell) {
    return AsciiCell('y', cell.foregroundColor, cell.backgroundColor);
  });

  EXPECT_EQ(grid[location].character, 'Y');
  EXPECT_EQ(grid[location].foregroundColor, 1);
  EXPECT_EQ(grid[location].backgroundColor, 3);
}

TEST(DrawParamsTest, DrawParamsWithOffsets_Constrain_GridIsParentsGrid) {
  Grid<AsciiCell, 2> grid0(ivec2(1, 1));
  Grid<AsciiCell, 2> grid1(ivec2(1, 1));

  DrawParams params0(ivec2(), ivec2(), ivec2(DrawParams::NoClamp, DrawParams::NoClamp), nullptr, &grid0);
  DrawParams params1(ivec2(), ivec2(), ivec2(DrawParams::NoClamp, DrawParams::NoClamp), nullptr, &grid1);

  DrawParams constrained = params0.Constrain(params1);

  AsciiCell const cell = AsciiCell('J', 0, 1);

  constrained.SetCell(ivec2(), cell);

  EXPECT_EQ(grid0[ivec2()], cell);
}

TEST(DrawParamsTest, DrawParamsWithOffsets_Constrain_OffsetsAreCombined) {
  Grid<AsciiCell, 2> grid(ivec2(5, 5));

  DrawParams params0(ivec2(1, 1), ivec2(), ivec2(DrawParams::NoClamp, DrawParams::NoClamp), nullptr, &grid);
  DrawParams params1(ivec2(2, 1), ivec2(), ivec2(DrawParams::NoClamp, DrawParams::NoClamp), nullptr, nullptr);

  DrawParams constrained = params0.Constrain(params1);

  AsciiCell const cell = AsciiCell('^', 7, 2);

  constrained.SetCell(ivec2(), cell);

  EXPECT_EQ(grid[ivec2(3, 2)], cell);
}

TEST(DrawParamsTest, DrawParamsWithClamps_Constrain_ClampsAreCombined) {
  Grid<AsciiCell, 2> grid(ivec2(5, 5));

  DrawParams params0(ivec2(1, 1), ivec2(0, 1), ivec2(3, 3), nullptr, &grid);
  DrawParams params1(ivec2(1, 1), ivec2(1, 1), ivec2(DrawParams::NoClamp, 1), nullptr, nullptr);

  DrawParams constrained = params0.Constrain(params1);

  AsciiCell const cell0  = AsciiCell('0', 0, 0);
  AsciiCell const cell1  = AsciiCell('1', 0, 0);
  AsciiCell const cell2  = AsciiCell('2', 0, 0);
  AsciiCell const cell3  = AsciiCell('3', 0, 0);
  AsciiCell const zeroed = grid[ivec2()];

  constrained.SetCell(ivec2(1, 0), cell0);
  constrained.SetCell(ivec2(-2, 0), cell1);
  constrained.SetCell(ivec2(-1, 1), cell2);
  constrained.SetCell(ivec2(2, 2), cell3);

  EXPECT_EQ(grid[ivec2(3, 2)], zeroed);
  EXPECT_EQ(grid[ivec2(0, 2)], cell1);
  EXPECT_EQ(grid[ivec2(1, 3)], zeroed);
  EXPECT_EQ(grid[ivec2(4, 4)], zeroed);
}

TEST(DrawParamsTest, DrawParamsWithDisplayMappings_Constrain_MappingsAreCombined) {
  Grid<AsciiCell, 2> grid(ivec2(1, 1));

  DrawParams::DisplayMapping const mapping0 = [](AsciiCell const & cell) {
    return AsciiCell(cell.character == 'B' ? 'C' : 'D', 2, cell.backgroundColor);
  };

  DrawParams::DisplayMapping const mapping1 = [](AsciiCell const & cell) {
    return AsciiCell(cell.character == 'A' ? 'B' : 'Q', 1, 1);
  };

  DrawParams params0(ivec2(), ivec2(), ivec2(DrawParams::NoClamp, DrawParams::NoClamp), mapping0, &grid);
  DrawParams params1(ivec2(), ivec2(), ivec2(DrawParams::NoClamp, DrawParams::NoClamp), mapping1, nullptr);

  DrawParams constrained = params0.Constrain(params1);

  constrained.SetCell(ivec2(), AsciiCell('A', 0, 0));

  EXPECT_EQ(grid[ivec2()].character, 'C');
  EXPECT_EQ(grid[ivec2()].foregroundColor, 2);
  EXPECT_EQ(grid[ivec2()].backgroundColor, 1);
}
