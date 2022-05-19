#include <stdio.h>
#define STB_TRUETYPE_IMPLEMENTATION  // force following include to generate implementation
#include "stb_truetype.h"

#include <algorithm>
#include <memory>
#include <vector>
#include <variant>

#include "Math/Shapes2D/Circle.h"
#include "Math/Shapes2D/Rect.h"

#define NOMINMAX
#include <Windows.h>

#include "Window/BlockFont.h"

namespace {
  char writeableChars[] = {
    'a',
    'b',
    'c',
    'd',
    'e',
    'f',
    'g',
    'h',
    'i',
    'j',
    'k',
    'l',
    'm',
    'n',
    'o',
    'p',
    'q',
    'r',
    's',
    't',
    'u',
    'v',
    'w',
    'x',
    'y',
    'z',
    'A',
    'B',
    'C',
    'D',
    'E',
    'F',
    'G',
    'H',
    'I',
    'J',
    'K',
    'L',
    'M',
    'N',
    'O',
    'P',
    'Q',
    'R',
    'S',
    'T',
    'U',
    'V',
    'W',
    'X',
    'Y',
    'Z',
    '0',
    '1',
    '2',
    '3',
    '4',
    '5',
    '6',
    '7',
    '8',
    '9',
    '!',
    '@',
    '#',
    '$',
    '%',
    '^',
    '&',
    '*',
    '(',
    ')',
    '`',
    '~',
    '-',
    '=',
    '[',
    ']',
    '\\',
    ';',
    '\'',
    ',',
    '.',
    '/',
    '_',
    '+',
    '{',
    '}',
    '|',
    ':',
    '"',
    '<',
    '>',
    '?',
  };

  enum class boundsShapeType {
    And,
    Or,
    Not,
    Rect,
    Circle,
    Count
  };

  struct BoundsAnd;
  struct BoundsOr;
  struct BoundsNot;
  struct BoundsRect;
  struct BoundsCircle;

  using BoundsShape = std::variant<
    BoundsAnd,
    BoundsOr,
    BoundsNot,
    BoundsRect,
    BoundsCircle
  >;

  struct BoundsAnd {
    BoundsAnd(void) = default;

    template<typename ... Params>
    BoundsAnd(Params && ... params) :
      list({ std::forward<Params &&>(params)... })
    {}

    std::vector<BoundsShape> list;
  };

  struct BoundsOr {
    BoundsOr(void) = default;

    template<typename ... Params>
    BoundsOr(Params && ... params) :
      list({ std::forward<Params &&>(params)... })
    {}

    std::vector<BoundsShape> list;
  };

  struct BoundsNot {
    BoundsNot(void) = default;
    BoundsNot(BoundsShape const & bounds) :
      bounds(std::make_shared<BoundsShape>(bounds))
    {}
    BoundsNot(BoundsShape && bounds) :
      bounds(std::make_shared<BoundsShape>(std::move(bounds)))
    {}

    std::shared_ptr<BoundsShape> bounds;
  };

  struct BoundsRect {
    BoundsRect(void) = default;
    BoundsRect(float left, float top, float right, float bottom) :
      rect(fvec2((left + right) * 0.5f, (top + bottom) * 0.5f), fvec2(right - left, top - bottom))
    {}

    Rect rect;
  };

  struct BoundsCircle {
    BoundsCircle(void) = default;
    BoundsCircle(float xPos, float yPos, float radius) :
      circle(fvec2(xPos, yPos), radius)
    {}

    Circle circle;
  };

  bool IsWithin(BoundsShape const & shape, fvec2 const & pos);

  bool IsWithinHelp(BoundsAnd const & shape, fvec2 const & pos) {
    for (auto const & subshape : shape.list) {
      if (!IsWithin(subshape, pos)) {
        return false;
      }
    }

    return true;
  }

