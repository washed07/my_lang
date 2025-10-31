/**
 * @file modifier.h
 * @brief Modifier handling definitions for My Language.
 * @details Defines modifier types and utility functions.
 * @copyright Copyright (c) 2025 Karson P. Califf
 */

#pragma once

#include "ml/basic/flags.h"
#include <cstdint>
#include <string>

namespace ml::basic {

/**
 * @enum Modifier modifier.h
 * @brief Enumeration of modifier flags.
 */
enum class Modifier : uint8_t {
  None = 0,
  Static = 1 << 1,
  Constant = 1 << 2,
  Array = 1 << 3,
  Init = 1 << 4,
  Nullable = 1 << 5,
};
ENABLE_BITMASKS(Modifier)

/**
 * @brief Checks if the given string is a valid modifier.
 * @param str The string to check.
 * @return True if the string is a valid modifier, false otherwise.
 */
inline bool ismod(const std::string str) {
  return str == "static" || str == "const" || str == "init";
}

/**
 * @brief Converts a string to its corresponding Modifier enum value.
 * @param str The string representation of the modifier.
 * @return The corresponding Modifier enum value.
 */
inline Modifier getmod(const std::string str) {
  if (str == "static") {
    return Modifier::Static;
  } else if (str == "const") {
    return Modifier::Constant;
  } else if (str == "init") {
    return Modifier::Init;
  } else {
    return Modifier::None;
  }
}

} // namespace ml::basic
