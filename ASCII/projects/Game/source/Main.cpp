/*
 * Copywrite 2021 Dodge Lafnitzegger
 */

#include <cmath>
#include <memory>

#include "Systems/Input/InputManager.h"
#include "Systems/UpdateManager.h"

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
  auto window = std::make_shared<AsciiWindow>();

  AsciiFont font = window->GetFont();
  font.size = ivec2(8, 8);

  font.colors[BlackIndex]   = Color::Black;
  font.colors[WhiteIndex]   = Color::White;

  font.colors[RainbowIndex] = ColorFromHueRotation(0.0f);

  window->SetFont(font);

  int const width  = 200;
  int const height = width / 2;

  float const colorRotSpeed          = 0.01f;
  int   const colorRotUpdateThrottle = 16;

  int currentFrame = 0;
  float colorRot = 0.0f;

  fvec2 charPos(width / 2, height / 2);

  InputManager inputManager(window);

  auto buttonManager = inputManager.GetButtonManager();

  struct Shot {
    fvec2 pos;
    fvec2 vel;
    int lifeLeft;
  };

  float const shotSpeed    = 60.0f;
  float const charSpeed    = 30.0f;
  int const   shotLifetime = 100;

  std::vector<Shot> shots;

  auto onMouseDown = buttonManager->AddButtonEvent(AsciiButton::Mouse1, [&](bool isDown) {
    if (isDown) {
      ivec2 const mousePos = inputManager.GetMouseManager()->GetMousePosition();

      fvec2 shotVel = mousePos - charPos;
      if (shotVel.x != 0.0f || shotVel.y != 0.0f) {
        float const shotVelLength = shotVel.Length();
        shotVel /= fvec2(shotVelLength, shotVelLength);
        shotVel *= fvec2(shotSpeed, shotSpeed);
      }

      Shot newShot;
      newShot.pos = charPos;
      newShot.vel = shotVel;
      newShot.lifeLeft = shotLifetime;
      shots.emplace_back(newShot);
    }
  });

  auto onQ = buttonManager->AddButtonEvent(AsciiButton::Q, [&window](bool isDown) {
    window->Sleep(1000);
  });

  UpdateManager updateManager(0.1f, 1.0f / 30.0f);

  auto inputAndPhysics = updateManager.AddOnFixedUpdate([&](float dt) {
    inputManager.ProcessInput();

    fvec2 charVel;

    if (buttonManager->GetButtonState(AsciiButton::A)) {
      charVel.x -= 1.0f;
    }
    if (buttonManager->GetButtonState(AsciiButton::D)) {
      charVel.x += 1.0f;
    }
    if (buttonManager->GetButtonState(AsciiButton::W)) {
      charVel.y -= 1.0f;
    }
    if (buttonManager->GetButtonState(AsciiButton::S)) {
      charVel.y += 1.0f;
    }

    float const distTravelled = charSpeed * dt;

    //charVel.Normalize();
    float const charVelLength = charVel.Length();
    if (charVel.x != 0.0f || charVel.y != 0.0f) {
      charVel /= fvec2(charVelLength, charVelLength);
      charVel *= fvec2(distTravelled, distTravelled);
    }

    charPos += charVel;

    charPos.x = std::min(std::max(charPos.x, 0.0f), float(width - AnimWidth));
    charPos.y = std::min(std::max(charPos.y, 0.0f), float(height - AnimHeight));

    for (int i = shots.size() - 1; i >= 0; --i) {
      Shot& shot = shots[i];

      bool shouldDestroy = false;

      if (shot.pos.x < 0.0f || shot.pos.x >= float(width) - 0.5f) {
        shouldDestroy = true;
      }
      else if (shot.pos.y < 0.0f || shot.pos.y >= float(height) - 0.5f) {
        shouldDestroy = true;
      }
      else if (shot.lifeLeft == 0) {
        shouldDestroy = true;
      }

      if (shouldDestroy) {
        shots.erase(shots.begin() + i);
      }
      else {
        ivec2 const shotDrawPos = shot.pos + fvec2(0.5f, 0.5f);

        --shot.lifeLeft;
        shot.pos += shot.vel * fvec2(dt, dt);
      }
    }

    ++currentFrame;
  });

  int dynamicFrame = 0;

  auto draw = updateManager.AddOnDynamicUpdate([&](float dt, float progress) {
    ivec2 const charDrawPos(charPos + ivec2(0.5f, 0.5f));

    Grid<AsciiCell, 2> grid(ivec2(width, height));

    if (currentFrame % colorRotUpdateThrottle == 0) {
      font.colors[RainbowIndex] = ColorFromHueRotation(colorRot);

      window->SetFont(font);
    }

    colorRot += colorRotSpeed;

    int const xCell = dynamicFrame % grid.Count();
    grid.Data()[xCell].character       = 'X';
    grid.Data()[xCell].foregroundColor = WhiteIndex;
    grid.Data()[xCell].backgroundColor = BlackIndex;

    for (int i = shots.size() - 1; i >= 0; --i) {
      Shot & shot = shots[i];

      float const extraTime = progress * updateManager.GetFixedUpdateDt();

      ivec2 const shotDrawPos =
        shot.pos +
        shot.vel * fvec2(extraTime, extraTime) +
        fvec2(0.5f, 0.5f)
      ;

      if (shotDrawPos.x < 0 || shotDrawPos.x >= width) {
        continue;
      }

      if (shotDrawPos.y < 0 || shotDrawPos.y >= height) {
        continue;
      }

      grid[shotDrawPos].character       = '.';
      grid[shotDrawPos].foregroundColor = RainbowIndex;
      grid[shotDrawPos].backgroundColor = BlackIndex;
    }

    Grid<char, 2> const anim = GetCharacter(currentFrame / 2);

    for (int i = 0; i < AnimHeight; ++i) {
      for (int j = 0; j < AnimWidth; ++j) {
        ivec2 const animCell(j, i);
        ivec2 const worldCell(charDrawPos.x + j, charDrawPos.y + i);

        if (anim[animCell] != ' ') {
          grid[worldCell].character       = anim[animCell];
          grid[worldCell].foregroundColor = RainbowIndex;
          grid[worldCell].backgroundColor = BlackIndex;
        }
      }
    }

    window->Draw(grid);

    ++dynamicFrame;
  });

  while (true) {
    updateManager.Update();
  }
}
