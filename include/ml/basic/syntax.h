/**
 * @file syntax.h
 * @brief Syntax handling definitions for My Language.
 * @details Defines syntax utility functions.
 * @copyright Copyright (c) 2025 Karson P. Califf
 */

#pragma once

#include <string>

namespace ml::basic {

/**
 * @brief Checks if the given character is a whitespace character.
 * @param c The character to check.
 * @return True if the character is a whitespace, false otherwise.
 */
inline bool isWsp(const char c) {
  return c == ' ' || c == '\t' || c == '\r' || c == '\n';
}

/**
 * @brief Checks if the given string is a keyword.
 * @param str The string to check.
 * @return True if the string is a keyword, false otherwise.
 */
inline bool isKwy(const std::string str) {
  if (str.empty()) {
    return false;
  }

  switch (str.length()) {
  case 2:
    if (str == "if") {
      return true;
    }
    if (str == "fn") {
      return true;
    }
    if (str == "in") {
      return true;
    }
    return false;
  case 3:
    if (str == "for") {
      return true;
    }
    if (str == "let") {
      return true;
    }
    if (str == "cls") {
      return true;
    }
    if (str == "rec") {
      return true;
    }
    if (str == "pub") {
      return true;
    }
    if (str == "pri") {
      return true;
    }
    if (str == "pro") {
      return true;
    }
    return false;
  case 4:
    if (str == "elif") {
      return true;
    }
    if (str == "else") {
      return true;
    }
    if (str == "case") {
      return true;
    }
    if (str == "this") {
      return true;
    }
    if (str == "null") {
      return true;
    }
    if (str == "true") {
      return true;
    }
    return false;
  case 5:
    if (str == "while") {
      return true;
    }
    if (str == "break") {
      return true;
    }
    if (str == "const") {
      return true;
    }
    if (str == "init") {
      return true;
    }
    if (str == "false") {
      return true;
    }
    return false;
  case 6:
    if (str == "return") {
      return true;
    }
    if (str == "switch") {
      return true;
    }
    return false;
  case 7:
    if (str == "default") {
      return true;
    }
    return false;
  case 8:
    if (str == "continue") {
      return true;
    }
    return false;
  default:
    return false;
  }
}

/**
 * @brief Determines the length of the operator at the start of the string.
 * @param str The string to check.
 * @return The length of the operator if found, 0 otherwise.
 */
inline uint8_t opLen(const std::string str) {
  if (str.empty()) {
    return false;
  }

  switch (str[0]) {
  case '+':
    if (str[1] == '=') {
      return 2; // +=
    }
    if (str[1] == '+') {
      return 2; // ++
    }
    return str[1] == '\0' ? 1 : 0; // +

  case '-':
    if (str[1] == '=') {
      return 2; // -=
    }
    if (str[1] == '-') {
      return 2; // --
    }
    return str[1] == '\0' ? 1 : 0; // -

  case '*':
    if (str[1] == '=') {
      return 2; // *=
    }
    if (str[1] == '*') {
      return 2; // **
    }
    return str[1] == '\0' ? 1 : 0; // *

  case '/':
    if (str[1] == '=') {
      return 2; // /=
    }
    return str[1] == '\0' ? 1 : 0; // /

  case '%':
    if (str[1] == '%') {
      return 2; // %%
    }
    return str[1] == '\0' ? 1 : 0; // %

  case '=':
    if (str[1] == '=') {
      return 2; // ==
    }
    return str[1] == '\0' ? 1 : 0; // =

  case '!':
    if (str[1] == '=') {
      return 2; // !=
    }
    return str[1] == '\0' ? 1 : 0; // !

  case '<':
    if (str[1] == '=') {
      return 2; // <=
    }
    if (str[1] == '<') {
      return 2; // <<
    }
    return str[1] == '\0' ? 1 : 0; // <

  case '>':
    if (str[1] == '=') {
      return 2; // >=
    }
    if (str[1] == '>') {
      return 2; // >>
    }
    return str[1] == '\0' ? 1 : 0; // >
  case '.':
    if (str[1] == '.') {
      return 2; // ..
    }
    if (str[1] == '=') {
      return 2; // .=
    }
    return str[1] == '\0' ? 1 : 0; // .
  case '&':
    if (str[1] == '&') {
      return 2; // &&
    }
    return str[1] == '\0' ? 1 : 0; // &

  case '|':
    if (str[1] == '|') {
      return 2; // ||
    }
    return str[1] == '\0' ? 1 : 0; // |

  case '?':
    if (str[1] == '?') {
      return 2; // ??
    }
    return str[1] == '\0' ? 1 : 0; // ?

  case '^':
  case '~':
    return str[1] == '\0' ? 1 : 0; // ^, ~

  default:
    return 0; // Not an operator
  }
}

/**
 * @brief Checks if the given string is an operator.
 * @param str The string to check.
 * @return True if the string is an operator, false otherwise.
 */
inline bool isOp(const std::string str) { return opLen(str) != 0; }

/**
 * @brief Checks if the given string is a comparison operator.
 * @param str The string to check.
 * @return True if the string is a comparison operator, false otherwise.
 */
inline bool isCmp(const std::string str) {
  if (str.empty()) {
    return false;
  }

  switch (str[0]) {
  case '=':
    if (str[1] == '=') {
      return true; // ==
    }
    return false;
  case '!':
    if (str[1] == '=') {
      return true; // !=
    }
    return true;
  case '<':
    if (str[1] == '=') {
      return true; // <=
    }
    if (str[1] == '<') {
      return true; // <<
    }
    return true; // <
  case '>':
    if (str[1] == '=') {
      return true; // >=
    }
    if (str[1] == '>') {
      return true; // >>
    }
    return true; // >
  default:
    return false;
  }
}

/**
 * @brief Checks if the given string is an assignment operator.
 * @param str The string to check.
 * @return True if the string is an assignment operator, false otherwise.
 */
inline bool isAsn(const std::string str) {
  if (str.empty() || str.length() > 2) {
    return false;
  }

  switch (str[0]) {
  case '=':
    if (str[1] == '=') {
      return false;
    }
    return true;
  case '+':
    if (str[1] == '=') {
      return true;
    }
    return false;
  case '-':
    if (str[1] == '=') {
      return true;
    }
    return false;
  case '*':
    if (str[1] == '=') {
      return true;
    }
    return false;
  case '/':
    if (str[1] == '=') {
      return true;
    }
    return false;
  default:
    return false;
  }
}

inline bool isDel(const std::string str) {
  if (str.empty()) {
    return false;
  }

  switch (str[0]) {
  case '(':
    return true;
  case ')':
    return true;
  case '[':
    return true;
  case ']':
    return true;
  case '{':
    return true;
  case '}':
    return true;
  case ':':
    return true;
  case ';':
    return true;
  case '.':
    return true;
  case ',':
    return true;
  default:
    return false;
  }
}

} // namespace ml::basic