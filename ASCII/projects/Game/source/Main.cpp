/*
 * Copywrite 2021 Dodge Lafnitzegger
 */

#include <cmath>
#include <memory>

#include "Systems/Input/InputManager.h"
#include "Systems/UpdateManager.h"

namespace {
  int const BlackIndex = 0;
  int const WhiteIndex = 1;
  int const RedIndex   = 2;

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

  font.colors[BlackIndex] = Color::Green;
  font.colors[WhiteIndex] = Color::White;

  font.colors[RedIndex] = Color::Red;

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
    fvec2      vel;
    union {
      struct {
        int animationFrame;
        bool onGround;
      } playerData;
      struct {
        float lifeLeft;
      } shotData;
    };
  };

  struct Terrain {
    fvec2 upperLeft;
    fvec2 lowerRight;
  };

  std::vector<Terrain> terrain = {
    { fvec2(0.25f * width, 0.75f * height), fvec2(0.75f * width, 0.80f * height) },

    { ivec2(0.00f * width, 0.98f * height), ivec2(0.05f * width, 1.00f * height) },
    { ivec2(0.05f * width, 0.93f * height), ivec2(0.10f * width, 0.95f * height) },
    { ivec2(0.10f * width, 0.88f * height), ivec2(0.15f * width, 0.90f * height) },
    { ivec2(0.15f * width, 0.83f * height), ivec2(0.20f * width, 0.85f * height) },
    { ivec2(0.20f * width, 0.78f * height), ivec2(0.25f * width, 0.80f * height) },
    { ivec2(0.25f * width, 0.73f * height), ivec2(0.30f * width, 0.75f * height) },
  };

  std::vector<std::shared_ptr<Entity>> entities;
  auto player = std::make_shared<Entity>();
  entities.emplace_back(player);

  player->type    = EntityType::Player;
  player->lastPos = fvec2(width / 2, height / 2);
  player->pos     = player->lastPos;

  player->playerData.animationFrame = 0;

  float const shotSpeed    = 60.0f;
  float const charSpeed    = 10.0f;
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
      newShot->vel               = shotVel;
      newShot->shotData.lifeLeft = shotLifetime;
      newShot->type              = EntityType::Shot;

      entities.emplace_back(newShot);
    }
  });

  auto onJump = buttonManager->AddButtonEvent(AsciiButton::Space, [&](bool isDown) {
    if (isDown && player->playerData.onGround) {
      player->vel.y = -40.0f;
      player->playerData.onGround = false;
    }
  });

  float waitUntil = 0.0f;
  float runTime   = 0.0f;

  auto onQ = buttonManager->AddButtonEvent(AsciiButton::Q, [&](bool isDown) {
    waitUntil = runTime + 10.0f;
  });

  UpdateManager updateManager(0.1f, 0.0f);

  enum class Direction {
    Invalid,
    Up,
    Left,
    Down,
    Right,
  };

  auto IsInTerrain = [&](fvec2 const & pos) -> bool {
    for (auto const & terrainPiece : terrain) {
      if (
        pos.x > terrainPiece.upperLeft.x &&
        pos.y > terrainPiece.upperLeft.y &&
        pos.x <= terrainPiece.lowerRight.x &&
        pos.y <= terrainPiece.lowerRight.y
      ) {
        return true;
      }
    }
    return false;
  };

  auto LineSegmentsIntersect = [&](
    fvec2 const & p00,
    fvec2 const & p01,
    fvec2 const & p10,
    fvec2 const & p11
  ) -> bool {
      fvec2 const n0 = fvec2(p00.y - p01.y, p01.x - p00.x).Normalize();
      fvec2 const n1 = fvec2(p10.y - p11.y, p11.x - p10.x).Normalize();

      fvec2 const d00 = p00 - p10;
      fvec2 const d01 = p01 - p10;
      fvec2 const d10 = p10 - p00;
      fvec2 const d11 = p11 - p00;

      float const a00 = d00.Dot(n1);
      float const a01 = d01.Dot(n1);
      float const a10 = d10.Dot(n0);
      float const a11 = d11.Dot(n0);

      bool const c0 = a00 * a01 < 0.0f;
      bool const c1 = a10 * a11 < 0.0f;

      return c0 && c1;
  };

  auto PopOutOfTerrain = [&]() -> Direction {
    for (auto const & terrainPiece : terrain) {
      if (
        player->pos.x > terrainPiece.upperLeft.x &&
        player->pos.y > terrainPiece.upperLeft.y &&
        player->pos.x <= terrainPiece.lowerRight.x &&
        player->pos.y <= terrainPiece.lowerRight.y
      ) {
        bool intersectsTop    = false;
        bool intersectsLeft   = false;
        bool intersectsRight  = false;
        bool intersectsBottom = false;

        fvec2 const lowerLeft(terrainPiece.upperLeft.x, terrainPiece.lowerRight.y);
        fvec2 const upperRight(terrainPiece.lowerRight.x, terrainPiece.upperLeft.y);

        if (LineSegmentsIntersect(
          player->lastPos,
          player->pos,
          terrainPiece.upperLeft,
          upperRight
        )) {
          intersectsTop = true;
        }

        if (LineSegmentsIntersect(
          player->lastPos,
          player->pos,
          terrainPiece.upperLeft,
          lowerLeft
        )) {
          intersectsLeft = true;
        }

        if (LineSegmentsIntersect(
          player->lastPos,
          player->pos,
          lowerLeft,
          terrainPiece.lowerRight
        )) {
          intersectsBottom = true;
        }

        if (LineSegmentsIntersect(
          player->lastPos,
          player->pos,
          upperRight,
          terrainPiece.lowerRight
        )) {
          intersectsRight = true;
        }

        fvec2 const vel = player->pos - player->lastPos;

        if (vel.x >= 0.0f) {
          intersectsRight = false;
        }
        if (vel.x <= 0.0f) {
          intersectsLeft = false;
        }
        if (vel.y >= 0.0f) {
          intersectsBottom = false;
        }
        if (vel.y <= 0.0f) {
          intersectsTop = false;
        }

        if (intersectsTop) {
          player->pos.y = terrainPiece.upperLeft.y - 1.0f;
          return Direction::Up;
        }
        else if (intersectsLeft) {
          player->pos.x = terrainPiece.upperLeft.x - 1.0f;
          return Direction::Left;
        }
        else if (intersectsBottom) {
          player->pos.y = terrainPiece.lowerRight.y + 1.0f;
          return Direction::Down;
        }
        else if (intersectsRight) {
          player->pos.x = terrainPiece.lowerRight.x + 1.0f;
          return Direction::Right;
        }

        break;
      }
    }

    return Direction::Invalid;
  };

  auto inputAndPhysics = updateManager.AddOnFixedUpdate([&](float dt) {
    runTime += dt;
    if (waitUntil > runTime) {
      return;
    }

    for (auto & entity : entities) {
      entity->lastPos = entity->pos;
    }

    if (!IsInTerrain(player->pos + fvec2(0.0f, 1.1f))) {
      player->playerData.onGround = false;
    }

    if (!player->playerData.onGround) {
      player->vel.y += 80.0f * dt;
    }

    if (player->vel.LengthSquared() < 0.01f) {
      player->pos = ivec2(player->pos + 0.5f);
    }

    player->pos += player->vel * dt;

    if (player->pos.x < 0.0f) {
      player->pos.x = 0.0f;
      player->vel.x = std::min(player->vel.x, 0.0f);
    }
    else if(player->pos.x >= float(width) - 0.5f) {
      player->pos.x = float(width) - 0.5f;
      player->vel.x = std::max(player->vel.x, 0.0f);
    }

    if (player->pos.y < 0.0f) {
      player->pos.y = 0.0f;
      player->vel.y = std::min(player->vel.y, 0.0f);
    }
    else if (player->pos.y >= float(height) - 0.5f) {
      player->pos.y = float(height) - 0.5f;
      player->vel.y = std::max(player->vel.y, 0.0f);
      player->playerData.onGround = true;
    }

    if (IsInTerrain(player->pos)) {
      Direction popDir = PopOutOfTerrain();
      switch (popDir) {
        case Direction::Up: {
          player->playerData.onGround = true;
          player->vel.y = 0.0f;
        } break;
        case Direction::Left: {
          player->vel.x = 0.0f;
        } break;
        case Direction::Down: {
          player->vel.y = 0.0f;
        } break;
        case Direction::Right: {
          player->vel.x = 0.0f;
        } break;
      }
    }

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
      else if (IsInTerrain(shot.pos)) {
        shouldDestroy = true;
      }

      if (shouldDestroy) {
        entities.erase(entities.begin() + i);
      }
      else {
        ivec2 const shotDrawPos = shot.pos + 0.5f;

        --shot.shotData.lifeLeft;
        shot.pos += shot.vel * dt;
      }
    }
  });

  int dynamicFrame = 0;

  auto draw = updateManager.AddOnDynamicUpdate([&](float dt, float progress) {
    inputManager.ProcessInput();

    fvec2 charVel;

    if (buttonManager->GetButtonState(AsciiButton::A)) {
      charVel.x -= 1.0f;
    }
    if (buttonManager->GetButtonState(AsciiButton::D)) {
      charVel.x += 1.0f;
    }

    float const distTravelled = charSpeed * dt;

    charVel.Normalize();
    charVel *= distTravelled;

    player->pos += charVel;

    if (IsInTerrain(player->pos)) {
      Direction popDir = PopOutOfTerrain();
      switch (popDir) {
        case Direction::Up: {
          player->playerData.onGround = true;
          player->vel.y = 0.0f;
        } break;
        case Direction::Left: {
          player->vel.x = 0.0f;
        } break;
        case Direction::Down: {
          player->vel.y = 0.0f;
        } break;
        case Direction::Right: {
          player->vel.x = 0.0f;
        } break;
      }
    }

    player->pos.x = std::min(std::max(player->pos.x, 0.0f), float(width - AnimWidth));
    player->pos.y = std::min(std::max(player->pos.y, 0.0f), float(height - AnimHeight));

    //ivec2 const charDrawPos = player->pos + 0.5f;

    ivec2 const charDrawPos =
      player->lastPos * (1.0f - progress) +
      player->pos * progress +
      0.5f
    ;

    Grid<AsciiCell, 2> grid(ivec2(width, height));

    int const xCell = dynamicFrame % grid.Count();
    grid.Data()[xCell].character = 'X';
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

      grid[drawPos].character = '.';
      grid[drawPos].foregroundColor = RedIndex;
      grid[drawPos].backgroundColor = BlackIndex;
    }

    Grid<char, 2> const anim = GetCharacter(dynamicFrame / 2);

    for (int i = 0; i < AnimHeight; ++i) {
      for (int j = 0; j < AnimWidth; ++j) {
        ivec2 const animCell(j, i);
        ivec2 const worldCell(charDrawPos.x + j, charDrawPos.y + i);

        if (anim[animCell] != ' ') {
          grid[worldCell].character = anim[animCell];
          grid[worldCell].foregroundColor = RedIndex;
          grid[worldCell].backgroundColor = BlackIndex;
        }
      }
    }

    for (Terrain const & terrainPiece : terrain) {
      int xBegin = int(terrainPiece.upperLeft.x + 0.5f);
      int xEnd   = int(terrainPiece.lowerRight.x + 0.5f);
      int yBegin = int(terrainPiece.upperLeft.y + 1.5f);
      int yEnd   = int(terrainPiece.lowerRight.y + 1.5f);

      for (int i = yBegin; i < yEnd; ++i) {
        if (i < 0 || i >= height) {
          continue;
        }

        for (int j = xBegin; j < xEnd; ++j) {
          if (j < 0 || j >= width) {
            continue;
          }

          ivec2 const cell(j, i);
          grid[cell].character = (i + j) % 2 ? '[' : ']';
          grid[cell].foregroundColor = WhiteIndex;
          grid[cell].backgroundColor = BlackIndex;
        }
      }
    }

    window->Draw(grid);

    ++dynamicFrame;
  });

  while (!inputManager.GetButtonManager()->GetButtonState(AsciiButton::Escape)) {
    updateManager.Update();
  }
}
