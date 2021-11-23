/*
 * Copywrite 2021 Dodge Lafnitzegger
 */

#include <functional>
#include <map>
#include <queue>

#include "Containers/DynamicArray.h"
#include "Math/Vector.h"
#include "Window/Window.h"

namespace {
  static int const   c_menuWidth            = 3;
  static int const   c_width                = 80;
  static int const   c_height               = 80;
  static ivec2 const c_bufferSize           = ivec2(c_width, c_height);
  static char const  c_seperatorChar        = '|';
  static ivec2 const c_menuSize             = ivec2(c_menuWidth, c_height);

  static int const   c_backgroundColorIndex = 0;
  static int const   c_utilityColorIndex    = 1;

  static int const   c_unsetValue           = -1;
  static int const   c_externalValue        = -2;

  static int const   c_paintCount                 = 6;
  static int const   c_paintIndices[c_paintCount] = {
    2,
    3,
    4,
    5,
    6,
    7,
  };

  static int        c_paintToValueMap[16] = { 0 };
  static bool const c_paintToValueMapSet  = []() {
    for (int i = 0; i < c_paintCount; ++i) {
      c_paintToValueMap[c_paintIndices[i]] = i;
    }

    return true;
  }();

  static int const   c_checkZoneRadius = 1;
  static int const   c_checkZoneWidth  = c_checkZoneRadius * 2 + 1;
  static ivec2 const c_checkZoneSize   = ivec2(c_checkZoneWidth, c_checkZoneWidth);
  static ivec2 const c_checkZoneOffset = ivec2(-c_checkZoneRadius, -c_checkZoneRadius);
  static int const   c_evalKeySize     = c_checkZoneWidth * c_checkZoneWidth - 1;

  static int const   c_iterationMagChange    = 10;
  static int const   c_maxIterationMagnitude = 5;

  ivec2 GetBufferTopLeft(int bufferIndex) {
    int const leftBoundary = c_menuWidth + 1 + bufferIndex * (c_width + 1);

    return ivec2(leftBoundary, 0);
  }

  Grid<AsciiCell, 2> SetBuffer(
    Grid<AsciiCell, 2> const & screen,
    int                        bufferIndex,
    Grid<AsciiCell, 2> const & buffer
  ) {
    Grid<AsciiCell, 2> result = screen;

    ivec2 const c_bufferSize = ivec2(c_width, c_height);

    for (ivec2 i; i != c_bufferSize; i = result.GetNextCoord(i, c_bufferSize)) {
      ivec2 const coord = GetBufferTopLeft(bufferIndex) + i;

      result[coord] = buffer[i];
    }

    return result;
  }

  Grid<AsciiCell, 2> GetBuffer(
    Grid<AsciiCell, 2> const & screen,
    int                        bufferIndex
  ) {
    Grid<AsciiCell, 2> result(ivec2(c_width, c_height));

    for (ivec2 i; i != c_bufferSize; i = result.GetNextCoord(i, c_bufferSize)) {
      ivec2 const coord = GetBufferTopLeft(bufferIndex) + i;

      result[i] = screen[coord];
    }

    return result;
  }

  Grid<int, 2> GetEvalGridFromBuffer(Grid<AsciiCell, 2> const & buffer) {
    Grid<int, 2> result(buffer.GetSize());

    for (ivec2 i; i != buffer.GetSize(); i = buffer.GetNextCoord(i, buffer.GetSize())) {
      if (buffer[i].backgroundColor == c_backgroundColorIndex) {
        result[i] = c_unsetValue;
      }
      else {
        result[i] = c_paintToValueMap[buffer[i].backgroundColor];
      }
    }

    return result;
  }

  Grid<AsciiCell, 2> GetBufferFromEvalGrid(Grid<int, 2> const & evalGrid) {
    Grid<AsciiCell, 2> result(evalGrid.GetSize());

    for (ivec2 i; i != evalGrid.GetSize(); i = evalGrid.GetNextCoord(i, evalGrid.GetSize())) {

      if (evalGrid[i] == c_unsetValue) {
        result[i].backgroundColor = c_backgroundColorIndex;
      }
      else {
        result[i].backgroundColor = c_paintIndices[evalGrid[i]];
      }

      result[i].foregroundColor = c_utilityColorIndex;
      result[i].character       = ' ';
    }

    return result;
  }

