/*
 * Copywrite 2022 Dodge Lafnitzegger
 */

#include "Widget/DrawParams.h"
#include "gtest/gtest.h"

TEST(DrawParamsTest, AnyDrawParams_DrawAttemptOutsideOfGrid_DoesNothing) {
  DrawParams params;
  params.clampZoneOffset = ivec2(-50, -50);
  params.clampZoneSize   = ivec2(100, 100);

  Grid<AsciiCell, 2> grid(ivec2(4, 4));

  params.Draw(grid, ivec2(-2, 2), AsciiCell('w', 3, 9));

  for (AsciiCell const & cell : grid) {
    EXPECT_EQ(cell.character, ' ');
    EXPECT_EQ(cell.foregroundColor, 0);
    EXPECT_EQ(cell.backgroundColor, 0);
  }
}

TEST(DrawParamsTest, AnyDrawParams_DrawAttemptOutsideOfClampedArea_DoesNothing) {
  DrawParams params;
  params.clampZoneOffset = ivec2(1, 1);
  params.clampZoneSize   = ivec2(2, 2);

  Grid<AsciiCell, 2> grid(ivec2(4, 4));

  params.Draw(grid, ivec2(3, 1), AsciiCell('q', 1, 2));

  for (AsciiCell const & cell : grid) {
    EXPECT_EQ(cell.character, ' ');
    EXPECT_EQ(cell.foregroundColor, 0);
    EXPECT_EQ(cell.backgroundColor, 0);
  }
}

TEST(DrawParamsTest, AnyDrawParams_ForcedDrawAttemptOutsideOfClampedArea_Draws) {
  DrawParams params;
  params.clampZoneOffset = ivec2(1, 1);
  params.clampZoneSize   = ivec2(2, 2);

  Grid<AsciiCell, 2> grid(ivec2(4, 4));

  params.Draw(grid, ivec2(3, 3), AsciiCell('a', 1, 1), DrawParams::IgnoreClamp);

  EXPECT_EQ(grid[ivec2(3, 3)].character, 'a');
  EXPECT_EQ(grid[ivec2(3, 3)].foregroundColor, 1);
  EXPECT_EQ(grid[ivec2(3, 3)].backgroundColor, 1);
}

TEST(DrawParamsTest, AnyDrawParams_ForcedDrawAttemptOutsideOfGrid_DoesNothing) {
  DrawParams params;
  params.clampZoneOffset = ivec2(2, 2);
  params.clampZoneSize   = ivec2(6, 3);

  Grid<AsciiCell, 2> grid(ivec2(5, 5));

  params.Draw(grid, ivec2(6, 3), AsciiCell('1', 0, 7), DrawParams::IgnoreClamp);

  for (AsciiCell const & cell : grid) {
    EXPECT_EQ(cell.character, ' ');
    EXPECT_EQ(cell.foregroundColor, 0);
    EXPECT_EQ(cell.backgroundColor, 0);
  }
}

TEST(DrawParamsTest, DrawParamsWithNegativeClampSizeX_DrawAttempt_DoesNothing) {
  DrawParams params;
  params.clampZoneOffset = ivec2(2, 2);
  params.clampZoneSize   = ivec2(-3, 3);

  Grid<AsciiCell, 2> grid(ivec2(5, 5));

  params.Draw(grid, ivec2(1, 3), AsciiCell('0', 3, 5));

  for (AsciiCell const & cell : grid) {
    EXPECT_EQ(cell.character, ' ');
    EXPECT_EQ(cell.foregroundColor, 0);
    EXPECT_EQ(cell.backgroundColor, 0);
  }
}

TEST(DrawParamsTest, DrawParamsWithNegativeClampSizeY_DrawAttempt_DoesNothing) {
  DrawParams params;
  params.clampZoneOffset = ivec2(0, 4);
  params.clampZoneSize   = ivec2(3, -10);

  Grid<AsciiCell, 2> grid(ivec2(5, 5));

  params.Draw(grid, ivec2(2, 1), AsciiCell('.', 7, 3));

  for (AsciiCell const & cell : grid) {
    EXPECT_EQ(cell.character, ' ');
    EXPECT_EQ(cell.foregroundColor, 0);
    EXPECT_EQ(cell.backgroundColor, 0);
  }
}

