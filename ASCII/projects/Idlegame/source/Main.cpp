/*
 * Copywrite 2022 Dodge Lafnitzegger
 */

#include <cstdlib>
#include <ctime>
#include <map>
#include <memory>
#include <variant>

#include "Containers/DynamicArray.h"
#include "Containers/Grid.h"
#include "General/Color.h"
#include "Window/Window.h"
#include "Systems/Input/InputManager.h"
#include "Systems/UpdateManager.h"

namespace {
  int const c_invalid = -1;

  float GetRandomValue(void) {
    return float(std::rand() % 11447) / 11447.0f;
  }

  class PerlinNoise {
  public:
    PerlinNoise(int detail) {
      for (int i = 0; i < detail; ++i) {
        int const layerSize = (1 << (i + 1)) + 1;

        Grid<float, 2> nextGrid(ivec2(layerSize, layerSize));

        for (float & value : nextGrid) {
          value = GetRandomValue();
        }

        m_magnitudes.emplace_back(nextGrid);
      }
    }

    float GetValue(fvec2 const & location) {
      float total = 0.0f;

      for (int i = 0; i < m_magnitudes.size(); ++i) {
        int const   layerSize = (1 << (i + 1)) + 1;
        float const scale     = 1.0f / ((i < m_magnitudes.size() - 1) ? (layerSize - 1) : (1 << i));

        Grid<float, 2> currentGrid = m_magnitudes[i];

        ivec2 const index = location * (currentGrid.GetSize() - ivec2(1, 1));
        fvec2 const delta = (location * (currentGrid.GetSize() - ivec2(1, 1))) - index;

        int const nextX = (index.x == currentGrid.GetSize().x - 1) ? 0 : 1;
        int const nextY = (index.y == currentGrid.GetSize().y - 1) ? 0 : 1;

        ivec2 const i0 = index + ivec2(0,     0);
        ivec2 const i1 = index + ivec2(nextX, 0);
        ivec2 const i2 = index + ivec2(0,     nextY);
        ivec2 const i3 = index + ivec2(nextX, nextY);

        float const v0 = currentGrid[i0];
        float const v1 = currentGrid[i1];
        float const v2 = currentGrid[i2];
        float const v3 = currentGrid[i3];

        float const v01 = (1.0f - delta.x) * v0 + delta.x * v1;
        float const v23 = (1.0f - delta.x) * v2 + delta.x * v3;

        float const v = (1.0f - delta.y) * v01 + delta.y * v23;

        total += scale * v;
      }

      return total;
    };

  private:
    std::vector<Grid<float, 2>> m_magnitudes;
  };

  enum class TerrainType {
    Plains,
    Lake,
    Forest,
    Mountain,

    Count
  };

  enum class Direction {
    North,
    East,
    South,
    West,
    Local,

    Count
  };

  enum class StructureType {
    LoggingCamp,
    Quarry,
    Mine,
    Farm,
    Fort,
    Hospital,
    PublicSpace,
    Storage,
    Woodshop,
    StoneCutter,
    Forge,
    Lab,
    Workshop,
    Inn,
    Kitchen,
    Market,
    TownCenter,

    Count
  };

  enum class ItemType {
    Wood,
    Stone,
    Metal,
    WorkedWood,
    WorkedStone,
    WorkedMetal,
    Grain,
    Fruit,
    Meat,
    Fish,
    Herbs,
    Potion,
    Tool,
    Food,

    Count
  };

  struct Faction {
    Faction(std::string const & name) :
      name(name)
    {}

    std::string name;
  };

  using FactionIndex = int;

  enum class ItemTier {
    Basic,
    Quality,
    Exotic,

    Count
  };

  struct ItemInfo {
    ItemInfo(ItemType type, FactionIndex faction, ItemTier tier) :
      type(type),
      faction(faction),
      tier(tier)
    {}

    ItemType     type;
    FactionIndex faction;
    ItemTier     tier;
  };

  using ItemIndex = int;

