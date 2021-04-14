/*
 * Copywrite 2021 Dodge Lafnitzegger
 */

#ifndef DCUTILITY_COLOR_H
#define DCUTILITY_COLOR_H

struct Color {
  static Color const Black;
  static Color const White;
  static Color const Red;
  static Color const Green;
  static Color const Blue;
  static Color const Yellow;
  static Color const Purple;
  static Color const Cyan;
  static Color const Brown;
  static Color const Orange;
  static Color const Pink;
  static Color const Indigo;
  static Color const DarkGrey;
  static Color const Grey;
  static Color const LightGrey;

  Color(void) : r(0), g(0), b(0)
  {}

  Color(unsigned char r, unsigned char g, unsigned char b) :
    r(r),
    g(g),
    b(b)
  {}

  unsigned char r;
  unsigned char g;
  unsigned char b;
};

#endif // DCUTILITY_COLOR_H
