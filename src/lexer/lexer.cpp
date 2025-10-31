/**
 * @file lexer.cpp
 * @brief Lexer source code for My Language.
 * @copyright Copyright (c) 2025 Karson P. Califf
 */

#include "ml/lexer/lexer.h"

namespace ml::lexer {

bool Lexer::isEof() const {
  return this->current_.index >= this->source_.length();
}

std::string Lexer::value() {
  if (!this->value_dirty_) {
    return this->cached_value_;
  }

  this->cached_value_ = this->source_.substr(
      this->start_.index, this->current_.index - this->start_.index);
  this->value_dirty_ = false;

  return this->cached_value_;
}

std::string Lexer::look() {
  if (!this->look_dirty_) {
    return this->cached_look_;
  }

  this->cached_look_ = this->source_.substr(this->current_.index, 1);
  this->look_dirty_ = false;

  return this->cached_look_;
}

char Lexer::peek() {
  if (!this->peek_dirty_) {
    return this->cached_peek_;
  }

  if (this->isEof()) {
    return '\0';
  } else {
    this->cached_peek_ = this->source_[this->current_.index];
    this->peek_dirty_ = false;
    return this->cached_peek_;
  }
}

char Lexer::advance() {
  if (this->isEof()) {
    return '\0';
  } else {
    char current_char = this->source_[this->current_.index];

    this->peek_dirty_ = true;
    this->look_dirty_ = true;
    this->value_dirty_ = true;
    this->current_.index++;

    if (current_char == '\n') {
      this->current_.column = 1;
      this->current_.line++;
    } else {
      this->current_.column++;
    }

    return this->peek();
  }
}

void Lexer::take(const std::function<bool(char)> predicate) {
  while (predicate(this->peek())) {
    if (this->isEof()) {
      break;
    }
    this->advance();
  }
}

void Lexer::ignore() { this->start_ = this->current_; }

std::unique_ptr<Token> Lexer::makeToken(const TokenKind kind) {

  std::string value = this->value();

  basic::Locus start = this->start_;
  this->ignore();

  return std::make_unique<Token>(Token(kind, value, start, this->current_));
}

std::unique_ptr<Token> Lexer::lexAlpha() {
  if (std::isalpha(this->peek()) || this->peek() == '_') {
    this->take([](char c) {
      return std::isalnum(static_cast<unsigned char>(c)) || c == '_';
    });

    if (basic::isKwy(this->value())) {
      return this->makeToken(TokenKind::Keyword);
    } else {
      return this->makeToken(TokenKind::Identifier);
    }
  } else {
    return nullptr;
  }
}

std::unique_ptr<Token> Lexer::lexNumeric() {
  if (std::isdigit(this->peek())) {
    this->take(std::isdigit);

    if (this->peek() == '.') {
      // Check if this is a range operator '..'/'...' instead of a float
      if (this->current_.index + 1 < this->source_.length() &&
          this->source_[this->current_.index + 1] == '.') {
        return this->makeToken(TokenKind::Integer);
      } else {
        this->advance();
        this->take(std::isdigit);
        return this->makeToken(TokenKind::Float);
      }
    } else {
      return this->makeToken(TokenKind::Integer);
    }
  } else {
    return nullptr;
  }
}

std::unique_ptr<Token> Lexer::lexCharacter() {
  if (this->peek() == '\'') {
    this->advance(); // Opening quote
    if (this->peek() == '\\') {
      this->advance(); // Escape character
      this->advance(); // Escaped character
    } else if (this->peek() != '\'') {
      this->advance(); // Character
    } else {
      basic::Error err(basic::ErrorLevel::Error, "Empty character literal",
                       "Add a character between the single quotes (').",
                       this->start_, this->start_, "<input>", this->source_);
      err.log();
    }

    if (this->peek() != '\'') {
      basic::Error err(
          basic::ErrorLevel::Error, "Unterminated character literal",
          "Add a closing single quote (') to terminate the character literal.",
          this->start_, this->start_, "<input>", this->source_);
      err.log();
    } else {
      this->advance(); // Closing quote
    }
    return this->makeToken(TokenKind::Character);
  } else {
    return nullptr;
  }
}

std::unique_ptr<Token> Lexer::lexString() {
  if (this->peek() == '"') {
    this->advance(); // Opening quote

    while (this->peek() != '"') {
      if (this->isEof()) {
        basic::Error err(basic::ErrorLevel::Error,
                         "Unterminated string literal",
                         "Add a closing double quote (\") to terminate the "
                         "string literal.",
                         this->start_, this->start_, "<input>", this->source_);
        err.log();
        break;
      }

      this->advance();
    }
    this->advance(); // Closing quote
    return this->makeToken(TokenKind::String);
  } else {
    return nullptr;
  }
}

std::unique_ptr<Token> Lexer::lexOperator() {
  if (basic::isOp(this->look())) {
    this->advance(); // Operator
    if (basic::isOp(this->value() + this->peek())) {
      this->advance();
    }
    return this->makeToken(TokenKind::Operator);
  } else {
    return nullptr;
  }
}

std::unique_ptr<Token> Lexer::lexDelimiter() {
  if (basic::isDel(this->look())) {
    this->advance(); // Delimiter
    return this->makeToken(TokenKind::Delimiter);
  } else {
    return nullptr;
  }
}

std::unique_ptr<Token> Lexer::next() {
  this->take(basic::isWsp);
  this->ignore();

  if (this->isEof()) {
    // Create EOF token with empty value
    basic::Locus start = this->current_;
    return std::make_unique<Token>(
        Token(TokenKind::Eof, "", start, this->current_));
  } else if (auto alpha = this->lexAlpha()) {
    return alpha;
  } else if (auto numeric = this->lexNumeric()) {
    return numeric;
  } else if (auto character = this->lexCharacter()) {
    return character;
  } else if (auto string = this->lexString()) {
    return string;
  } else if (auto op = this->lexOperator()) {
    return op;
  } else if (auto del = this->lexDelimiter()) {
    return del;
  }
  return this->makeToken(TokenKind::None);
}

void Lexer::reset() {
  this->current_ = basic::Locus(1, 1, 0);
  this->start_ = basic::Locus(1, 1, 0);
  this->peek_dirty_ = true;
  this->look_dirty_ = true;
  this->value_dirty_ = true;
}

std::vector<std::unique_ptr<Token>> Lexer::lex(const std::string source) {
  this->source_ = source;
  this->reset();

  auto tokens = std::vector<std::unique_ptr<Token>>();

  while (true) {

    std::unique_ptr<Token> next = this->next();

    TokenKind kind = next->kind;
    tokens.push_back(std::move(next));

    if (kind == TokenKind::Eof || kind == TokenKind::None) {
      break;
    }
  }

  return tokens;
}

} // namespace ml::lexer