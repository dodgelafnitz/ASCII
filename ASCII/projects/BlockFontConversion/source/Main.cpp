/*
 * Copywrite 2022 Dodge Lafnitzegger
 */

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

namespace {
  bool GetInfoFromArgs(
    int                  argc,
    char const * const * argv,
    int &                o_glyphSize,
    std::string &        o_inputFilename,
    std::string &        o_outputFilename
  ) {
    o_glyphSize = 0;

    if (argc == 4) {
      o_glyphSize      = std::atoi(argv[1]);
      o_inputFilename  = argv[2];
      o_outputFilename = argv[3];
    }
    else if (argc == 3) {
      o_glyphSize     = std::atoi(argv[1]);
      o_inputFilename = argv[2];

      int const fileTypeIndex = o_inputFilename.find_last_of('.');

      if (fileTypeIndex > 0) {
        o_outputFilename = o_inputFilename.substr(0, fileTypeIndex);
      }
      else {
        o_outputFilename = o_inputFilename;
      }
      o_outputFilename += ".cpp";
    }
    else {
      std::string programName        = argv[0];
      int const   fileSeparatorIndex = std::max<int>(programName.find_last_of('\\'), programName.find_last_of('/'));

      if (fileSeparatorIndex != -1) {
        programName = programName.substr(fileSeparatorIndex + 1);
      }

      std::cerr <<
        "Usage:" << std::endl <<
        "  " << programName << " <glyph_size> <input_file> [<output_file>]" << std::endl <<
        std::endl <<
        "  glyph_size:             The edge length of the square glyphs in the image." << std::endl <<
        "  input_file:             The file to read the image from." << std::endl <<
        "  output_file (optional): The file to store the block font definitions in. If blank," << std::endl <<
        "                          this will be the same as input_file but with a .cpp extension." << std::endl
      ;

      return false;
    }

    if (o_glyphSize < 1) {
      std::cerr << "Glyph size must be greater than 0." << std::endl;

      return false;
    }

    return true;
  }

  std::string ConvertImageToString(stbi_uc const * image, int glyphSize) {
    std::vector<unsigned char> glyphs[256];

    for (std::vector<unsigned char> & glyph : glyphs) {
      glyph.reserve(size_t(glyphSize) * glyphSize);
    }

    for (int i = 0; i < glyphSize * 16; ++i) {
      for (int j = 0; j < glyphSize * 16; ++j) {
        int const glyphXPos = j / glyphSize;
        int const glyphYPos = i / glyphSize;

        int const index = glyphYPos * 16 + glyphXPos;
        int const imageIndex = i * glyphSize * 16 + j;

        glyphs[index].emplace_back(image[imageIndex]);
      }
    }

    std::string result;

    result += 
      "// Generated to contain glyph data.\n"
      "static unsigned char const s_glyphs[256][144] = {\n"
    ;

    int glyphindex        = 0;
    bool previousWasEmpty = true;
    for (int i = 0; i < 256; ++i) {
      std::vector<unsigned char> & glyph = glyphs[i];
      bool isEmpty = true;
      for (unsigned char value : glyph) {
        if (value != 0) {
          isEmpty = false;
          break;
        }
      }

      std::string charNotify;
      if (i >= ' ' && i <= '~') {
        charNotify = ": '" + std::string(1, char(i)) + "'";
      }

      if (isEmpty) {
        result += "  { 0 }, // char " + std::to_string(i) + charNotify + "\n";
        previousWasEmpty = true;
      }
      else {
        if (previousWasEmpty && i != 0) {
          result += "\n";
        }

        result += "  // char " + std::to_string(i) + charNotify + "\n";
        result += "  {\n";

        for (int j = 0; j < glyphSize; ++j) {
          result += "    ";
          for (int k = 0; k < glyphSize; ++k) {
            int const index = j * glyphSize + k;
            int const value = glyph[index];

            std::string const valueString = std::to_string(value);

            result += valueString + "," + std::string(3 - valueString.length(), ' ');

            if (k != glyphSize - 1) {
              result += " ";
            }
          }
          result += "\n";
        }

        result += "  },\n";
        result += "\n";

        previousWasEmpty = false;
      }
    }

    result += "};\n";

    return result;
  }

  bool ConvertFileToCpp(int glyphSize, std::string const & inputFilename, std::string const & outputFilename) {
    int width;
    int height;
    int components;
    stbi_uc const * const image = stbi_load(inputFilename.c_str(), &width, &height, &components, 1);

    if (image == nullptr) {
      std::cerr << "Didn't read image. stbi error: " << stbi_failure_reason() << std::endl;
      return false;
    }

    int const expectedSize = glyphSize * 16;

    if (width != expectedSize || height != expectedSize) {
      std::cerr <<
        "Size of '" << inputFilename << "' not correct for glyph size " << glyphSize << "." << std::endl <<
        "Width and height expected to be " << expectedSize << " pixels." << std::endl;

      return false;
    }

    std::string const fileOutput = ConvertImageToString(image, glyphSize);

    std::ofstream outputFile(outputFilename);
    outputFile << fileOutput;

    return true;
  }
}

int main(int argc, char const * const * argv) {
  std::string inputFilename;
  std::string outputFilename;
  int         glyphSize;
  
  if (!GetInfoFromArgs(argc, argv, glyphSize, inputFilename, outputFilename)) {
    return 1;
  }

  if (!ConvertFileToCpp(glyphSize, inputFilename, outputFilename)) {
    return 1;
  }

  return 0;
}