  enum class SkillType {
    Logging,
    Quarrying,
    Mining,
    Farming,
    Gathering,
    Fishing,
    Attacking,
    Defending,
    Healing,
    Entertaining,
    Logistics,
    Carpentry,
    Stonecutting,
    Smithing,
    Alchemy,
    Building,
    Crafting,
    Cooking,
    Negotiation,
    Pathfinding,
    Leadership,

    Count
  };

  struct SkillInfo {
    SkillInfo(SkillType type, float scale) :
      type(type),
      scale(scale)
    {}

    SkillType type;
    float     scale;
  };

  static int const c_maxToolStats = 2;

  struct ToolInfo {
    ToolInfo(std::string const & name, FactionIndex faction) :
      name(name),
      faction(faction)
    {}

    ToolInfo & AddSkill(SkillType type, float scale) {
      if (!skills.Full()) {
        skills.Emplace(type, scale);
      }

      return *this;
    }

    ToolInfo & AddCost(ItemIndex item, float cost) {
      if (costs.count(item) != 0) {
        costs[item] += cost;
      }
      else {
        costs[item] = cost;
      }

      return *this;
    }

    std::string                             name;
    FactionIndex                            faction;
    DynamicArray<SkillInfo, c_maxToolStats> skills;
    std::map<ItemIndex, float>              costs;
  };

  using ToolIndex = int;

  struct BuildingInfo {
    BuildingInfo(std::string const & name, FactionIndex faction, StructureType type, int maxWorkers, float laborScale) :
      name(name),
      faction(faction),
      type(type),
      maxWorkers(maxWorkers),
      laborScale(laborScale)
    {}

    BuildingInfo & AddCost(ItemIndex item, float cost) {
      if (costs.count(item) != 0) {
        costs[item] += cost;
      }
      else {
        costs[item] = cost;
      }

      return *this;
    }

    std::string              name;
    FactionIndex             faction;
    StructureType            type;
    int                      maxWorkers;
    float                    laborScale;
    std::map<ItemIndex, int> costs;
  };

  using BuildingIndex = int;

  struct QuestInfo {
    std::string              name;
    FactionIndex             faction;
    ivec2                    location;
    std::map<ItemIndex, int> costs;
    std::vector<SkillInfo>   skillRequirements;
  };

  using QuestIndex = int;

  enum class GoalType {
    GoToTerrain,
    GoToStructure,
    GoToPoi,
    GoToQuest,
    GoHome,

    Count
  };

  enum class PoiType {
    EnemyCity,
    AlliedCity,
    ManyEnemies,

    Count
  };

  static int const c_maxTools = 2;

  enum class JobType {
    Logger,
    Quarrier,
    Miner,
    Gatherer,
    Fisher,
    Soldier,
    Healer,
    Entertainer,
    Merchant,
    Carpenter,
    Stonecutter,
    Blacksmith,
    Alchemist,
    Builder,
    Crafter,
    Chef,
    Scout,

    Count
  };

  enum class StatType {
    Happiness,
    Nutrition,
    Energy,
    Health,

    Count
  };

  struct CharacterData {
    std::string                         name;
    DynamicArray<ToolIndex, c_maxTools> tools;
    int                                 gold;
    float                               preferences[int(JobType::Count)];
    float                               stats[int(StatType::Count)];
  };

  struct PartyData {
    std::string                name;
    std::vector<CharacterData> characters;
    int                        gold;
  };

  struct Goal {
    GoalType type;
    int      value;
  };

  struct InventoryElement {
    ItemIndex item;
    float     amount;
  };

  struct MovableData {
    FactionIndex                  faction;
    ivec2                         location;
    std::vector<Goal>             goals;
    std::vector<InventoryElement> inventory;

    std::variant<
      PartyData,
      CharacterData
    > data;
  };

  struct GeneralizedGoal {
    FactionIndex faction;
    Goal         goal;
  };

  struct FlowCell {
    float directionWeights[int(Direction::Count)];
  };

