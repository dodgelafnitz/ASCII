/*
 * Copywrite 2021 Dodge Lafnitzegger
 */

#include <coroutine>
#include <cstdlib>
#include <ctime>

#include "Containers/Grid.h"
#include "Math/Shapes2D/Collision.h"
#include "Window/Window.h"

namespace {
  int const s_width    = 100;
  int const s_height   = 100;
  int const s_cellSize = 8;

  int const s_wallDist = 1;

  Color const s_rectColor      = Color::Red;
  Color const s_circleColor    = Color::Green;
  Color const s_bothColor      = Color::Yellow;
  Color const s_intersectColor = Color::Blue;
  Color const s_poiColor       = Color::Pink;

  int const s_rectColorIndex      = 1;
  int const s_circleColorIndex    = 2;
  int const s_bothColorIndex      = 3;
  int const s_intersectColorIndex = 4;
  int const s_poiColorIndex       = 5;

  ivec2 GetRandomPoint(void) {
    return ivec2(
      std::rand() % (s_width - s_wallDist) + s_wallDist,
      std::rand() % (s_height - s_wallDist) + s_wallDist
    );
  }

  Rect GetRandomRect(void) {
    ivec2 const point = GetRandomPoint();

    int const maxWidth  = s_width / 2;
    int const maxHeight = s_height / 2;

    ivec2 const size(std::rand() % maxWidth, std::rand() % maxHeight);

    return Rect(point, size);
  }

  Circle GetRandomCircle(void) {
    ivec2 const point = GetRandomPoint();

    int const maxRadius  = std::min(s_width / 2, s_height / 2);

    return Circle(point, std::rand() % maxRadius);
  }

  LineSegment GetRandomLineSegment(void) {
    return LineSegment(GetRandomPoint(), GetRandomPoint());
  }

  static int const c_poiCount = 20;

  void SetPois(Circle const & circle, Rect const & rect, ivec2(&o_pois)[c_poiCount], int & o_poiCount) {
    o_poiCount = 0;

    fvec2 const intersect = rect.Clamp(circle.GetPosition());

    if (circle.Contains(intersect)) {
      fvec2 const halfSize = rect.GetDimensions() * 0.5f;
      fvec2 const offHalfSize = fvec2(halfSize.x, -halfSize.y);

      fvec2 const corners[] = {
        rect.GetCenter() + halfSize,
        rect.GetCenter() + offHalfSize,
        rect.GetCenter() - halfSize,
        rect.GetCenter() - offHalfSize,
      };

      int const edges[][2] = {
        { 0, 1 },
        { 1, 2 },
        { 2, 3 },
        { 3, 0 },
        { 0, 3 },
        { 3, 2 },
        { 2, 1 },
        { 1, 0 },
      };

      for (int i = 0; i < sizeof(edges) / sizeof(*edges); ++i) {
        LineSegment const edge(corners[edges[i][0]], corners[edges[i][1]]);

        Intersection2D intersection = Intersect(circle, edge);

        if (intersection.intersects) {
          o_pois[o_poiCount++] = intersection.point + 0.5f;
        }
      }

      fvec2 const containedCircleExtents[] = {
        circle.GetPosition() + fvec2(circle.GetRadius(),  0.0f),
        circle.GetPosition() + fvec2(-circle.GetRadius(), 0.0f),
        circle.GetPosition() + fvec2(0.0f,                circle.GetRadius()),
        circle.GetPosition() + fvec2(0.0f,                -circle.GetRadius()),
      };

      for (int i = 0; i < sizeof(containedCircleExtents) / sizeof(*containedCircleExtents); ++i) {
        if (rect.Contains(containedCircleExtents[i])) {
          o_pois[o_poiCount++] = containedCircleExtents[i] + 0.5f;
        }
      }
    }
  }

  int s_delta = 0;

  float GetT(void) {
    float val = std::sin(s_delta / 10.0f);

    return val * val;
  }

  Line GetRandomLine(void) {
    return GetRandomLineSegment().GetLine();
  }