  using EvalKey = ivec<c_evalKeySize>;

  EvalKey GetEvalKeyForSpot(ivec2 const & location, Grid<int, 2> const & evalGrid) {
    EvalKey result;

    int keyIndex = 0;
    for (
      ivec2 i;
      i != c_checkZoneSize && keyIndex != c_evalKeySize;
      i = evalGrid.GetNextCoord(i, c_checkZoneSize)
    ) {
      ivec2 const checkLocation = location + c_checkZoneOffset + i;

      if (c_checkZoneOffset + i == ivec2(0, 0)) {
        continue;
      }

      if (
        checkLocation.x < 0 ||
        checkLocation.x >= evalGrid.GetSize().x ||
        checkLocation.y < 0 ||
        checkLocation.y >= evalGrid.GetSize().y
      ) {
        result[keyIndex] = c_externalValue;
      }
      else {
        result[keyIndex] = evalGrid[checkLocation];
      }

      ++keyIndex;
    }

    return result;
  }

  using EvalStore = ivec<c_paintCount>;

  using EvalMap = std::map<EvalKey, EvalStore, std::function<bool(EvalKey const &, EvalKey const &)>>;

  /*
  EvalKeyGroup GetGroupFromKey(EvalKey const & key) {
    EvalKeyGroup result;

    result.x = ivec4(key[0], key[1], key[2], 0);
    result.y = ivec4(key[2], key[4], key[7], 1);
    result.z = ivec4(key[0], key[3], key[5], 2);
    result.w = ivec4(key[5], key[6], key[7], 3);

    return result;
  }
  */

  EvalMap GetRateMapping(Grid<int, 2> const & evalGrid) {
    EvalMap mapping([](EvalKey const & key0, EvalKey const & key1) -> bool {
      for (int i = 0; i < EvalKey::Size; ++i) {
        if (key0[i] < key1[i]) {
          return true;
        }
        else if (key0[i] > key1[i]) {
          return false;
        }
      }

      return false;
    });

    for (ivec2 i; i != evalGrid.GetSize(); i = evalGrid.GetNextCoord(i, evalGrid.GetSize())) {
      EvalKey const key = GetEvalKeyForSpot(i, evalGrid);

      if (!mapping.contains(key)) {
        mapping.emplace(std::make_pair(key, EvalStore()));
      }

      ++mapping[key][evalGrid[i]];
    }

    return mapping;
  }

  bool CanMatchEvalKey(EvalKey const & key0, EvalKey const & key1) {
    for (int i = 0; i < EvalKey::Size; ++i) {
      if ((key0[i] == c_unsetValue && key1[i] != c_externalValue) || (key1[i] == c_unsetValue && key0[i] != c_externalValue)) {
        continue;
      }

      if (key0[i] != key1[i]) {
        return false;
      }
    }

    return true;
  }

  EvalStore GetEvalMatches(EvalKey const & key, EvalMap const & map) {
    EvalStore result;

    for (auto const & mapping : map) {
      if (CanMatchEvalKey(key, mapping.first)) {
        result += mapping.second;
      }
    }

    return result;
  }

  struct QueueData {
    QueueData(void) = default;
    QueueData(ivec2 const & location, int uncertainty) :
      location(location),
      uncertainty(uncertainty)
    {}

    bool operator <(QueueData const & data) const {
      return uncertainty > data.uncertainty;
    }

    ivec2 location;
    int   uncertainty = -1;
  };

  struct RunningEvalData {
    Grid<int, 2>                   grid;
    EvalMap                        mapping;
    std::priority_queue<QueueData> workQueue;
    bool                           succeeded;
  };

  RunningEvalData InitializeEvalData(Grid<int, 2> const & evalGrid) {
    RunningEvalData result;

    result.grid      = Grid<int, 2>(evalGrid.GetSize(), c_unsetValue);
    result.mapping   = GetRateMapping(evalGrid);
    result.succeeded = false;

    result.workQueue.emplace(ivec2(0,                        0),                        0);
    result.workQueue.emplace(ivec2(evalGrid.GetSize().x - 1, 0),                        0);
    result.workQueue.emplace(ivec2(0,                        evalGrid.GetSize().y - 1), 0);
    result.workQueue.emplace(ivec2(evalGrid.GetSize().x - 1, evalGrid.GetSize().y - 1), 0);

    return result;
  }