  struct CellData {
    TerrainType                terrain;
    BuildingIndex              beingBuilt;
    std::vector<BuildingIndex> buildings;
    std::vector<ItemIndex>     resources;
  };

  struct MountainRangeInfo {
    int   count;
    int   thickness;
    int   length;
    int   rangeStartPadding;
    float baseWalkWeight;
    float dirWalkWeight;
    float density;
  };

  struct ForestInfo {
    float density;
    int   detail;
    float roughness;
  };

  struct LakeInfo {
    int count;
    int minBaseSize;
    int maxBaseSize;
  };

  struct CityInfo {
    int minCount;
    int maxCount;
    int requiredDistance;
  };

  struct MapGenerationInfo {
    MountainRangeInfo mountainRanges;
    ForestInfo        forests;
    LakeInfo          lakes;
    CityInfo          cities;
  };


  struct GameInfo {
    std::vector<Faction>      factions;
    std::vector<ItemInfo>     items;
    std::vector<ToolInfo>     tools;
    std::vector<BuildingInfo> buildings;
    std::vector<QuestInfo>    quests;

    MapGenerationInfo         generation;
  };

  struct GameData {
    GameInfo const *                             info;
    Grid<CellData, 2>                            map;
    std::map<GeneralizedGoal, Grid<FlowCell, 2>> flow;
    std::vector<MovableData>                     movables;
    std::string                                  playerName;
  };

  static float const c_pi = 3.14159265358979323f;

  fvec2 GetVectorFromDirection(Direction dir) {
    switch (dir) {
      case Direction::North: return fvec2( 0.0f, -1.0f);
      case Direction::East:  return fvec2( 1.0f,  0.0f);
      case Direction::South: return fvec2( 0.0f,  1.0f);
      case Direction::West:  return fvec2(-1.0f,  0.0f);
      default:               return fvec2( 0.0f,  0.0f);
    }
  }

  ivec2 GetIntVectorFromDirection(Direction dir) {
    return GetVectorFromDirection(dir);
  }

  int GetRandomWeightedIndex(float const * weights, int count) {
    if (weights == nullptr || count == 0) {
      return c_invalid;
    }

    int   currentIndex = 0;
    float totalWeight  = weights[0];

    for (int i = 1; i < count; ++i) {
      totalWeight += weights[i];
      float const randomFloat = float(std::rand() % 10153) / 10153.0f;
      if (randomFloat * totalWeight < weights[i]) {
        currentIndex = i;
      }
    }

    return currentIndex;
  }

  void GenerateMountainRange(ivec2 const & size, MountainRangeInfo const & rangeInfo, Grid<TerrainType, 2> & io_ranges) {
    float const randomAngle = (float(std::rand() % 11017) / 11017.0f) * 2.0f * c_pi;
    fvec2 const randomDir   = fvec2(std::cos(randomAngle), std::sin(randomAngle));

    ivec2 const startMin  = ivec2(rangeInfo.rangeStartPadding, rangeInfo.rangeStartPadding);
    ivec2 const startMax  = ivec2(size.x - rangeInfo.rangeStartPadding, size.y - rangeInfo.rangeStartPadding);
    ivec2 const startSize = startMax - startMin;
    ivec2 const startPos  = ivec2(startMin.x + (std::rand() % startSize.x), startMin.y + (std::rand() % startSize.y));

    ivec2 thickenDelta;
    do {
      Direction const thickenDirection = Direction(std::rand() % (int(Direction::Count) - 1));

      thickenDelta = GetIntVectorFromDirection(thickenDirection);
    } while (std::abs(fvec2(thickenDelta).Dot(randomDir)) > 0.8f);

    float walkWeights[int(Direction::Count)];
    for (int i = 0; i < int(Direction::Count); ++i) {
      fvec2 const dirWeight = GetVectorFromDirection(Direction(i));
      walkWeights[i] = rangeInfo.baseWalkWeight + std::max(dirWeight.Dot(randomDir), 0.0f) * rangeInfo.dirWalkWeight;
    }
    walkWeights[int(Direction::Local)] = 0.0f;

    ivec2 currentLocation = startPos;
    for (int i = 0; i < rangeInfo.length; ++i) {
      int const localThickness = std::min(rangeInfo.thickness, std::min(i + 1, rangeInfo.length - i));
      for (int j = 0; j < localThickness; ++j) {
        ivec2 const mountainLocation = currentLocation + thickenDelta * j;

        if (mountainLocation.x >= 0 && mountainLocation.x < size.x && mountainLocation.y >= 0 && mountainLocation.y < size.y) {
          if (GetRandomValue() < rangeInfo.density) {
            io_ranges[mountainLocation] = TerrainType::Mountain;
          }
        }
      }

      int const       dirIndex = GetRandomWeightedIndex(walkWeights, int(Direction::Count));
      Direction const dir      = Direction(dirIndex);

      currentLocation += GetIntVectorFromDirection(dir);
    }
  }