  bool IsWithinHelp(BoundsOr const & shape, fvec2 const & pos) {
    for (auto const & subshape : shape.list) {
      if (IsWithin(subshape, pos)) {
        return true;
      }
    }

    return false;
  }

  bool IsWithinHelp(BoundsNot const & shape, fvec2 const & pos) {
    if (shape.bounds) {
      return !IsWithin(*shape.bounds, pos);
    }

    return true;
  }

  bool IsWithinHelp(BoundsRect const & shape, fvec2 const & pos) {
    return shape.rect.Contains(pos);
  }

  bool IsWithinHelp(BoundsCircle const & shape, fvec2 const & pos) {
    return shape.circle.Contains(pos);
  }

  bool IsWithin(BoundsShape const & shape, fvec2 const & pos) {
    if (std::holds_alternative<BoundsAnd>(shape)) {
      return IsWithinHelp(std::get<BoundsAnd>(shape), pos);
    }
    if (std::holds_alternative<BoundsOr>(shape)) {
      return IsWithinHelp(std::get<BoundsOr>(shape), pos);
    }
    if (std::holds_alternative<BoundsNot>(shape)) {
      return IsWithinHelp(std::get<BoundsNot>(shape), pos);
    }
    if (std::holds_alternative<BoundsRect>(shape)) {
      return IsWithinHelp(std::get<BoundsRect>(shape), pos);
    }
    if (std::holds_alternative<BoundsCircle>(shape)) {
      return IsWithinHelp(std::get<BoundsCircle>(shape), pos);
    }

    return false;
  }

  BoundsShape BoundsDonut(float xPos, float yPos, float radius, float thickness) {
    return BoundsAnd(
      BoundsCircle(xPos, yPos, radius + thickness),
      BoundsNot(BoundsCircle(xPos, yPos, radius - thickness))
    );
  }

  BoundsShape BoundsRoundedRect(float left, float top, float right, float bottom, float radius) {
    return BoundsOr(
      BoundsRect(left + radius, top, right - radius, bottom),
      BoundsRect(left, top - radius, right, bottom + radius),
      BoundsCircle(left + radius, top - radius, radius),
      BoundsCircle(right - radius, top - radius, radius),
      BoundsCircle(left + radius, bottom + radius, radius),
      BoundsCircle(right - radius, bottom + radius, radius)
    );
  }

  BoundsShape BoundsRoundedDonutRect(float left, float top, float right, float bottom, float radius, float thickness) {
    return BoundsAnd(
      BoundsRoundedRect(left - thickness, top + thickness, right + thickness, bottom - thickness, radius + thickness),
      BoundsNot(BoundsRoundedRect(left + thickness, top - thickness, right - thickness, bottom + thickness, radius - thickness))
    );
  }

  BoundsShape BoundsLCurve(float xPos, float yPos, float xTail, float yTail, float radius, float thickness) {
    bool const xDir = xTail > 0.0f;
    bool const yDir = yTail > 0.0f;

    float const donutXPos   = xPos + (xDir ? radius : -radius);
    float const donutYPos   = yPos + (yDir ? radius : -radius);
    float const leftDonut   = xPos - thickness + (xDir ? 0.0f : -radius);
    float const topDonut    = yPos + thickness + (yDir ? radius : 0.0f);
    float const rightDonut  = xPos + thickness + (xDir ? radius : 0.0f);
    float const bottomDonut = yPos - thickness + (yDir ? 0.0f : -radius);
    float const leftTail    = xPos + (xDir ? radius : xTail);
    float const topTail     = yPos + (yDir ? yTail : -radius);
    float const rightTail   = xPos + (xDir ? xTail : -radius);
    float const bottomTail  = yPos + (yDir ? radius : yTail);

    return BoundsOr(
      BoundsAnd(
        BoundsDonut(donutXPos, donutYPos, radius, thickness),
        BoundsRect(leftDonut, topDonut, rightDonut, bottomDonut)
      ),
      BoundsRect(leftTail, yPos + thickness, rightTail, yPos - thickness),
      BoundsRect(xPos - thickness, topTail, xPos + thickness, bottomTail)
    );
  }

