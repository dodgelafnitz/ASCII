/*
 * Copywrite 2021 Dodge Lafnitzegger
 */

#ifndef ASCII_WINDOW_WINDOW_H
#define ASCII_WINDOW_WINDOW_H

class AsciiWindow {
public:
  void Draw(Grid<AsciiCell> const & draw);

  std::vector<AsciiInputEvent> PollInput(void);

  std::string GetClipboard(void) const;
  void SetClipboard(std::string const & clipboard);

  std::string GetTitle(void) const;
  void SetTitle(std::string const & title);

  AsciiFont GetFont(void) const;
  void SetFont(AsciiFont const & font);

  int GetRunMs(void) const;
  void Sleep(int milliseconds);
};

#endif // ASCII_WINDOW_WINDOW_H