  void GenerateLake(ivec2 const & size, LakeInfo const & lakeInfo, Grid<TerrainType, 2> & io_lakes) {
    int const baseSizeRange = lakeInfo.maxBaseSize - lakeInfo.minBaseSize;
    int const baseSize      = ((baseSizeRange != 0) ? (std::rand() % baseSizeRange) : 0) + lakeInfo.minBaseSize;

    ivec2 const startPos = ivec2(std::rand() % size.x, std::rand() % size.y);

    int                totalAdded     = 0;
    std::vector<ivec2> positionsToAdd;

    positionsToAdd.emplace_back(startPos);

    while (!positionsToAdd.empty() && totalAdded < baseSize) {
      int const randomIndex = std::rand() % positionsToAdd.size();

      ivec2 const position = positionsToAdd[randomIndex];
      positionsToAdd[randomIndex] = positionsToAdd.back();
      positionsToAdd.pop_back();

      if (io_lakes[position] == TerrainType::Lake) {
        continue;
      }

      for (int i = 0; i < int(Direction::Count); ++i) {
        ivec2 const nextDir = GetIntVectorFromDirection(Direction(i));
        ivec2 const nextPos = position + nextDir;

        if (nextPos.x < 0 || nextPos.x >= size.x || nextPos.y < 0 || nextPos.y >= size.y) {
          continue;
        }

        if (io_lakes[nextPos] == TerrainType::Lake) {
          continue;
        }

        positionsToAdd.emplace_back(nextPos);
      }

      io_lakes[position] = TerrainType::Lake;
      ++totalAdded;
    }
  }

  void CleanLakeEdges(Grid<TerrainType, 2> & io_lakes) {
    bool noChange = false;

    while (!noChange) {
      noChange = true;

      for (ivec2 i; i != io_lakes.GetSize(); i = io_lakes.GetNextCoord(i, io_lakes.GetSize())) {
        if (io_lakes[i] == TerrainType::Lake) {
          continue;
        }

        int adjacentLandCount = 0;

        for (int j = 0; j < int(Direction::Count); ++j) {
          ivec2 const adjDir = GetIntVectorFromDirection(Direction(j));

          ivec2 const adjLocation = i + adjDir;

          if (adjLocation.x < 0 || adjLocation.x >= io_lakes.GetSize().x || adjLocation.y < 0 || adjLocation.y >= io_lakes.GetSize().y) {
            continue;
          }

          if (io_lakes[adjLocation] != TerrainType::Lake) {
            ++adjacentLandCount;
          }
        }

        if (adjacentLandCount <= 2) {
          io_lakes[i] = TerrainType::Lake;
          noChange = false;
        }
      }
    }
  }