TEST(DrawParamsTest, DrawParamsWith0ClampSizeX_DrawAttempt_DoesNothing) {
  DrawParams params;
  params.clampZoneOffset = ivec2(0, 4);
  params.clampZoneSize   = ivec2(0, 1);

  Grid<AsciiCell, 2> grid(ivec2(5, 5));

  params.Draw(grid, ivec2(0, 4), AsciiCell(';', 2, 0));

  for (AsciiCell const & cell : grid) {
    EXPECT_EQ(cell.character, ' ');
    EXPECT_EQ(cell.foregroundColor, 0);
    EXPECT_EQ(cell.backgroundColor, 0);
  }
}

TEST(DrawParamsTest, DrawParamsWith0ClampSizeY_DrawAttempt_DoesNothing) {
  DrawParams params;
  params.clampZoneOffset = ivec2(1, 1);
  params.clampZoneSize   = ivec2(2, 0);

  Grid<AsciiCell, 2> grid(ivec2(3, 3));

  params.Draw(grid, ivec2(2, 1), AsciiCell('X', 7, 3));

  for (AsciiCell const & cell : grid) {
    EXPECT_EQ(cell.character, ' ');
    EXPECT_EQ(cell.foregroundColor, 0);
    EXPECT_EQ(cell.backgroundColor, 0);
  }
}

TEST(DrawParamsTest, DrawParamsWithNegativeClampSizeX_ForcedDrawAttempt_Draws) {
  DrawParams params;
  params.clampZoneOffset = ivec2(5, 5);
  params.clampZoneSize   = ivec2(-3, 2);

  Grid<AsciiCell, 2> grid(ivec2(10, 10));

  params.Draw(grid, ivec2(4, 7), AsciiCell('X', 1, 6), DrawParams::IgnoreClamp);

  EXPECT_EQ(grid[ivec2(4, 7)].character, 'X');
  EXPECT_EQ(grid[ivec2(4, 7)].foregroundColor, 1);
  EXPECT_EQ(grid[ivec2(4, 7)].backgroundColor, 6);
}

TEST(DrawParamsTest, DrawParamsWithNegativeClampSizeY_ForcedDrawAttempt_Draws) {
  DrawParams params;
  params.clampZoneOffset = ivec2(5, 5);
  params.clampZoneSize   = ivec2(3, -6);

  Grid<AsciiCell, 2> grid(ivec2(10, 10));

  params.Draw(grid, ivec2(6, 2), AsciiCell('-', 7, 4), DrawParams::IgnoreClamp);

  EXPECT_EQ(grid[ivec2(6, 2)].character, '-');
  EXPECT_EQ(grid[ivec2(6, 2)].foregroundColor, 7);
  EXPECT_EQ(grid[ivec2(6, 2)].backgroundColor, 4);
}

TEST(DrawParamsTest, DrawParamsWithOffsetSet_DrawAttempt_DrawsCorrectly) {
  DrawParams params;
  params.clampZoneOffset = ivec2(0, 0);
  params.clampZoneSize   = ivec2(10, 10);
  params.drawOffset      = ivec2(3, 3);

  Grid<AsciiCell, 2> grid(ivec2(10, 10));

  params.Draw(grid, ivec2(1, 3), AsciiCell('U', 3, 1));

  EXPECT_EQ(grid[ivec2(4, 6)].character, 'U');
  EXPECT_EQ(grid[ivec2(4, 6)].foregroundColor, 3);
  EXPECT_EQ(grid[ivec2(4, 6)].backgroundColor, 1);
}

TEST(DrawParamsTest, DrawParamsWithOffsetSet_DrawAttemptOffsetOutOfClampedArea_DoesNothing) {
  DrawParams params;
  params.clampZoneOffset = ivec2(1, 1);
  params.clampZoneSize   = ivec2(4, 4);
  params.drawOffset      = ivec2(2, 3);

  Grid<AsciiCell, 2> grid(ivec2(4, 7));

  params.Draw(grid, ivec2(1, 3), AsciiCell('C', 1, 1));

  for (AsciiCell const & cell : grid) {
    EXPECT_EQ(cell.character, ' ');
    EXPECT_EQ(cell.foregroundColor, 0);
    EXPECT_EQ(cell.backgroundColor, 0);
  }
}

TEST(DrawParamsTest, DrawParamsWithOffsetSet_DrawAttemptOffsetIntoClampedArea_DrawsCorrectly) {
  DrawParams params;
  params.clampZoneOffset = ivec2(1, 1);
  params.clampZoneSize   = ivec2(4, 4);
  params.drawOffset      = ivec2(2, -3);

  Grid<AsciiCell, 2> grid(ivec2(4, 7));

  params.Draw(grid, ivec2(1, 6), AsciiCell('R', 0, 5));

  EXPECT_EQ(grid[ivec2(3, 3)].character, 'R');
  EXPECT_EQ(grid[ivec2(3, 3)].foregroundColor, 0);
  EXPECT_EQ(grid[ivec2(3, 3)].backgroundColor, 5);
}

