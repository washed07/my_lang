/**
 * @file error.h
 * @brief Error handling definitions for My Language.
 * @details Defines the Error class and related functions.
 * @copyright Copyright (c) 2025 Karson P. Califf
 */

#pragma once

#include "locus.h"
#include <algorithm>
#include <cstdint>
#include <cstdio>
#include <exception>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#ifdef _WIN32
#include <io.h>
#include <windows.h>
#define isatty _isatty
#define fileno _fileno
#else
#include <unistd.h>
#endif

namespace ml::basic {

const std::string RESET = "\033[0m";
const std::string BOLD = "\033[1m";
const std::string DIM = "\033[2m";
const std::string UNDERLINE = "\033[4m";
const std::string RED = "\033[91m";
const std::string GREEN = "\033[92m";
const std::string YELLOW = "\033[93m";
const std::string BLUE = "\033[94m";
const std::string MAGENTA = "\033[95m";
const std::string CYAN = "\033[96m";
const std::string WHITE = "\033[97m";

/**
 * @enum ErrorLevel error.h
 * @brief Enumeration of error severity levels.
 */
enum class ErrorLevel { Info, Warning, Error, Fatal };

/**
 * @class Error error.h
 * @brief Represents an error with detailed information.
 * @details Inherits from std::exception and provides methods for error
 * reporting.
 */
class Error : public std::exception {
private:
  const std::string src_;  // Source code context for the error
  const std::string file_; // File name where the error occurred
  const Locus start_;      // Start location of the error
  const Locus end_;        // End location of the error

  // Helper methods

  bool supportsColor() const noexcept;
  std::string getLevelColor() const noexcept;
  std::vector<std::string> getSourceLines() const noexcept;
  std::string getErrorLine() const noexcept;
  int getLineNumberWidth() const noexcept;

public:
  /**
   * @var code
   * @brief The optional error code associated with the error.
   */
  const uint64_t code;

  /**
   * @var level
   * @brief The severity level of the error.
   */
  ErrorLevel level;

  /**
   * @var desc
   * @brief A brief description of the error.
   */
  const std::string desc;

  /**
   * @var help
   * @brief A detailed help message for the error.
   */
  const std::string help;

  Error(ErrorLevel level, std::string desc, std::string help, Locus start,
        Locus end, std::string file, std::string source) noexcept
      : src_(std::move(source)), file_(std::move(file)), start_(start),
        end_(end), code(0), level(level), desc(std::move(desc)),
        help(std::move(help)) {}

  Error(ErrorLevel level, std::string desc, std::string help, Locus start,
        Locus end, std::string file, std::string source, uint64_t code) noexcept
      : src_(std::move(source)), file_(std::move(file)), start_(start),
        end_(end), code(code), level(level), desc(std::move(desc)),
        help(std::move(help)) {}

  /**
   * @brief Returns a brief description of the error.
   * @return A C-style string describing the error.
   */
  const char *what() const noexcept override;

  /**
   * @brief Returns a detailed help message for the error.
   * @return A C-style string with help information.
   */
  const char *why() const noexcept;

  /**
   * @brief Returns a detailed description of how to fix the error.
   * @return A C-style string with fix information.
   */
  const char *how() const noexcept;

  /**
   * @brief Returns a string representation of the error location.
   * @return A string in the format "[line:column]".
   */
  const std::string where() const noexcept;

  /**
   * @brief Returns the snippet of source code where the error occurred.
   * @return A string containing the relevant source code snippet.
   */
  std::string snippet() const noexcept;

  /**
   * @brief Returns a highlighted version of the error snippet.
   * @return A string with carets (^) indicating the error location.
   */
  std::string highlight() const noexcept;

  /**
   * @brief Formats the complete error message for display.
   * @return A formatted string representing the error.
   */
  std::string format() const noexcept;

  /**
   * @brief Returns the full source code context.
   * @return A string containing the entire source code.
   */
  std::string context() const noexcept;

  /**
   * @brief Returns the starting index of the error in the source code.
   * @return The starting index as a uint64_t.
   */
  uint64_t to() const noexcept;

  /**
   * @brief Returns the ending index of the error in the source code.
   * @return The ending index as a uint64_t.
   */
  uint64_t from() const noexcept;

  /**
   * @brief Logs the error message to standard error output.
   */
  void log() const noexcept;
};

} // namespace ml::basic
