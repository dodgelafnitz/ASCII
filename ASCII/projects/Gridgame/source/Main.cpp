/*
 * Copywrite 2022 Dodge Lafnitzegger
 */

#include "Window/Window.h"
#include "Systems/Input/InputManager.h"

namespace {
  ivec2 const  c_gridSize       = ivec2(10, 4);
  int const    c_bottomSize     = 3;
  ivec2 const  c_screenSize     = ivec2(c_gridSize.x * 4 + 1, c_gridSize.y * 4 + 2 + c_bottomSize);
  char const * c_windowName     = "Grid Game";
  char const   c_instructions[] = "Click the boxes to turn them blue!";

  bool CellExistsForLocation(ivec2 const & location) {
    return (location.y < c_gridSize.y * 4) && (location.x % 4) && (location.y % 4) && (location.x < c_screenSize.x);
  }

  ivec2 GetCellForLocation(ivec2 const & location) {
    return location / 4;
  }
}

int main(void) {
  auto               window = std::make_shared<AsciiWindow>();
  Grid<AsciiCell, 2> screen(c_screenSize);

  for (int i = 0; i < c_gridSize.x * 4 + 1; ++i) {
    for (int j = 0; j < c_gridSize.y * 4 + 1; ++j) {
      ivec2 const location(i, j);

      if ((i % 4) == 0) {
        if ((j % 4) == 0) {
          screen[location].character = '+';
        }
        else {
          screen[location].character = '|';
        }
      }
      else if ((j % 4) == 0) {
        screen[location].character = '-';
      }
    }
  }

  for (AsciiCell & cell : screen) {
    cell.backgroundColor = 0;
    cell.foregroundColor = 1;
  }

  for (int i = 0; i < screen.GetSize().x; ++i) {
    int const splitY = c_gridSize.y * 4 + 1;

    screen[ivec2(i, splitY)].character = '=';
  }

  for (int i = 0; i < sizeof(c_instructions) / sizeof(*c_instructions) - 1; ++i) {
    int const instructionsY = c_gridSize.y * 4 + 3;

    screen[ivec2(i + 1, instructionsY)].character = c_instructions[i];
  }

  window->SetTitle(c_windowName);

  AsciiFont font;

  font.colors[0] = Color::Black;
  font.colors[1] = Color::White;
  font.colors[2] = Color::Blue;
  font.colors[3] = Color::Green;
  font.colors[4] = Color::Yellow;
  font.colors[5] = Color::Red;

  window->SetFont(font);

  int c_minColor = 2;
  int c_maxColor = 6;


  Grid<int, 2> gridColors(c_gridSize, c_minColor);
  bool shouldEnd = false;
  ivec2 mousePos;

  std::shared_ptr<InputManager> inputManager = std::make_shared<InputManager>(window);

  auto const onEscape = inputManager->GetButtonManager()->AddButtonEvent(AsciiButton::Escape, [&](bool) {
    shouldEnd = true;
  });

  auto const onMouseMove = inputManager->GetMouseManager()->AddMousePositionEvent([&](ivec2 const & pos) {
    mousePos = pos;
  });

  auto const onMouseDown = inputManager->GetButtonManager()->AddButtonEvent(AsciiButton::Mouse1, [&](bool isDown) {
    if (isDown) {
      if (CellExistsForLocation(mousePos)) {
        int & color = gridColors[GetCellForLocation(mousePos)];
        if (color > c_minColor) {
          --color;
        }
      }
    }
  });

  while (!shouldEnd) {
    inputManager->ProcessInput();

    for (ivec2 location; location != screen.GetSize(); location = screen.GetNextCoord(location, screen.GetSize())) {
      if (CellExistsForLocation(location)) {
        screen[location].backgroundColor = gridColors[GetCellForLocation(location)];
      }
    }

    for (int & color : gridColors) {
      if ((color < c_maxColor - 1) && ((std::rand() % 1500) == 0)) {
        ++color;
      }
    }

    window->Draw(screen); 
    window->Sleep(1);
  }
}

