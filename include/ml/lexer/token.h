/**
 * @file token.h
 * @brief Token definitions for My Language.
 * @details Defines the Token struct and TokenKind enum.
 * @copyright Copyright (c) 2025 Karson P. Califf
 */

#pragma once

#include "ml/basic/locus.h"
#include <ostream>
#include <string>

namespace ml::lexer {

/**
 * @enum TokenKind token.h
 * @brief Enumeration of different kinds of tokens.
 */
enum class TokenKind {
  None,
  Integer,
  Float,
  Boolean,
  Character,
  String,
  Identifier,
  Keyword,
  Operator,
  Delimiter,
  Eof
};

/**
 * @brief Converts a TokenKind to its string representation.
 * @param kind The TokenKind to convert.
 * @return The string representation of the TokenKind.
 */
inline std::string tokenKindName(const TokenKind kind) {
  switch (kind) {
  case TokenKind::None:
    return "None";
  case TokenKind::Integer:
    return "Integer";
  case TokenKind::Float:
    return "Float";
  case TokenKind::Boolean:
    return "Boolean";
  case TokenKind::Character:
    return "Character";
  case TokenKind::String:
    return "String";
  case TokenKind::Identifier:
    return "Identifier";
  case TokenKind::Keyword:
    return "Keyword";
  case TokenKind::Operator:
    return "Operator";
  case TokenKind::Delimiter:
    return "Delimiter";
  case TokenKind::Eof:
    return "Eof";
  default:
    return "Unknown";
  }
}

/**
 * @struct Token token.h
 * @brief Represents a lexical token.
 * @details Contains the kind, value, and source location of the token.
 */
struct Token {

  /**
   * @var kind
   * @brief The kind of the token.
   */
  TokenKind kind;

  /**
   * @var value
   * @brief The string value of the token.
   */
  std::string value;
  /**
   * @var start
   * @brief The starting locus of the token in the source code.
   */
  basic::Locus start;

  /**
   * @var end
   * @brief The ending locus of the token in the source code.
   */
  basic::Locus end;

  Token() : kind(TokenKind::None), value("\0"), start(1, 1), end(1, 1) {}

  Token(TokenKind kind, std::string value, basic::Locus start, basic::Locus end)
      : kind(kind), value(value), start(start), end(end) {}

  /**
   * @brief Converts the Token to a string representation.
   * @return A string containing the token's details.
   */
  inline operator std::string() const {
    std::string repr("[");
    repr += (std::string)this->start += "-";
    repr += (std::string)this->end += "] ";
    repr += tokenKindName(this->kind) + " ";
    repr += this->value;
    return repr;
  }

  /**
   * @brief Creates a default Token instance.
   * @return A Token with default values.
   */
  static const Token Default() {
    return Token(TokenKind::None, "\0", basic::Locus(0, 0), basic::Locus(0, 0));
  }
};

} // namespace ml::lexer