TEST(DrawParamsTest, DrawParamsWithDisplayMapping_DrawAttempt_DrawsMappedCell) {
  DrawParams params;
  params.clampZoneOffset = ivec2(0, 0);
  params.clampZoneSize   = ivec2(2, 2);
  params.displayMapping  = [](AsciiCell const & input) {
    char const capitalized = (input.character >= 'a' && input.character <= 'z') ? input.character + 'A' - 'a' : input.character;
    return AsciiCell(capitalized, input.backgroundColor, input.foregroundColor);
  };

  Grid<AsciiCell, 2> grid(ivec2(3, 3));

  params.Draw(grid, ivec2(1, 1), AsciiCell('h', 3, 1));

  EXPECT_EQ(grid[ivec2(1, 1)].character, 'H');
  EXPECT_EQ(grid[ivec2(1, 1)].foregroundColor, 1);
  EXPECT_EQ(grid[ivec2(1, 1)].backgroundColor, 3);
}

TEST(DrawParamsTest, DrawParamsWithDisplayMapping_ForcedCharacterDrawAttempt_DrawsMappedCellWithForcedCharacter) {
  DrawParams params;
  params.clampZoneOffset = ivec2(0, 0);
  params.clampZoneSize   = ivec2(2, 2);
  params.displayMapping  = [](AsciiCell const & input) {
    char const capitalized = (input.character >= 'a' && input.character <= 'z') ? input.character + 'A' - 'a' : input.character;
    return AsciiCell(capitalized, input.backgroundColor, input.foregroundColor);
  };

  Grid<AsciiCell, 2> grid(ivec2(3, 3));

  params.Draw(grid, ivec2(1, 1), AsciiCell('h', 3, 1), DrawParams::ForceCharacter);

  EXPECT_EQ(grid[ivec2(1, 1)].character, 'h');
  EXPECT_EQ(grid[ivec2(1, 1)].foregroundColor, 1);
  EXPECT_EQ(grid[ivec2(1, 1)].backgroundColor, 3);
}

TEST(DrawParamsTest, DrawParamsWithDisplayMapping_ForcedForegroundDrawAttempt_DrawsMappedCellWithForcedForeground) {
  DrawParams params;
  params.clampZoneOffset = ivec2(0, 0);
  params.clampZoneSize   = ivec2(2, 2);
  params.displayMapping  = [](AsciiCell const & input) {
    char const capitalized = (input.character >= 'a' && input.character <= 'z') ? input.character + 'A' - 'a' : input.character;
    return AsciiCell(capitalized, input.backgroundColor, input.foregroundColor);
  };

  Grid<AsciiCell, 2> grid(ivec2(3, 3));

  params.Draw(grid, ivec2(1, 1), AsciiCell('h', 3, 1), DrawParams::ForceForeground);

  EXPECT_EQ(grid[ivec2(1, 1)].character, 'H');
  EXPECT_EQ(grid[ivec2(1, 1)].foregroundColor, 3);
  EXPECT_EQ(grid[ivec2(1, 1)].backgroundColor, 3);
}

TEST(DrawParamsTest, DrawParamsWithDisplayMapping_ForcedBackgrounddDrawAttempt_DrawsMappedCellWithForcedBackground) {
  DrawParams params;
  params.clampZoneOffset = ivec2(0, 0);
  params.clampZoneSize   = ivec2(2, 2);
  params.displayMapping  = [](AsciiCell const & input) {
    char const capitalized = (input.character >= 'a' && input.character <= 'z') ? input.character + 'A' - 'a' : input.character;
    return AsciiCell(capitalized, input.backgroundColor, input.foregroundColor);
  };

  Grid<AsciiCell, 2> grid(ivec2(3, 3));

  params.Draw(grid, ivec2(1, 1), AsciiCell('h', 3, 1), DrawParams::ForceBackground);

  EXPECT_EQ(grid[ivec2(1, 1)].character, 'H');
  EXPECT_EQ(grid[ivec2(1, 1)].foregroundColor, 1);
  EXPECT_EQ(grid[ivec2(1, 1)].backgroundColor, 1);
}