  int GetRandomWeightedIndex(EvalStore const & store, Vector<bool, c_paintCount> const & invalidPaints) {
    int resultIndex = c_unsetValue;
    int totalWeight = 0;

    for (int i = 0; i < store.Size; ++i) {
      if (invalidPaints[i]) {
        continue;
      }

      totalWeight += store[i];

      if (totalWeight != 0) {
        if (std::rand() % totalWeight < store[i]) {
          resultIndex = i;
        }
      }
    }

    return resultIndex;
  }

  int GetUncertainty(EvalKey const & key, EvalStore const & store) {
    int const totalWeight = store.Sum();

    int optionCount = 0;
    for (int i = 0; i < store.Size; ++i) {
      if (store[i] != 0) {
        ++optionCount;
      }
    }

    int uncertainSurroundingCount = 0;
    for (int i = 0; i < key.Size; ++i) {
      if (key[i] == c_unsetValue) {
        ++uncertainSurroundingCount;
      }
    }

    int const result =
      optionCount *
      uncertainSurroundingCount *
    1;

    return result;
  }

  bool BuildAnotherEvalGrid(RunningEvalData & evaluation, int iterations) {
    for (int iteration = 0; iteration < iterations && !evaluation.workQueue.empty(); ++iteration) {
      ivec2 const evalLocation = evaluation.workQueue.top().location;
      evaluation.workQueue.pop();

      if (evaluation.grid[evalLocation] != c_unsetValue) {
        --iteration;
        continue;
      }

      EvalKey   key     = GetEvalKeyForSpot(evalLocation, evaluation.grid);
      EvalStore matches = GetEvalMatches(key, evaluation.mapping);

      if (matches.Sum() == 0) {
        return false;
      }

      bool repeat;
      int  repeatCount = 0;
      Vector<bool, c_paintCount> invalidPaints;

      do {
        repeat = false;

        int const newIndex = GetRandomWeightedIndex(matches, invalidPaints);

        if (newIndex == -1) {
          repeatCount = c_paintCount;
          break;
        }

        evaluation.grid[evalLocation] = newIndex;

        for (
          ivec2 i;
          i != c_checkZoneSize;
          i = Grid<int, 2>::GetNextCoord(i, c_checkZoneSize)
        ) {
          if (c_checkZoneOffset + i == ivec2(0, 0)) {
            continue;
          }

          ivec2 const checkLocation = evalLocation + c_checkZoneOffset + i;

          if (
            checkLocation.x < 0 ||
            checkLocation.x >= evaluation.grid.GetSize().x ||
            checkLocation.y < 0 ||
            checkLocation.y >= evaluation.grid.GetSize().y
          ) {
            continue;
          }

          if (evaluation.grid[checkLocation] == c_unsetValue) {
            continue;
          }

          EvalKey   oldKey     = GetEvalKeyForSpot(checkLocation, evaluation.grid);
          EvalStore oldMatches = GetEvalMatches(oldKey, evaluation.mapping);

          if (oldMatches[evaluation.grid[checkLocation]] == 0) {
            evaluation.grid[evalLocation] = c_unsetValue;
            invalidPaints[newIndex] = true;
            ++repeatCount;
            repeat = true;
            break;
          }
        }

      } while (repeat && repeatCount < c_paintCount);

      if (repeatCount == c_paintCount) {
        return false;
      }

      for (
        ivec2 i;
        i != c_checkZoneSize;
        i = Grid<int, 2>::GetNextCoord(i, c_checkZoneSize)
      ) {
        ivec2 const checkLocation = evalLocation + c_checkZoneOffset + i;

        if (checkLocation == evalLocation) {
          continue;
        }

        if (
          checkLocation.x < 0 ||
          checkLocation.x >= evaluation.grid.GetSize().x ||
          checkLocation.y < 0 ||
          checkLocation.y >= evaluation.grid.GetSize().y
        ) {
          continue;
        }

        if (evaluation.grid[checkLocation] != c_unsetValue) {
          continue;
        }

        EvalKey   newKey         = GetEvalKeyForSpot(checkLocation, evaluation.grid);
        EvalStore newMatches     = GetEvalMatches(newKey, evaluation.mapping);
        int       newUncertainty = GetUncertainty(newKey, newMatches);

        evaluation.workQueue.emplace(checkLocation, newUncertainty);
      }
    }

    if (evaluation.workQueue.empty()) {
      for (int cellVal : evaluation.grid) {
        if (cellVal == c_unsetValue) {
          return false;
        }
      }

      evaluation.succeeded = true;
      return false;
    }

    return true;
  }
}

