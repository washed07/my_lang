/**
 * @file accessor.h
 * @brief Accessor definitions.
 * @details Defines accessor types and utility functions.
 * @copyright Copyright (c) 2025 Karson P. Califf
 */

#pragma once

#include <string>

namespace ml::basic {

/**
 * @enum Accessor accessor.h
 * @brief Enumeration of accessor types.
 */
enum class Accessor { Public, Private, Protected };

/**
 * @brief Utility function to check if a string is a valid accessor.
 * @param str The string to check.
 * @return True if the string is a valid accessor, false otherwise.
 */
inline bool isacc(const std::string str) {
  return str == "pub" || str == "pri" || str == "pro";
}

/**
 * @brief Utility function to get the Accessor enum from a string.
 * @param str The string representing the accessor.
 * @return The corresponding Accessor enum value.
 */
inline Accessor getacc(const std::string str) {
  if (str == "pub") {
    return Accessor::Public;
  } else if (str == "pri") {
    return Accessor::Private;
  } else if (str == "pro") {
    return Accessor::Protected;
  } else {
    return Accessor::Private;
  }
}

/**
 * @brief Utility function to convert an Accessor enum to a string.
 * @param acc The Accessor enum value.
 * @return The corresponding string representation.
 */
inline std::string accstr(const Accessor acc) {
  switch (acc) {
  case Accessor::Public:
    return "public";
  case Accessor::Private:
    return "private";
  case Accessor::Protected:
    return "protected";
  default:
    return "private";
  }
}

inline bool canAccess(const Accessor member_acc, const Accessor accessor_acc) {
  if (member_acc == Accessor::Public) {
    return true;
  } else if (member_acc == Accessor::Private) {
    return accessor_acc == Accessor::Private;
  } else if (member_acc == Accessor::Protected) {
    return accessor_acc == Accessor::Protected ||
           accessor_acc == Accessor::Private;
  }
  return false;
}

} // namespace ml::basic
