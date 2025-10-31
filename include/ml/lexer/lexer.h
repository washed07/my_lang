/**
 * @file lexer.h
 * @brief Lexer definitions for My Language.
 * @details Defines the lexer class and related functions for tokenizing source
 * code.
 * @copyright Copyright (c) 2025 Karson P. Califf
 */

#pragma once

#include "cstdint"
#include "ml/basic/error.h"
#include "ml/basic/locus.h"
#include "ml/basic/syntax.h"
#include "ml/lexer/token.h"
#include <cctype>
#include <functional>
#include <memory>
#include <string>
#include <utility>
#include <vector>

namespace ml::lexer {

/**
 * @class Lexer lexer.h
 * @brief Lexer for tokenizing source code.
 * @details Provides methods to lex source code into tokens.
 * @note When creating an Abstract Syntax Tree (AST), use the Parser class
 * instead as it already integrates the Lexer.
 */
class Lexer {
private:
  std::string source_;            // Source code to be lexed
  char cached_peek_ = '\0';       // Cached character for peek
  bool peek_dirty_ = true;        // Dirty flag for cached peek
  std::string cached_look_ = "";  // Cached string for look
  bool look_dirty_ = true;        // Dirty flag for cached look
  std::string cached_value_ = ""; // Cached string for value
  bool value_dirty_ = true;       // Dirty flag for cached value
  basic::Locus start_,
      current_ = basic::Locus(1, 1, 0); // Current and start loci

  /**
   * @brief Checks if the lexer has reached the end of the source code.
   * @return True if the end of the source code is reached, false otherwise.
   */
  bool isEof() const;

  /**
   * @brief Gets the current lexeme value.
   * @return The current lexeme as a string.
   */
  std::string value();

  /**
   * @brief Peeks at the next character in the source code without advancing.
   * @return The next character, or '\0' if at the end of the source code.
   */
  std::string look();

  /**
   * @brief Peeks at the current character in the source code without advancing.
   * @return The current character, or '\0' if at the end of the source code.
   */
  char peek();

  /**
   * @brief Advances the lexer to the next character.
   * @return The current character before advancing, or '\0' if at the end of
   * the source code.
   */
  char advance();

  /**
   * @brief Takes characters from the source code while the predicate is true.
   * @param predicate A function that takes a character and returns true to
   * continue taking characters.
   */
  void take(const std::function<bool(char)> predicate);

  /**
   * @brief Ignores the current lexeme and resets the start locus.
   */
  void ignore();

  /**
   * @brief Creates a token of the specified kind.
   * @param kind The kind of token to create.
   * @return A unique pointer to the created token.
   */
  std::unique_ptr<Token> makeToken(const TokenKind kind);

  /**
   * @brief Lexes an alphanumeric token (identifier or keyword).
   * @return A unique pointer to the lexed token, or nullptr if not applicable.
   */
  std::unique_ptr<Token> lexAlpha();

  /**
   * @brief Lexes a numeric token (integer or float).
   * @return A unique pointer to the lexed token, or nullptr if not applicable.
   */
  std::unique_ptr<Token> lexNumeric();

  /**
   * @brief Lexes a character token.
   * @return A unique pointer to the lexed token, or nullptr if not applicable.
   */
  std::unique_ptr<Token> lexCharacter();

  /**
   * @brief Lexes a string token.
   * @return A unique pointer to the lexed token, or nullptr if not applicable.
   */
  std::unique_ptr<Token> lexString();

  /**
   * @brief Lexes an operator token.
   * @return A unique pointer to the lexed token, or nullptr if not applicable.
   */
  std::unique_ptr<Token> lexOperator();

  /**
   * @brief Lexes a delimiter token.
   * @return A unique pointer to the lexed token, or nullptr if not applicable.
   */
  std::unique_ptr<Token> lexDelimiter();

  /**
   * @brief Retrieves the next token from the source code.
   * @return A unique pointer to the next token.
   */
  std::unique_ptr<Token> next();

  /**
   * @brief Resets the lexer's state.
   */
  void reset();

public:
  explicit Lexer(const std::string source) : source_(source) {}

  /**
   * @brief Gets the source code being lexed.
   * @return The source code as a string.
   */
  const std::string &source() const { return this->source_; }

  /**
   * @brief Gets the start and current loci.
   * @return The start locus.
   */
  const basic::Locus &start() const { return this->start_; }

  /**
   * @brief Gets the current locus.
   * @return The current locus.
   */
  const basic::Locus &current() const { return this->current_; }

  /**
   * @brief Lexes the entire source code into a vector of tokens.
   * @param source The source code to lex.
   * @return A vector of unique pointers to the lexed tokens.
   */
  std::vector<std::unique_ptr<Token>> lex(const std::string source);
};
} // namespace ml::lexer