int main(void) {
  AsciiWindow window;

  AsciiFont font;
  font.size = ivec2(8, 8);

  window.SetFont(font);

  int currentPaint = 0;
  int paintSize    = 0;

  Grid<AsciiCell, 2> screen(
    ivec2(c_width * 2 + 2 + c_menuWidth, c_height),
    AsciiCell(c_utilityColorIndex, c_paintIndices[currentPaint])
  );

  Grid<int, 2> screenPainMask(ivec2(c_width * 2 + 2 + c_menuWidth, c_height), true);

  int const seperator0X = c_menuWidth;
  int const seperator1X = c_menuWidth + 1 + c_width;

  for (int i = 0; i < c_height; ++i) {
    screen[ivec2(seperator0X, i)].character       = c_seperatorChar;
    screen[ivec2(seperator0X, i)].foregroundColor = c_utilityColorIndex;
    screen[ivec2(seperator0X, i)].backgroundColor = c_backgroundColorIndex;
    screen[ivec2(seperator1X, i)].character       = c_seperatorChar;
    screen[ivec2(seperator1X, i)].foregroundColor = c_utilityColorIndex;
    screen[ivec2(seperator1X, i)].backgroundColor = c_backgroundColorIndex;

    screenPainMask[ivec2(seperator0X, i)] = false;
    screenPainMask[ivec2(seperator1X, i)] = false;
  }

  for (ivec2 i; i != c_menuSize; i = screenPainMask.GetNextCoord(i, c_menuSize)) {
    screenPainMask[i] = false;
    screen[i].foregroundColor = c_utilityColorIndex;
    screen[i].backgroundColor = c_backgroundColorIndex;
  }

  ivec2 mousePos;
  bool  mouseDown = false;

  RunningEvalData runData;
  runData.succeeded = true;

  int  fromBuffer         = 0;
  int  toBuffer           = 1;
  bool evaluating         = false;
  int  iterations         = 10;
  int  iterationMagnitude = 1;

  while (true) {
    std::vector<AsciiInputEvent> input = window.PollInput();

    for (AsciiInputEvent const & event : input) {
      switch (event.type) {
        case AsciiInputType::MousePosition: {
          mousePos = event.mousePositionEvent;
        } break;

        case AsciiInputType::Button: {
          if (event.buttonEvent.button == AsciiButton::Mouse1) {
            mouseDown = event.buttonEvent.isDown;
          }
          else if (event.buttonEvent.isDown) {
            switch (event.buttonEvent.button) {
              case AsciiButton::Q: {
                  currentPaint = 0;
              } break;
              case AsciiButton::W: {
                  currentPaint = 1;
              } break;
              case AsciiButton::E: {
                  currentPaint = 2;
              } break;
              case AsciiButton::A: {
                  currentPaint = 3;
              } break;
              case AsciiButton::S: {
                  currentPaint = 4;
              } break;
              case AsciiButton::D: {
                  currentPaint = 5;
              } break;

              case AsciiButton::Key1: {
                paintSize = 0;
              } break;
              case AsciiButton::Key2: {
                paintSize = 1;
              } break;
              case AsciiButton::Key3: {
                paintSize = 2;
              } break;
              case AsciiButton::Key4: {
                paintSize = 3;
              } break;

              case AsciiButton::ForwardSlash: {
                evaluating = false;
              } break;

              case AsciiButton::Comma: {
                if (iterationMagnitude > 0) {
                  --iterationMagnitude;
                  iterations /= c_iterationMagChange;
                }
              } break;
              case AsciiButton::Period: {
                if (iterationMagnitude < c_maxIterationMagnitude) {
                  ++iterationMagnitude;
                  iterations *= c_iterationMagChange;
                }
              } break;

              case AsciiButton::Z: {
                if (!evaluating) {
                  auto const evalGrid = GetEvalGridFromBuffer(GetBuffer(screen, 1));

                  fromBuffer = 1;
                  toBuffer   = 0;
                  runData    = InitializeEvalData(evalGrid);
                  evaluating = true;
                }
              } break;
              case AsciiButton::X: {
                if (!evaluating) {
                  auto const evalGrid = GetEvalGridFromBuffer(GetBuffer(screen, 0));

                  fromBuffer = 0;
                  toBuffer   = 1;
                  runData    = InitializeEvalData(evalGrid);
                  evaluating = true;
                }
              } break;
            }
          }
        } break;
      }
    }

    if (mouseDown && !evaluating) {
      int const   paintZoneWidth  = 1 + 2 * paintSize;
      ivec2 const paintZoneSize   = ivec2(paintZoneWidth, paintZoneWidth);
      ivec2 const paintZoneOffset = ivec2(-paintSize, -paintSize);

      for (ivec2 i; i != paintZoneSize; i = Grid<int, 2>::GetNextCoord(i, paintZoneSize)) {
        ivec2 const drawPos = mousePos + paintZoneOffset + i;

        if (drawPos.x >= 0 && drawPos.x < screen.GetSize().x && drawPos.y >= 0 && drawPos.y < screen.GetSize().y) {
          if (screenPainMask[drawPos]) {
            screen[drawPos].backgroundColor = c_paintIndices[currentPaint];
          }
        }
      }
    }

    if (evaluating) {
      bool const shouldContinue = BuildAnotherEvalGrid(runData, iterations);

      if (!shouldContinue) {
        if (runData.succeeded) {
          evaluating = false;
        }
        else {
          auto const evalGrid = GetEvalGridFromBuffer(GetBuffer(screen, fromBuffer));

          runData    = InitializeEvalData(evalGrid);
          evaluating = true;
        }
      }

      screen = SetBuffer(screen, toBuffer, GetBufferFromEvalGrid(runData.grid));

      if (!runData.workQueue.empty()) {
        ivec2 const workLocation = runData.workQueue.top().location;

        ivec2 const workDrawLoc = GetBufferTopLeft(toBuffer) + workLocation;

        screen[workDrawLoc].character = 'X';
      }
    }

    screen[ivec2(0, 1)].character = '[';
    screen[ivec2(0, 1)].foregroundColor = c_utilityColorIndex;
    screen[ivec2(1, 1)].backgroundColor = c_paintIndices[currentPaint];
    screen[ivec2(2, 1)].character = ']';
    screen[ivec2(2, 1)].foregroundColor = c_utilityColorIndex;

    screen[ivec2(0, 3)].character = '[';
    screen[ivec2(0, 3)].foregroundColor = c_utilityColorIndex;
    screen[ivec2(1, 3)].character = '1' + paintSize;
    screen[ivec2(1, 3)].foregroundColor = c_utilityColorIndex;
    screen[ivec2(2, 3)].character = ']';
    screen[ivec2(2, 3)].foregroundColor = c_utilityColorIndex;

    screen[ivec2(0, 5)].character = '[';
    screen[ivec2(0, 5)].foregroundColor = c_utilityColorIndex;
    screen[ivec2(1, 5)].character = (evaluating || runData.succeeded) ? 'X' : ' ';
    screen[ivec2(1, 5)].foregroundColor = c_utilityColorIndex;
    screen[ivec2(2, 5)].character = ']';
    screen[ivec2(2, 5)].foregroundColor = c_utilityColorIndex;

    screen[ivec2(0, 7)].character = '[';
    screen[ivec2(0, 7)].foregroundColor = c_utilityColorIndex;
    screen[ivec2(1, 7)].character = '1' + iterationMagnitude - 1;
    screen[ivec2(1, 7)].foregroundColor = c_utilityColorIndex;
    screen[ivec2(2, 7)].character = ']';
    screen[ivec2(2, 7)].foregroundColor = c_utilityColorIndex;

    window.Draw(screen);

    window.Sleep(1);
  }
}