  void DestroyIslands(Grid<TerrainType, 2> & io_lakes) {
    Grid<int, 2>     associations = Grid<int, 2>(io_lakes.GetSize(), c_invalid);
    std::vector<int> islandSizes;

    for (ivec2 i; i != io_lakes.GetSize(); i = io_lakes.GetNextCoord(i, io_lakes.GetSize())) {
      if (io_lakes[i] == TerrainType::Lake) {
        continue;
      }

      DynamicArray<int, int(Direction::Count)> neighborIslands;

      for (int j = 0; j < int(Direction::Count); ++j) {
        ivec2 const adjDir = GetIntVectorFromDirection(Direction(j));
        ivec2 const adjLocation = i + adjDir;

        if (adjLocation.x < 0 || adjLocation.x >= io_lakes.GetSize().x || adjLocation.y < 0 || adjLocation.y >= io_lakes.GetSize().y) {
          continue;
        }

        if (associations[adjLocation] != c_invalid) {
          neighborIslands.Emplace(associations[adjLocation]);
        }
      }

      if (neighborIslands.Empty()) {
        associations[i] = islandSizes.size();
        islandSizes.emplace_back(1);
      }
      else {
        int const keepIndex = neighborIslands[0];

        for (int j = 1; j < neighborIslands.Count(); ++j) {
          if (neighborIslands[j] != keepIndex) {
            int const oldIslandIndex = neighborIslands[j];

            for (ivec2 k; k != i; k = io_lakes.GetNextCoord(k, io_lakes.GetSize())) {
              if (associations[k] == oldIslandIndex) {
                associations[k] = keepIndex;
              }
            }

            islandSizes[keepIndex] += islandSizes[oldIslandIndex];
            islandSizes[oldIslandIndex] = 0;
          }
        }

        associations[i] = keepIndex;
        ++islandSizes[keepIndex];
      }
    }

    if (islandSizes.empty()) {
      return;
    }

    int biggestIsland     = 0;
    int biggestIslandSize = islandSizes[0];
    for (int i = 1; i < islandSizes.size(); ++i) {
      if (islandSizes[i] > biggestIslandSize) {
        biggestIsland     = i;
        biggestIslandSize = islandSizes[i];
      }
    }

    for (ivec2 i; i != io_lakes.GetSize(); i = io_lakes.GetNextCoord(i, io_lakes.GetSize())) {
      if (associations[i] != biggestIsland) {
        io_lakes[i] = TerrainType::Lake;
      }
    }
  }

  Grid<TerrainType, 2> GenerateMountainRanges(ivec2 const & size, MountainRangeInfo const & rangeInfo) {
    Grid<TerrainType, 2> result = Grid<TerrainType, 2>(size, TerrainType::Plains);

    for (int i = 0; i < rangeInfo.count; ++i) {
      GenerateMountainRange(size, rangeInfo, result);
    }

    return result;
  }

  Grid<TerrainType, 2> GenerateForests(ivec2 const & size, ForestInfo const & forestInfo) {
    Grid<TerrainType, 2> result = Grid<TerrainType, 2>(size, TerrainType::Plains);

    PerlinNoise noise(forestInfo.detail);

    for (ivec2 index; index != size; index = result.GetNextCoord(index, size)) {
      fvec2 const noiseLocation = fvec2(index) / fvec2(size - ivec2(1, 1));

      float const roughnessShift = (GetRandomValue() - 0.5f) * forestInfo.roughness;

      if (noise.GetValue(noiseLocation) + roughnessShift < forestInfo.density) {
        result[index] = TerrainType::Forest;
      }
    }

    return result;
  }

  Grid<TerrainType, 2> GenerateLakes(ivec2 const & size, LakeInfo const & lakeInfo) {
    Grid<TerrainType, 2> result = Grid<TerrainType, 2>(size, TerrainType::Plains);

    for (int i = 0; i < lakeInfo.count; ++i) {
      GenerateLake(size, lakeInfo, result);
    }

    CleanLakeEdges(result);

    DestroyIslands(result);

    return result;
  }

  void ApplyTerrain(TerrainType type, Grid<TerrainType, 2> const & newTerrain, Grid<TerrainType, 2> & io_terrain) {
    for (int i = 0; i < io_terrain.Count(); ++i) {
      if (newTerrain.Data()[i] == type) {
        io_terrain.Data()[i] = type;
      }
    }
  }

