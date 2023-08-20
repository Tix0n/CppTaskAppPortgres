#include "consolepainter.h"

#include <iomanip>
#include <iostream>

ConsolePainter::ConsolePainter() {}

void ConsolePainter::SetColor(Color foreground, Color background) {
  int foreground_code = foreground + 30;
  SetConsoleColor(foreground_code);
}

void ConsolePainter::ResetColor() { SetConsoleColor(0); }

void ConsolePainter::DrawBoxWithText(const std::string& text,
                                     Color border_color) {
  DrawHorizontalLine('-', kDefaultWidth, border_color);

  std::istringstream iss(text);
  std::string word;
  std::string line;
  while (iss >> word) {
    if (line.size() + word.size() + 1 <= kDefaultWidth - 2) {
      if (!line.empty()) {
        line += " ";
      }
      line += word;
    } else {
      SetColor(border_color);
      if (line.size() <= kDefaultWidth - 4) {
        std::cout << "| " << std::setw((kDefaultWidth - 4 - line.size()) / 2)
                  << "" << line
                  << std::setw((kDefaultWidth - 4 - line.size() + 1) / 2) << ""
                  << " |" << std::endl;
      } else {
        std::cout << "| " << std::right << std::setw(kDefaultWidth - 4) << line
                  << " |" << std::endl;
      }
      ResetColor();
      line = word;
    }
  }

  if (!line.empty()) {
    SetColor(border_color);
    if (line.size() <= kDefaultWidth - 4) {
      std::cout << "| " << std::setw((kDefaultWidth - 4 - line.size()) / 2)
                << "" << line
                << std::setw((kDefaultWidth - 4 - line.size() + 1) / 2) << ""
                << " |" << std::endl;
    } else {
      std::cout << "| " << std::right << std::setw(kDefaultWidth - 4) << line
                << " |" << std::endl;
    }
    ResetColor();
  }

  DrawHorizontalLine('-', kDefaultWidth, border_color);
}

void ConsolePainter::DrawHorizontalLine(char symbol, int width, Color color) {
  SetColor(color);
  for (int i = 0; i < width; ++i) {
    std::cout << symbol;
  }
  ResetColor();
  std::cout << std::endl;
}

void ConsolePainter::SetConsoleColor(int color_code) {
  std::cout << "\033[" << color_code << "m";
  std::cout.flush();
}
