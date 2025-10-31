/**
 * @file error.cpp
 * @brief Error source code for My Language.
 * @copyright Copyright (c) 2025 Karson P. Califf
 */

#include "ml/basic/error.h"

namespace ml::basic {

const char *Error::what() const noexcept { return this->desc.c_str(); }

const char *Error::why() const noexcept { return this->help.c_str(); }

const char *Error::how() const noexcept { return this->desc.c_str(); }

const std::string Error::where() const noexcept {
  return std::string("[") + std::to_string(this->start_.line) + ":" +
         std::to_string(this->end_.line) + "]";
}

std::string Error::snippet() const noexcept {
  return this->src_.substr(this->start_.index,
                           this->end_.index - this->start_.index);
}

std::string Error::highlight() const noexcept {
  std::string highlight = this->snippet() + "\n";
  for (uint64_t i = this->start_.index; i < this->end_.index; i++) {
    highlight += "^";
  }
  highlight += "\n";
  return highlight;
}

std::string Error::context() const noexcept { return this->src_; }

uint64_t Error::from() const noexcept { return this->start_.index; }

uint64_t Error::to() const noexcept { return this->end_.index; }

bool Error::supportsColor() const noexcept {
#ifdef _WIN32
  // Enable ANSI color support on Windows 10+
  HANDLE hOut = GetStdHandle(STD_ERROR_HANDLE);
  if (hOut != INVALID_HANDLE_VALUE) {
    DWORD dwMode = 0;
    if (GetConsoleMode(hOut, &dwMode)) {
      dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
      SetConsoleMode(hOut, dwMode);
      return true;
    }
  }
  return false;
#else
  return isatty(fileno(stderr));
#endif
}

std::string Error::getLevelColor() const noexcept {
  if (!supportsColor())
    return "";

  switch (this->level) {
  case ErrorLevel::Info:
    return CYAN;
  case ErrorLevel::Warning:
    return YELLOW;
  case ErrorLevel::Error:
    return RED;
  default:
    return WHITE;
  }
}

std::vector<std::string> Error::getSourceLines() const noexcept {
  std::vector<std::string> lines;
  std::stringstream ss(src_);
  std::string line;
  while (std::getline(ss, line)) {
    lines.push_back(line);
  }
  return lines;
}

std::string Error::getErrorLine() const noexcept {
  if (this->start_.line == 0)
    return this->snippet();

  auto lines = this->getSourceLines();
  if (this->start_.line <= lines.size()) {
    return lines[start_.line - 1];
  }
  return "";
}

int Error::getLineNumberWidth() const noexcept {
  uint64_t max_line = (this->start_.line > this->end_.line) ? this->start_.line
                                                            : this->end_.line;
  if (max_line == 0)
    max_line = 1;
  return (int)std::to_string(max_line).length();
}

std::string Error::format() const noexcept {
  std::stringstream ss;
  ss << this->what() << " at " << this->where() << "\n";
  ss << this->highlight();
  return ss.str();
}

void Error::log() const noexcept {
  bool use_colors = this->supportsColor();
  std::string level_color = this->getLevelColor();
  std::string reset = use_colors ? RESET : "";
  std::string bold = use_colors ? BOLD : "";
  std::string dim = use_colors ? DIM : "";
  std::string blue = use_colors ? BLUE : "";
  std::string white = use_colors ? WHITE : "";

  // Error level prefix
  std::string level_str;
  switch (this->level) {
  case ErrorLevel::Info:
    level_str = "Info";
    break;
  case ErrorLevel::Warning:
    level_str = "Warning";
    break;
  case ErrorLevel::Error:
    level_str = "Error";
    break;
  default:
    level_str = "Unknown";
    break;
  }

  std::cerr << level_color << bold << level_str << reset;
  if (code != 0) {
    std::cerr << dim << "[" << std::setfill('0') << std::setw(4) << code << "]"
              << reset;
  }
  std::cerr << ": " << bold << this->what() << reset << std::endl;

  if (this->start_.line > 0) {
    uint64_t display_column = (this->start_.column > 1)
                                  ? this->start_.column - 1
                                  : this->start_.column;
    std::cerr << dim << "   --> " << file_ << ":" << this->start_.line << ":"
              << display_column << reset << std::endl;
  }

  std::cerr << dim << "  |" << reset << std::endl;

  if (start_.line > 0) {
    int line_width = this->getLineNumberWidth();
    std::string error_line = this->getErrorLine();

    std::cerr << dim << std::setw(line_width) << this->start_.line << " | "
              << reset << error_line << std::endl;

    std::cerr << dim << std::string(line_width, ' ') << " | " << reset;

    uint64_t error_start =
        (this->start_.column > 1) ? this->start_.column - 1 : 0;
    uint64_t error_length =
        (end_.column > start_.column) ? (end_.column - start_.column) : 2;

    for (uint64_t i = 0; i < error_start; i++) {
      std::cerr << " ";
    }

    std::cerr << level_color << bold;
    for (uint64_t i = 0; i < error_length; i++) {
      std::cerr << "^";
    }
    std::cerr << reset << std::endl;

    std::cerr << dim << std::string(line_width, ' ') << " | " << reset
              << std::endl;
    std::cerr << dim << std::string(line_width, ' ') << " | " << reset;
    std::cerr << blue << "help: " << reset << this->help << std::endl;
    std::cerr << std::endl;
  }

  std::cerr << std::endl;
  std::cerr.flush();
}

} // namespace ml::basic