  Grid<TerrainType, 2> GenerateMapTerrain(ivec2 const & size, GameInfo const & info ) {
    Grid<TerrainType, 2> result(size, TerrainType::Plains);

    auto const forests = GenerateForests(size, info.generation.forests);
    ApplyTerrain(TerrainType::Forest, forests, result);

    auto const mountains = GenerateMountainRanges(size, info.generation.mountainRanges);
    ApplyTerrain(TerrainType::Mountain, mountains, result);

    auto const lakes = GenerateLakes(size, info.generation.lakes);
    ApplyTerrain(TerrainType::Lake, lakes, result);

    return result;
  };

  std::vector<ivec2> GenerateCityLocations(CityInfo const & cityInfo, Grid<TerrainType, 2> const & terrain) {
    std::vector<ivec2> possibleCityLocations;

    for (ivec2 i; i != terrain.GetSize(); i = terrain.GetNextCoord(i, terrain.GetSize())) {
      if (terrain[i] == TerrainType::Lake) {
        continue;
      }

      bool hasLocation[int(TerrainType::Count)] = { 0 };

      for (int j = 0; j < int(Direction::Count); ++j) {
        if (Direction(j) == Direction::Local) {
          continue;
        }

        ivec2 const adjDir = GetIntVectorFromDirection(Direction(j));
        ivec2 const adjLocation = i + adjDir;

        if (adjLocation.x < 0 || adjLocation.x >= terrain.GetSize().x || adjLocation.y < 0 || adjLocation.y >= terrain.GetSize().y) {
          continue;
        }

        hasLocation[int(terrain[adjLocation])] = true;
      }

      int uniqueAdjacents = 0;

      for (int j = 0; j < int(TerrainType::Count); ++j) {
        if (hasLocation[j]) {
          ++uniqueAdjacents;
        }
      }

      if (uniqueAdjacents >= 3) {
        possibleCityLocations.emplace_back(i);
      }
    }

    std::vector<ivec2> chosenCityLocations;

    while (chosenCityLocations.size() < cityInfo.maxCount && !possibleCityLocations.empty()) {
      int const chosenLocationIndex = std::rand() % possibleCityLocations.size();

      ivec2 const cityLocation = possibleCityLocations[chosenLocationIndex];
      possibleCityLocations[chosenLocationIndex] = possibleCityLocations.back();
      possibleCityLocations.pop_back();

      bool tooClose = false;
      for (ivec2 const & location : chosenCityLocations) {
        int const dist = std::abs(location.x - cityLocation.x) + std::abs(location.y - cityLocation.y);

        if (dist < cityInfo.requiredDistance) {
          tooClose = true;
          break;
        }
      }

      if (!tooClose) {
        chosenCityLocations.emplace_back(cityLocation);
      }
    }

    return chosenCityLocations;
  }

  BuildingIndex GetUniqueTownCenter(std::vector<BuildingInfo> const & buildings, std::vector<FactionIndex> const & consumedFactions) {
    for (BuildingIndex i = 0; i < buildings.size(); ++i) {
      BuildingInfo const & building = buildings[i];

      if (building.type != StructureType::TownCenter) {
        continue;
      }

      bool isBlockedFaction = false;
      for (FactionIndex faction : consumedFactions) {
        if (building.faction == faction) {
          isBlockedFaction = true;
          break;
        }
      }

      if (isBlockedFaction) {
        continue;
      }

      return i;
    }

    return c_invalid;
  }