  struct glyphRule {
    glyphRule(void) = default;
    glyphRule(char character, BoundsShape const & shape) :
      character(character),
      shape(shape)
    {}

    char character;
    BoundsShape shape;
  };

  float const c_glyphTop         =  1.0f;
  float const c_glyphMiddle      =  0.6f;
  float const c_glyphLowerMiddle =  0.3f;
  float const c_glyphBottom      =  0.0f;
  float const c_glyphUnder       = -0.3f;
  float const c_glyphLeft        =  0.2f;
  float const c_glyphLeftCenter  =  0.3f;
  float const c_glyphCenter      =  0.5f;
  float const c_glyphRightCenter =  0.7f;
  float const c_glyphRight       =  0.8f;

  float const c_glyphThickness   =  0.04f;

  glyphRule const s_glyphs[] = {
    glyphRule('a', BoundsOr(
      BoundsAnd(
        BoundsCircle(c_glyphCenter, c_glyphMiddle, (c_glyphRight - c_glyphCenter) + c_glyphThickness),
        BoundsNot(BoundsCircle(c_glyphCenter, c_glyphMiddle, (c_glyphRight - c_glyphCenter) - c_glyphThickness)),
        BoundsRect(0.0f, c_glyphTop, 1.0f, c_glyphMiddle)
      ),
      BoundsCircle(c_glyphLeft, c_glyphMiddle, c_glyphThickness),
      BoundsRect(c_glyphRight - c_glyphThickness, c_glyphMiddle, c_glyphRight + c_glyphThickness, c_glyphBottom - c_glyphThickness),
      BoundsAnd(
        BoundsCircle(c_glyphCenter, c_glyphLowerMiddle, (c_glyphRight - c_glyphCenter) + c_glyphThickness),
        BoundsNot(BoundsCircle(c_glyphCenter, c_glyphLowerMiddle, (c_glyphRight - c_glyphCenter) - c_glyphThickness))
      )
    )),
    glyphRule('t', BoundsOr(
      //BoundsLCurve(0.5f, 0.5f, 0.5f, 0.5f, 0.3f, c_glyphThickness),
      BoundsLCurve(0.5f, 0.5f, 0.5f, 0.5f, 0.3f, c_glyphThickness)
    )),
  };

  enum class PrintTest {
    Original,
    MyTtf,
    MyRulesSingle,
    PrintTtf,
    AllText,
    BitImage,
  };
}

unsigned char ttf_buffer[1 << 25];

const PrintTest s_printTest = PrintTest::BitImage;

