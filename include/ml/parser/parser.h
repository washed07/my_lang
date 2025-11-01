/**
 * @file parser.h
 * @brief Parser definitions for My Language.
 * @details Defines the Parser class and related functions for parsing source
 * code into an Abstract Syntax Tree (AST).
 * @copyright Copyright (c) 2025 Karson P. Califf
 */

#pragma once

#include "ml/ast/ast.h"
#include "ml/lexer/lexer.h"
#include "ml/lexer/token.h"
#include <cstdint>
#include <memory>
#include <vector>

namespace ml::parser {

/**
 * @class Parser parser.h
 * @brief Parser for converting source code into an Abstract Syntax Tree (AST).
 * @details Provides methods to parse source code into AST nodes.
 */
class Parser {
private:
  ml::lexer::Lexer lexer_; // The lexer instance for tokenizing source code
  std::vector<std::unique_ptr<ml::lexer::Token>> tokens_; // List of tokens
  uint64_t index_ = 0;          // Current index in the tokens list
  ml::lexer::Token last_token_; // The last consumed token

  /**
   * @brief Peeks at the current token without consuming it.
   * @return A pointer to the current token.
   */
  const ml::lexer::Token *peek() const;

  /**
   * @brief Peeks at a token at a specific offset without consuming it.
   * @param offset The offset from the current token.
   * @return A pointer to the token at the specified offset.
   */
  const ml::lexer::Token *look(const uint64_t offset) const;

  /**
   * @brief Advances to the next token and returns the current one.
   * @return A pointer to the current token before advancing.
   */
  const ml::lexer::Token *advance();

  /**
   * @brief Checks if the parser has reached the end of the token list.
   * @return True if the end of the token list is reached, false otherwise.
   */
  bool isEof() const;

  /**
   * @brief Expects the current token to be of a specific kind.
   * @param kind The expected TokenKind.
   * @param message The error message to display if the token does not match.
   * @return A pointer to the expected token.
   */
  const ml::lexer::Token *expectToken(const ml::lexer::TokenKind kind,
                                      const std::string &message);

  /**
   * @brief Expects the current token to have a specific value.
   * @param value The expected token value.
   * @param message The error message to display if the token does not match.
   * @return A pointer to the expected token.
   */
  const ml::lexer::Token *expectValue(const std::string &value,
                                      const std::string &message);

  /**
   * @brief Matches the current token against a specific kind and advances if it
   * matches.
   * @param kind The TokenKind to match.
   * @return True if the current token matches the kind, false otherwise.
   */
  bool matchToken(const ml::lexer::TokenKind kind);

  /**
   * @brief Matches the current token against a specific value and advances if
   * it matches.
   * @param value The expected token value.
   * @return True if the current token matches the value, false otherwise.
   */
  bool matchValue(const std::string &value);

  /**
   * @brief Checks if the current token is of a specific kind.
   * @param kind The TokenKind to check.
   * @return True if the current token is of the specified kind, false
   * otherwise.
   */
  bool checkToken(const ml::lexer::TokenKind kind);

  /**
   * @brief Checks if the current token has a specific value.
   * @param value The expected token value.
   * @return True if the current token has the specified value, false otherwise.
   */
  bool checkValue(const std::string &value);

  /**
   * @brief Parses the entire program and returns the AST root node.
   * @return A unique pointer to the Program AST node.
   */
  std::unique_ptr<ml::ast::Program> parseProgram();

  /**
   * @brief Parses a single statement.
   * @return A unique pointer to the parsed Statement AST node.
   */
  std::unique_ptr<ml::ast::Statement> parseStatement();

  /**
   * @brief Parses a return statement.
   * @return A unique pointer to the ReturnStatement AST node.
   */
  std::unique_ptr<ml::ast::ReturnStatement> parseReturn();

  /**
   * @brief Parses a break statement.
   * @return A unique pointer to the BreakStatement AST node.
   */
  std::unique_ptr<ml::ast::BreakStatement> parseBreak();

  /**
   * @brief Parses a continue statement.
   * @return A unique pointer to the ContinueStatement AST node.
   */
  std::unique_ptr<ml::ast::ContinueStatement> parseContinue();

  /**
   * @brief Parses a block statement.
   * @return A unique pointer to the BlockStatement AST node.
   */
  std::unique_ptr<ml::ast::BlockStatement> parseBlock();