  Grid<CellData, 2> GenerateMap(ivec2 const & size, GameInfo const & info) {
    Grid<CellData, 2> result = Grid<CellData, 2>(size);

    while (true) {
      Grid<TerrainType, 2> const terrain = GenerateMapTerrain(size, info);

      std::vector<ivec2> const cityLocations = GenerateCityLocations(info.generation.cities, terrain);

      if (cityLocations.size() >= info.generation.cities.minCount) {
        std::vector<FactionIndex> placedFactions;

        for (ivec2 i; i != size; i = result.GetNextCoord(i, size)) {
          BuildingIndex const townCenter = GetUniqueTownCenter(info.buildings, placedFactions);

          if (townCenter == c_invalid) {
            break;
          }

          placedFactions.emplace_back(info.buildings[townCenter].faction);

          result[i].terrain    = terrain[i];
          result[i].beingBuilt = c_invalid;
          result[i].buildings.emplace_back(townCenter);
        }

        break;
      }
    }


  }
}

int main(void) {
  std::srand(std::time(nullptr));

  auto window = std::make_shared<AsciiWindow>();

  window->SetTitle("Idle");

  AsciiFont font;

  font.size = ivec2(12, 12);
  font.colors[0] = Color::Black;
  font.colors[1] = Color::White;
  font.colors[2] = Color::Red;
  font.colors[3] = Color::Green;
  font.colors[4] = Color::Yellow;
  font.colors[5] = Color::Blue;
  font.colors[6] = Color::Grey;

  window->SetFont(font);

  auto updateManager = std::make_shared<UpdateManager>(window, 0.1f, 1.0f / 60.0f);
  auto inputManager  = std::make_shared<InputManager>(window);

  GameInfo info;

  info.generation.mountainRanges.count              = 5;
  info.generation.mountainRanges.thickness          = 7;
  info.generation.mountainRanges.length             = 200;
  info.generation.mountainRanges.rangeStartPadding  = 8;
  info.generation.mountainRanges.baseWalkWeight     = 0.5f;
  info.generation.mountainRanges.dirWalkWeight      = 6.0f;
  info.generation.mountainRanges.density            = 0.7f;

  info.generation.forests.density   = 0.45f;
  info.generation.forests.detail    = 4;
  info.generation.forests.roughness = 0.05f;

  info.generation.lakes.count       = 4;
  info.generation.lakes.maxBaseSize = 1000;
  info.generation.lakes.minBaseSize = 100;

  info.generation.cities.minCount         = 4;
  info.generation.cities.maxCount         = 7;
  info.generation.cities.requiredDistance = 20;

  Grid<AsciiCell, 2> screen;

  auto GenerateMap = [&](void) {
    auto terrain = GenerateMapTerrain(ivec2(80, 80), info);

    screen.Clear();
    screen.SetSize(terrain.GetSize());

    for (ivec2 i; i != terrain.GetSize(); i = terrain.GetNextCoord(i, terrain.GetSize())) {
      screen[i].backgroundColor = 0;

      switch (terrain[i]) {
        case TerrainType::Mountain: {
          screen[i].character = '^';
          screen[i].foregroundColor = 2;
        } break;
        case TerrainType::Forest: {
          screen[i].character = '0';
          screen[i].foregroundColor = 3;
        } break;
        case TerrainType::Lake: {
          screen[i].character = '~';
          screen[i].foregroundColor = 5;
        } break;
        case TerrainType::Plains: {
          screen[i].character = '.';
          screen[i].foregroundColor = 4;
        } break;
        default: {
          screen[i].character = ' ';
          screen[i].foregroundColor = 1;
        } break;
      };
    }
  };

  GenerateMap();

  auto onSpace = inputManager->GetButtonManager()->AddButtonEvent(AsciiButton::Space, [&](bool down) {
    if (!down) {
      GenerateMap();
    }
  });

  bool shouldEnd = false;

  auto exitDelegate = inputManager->GetButtonManager()->AddButtonEvent(AsciiButton::Escape, [&](bool down) {
    if (down) {
      shouldEnd = true;
    }
  });
  auto drawDelegate = updateManager->AddOnDynamicUpdate([&](float dt, float) {
    window->Draw(screen);
  });

  auto inputDelegate = updateManager->AddOnDynamicUpdate([&](float dt, float) {
    inputManager->ProcessInput();
  });

  while (!shouldEnd) {
    updateManager->Update();
  }
}