int main(int argc, char ** argv)
{
  if constexpr (s_printTest == PrintTest::Original) {
    stbtt_fontinfo font;
    unsigned char * bitmap;
    int w, h, i, j, c = (argc > 1 ? atoi(argv[1]) : 'a'), s = (argc > 2 ? atoi(argv[2]) : 16);

    fread(ttf_buffer, 1, 1 << 25, fopen(argc > 3 ? argv[3] : "c:/windows/fonts/consola.ttf", "rb"));

    stbtt_InitFont(&font, ttf_buffer, stbtt_GetFontOffsetForIndex(ttf_buffer, 0));
    bitmap = stbtt_GetCodepointBitmap(&font, 0, stbtt_ScaleForPixelHeight(&font, s), c, &w, &h, 0, 0);

    for (j = 0; j < h; ++j) {
      for (i = 0; i < w; ++i)
        putchar(" .:ioVM@"[bitmap[j * w + i] >> 5]);
      putchar('\n');
    }
  }
  else if constexpr (s_printTest == PrintTest::MyTtf) {
    stbtt_fontinfo font;
    char const text[] = "Hello World! g/\\";

    fread(ttf_buffer, 1, 1 << 25, fopen("c:/windows/fonts/consola.ttf", "rb"));

    stbtt_InitFont(&font, ttf_buffer, stbtt_GetFontOffsetForIndex(ttf_buffer, 0));

    struct glyphInfo {
      int width;
      int height;
      int offsetX;
      int offsetY;
      unsigned char * bitmap;
    };

    int const glyphSize = 16;

    int minX;
    int minY;
    int maxX;
    int maxY;
    bool extentsSet = false;

    glyphInfo glyphs[100];
    int glyphCount = 0;

    for (int i = 0; i < sizeof(text) / sizeof(*text); ++i) {
      glyphInfo & glyph = glyphs[glyphCount];
      glyph.bitmap = stbtt_GetCodepointBitmap(&font, 0, stbtt_ScaleForPixelHeight(&font, glyphSize), text[i], &glyph.width, &glyph.height, &glyph.offsetX, &glyph.offsetY);

      int glyphMinX = -glyph.offsetX;
      int glyphMinY = -glyph.offsetY - glyph.height;
      int glyphMaxX = glyph.offsetX + glyph.width;
      int glyphMaxY = glyph.height;

      if (!extentsSet) {
        minX = glyphMinX;
        minY = glyphMinY;
        maxX = glyphMaxX;
        maxY = glyphMaxY;

        extentsSet = true;
      }
      else {
        minX = std::min(minX, glyphMinX);
        minY = std::min(minY, glyphMinY);
        maxX = std::max(maxX, glyphMaxX);
        maxY = std::max(maxY, glyphMaxY);
      }

      ++glyphCount;
    }

    int const glyphWidth = maxX - minX;
    int const glyphHeight = maxY - minY;

    int const glyphExt = std::max(glyphWidth, glyphHeight);

    for (int i = 0; i < glyphExt; ++i) {
      for (int j = 0; j < glyphCount; ++j) {
        glyphInfo & glyph = glyphs[j];

        for (int k = 0; k < glyphExt; ++k) {
          int const xPos = k - glyph.offsetX + minX;
          int const yPos = -glyphHeight - glyph.offsetY + i - minY;

          if (xPos < 0 || xPos >= glyph.width || yPos < 0 || yPos >= glyph.height) {
            putchar(' ');
          }
          else {
            putchar(" .:ioVM@"[glyphs[j].bitmap[yPos * glyphs[j].width + xPos] >> 5]);
          }
        }
      }
      putchar('\n');
    }
  }
  else if constexpr (s_printTest == PrintTest::MyRulesSingle) {

    float const c_scale = 40.0f;

    float const c_leftmost   = -0.2f;
    float const c_rightmost  =  1.2f;
    float const c_topmost    =  1.2f;
    float const c_bottommost = -0.5f;

    float const c_invScale = 1.0f / c_scale;

    char character = 't';

    BoundsShape const * shape = nullptr;

    for (auto const & glyph : s_glyphs) {
      if (glyph.character == character) {
        shape = &glyph.shape;
      }
    }

    if (shape) {
      putchar('+');
      putchar('-');
      for (float xPos = c_leftmost; xPos <= c_rightmost; xPos += c_invScale) {
        putchar('-');
      }
      putchar('-');
      putchar('+');
      putchar('\n');

      for (float yPos = c_topmost; yPos >= c_bottommost; yPos -= c_invScale) {
        putchar('|');
        putchar(' ');
        for (float xPos = c_leftmost; xPos <= c_rightmost; xPos += c_invScale) {
          if (IsWithin(*shape, fvec2(xPos, yPos))) {
            putchar('X');
          }
          else {
            putchar(' ');
          }
        }
        putchar(' ');
        putchar('|');
        putchar('\n');
      }

      putchar('+');
      putchar('-');
      for (float xPos = c_leftmost; xPos <= c_rightmost; xPos += c_invScale) {
        putchar('-');
      }
      putchar('-');
      putchar('+');
      putchar('\n');
    }
  }
  else if constexpr (s_printTest == PrintTest::PrintTtf) {
    stbtt_fontinfo font;
    char const text[] = "Hello World! g/\\";
    int const textSize = std::strlen(text);

    fread(ttf_buffer, 1, 1 << 25, fopen("c:/windows/fonts/trebuc.ttf", "rb"));

    stbtt_InitFont(&font, ttf_buffer, stbtt_GetFontOffsetForIndex(ttf_buffer, 0));

    struct glyphInfo {
      int width;
      int height;
      int offsetX;
      int offsetY;
      unsigned char * bitmap;
    };

    int const glyphSize = 16;

    int minX;
    int minY;
    int maxX;
    int maxY;
    bool extentsSet;

    glyphInfo glyphs[256];
    int glyphCount;

    for (int i = 32; i > 0; --i) {
      extentsSet = false;
      glyphCount = 0;

      for (int j = 0; j < 256; ++j) {
        glyphInfo & glyph = glyphs[glyphCount];
        glyph.bitmap = stbtt_GetCodepointBitmap(&font, 0, stbtt_ScaleForPixelHeight(&font, i), char(j), &glyph.width, &glyph.height, &glyph.offsetX, &glyph.offsetY);

        int glyphMinX = -glyph.offsetX;
        int glyphMinY = -glyph.offsetY - glyph.height;
        int glyphMaxX = glyph.offsetX + glyph.width;
        int glyphMaxY = glyph.height;

        if (!extentsSet) {
          minX = glyphMinX;
          minY = glyphMinY;
          maxX = glyphMaxX;
          maxY = glyphMaxY;

          extentsSet = true;
        }
        else {
          minX = std::min(minX, glyphMinX);
          minY = std::min(minY, glyphMinY);
          maxX = std::max(maxX, glyphMaxX);
          maxY = std::max(maxY, glyphMaxY);
        }

        ++glyphCount;
      }

      int const glyphWidth = maxX - minX;
      int const glyphHeight = maxY - minY;

      int const glyphExt = std::max(glyphWidth, glyphHeight);

      if (glyphExt == 12) {
        break;
      }
    }

    int const glyphWidth = maxX - minX;
    int const glyphHeight = maxY - minY;

    int const glyphExt = std::max(glyphWidth, glyphHeight);

    for (int i = 0; i < glyphExt; ++i) {
      for (int j = 0; j < textSize; ++j) {
        glyphInfo & glyph = glyphs[text[j]];

        for (int k = 0; k < glyphExt; ++k) {
          int const xPos = k - glyph.offsetX + minX;
          int const yPos = -glyphHeight - glyph.offsetY + i - minY;

          if (xPos < 0 || xPos >= glyph.width || yPos < 0 || yPos >= glyph.height) {
            putchar(' ');
          }
          else {
            putchar(" .:ioVM@"[glyph.bitmap[yPos * glyph.width + xPos] >> 5]);
          }
        }
      }
      putchar('\n');
    }
  }
  else if constexpr (s_printTest == PrintTest::AllText) {

    int const textSize = 12;

    CONSOLE_FONT_INFOEX fontInfo;
    fontInfo.cbSize = sizeof(fontInfo);
    fontInfo.dwFontSize.X = textSize;
    fontInfo.dwFontSize.Y = textSize;

    std::wcscpy(fontInfo.FaceName, L"Terminal");
    fontInfo.FontFamily = FF_DONTCARE;
    fontInfo.FontWeight = FW_NORMAL;
    SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), FALSE, &fontInfo);

    for (char const & character : writeableChars) {
      putchar(character);
    }
    putchar('\n');
  }
  else if constexpr (s_printTest == PrintTest::BitImage) {
    //CreateBitImages("temp.cpp");
  }

  while (1) {}
  return 0;
}

