/**
 * @file locus.h
 * @brief Locus handling definitions for My Language.
 * @details Defines the Locus struct for source code locations.
 * @copyright Copyright (c) 2025 Karson P. Califf
 */

#pragma once

#include <cstdint>
#include <string>

namespace ml::basic {

/**
 * @struct Locus locus.h
 * @brief Represents a location in the source code.
 */
struct Locus {
  /**
   * @var line
   * @brief The line number in the source code (1-based).
   */
  uint64_t line;
  /**
   * @var column
   * @brief The column number in the source code (1-based).
   */
  uint64_t column;

  /**
   * @var index
   * @brief The index in the source code (0-based).
   */
  uint64_t index;

  Locus() : line(0), column(0), index(0) {}

  Locus(uint64_t line, uint64_t column)
      : line(line), column(column), index(0) {}

  Locus(uint64_t line, uint64_t column, uint64_t index)
      : line(line), column(column), index(index) {}

  /**
   * @brief Converts the Locus to a string representation.
   * @return A string in the format "line:column (index index)".
   */
  inline operator const std::string() const {
    std::string location(std::to_string(this->line));
    location += ":" + std::to_string(this->column);
    if (this->index != 0) {
      location += " (index " + std::to_string(this->index) + ")";
    }
    return location;
  }
};

} // namespace ml::basic