  /**
   * @brief Parses a modifier statement.
   * @return A unique pointer to the ModifierStatement AST node.
   */
  std::unique_ptr<ml::ast::ModifierStatement> parseModifier();

  /**
   * @brief Parses an expression statement.
   * @return A unique pointer to the ExpressionStatement AST node.
   */
  std::unique_ptr<ml::ast::ExpressionStatement> parseExpressionStatement();

  /**
   * @brief Parses a declaration (variable, function, record, or class).
   * @param verbose Controls expected verbosity for variable declarations. i.e.
   * semi-colon requirement.
   * @return A unique pointer to the parsed Declaration AST node.
   */
  std::unique_ptr<ml::ast::VariableDeclaration> parseVariable(bool verbose,
                                                              bool semicolon);

  /**
   * @brief Parses a function declaration.
   * @return A unique pointer to the FunctionDeclaration AST node.
   */
  std::unique_ptr<ml::ast::FunctionDeclaration> parseFunction();

  /**
   * @brief Parses a record declaration.
   * @return A unique pointer to the RecordDeclaration AST node.
   */
  std::unique_ptr<ml::ast::RecordDeclaration> parseRecord();

  /**
   * @brief Parses a class declaration
   * @return A unique pointer to the ClassDeclaration AST node.
   */
  std::unique_ptr<ml::ast::ClassDeclaration> parseClass();

  /**
   * @brief Parses an if conditional statement.
   * @return A unique pointer to the IfConditional AST node.
   */
  std::unique_ptr<ml::ast::IfConditional> parseIf();

  /**
   * @brief Parses a switch conditional statement.
   * @return A unique pointer to the SwitchConditional AST node.
   */
  std::unique_ptr<ml::ast::SwitchConditional> parseSwitch();

  /**
   * @brief Parses a while conditional statement.
   * @return A unique pointer to the WhileConditional AST node.
   */
  std::unique_ptr<ml::ast::WhileConditional> parseWhile();

  /**
   * @brief Parses a for conditional statement.
   * @return A unique pointer to the ForConditional AST node.
   */
  std::unique_ptr<ml::ast::ForConditional> parseFor();

  /**
   * @brief Parses an expression.
   * @return A unique pointer to the parsed Expression AST node.
   */
  std::unique_ptr<ml::ast::Expression> parseExpression();

  /**
   * @brief Parses an assignment expression.
   * @return A unique pointer to the Assignment Expression AST node.
   */
  std::unique_ptr<ml::ast::Expression> parseAssignment();

  /**
   * @brief Parses a logical OR expression.
   * @return A unique pointer to the Logical OR Expression AST node.
   */
  std::unique_ptr<ml::ast::Expression> parseLogicalOr();

  /**
   * @brief Parses a logical AND expression.
   * @return A unique pointer to the Logical AND Expression AST node.
   */
  std::unique_ptr<ml::ast::Expression> parseLogicalAnd();

  /**
   * @brief Parses an equality expression.
   * @return A unique pointer to the Equality Expression AST node.
   */
  std::unique_ptr<ml::ast::Expression> parseEquality();

  /**
   * @brief Parses a comparison expression.
   * @return A unique pointer to the Comparison Expression AST node.
   */
  std::unique_ptr<ml::ast::Expression> parseComparison();

  /**
   * @brief Parses a term expression.
   * @return A unique pointer to the Term Expression AST node.
   */
  std::unique_ptr<ml::ast::Expression> parseTerm();

  /**
   * @brief Parses a factor expression.
   * @return A unique pointer to the Factor Expression AST node.
   */
  std::unique_ptr<ml::ast::Expression> parseFactor();

  /**
   * @brief Parses a unary expression.
   * @return A unique pointer to the Unary Expression AST node.
   */
  std::unique_ptr<ml::ast::Expression> parseUnary();

  /**
   * @brief Parses a postfix expression.
   * @return A unique pointer to the Postfix Expression AST node.
   */
  std::unique_ptr<ml::ast::Expression> parsePostfix();

  /**
   * @brief Parses a primary expression.
   * @return A unique pointer to the Primary Expression AST node.
   */
  std::unique_ptr<ml::ast::Expression> parsePrimary();

public:
  Parser() : lexer_(""), tokens_(), index_(0) {}

  /**
   * @brief Constructs a Parser with the given source code.
   * @param source The source code to parse.
   * @return A unique pointer to the Program AST node.
   */
  std::unique_ptr<ml::ast::Program> parse(const std::string &source);
};

} // namespace ml::parser
