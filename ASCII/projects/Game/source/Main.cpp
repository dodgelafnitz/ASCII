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

  float colorRot = 0.0f;

  InputManager inputManager(window);

  auto buttonManager = inputManager.GetButtonManager();

  enum class EntityType {
    Invalid,
    Player,
    Shot,
  };

  struct Entity {
    Entity(void) :
      type(EntityType::Invalid)
    {}

    EntityType type;
    fvec2      pos;
    fvec2      lastPos;
    union {
      struct {
        int animationFrame;
      } playerData;
      struct {
        float lifeLeft;
        fvec2 vel;
      } shotData;
    };
  };


  std::vector<std::shared_ptr<Entity>> entities;
  auto player = std::make_shared<Entity>();
  entities.emplace_back(player);

  player->type    = EntityType::Player;
  player->lastPos = fvec2(width / 2, height / 2);
  player->pos     = player->lastPos;

  player->playerData.animationFrame = 0;

  float const shotSpeed    = 60.0f;
  float const charSpeed    = 30.0f;
  int const   shotLifetime = 100;

  auto onMouseDown = buttonManager->AddButtonEvent(AsciiButton::Mouse1, [&](bool isDown) {
    if (isDown) {
      ivec2 const mousePos = inputManager.GetMouseManager()->GetMousePosition();

      fvec2 shotVel = mousePos - player->pos;
      shotVel.Normalize();
      shotVel *= shotSpeed;

      auto newShot = std::make_shared<Entity>();
      newShot->pos               = player->pos;
      newShot->lastPos           = player->pos;
      newShot->shotData.vel      = shotVel;
      newShot->shotData.lifeLeft = shotLifetime;
      newShot->type              = EntityType::Shot;

      entities.emplace_back(newShot);
    }
  });

  auto onQ = buttonManager->AddButtonEvent(AsciiButton::Q, [&window](bool isDown) {
    window->Sleep(1000);
  });

  UpdateManager updateManager(0.1f, 1.0f / 30.0f);

  auto inputAndPhysics = updateManager.AddOnFixedUpdate([&](float dt) {
    for (auto & entity : entities) {
      entity->lastPos = entity->pos;
    }

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

    charVel.Normalize();
    charVel *= distTravelled;

    player->pos += charVel;

    player->pos.x = std::min(std::max(player->pos.x, 0.0f), float(width - AnimWidth));
    player->pos.y = std::min(std::max(player->pos.y, 0.0f), float(height - AnimHeight));

    for (int i = entities.size() - 1; i >= 0; --i) {
      if (entities[i]->type != EntityType::Shot) {
        continue;
      }

      Entity & shot = *entities[i];

      bool shouldDestroy = false;

      if (shot.pos.x < 0.0f || shot.pos.x >= float(width) - 0.5f) {
        shouldDestroy = true;
      }
      else if (shot.pos.y < 0.0f || shot.pos.y >= float(height) - 0.5f) {
        shouldDestroy = true;
      }
      else if (shot.shotData.lifeLeft == 0) {
        shouldDestroy = true;
      }

      if (shouldDestroy) {
        entities.erase(entities.begin() + i);
      }
      else {
        ivec2 const shotDrawPos = shot.pos + 0.5f;

        --shot.shotData.lifeLeft;
        shot.pos += shot.shotData.vel * dt;
      }
    }
  });

  int dynamicFrame = 0;

  auto draw = updateManager.AddOnDynamicUpdate([&](float dt, float progress) {
    ivec2 const charDrawPos =
      player->lastPos * (1.0f - progress) +
      player->pos * progress +
      0.5f
    ;

    Grid<AsciiCell, 2> grid(ivec2(width, height));

    if (dynamicFrame % colorRotUpdateThrottle == 0) {
      font.colors[RainbowIndex] = ColorFromHueRotation(colorRot);

      window->SetFont(font);
    }

    colorRot += colorRotSpeed;

    int const xCell = dynamicFrame % grid.Count();
    grid.Data()[xCell].character       = 'X';
    grid.Data()[xCell].foregroundColor = WhiteIndex;
    grid.Data()[xCell].backgroundColor = BlackIndex;

    for (auto const & entity : entities) {
      if (entity->type != EntityType::Shot) {
        continue;
      }

      float const extraTime = progress * updateManager.GetFixedUpdateDt();

      ivec2 const drawPos =
        entity->lastPos * (1.0f - progress) +
        entity->pos * progress +
        0.5f
      ;

      if (drawPos.x < 0 || drawPos.x >= width) {
        continue;
      }

      if (drawPos.y < 0 || drawPos.y >= height) {
        continue;
      }

      grid[drawPos].character       = '.';
      grid[drawPos].foregroundColor = RainbowIndex;
      grid[drawPos].backgroundColor = BlackIndex;
    }

    Grid<char, 2> const anim = GetCharacter(dynamicFrame / 2);

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
