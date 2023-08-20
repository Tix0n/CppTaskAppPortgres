#ifndef SRC_CONSOLEPAINTER_H_
#define SRC_CONSOLEPAINTER_H_

#include <iostream>
#include <string>

class ConsolePainter {
 public:
  enum Color { DEFAULT, RED, GREEN, YELLOW, BLUE, PURPLE, AZURE, GRAY, WHITE };

  ConsolePainter();
  void DrawBoxWithText(const std::string& text, Color border_color);

 private:
  void SetConsoleColor(int color_code);
  void SetColor(Color foreground_code, Color background = DEFAULT);
  void ResetColor();
  void DrawHorizontalLine(char symbol, int width, Color color);

  static const int kDefaultWidth = 80;
};

#endif  // SRC_CONSOLEPAINTER_H_
