/*
 * Copywrite 2021 Dodge Lafnitzegger
 */

#include <cmath>

#include "Window/Window.h"

namespace {
  int const BlackIndex   = 0;
  int const WhiteIndex   = 1;
  int const RainbowIndex = 2;

  float const Pi = 3.141592653589793238462643383279;

  Color ColorFromHueRotation(float hueAngle) {
    hueAngle = std::fmod(hueAngle, Pi * 2.0f);

    Color result;

    float const thirdRot = 2.0f * Pi / 3.0f;

    if (hueAngle >= 0.0f && hueAngle < thirdRot) {
      float const delta = (hueAngle - 0.0f) / thirdRot;

      result.r = int(std::cos(delta * Pi / 2.0f) * 255);
      result.g = int(std::sin(delta * Pi / 2.0f) * 255);
    }
    else if (hueAngle >= thirdRot && hueAngle < 2.0f * thirdRot) {
      float const delta = (hueAngle - thirdRot) / thirdRot;

      result.g = int(std::cos(delta * Pi / 2.0f) * 255);
      result.b = int(std::sin(delta * Pi / 2.0f) * 255);
    }
    else {
      float const delta = (hueAngle - 2.0f * thirdRot) / thirdRot;

      result.b = int(std::cos(delta * Pi / 2.0f) * 255);
      result.r = int(std::sin(delta * Pi / 2.0f) * 255);
    }

    return result;
  }

  int const AnimWidth  = 1;
  int const AnimHeight = 2;

  Grid<char, 2> GetCharacter(int frame) {
    Grid<char, 2> result(ivec2(AnimWidth, AnimHeight));

    char const * anims[] = {
      "<"
      "A",
      "v"
      "A",
      ">"
      "A",
      "^"
      "A"
    };

    char const * anim = anims[frame % (sizeof(anims) / sizeof(*anims))];

    for (int i = 0; i < AnimWidth * AnimHeight; ++i) {
      result.Data()[i] = anim[i];
    }

    return result;
  }
};

int main(void) {
  AsciiWindow window;

  AsciiFont font = window.GetFont();
  font.size = ivec2(8, 8);

  font.colors[BlackIndex]   = Color::Black;
  font.colors[WhiteIndex]   = Color::White;

  font.colors[RainbowIndex] = ColorFromHueRotation(0.0f);

  window.SetFont(font);

  int const width  = 60;
  int const height = width;// / 2;

  float const colorRotSpeed          = 0.01f;
  int   const colorRotUpdateThrottle = 4;

  int currentFrame = 0;
  float colorRot = 0.0f;

  ivec2 charPos(width / 2, height / 2);

  while (true) {
    auto inputBuffer = window.PollInput();

    for (auto const & input : inputBuffer) {
      if (input.type == AsciiInputType::Button) {
        if (input.buttonEvent.isDown) {
          switch (input.buttonEvent.button) {
            case AsciiButton::Left: {
              --charPos.x;
            } break;
            case AsciiButton::Right: {
              ++charPos.x;
            } break;
            case AsciiButton::Up: {
              --charPos.y;
            } break;
            case AsciiButton::Down: {
              ++charPos.y;
            } break;
          }
        }
      }
    }

    charPos.x = std::min(std::max(charPos.x, 0), width - AnimWidth);
    charPos.y = std::min(std::max(charPos.y, 0), height - AnimHeight);

    Grid<AsciiCell, 2> grid(ivec2(width, height));

    if (currentFrame % colorRotUpdateThrottle == 0) {
      font.colors[RainbowIndex] = ColorFromHueRotation(colorRot);

      window.SetFont(font);
    }

    colorRot += colorRotSpeed;

    int const xCell = currentFrame % grid.Count();
    grid.Data()[xCell].character       = 'X';
    grid.Data()[xCell].foregroundColor = WhiteIndex;
    grid.Data()[xCell].backgroundColor = BlackIndex;

    Grid<char, 2> const anim = GetCharacter(currentFrame / 2);

    for (int i = 0; i < AnimHeight; ++i) {
      for (int j = 0; j < AnimWidth; ++j) {
        ivec2 const animCell(j, i);
        ivec2 const worldCell(charPos.x + j, charPos.y + i);

        if (anim[animCell] != ' ') {
          grid[worldCell].character       = anim[animCell];
          grid[worldCell].foregroundColor = RainbowIndex;
          grid[worldCell].backgroundColor = BlackIndex;
        }
      }
    }

    window.Draw(grid);

    ++currentFrame;

    window.Sleep(10);
  }
}