  int test_lines(AsciiWindow & window) {
    Line           line0;
    Line           line1;
    Intersection2D intersection;

    do {
      line0 = GetRandomLine();
      line1 = GetRandomLine();
      intersection = Intersect(line0, line1);

    } while (!intersection.intersects);

    while (true) {
      auto input = window.PollInput();

      for (auto const & inputKey : input) {
        if (inputKey.type == AsciiInputType::Button) {
          if (inputKey.buttonEvent.button == AsciiButton::Space && inputKey.buttonEvent.isDown) {
            do {
              line0 = GetRandomLine();
              line1 = GetRandomLine();
              intersection = Intersect(line0, line1);

            } while (!intersection.intersects);
          }
        }
      }

      intersection = Intersect(line0, line1);

      Grid<AsciiCell, 2> screen(ivec2(s_width, s_height));

      for (int i = 0; i < s_height; ++i) {
        for (int j = 0; j < s_width; ++j) {
          ivec2 const location(j, i);

          bool const inLine0 = line0.DistanceTo(location) <= 0.5f;
          bool const inLine1 = line1.DistanceTo(location) <= 0.5f;
          bool const isIntersect = intersection.intersects && ivec2(intersection.point + 0.5f) == location;

          if (!inLine0 && !inLine1) {
            continue;
          }

          int colorIndex = 0;

          if (isIntersect) {
            colorIndex = s_intersectColorIndex;
          }
          else if (inLine0) {
            colorIndex = s_circleColorIndex;
          }
          else if (inLine1) {
            colorIndex = s_rectColorIndex;
          }
          screen[location].foregroundColor = colorIndex;
          screen[location].character = isIntersect ? 'x' : '.';
        }
      }

      window.Draw(screen);

      window.Sleep(5);
      s_delta += 1;
    }
  }

  int test_shapes(AsciiWindow & window) {
    Rect           currentRect;
    Circle         currentCircle;
    LineSegment    currentLineSegment;
    Intersection2D intersection;
    ivec2          pois[c_poiCount];
    int            poiCount = 0;

    do {
      currentRect = GetRandomRect();
      //currentLineSegment = GetRandomLineSegment();
      currentCircle = GetRandomCircle();
      intersection = Intersect(currentCircle, currentRect);
      //intersection       = Intersect(currentCircle, currentLineSegment);

      SetPois(currentCircle, currentRect, pois, poiCount);
    } while (!intersection.intersects);

    while (true) {
      auto input = window.PollInput();

      for (auto const & inputKey : input) {
        if (inputKey.type == AsciiInputType::Button) {
          if (inputKey.buttonEvent.button == AsciiButton::Space && inputKey.buttonEvent.isDown) {
            do {
              currentRect = GetRandomRect();
              //currentLineSegment = GetRandomLineSegment();
              currentCircle = GetRandomCircle();
              intersection = Intersect(currentCircle, currentRect);
            } while (!intersection.intersects);
          }
        }
      }

      intersection = Intersect(currentCircle, currentRect);
      //intersection = Intersect(currentCircle, currentLineSegment);
      SetPois(currentCircle, currentRect, pois, poiCount);

      Grid<AsciiCell, 2> screen(ivec2(s_width, s_height));

      for (int i = 0; i < s_height; ++i) {
        for (int j = 0; j < s_width; ++j) {
          ivec2 const location(j, i);

          //bool const inLineSemgent = currentLineSegment.DistanceTo(location) < 0.5f;
          bool const inRect = currentRect.Contains(location);
          bool const inCircle = currentCircle.Contains(location);
          bool const isIntersect = intersection.intersects && ivec2(intersection.point + 0.5f) == location;

          bool isPoi = false;

          for (int k = 0; k < poiCount; ++k) {
            if (pois[k] == location) {
              isPoi = true;
              break;
            }
          }

          bool const inBoth = inRect && inCircle;
          bool const inEither = inRect || inCircle || isIntersect || isPoi;

          //bool const inBoth   = inLineSemgent && inCircle;
          //bool const inEither = inLineSemgent || inCircle || isIntersect || isPoi;

          if (!inEither) {
            continue;
          }

          int colorIndex = 0;

          if (isIntersect) {
            colorIndex = s_intersectColorIndex;
          }
          else if (isPoi) {
            colorIndex = s_poiColorIndex;
          }
          else if (inBoth) {
            colorIndex = s_bothColorIndex;
          }
          else if (inRect) {
            colorIndex = s_rectColorIndex;
          }
          //else if (inLineSemgent) {
          //  colorIndex = s_rectColorIndex;
          //}
          else if (inCircle) {
            colorIndex = s_circleColorIndex;
          }

          screen[location].foregroundColor = colorIndex;
          screen[location].character = isIntersect ? 'x' : '.';
        }
      }

      window.Draw(screen);

      window.Sleep(5);
      s_delta += 1;
    }
  }
}

int main(void) {

  std::srand(std::time(nullptr));

  AsciiWindow window;

  AsciiFont font;

  font.size = ivec2(s_cellSize, s_cellSize);
  font.colors[s_rectColorIndex]      = s_rectColor;
  font.colors[s_circleColorIndex]    = s_circleColor;
  font.colors[s_bothColorIndex]      = s_bothColor;
  font.colors[s_intersectColorIndex] = s_intersectColor;

  window.SetFont(font);

  test_lines(window);
}
