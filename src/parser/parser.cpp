/**
 * @file parser.cpp
 * @brief Parser source code for My Language.
 * @copyright Copyright (c) 2025 Karson P. Califf
 */

#include "ml/parser/parser.h"
#include "ml/basic/accessor.h"
#include "ml/basic/error.h"
#include "ml/basic/flags.h"
#include "ml/basic/modifier.h"

namespace ml::parser {

const ml::lexer::Token *Parser::peek() const {
  if (this->isEof()) {
    return nullptr;
  }
  return this->tokens_[this->index_].get();
}

const ml::lexer::Token *Parser::look(const uint64_t offset) const {
  if (this->index_ + offset >= this->tokens_.size()) {
    return nullptr;
  }
  return this->tokens_[this->index_ + offset].get();
}

const ml::lexer::Token *Parser::advance() {
  if (this->isEof()) {
    return nullptr;
  }
  this->last_token_ = *(this->tokens_[this->index_]);
  return this->tokens_[this->index_++].get();
}

bool Parser::isEof() const {
  if (this->index_ >= this->tokens_.size()) {
    return true;
  }
  if (this->index_ == this->tokens_.size() - 1) {
    auto *token = this->tokens_[this->index_].get();
    return token && token->value.empty();
  }
  return false;
}

const ml::lexer::Token *Parser::expectToken(const ml::lexer::TokenKind kind,
                                            const std::string &message) {
  if (auto *tok = this->peek(); tok->kind != kind || this->isEof()) {
    basic::Error err(basic::ErrorLevel::Error,
                     "Unexpected token: '" +
                         ml::lexer::tokenKindName(tok->kind) + "'",
                     "Expected token of kind: '" +
                         ml::lexer::tokenKindName(kind) + "' " + message,
                     tok ? tok->start : basic::Locus(0, 0),
                     tok ? tok->end : basic::Locus(0, 0), "<input>",
                     this->lexer_.source(), 0);
    err.log();
  }
  return this->advance();
}

const ml::lexer::Token *Parser::expectValue(const std::string &value,
                                            const std::string &message) {
  if (this->isEof()) {
    basic::Error err(basic::ErrorLevel::Error, "Unexpected end of input",
                     "Expected value: '" + value + "' " + message,
                     basic::Locus(1, 1), basic::Locus(1, 1), "<input>",
                     this->lexer_.source(), 0);
    err.log();
    return nullptr;
  }

  const auto *tok = this->peek();
  if (!tok || tok->value != value) {
    basic::Error err(basic::ErrorLevel::Error,
                     "Unexpected value: '" + (tok ? tok->value : "null") + "'",
                     "Expected value: '" + value + "' " + message,
                     tok ? tok->start : basic::Locus(1, 1),
                     tok ? tok->end : basic::Locus(1, 1), "<input>",
                     this->lexer_.source(), 0);
    err.log();
  }
  return this->advance();
}

bool Parser::matchToken(const ml::lexer::TokenKind kind) {
  if (auto *tok = this->peek(); !this->isEof() && tok->kind == kind) {
    this->advance();
    return true;
  }
  return false;
}

bool Parser::matchValue(const std::string &value) {
  if (auto *tok = this->peek(); !this->isEof() && tok->value == value) {
    this->advance();
    return true;
  }
  return false;
}

bool Parser::checkToken(const ml::lexer::TokenKind kind) {
  if (auto *tok = this->peek(); !this->isEof() && tok->kind == kind) {
    return true;
  }
  return false;
}

bool Parser::checkValue(const std::string &value) {
  if (auto *tok = this->peek(); !this->isEof() && tok->value == value) {
    return true;
  }
  return false;
}

std::unique_ptr<ml::ast::Program> Parser::parseProgram() {
  std::vector<std::unique_ptr<ml::ast::Statement>> statements;
  while (!this->isEof()) {
    auto stmt = this->parseStatement();
    if (stmt) {
      statements.push_back(std::move(stmt));
    } else {
      this->advance();
    }
  }
  basic::Locus start =
      statements.empty() ? basic::Locus(1, 1) : statements.front()->start;
  basic::Locus end =
      statements.empty() ? basic::Locus(1, 1) : statements.back()->end;
  return std::make_unique<ml::ast::Program>(start, end, std::move(statements));
}

std::unique_ptr<ml::ast::Statement> Parser::parseStatement() {
  if (this->checkValue("return")) {
    return this->parseReturn();
  } else if (this->checkValue("break")) {
    return this->parseBreak();
  } else if (this->checkValue("continue")) {
    return this->parseContinue();
  } else if (this->checkValue("{")) {
    return this->parseBlock();
  } else if (this->checkValue("let")) {
    return this->parseVariable(true, true);
  } else if (this->checkValue("fn") || basic::isacc(this->peek()->value)) {
    return this->parseFunction();
  } else if (this->checkValue("rec")) {
    return this->parseRecord();
  } else if (this->checkValue("cls")) {
    return this->parseClass();
  } else if (this->checkValue("if")) {
    return this->parseIf();
  } else if (this->checkValue("switch")) {
    return this->parseSwitch();
  } else if (this->checkValue("while")) {
    return this->parseWhile();
  } else if (this->checkValue("for")) {
    return this->parseFor();
  } else {
    return this->parseExpressionStatement();
  }
}

std::unique_ptr<ml::ast::ReturnStatement> Parser::parseReturn() {
  auto returnToken = this->expectValue("return", "to start return statement");
  if (this->matchValue(";")) {
    return std::make_unique<ml::ast::ReturnStatement>(
        returnToken->start, returnToken->end, nullptr);
  }
  auto expr = this->parseExpression();
  this->expectValue(";", "after return expression");
  return std::make_unique<ml::ast::ReturnStatement>(returnToken->start,
                                                    expr->end, std::move(expr));
}

std::unique_ptr<ml::ast::BreakStatement> Parser::parseBreak() {
  auto breakToken = this->expectValue("break", "");
  auto *semicolonToken = this->expectValue(";", "after break statement");
  return std::make_unique<ml::ast::BreakStatement>(breakToken->start,
                                                   semicolonToken->end);
}

std::unique_ptr<ml::ast::ContinueStatement> Parser::parseContinue() {
  auto continueToken = this->expectValue("continue", "");
  auto *semicolonToken = this->expectValue(";", "after continue statement");
  return std::make_unique<ml::ast::ContinueStatement>(continueToken->start,
                                                      semicolonToken->end);
}

std::unique_ptr<ml::ast::BlockStatement> Parser::parseBlock() {
  auto leftBrace = this->expectValue("{", "to start a block statement");
  std::vector<std::unique_ptr<ml::ast::Statement>> statements;
  while (!this->isEof() && !this->checkValue("}")) {
    if (auto stmt = this->parseStatement()) {
      statements.push_back(std::move(stmt));
    } else {
      this->advance();
    }
  }
  auto *rightBrace = this->expectValue("}", "to end a block statement");
  return std::make_unique<ml::ast::BlockStatement>(
      leftBrace->start, rightBrace->end, std::move(statements));
}

std::unique_ptr<ml::ast::ModifierStatement> Parser::parseModifier() {
  basic::Locus start = this->peek()->start;
  auto accessor = ml::basic::Accessor::Private;
  if (basic::isacc(this->peek()->value)) {
    auto accToken = this->advance();
    accessor = basic::getacc(accToken->value);
  }
  auto modifier = ml::basic::Modifier::None;
  basic::Locus end = start;
  while (basic::ismod(this->peek()->value)) {
    auto modToken = this->advance();
    modifier |= basic::getmod(modToken->value);
    end = modToken->end;
  }
  return std::make_unique<ml::ast::ModifierStatement>(start, end, accessor,
                                                      modifier);
}

std::unique_ptr<ml::ast::ExpressionStatement>
Parser::parseExpressionStatement() {
  auto expr = this->parseExpression();
  if (!expr) {
    return nullptr;
  }
  auto *semicolonToken = this->expectValue(";", "after expression statement");
  return std::make_unique<ml::ast::ExpressionStatement>(
      expr->start, semicolonToken->end, std::move(expr));
}

std::unique_ptr<ml::ast::VariableDeclaration>
Parser::parseVariable(bool verbose, bool semicolon) {
  if (verbose) {
    this->expectValue("let", "");
  }

  auto modifier = this->parseModifier();

  auto *identifierToken = this->expectToken(
      ml::lexer::TokenKind::Identifier, "after 'let' in variable declaration");
  std::unique_ptr<ml::ast::IdentifierExpression> identifier =
      std::make_unique<ml::ast::IdentifierExpression>(
          identifierToken->start, identifierToken->end, identifierToken->value);

  if (this->matchValue(":")) {
    auto typeIdentifierToken = this->expectToken(
        ml::lexer::TokenKind::Identifier, "after ':' in variable declaration");
    std::unique_ptr<ml::ast::IdentifierExpression> type;
    if (this->matchValue("[")) {
      std::unique_ptr<ml::ast::Expression> size;
      if (this->checkValue("]")) {
        size = std::make_unique<ml::ast::LiteralExpression>(
            typeIdentifierToken->start, typeIdentifierToken->end, "-1",
            ast::LiteralType::Integer);
      } else {
        size = this->parseExpression();
      }
      this->expectValue("]", "after array size in variable declaration");
      type = std::make_unique<ml::ast::ArrayIdentifierExpression>(
          typeIdentifierToken->start, typeIdentifierToken->end,
          typeIdentifierToken->value, std::move(size));
    } else {
      type = std::make_unique<ml::ast::IdentifierExpression>(
          typeIdentifierToken->start, typeIdentifierToken->end,
          typeIdentifierToken->value);
    }
    if (this->matchValue("?")) {
      modifier->modifier |= ml::basic::Modifier::Nullable;
    }
    std::unique_ptr<ml::ast::Expression> initializer = nullptr;
    if (this->matchValue("=")) {
      auto initExpr = this->parseExpression();
      initializer = std::move(initExpr);
    }
    if (semicolon) {
      this->expectValue(";", "after variable declaration");
    }
    return std::make_unique<ml::ast::VariableDeclaration>(
        identifierToken->start, initializer ? initializer->end : type->end,
        std::move(identifier), std::move(type), std::move(modifier),
        std::move(initializer));
  } else if (this->checkToken(ml::lexer::TokenKind::Identifier)) {
    basic::Error err(basic::ErrorLevel::Warning,
                     "Type annotation missing ':' in variable declaration",
                     "Assuming type annotation is present before type name",
                     this->peek()->start, this->peek()->end, "<input>",
                     this->lexer_.source(), 0);
    err.log();
    auto typeIdentifierToken = this->advance();
    std::unique_ptr<ml::ast::IdentifierExpression> type;
    if (this->matchValue("[")) {
      std::unique_ptr<ml::ast::Expression> size;
      if (this->checkValue("]")) {
        size = std::make_unique<ml::ast::LiteralExpression>(
            typeIdentifierToken->start, typeIdentifierToken->end, "-1",
            ast::LiteralType::Integer);
      } else {
        size = this->parseExpression();
      }
      this->expectValue("]", "after array size in variable declaration");
      type = std::make_unique<ml::ast::ArrayIdentifierExpression>(
          typeIdentifierToken->start, typeIdentifierToken->end,
          typeIdentifierToken->value, std::move(size));
      std::unique_ptr<ml::ast::Expression> initializer = nullptr;
      if (this->matchValue("=")) {
        auto initExpr = this->parseExpression();
        initializer = std::move(initExpr);
      }
      if (semicolon) {
        this->expectValue(";", "after variable declaration");
      }
      return std::make_unique<ml::ast::VariableDeclaration>(
          identifierToken->start, initializer ? initializer->end : type->end,
          std::move(identifier), std::move(type), std::move(modifier),
          std::move(initializer));
    } else {
      type = std::make_unique<ml::ast::IdentifierExpression>(
          typeIdentifierToken->start, typeIdentifierToken->end,
          typeIdentifierToken->value);
      std::unique_ptr<ml::ast::Expression> initializer = nullptr;
      if (this->matchValue("=")) {
        auto initExpr = this->parseExpression();
        initializer = std::move(initExpr);
      }
      if (semicolon) {
        this->expectValue(";", "after variable declaration");
      }
      return std::make_unique<ml::ast::VariableDeclaration>(
          identifierToken->start, initializer ? initializer->end : type->end,
          std::move(identifier), std::move(type), std::move(modifier),
          std::move(initializer));
    }
  } else {
    if (this->matchValue("?")) {
      modifier->modifier |= ml::basic::Modifier::Nullable;
    }
    std::unique_ptr<ml::ast::Expression> initializer = nullptr;
    if (this->matchValue("=")) {
      auto initExpr = this->parseExpression();
      initializer = std::move(initExpr);
    }
    if (semicolon) {
      this->expectValue(";", "after variable declaration");
    }
    return std::make_unique<ml::ast::VariableDeclaration>(
        identifierToken->start,
        initializer ? initializer->end : identifierToken->end,
        std::move(identifier),
        std::make_unique<ml::ast::IdentifierExpression>(
            ml::basic::Locus(0, 0), ml::basic::Locus(0, 0), "void"),
        std::move(modifier), std::move(initializer));
  }
}

std::unique_ptr<ml::ast::FunctionDeclaration> Parser::parseFunction() {
  auto modifier = this->parseModifier();
  this->expectValue("fn", "to start function declaration");

  if (basic::isacc(this->peek()->value)) {
    basic::Error err(
        basic::ErrorLevel::Error, "Invalid accessor position for function",
        "Function accessors are positioned before the 'fn' keyword",
        this->peek()->start, this->peek()->end, "<input>",
        this->lexer_.source(), 0);
    err.log();
  }
  std::unique_ptr<ml::ast::IdentifierExpression> identifier;
  if (hasFlag(modifier->modifier, ml::basic::Modifier::Init)) {
    identifier = std::make_unique<ml::ast::IdentifierExpression>(
        ml::basic::Locus(0, 0), ml::basic::Locus(0, 0), "init");
  } else {
    auto *identifierToken = this->expectToken(
        ml::lexer::TokenKind::Identifier, "after 'fn' in function declaration");
    identifier = std::make_unique<ml::ast::IdentifierExpression>(
        identifierToken->start, identifierToken->end, identifierToken->value);
  }

  if (this->matchValue("?")) {
    modifier->modifier |= ml::basic::Modifier::Nullable;
  }

  this->expectValue("(", "after function name in function declaration");
  std::vector<std::unique_ptr<ml::ast::VariableDeclaration>> parameters;
  if (!this->matchValue(")")) {
    do {
      auto param = this->parseVariable(false, false);
      if (param) {
        parameters.push_back(std::move(param));
      } else {
        break;
      }
    } while (this->matchValue(","));
    this->expectValue(")", "after function parameters in function declaration");
  }

  std::unique_ptr<ml::ast::IdentifierExpression> typeIdentifier =
      std::make_unique<ml::ast::IdentifierExpression>(
          ml::basic::Locus(0, 0), ml::basic::Locus(0, 0), "void");
  if (this->matchValue(":")) {
    auto typeIdentifierToken = this->expectToken(
        ml::lexer::TokenKind::Identifier, "after ':' in function declaration");

    if (this->matchValue("[")) {
      std::unique_ptr<ml::ast::Expression> size;
      if (this->checkValue("]")) {
        size = std::make_unique<ml::ast::LiteralExpression>(
            typeIdentifierToken->start, typeIdentifierToken->end, "-1",
            ast::LiteralType::Integer);
      } else {
        size = this->parseExpression();
      }
      this->expectValue("]", "after array size in variable declaration");
      typeIdentifier = std::make_unique<ml::ast::ArrayIdentifierExpression>(
          typeIdentifierToken->start, typeIdentifierToken->end,
          typeIdentifierToken->value, std::move(size));
    } else {
      typeIdentifier = std::make_unique<ml::ast::IdentifierExpression>(
          typeIdentifierToken->start, typeIdentifierToken->end,
          typeIdentifierToken->value);
    }
  } else if (this->checkToken(ml::lexer::TokenKind::Identifier)) {
    basic::Error err(basic::ErrorLevel::Warning,
                     "Type annotation missing ':' in function declaration",
                     "Assuming type annotation is present before type name",
                     this->last_token_.end, this->peek()->end, "<input>",
                     this->lexer_.source(), 0);
    err.log();
    auto typeIdentifierToken = this->expectToken(
        ml::lexer::TokenKind::Identifier, "after ':' in function declaration");
    if (this->matchValue("[")) {
      std::unique_ptr<ml::ast::Expression> size;
      if (this->checkValue("]")) {
        size = std::make_unique<ml::ast::LiteralExpression>(
            typeIdentifierToken->start, typeIdentifierToken->end, "-1",
            ast::LiteralType::Integer);
      } else {
        size = this->parseExpression();
      }
      this->expectValue("]", "after array size in variable declaration");
      typeIdentifier = std::make_unique<ml::ast::ArrayIdentifierExpression>(
          typeIdentifierToken->start, typeIdentifierToken->end,
          typeIdentifierToken->value, std::move(size));
    } else {
      typeIdentifier = std::make_unique<ml::ast::IdentifierExpression>(
          typeIdentifierToken->start, typeIdentifierToken->end,
          typeIdentifierToken->value);
    }
  }

  auto body = this->parseBlock();

  return std::make_unique<ml::ast::FunctionDeclaration>(
      identifier->start, body->end, std::move(identifier),
      std::move(typeIdentifier), std::move(modifier), std::move(parameters),
      std::move(body));
}

std::unique_ptr<ml::ast::RecordDeclaration> Parser::parseRecord() {
  this->expectValue("rec", "");
  auto modifier = this->parseModifier();
  auto *identifierToken = this->expectToken(
      ml::lexer::TokenKind::Identifier, "after 'rec' in record declaration");
  auto identifier = std::make_unique<ml::ast::IdentifierExpression>(
      identifierToken->start, identifierToken->end, identifierToken->value);

  // Parse fields
  this->expectValue("{", "after record name in record declaration");
  std::vector<std::unique_ptr<ml::ast::VariableDeclaration>> fields;
  while (!this->isEof() && !this->checkValue("}")) {
    auto field = this->parseVariable(false, true);
    if (field) {
      fields.push_back(std::move(field));
    } else {
      break;
    }
  }
  this->expectValue("}", "after record fields in record declaration");

  auto type = std::make_unique<ml::ast::IdentifierExpression>(
      identifierToken->start, identifierToken->end, identifierToken->value);

  return std::make_unique<ml::ast::RecordDeclaration>(
      identifierToken->start, this->last_token_.end, std::move(identifier),
      std::move(type), std::move(modifier), std::move(fields));
}

std::unique_ptr<ml::ast::ClassDeclaration> Parser::parseClass() {
  this->expectValue("cls", "");
  auto modifier = this->parseModifier();
  auto *identifierToken = this->expectToken(
      ml::lexer::TokenKind::Identifier, "after 'class' in class declaration");
  auto identifier = std::make_unique<ml::ast::IdentifierExpression>(
      identifierToken->start, identifierToken->end, identifierToken->value);
  std::vector<std::unique_ptr<ml::ast::VariableDeclaration>> fields;
  std::vector<std::unique_ptr<ml::ast::FunctionDeclaration>> methods;
  this->expectValue("{", "after class name in class declaration");
  uint64_t i = 0;
  while (!this->isEof() && !this->checkValue("}")) {
    const auto *token_i = this->look(i);
    if ((token_i && basic::isacc(token_i->value)) ||
        (token_i && basic::ismod(token_i->value))) {
      uint64_t j = i;
      const auto *token_j = this->look(j);
      while (token_j &&
             (basic::isacc(token_j->value) || basic::ismod(token_j->value))) {
        j++;
        token_j = this->look(j);
      }
      if (token_j && token_j->value == "fn") {
        this->index_ += i;
        auto method = this->parseFunction();
        if (method) {
          methods.push_back(std::move(method));
        }
        i = 0;
      } else {
        this->index_ += i;
        auto field = this->parseVariable(false, true);
        if (field) {
          fields.push_back(std::move(field));
        }
        i = 0;
      }
    } else if (token_i && token_i->value == "fn") {
      this->index_ += i;
      auto method = this->parseFunction();
      if (method) {
        methods.push_back(std::move(method));
      }
      i = 0;
    } else {
      i++;
    }
  }
  this->expectValue("}", "after class fields and methods in class declaration");

  auto type = std::make_unique<ml::ast::IdentifierExpression>(
      identifierToken->start, identifierToken->end, identifierToken->value);

  return std::make_unique<ml::ast::ClassDeclaration>(
      identifierToken->start, this->last_token_.end, std::move(identifier),
      std::move(type), std::move(modifier), std::move(fields),
      std::move(methods));
}

std::unique_ptr<ml::ast::IfConditional> Parser::parseIf() {
  this->expectValue("if", "to start if conditional");
  auto condition = this->parseExpression();
  auto thenBranch = this->parseBlock();

  std::vector<std::unique_ptr<ml::ast::IfConditional>> elifBranches = {};
  if (this->matchValue("elif")) {
    do {
      auto elifCondition = this->parseExpression();
      auto elifThenBranch = this->parseBlock();
      elifBranches.push_back(std::make_unique<ml::ast::IfConditional>(
          elifCondition->start, elifThenBranch->end, std::move(elifCondition),
          std::move(elifThenBranch),
          std::vector<std::unique_ptr<ml::ast::IfConditional>>{}, nullptr));
    } while (this->matchValue("elif"));
    std::unique_ptr<ml::ast::BlockStatement> elseBranch = nullptr;
    if (this->matchValue("else")) {
      elseBranch = this->parseBlock();
    }
    return std::make_unique<ml::ast::IfConditional>(
        condition->start,
        elseBranch ? elseBranch->end : elifBranches.back()->end,
        std::move(condition), std::move(thenBranch), std::move(elifBranches),
        std::move(elseBranch));
  }

  std::unique_ptr<ml::ast::BlockStatement> elseBranch = nullptr;
  if (this->matchValue("else")) {
    elseBranch = this->parseBlock();
  }

  return std::make_unique<ml::ast::IfConditional>(
      condition->start, elseBranch ? elseBranch->end : thenBranch->end,
      std::move(condition), std::move(thenBranch), std::move(elifBranches),
      std::move(elseBranch));
}

std::unique_ptr<ml::ast::SwitchConditional> Parser::parseSwitch() {
  this->expectValue("switch", "to start switch conditional");
  auto switchExpression = this->parseExpression();
  this->expectValue("{", "after switch expression in switch conditional");
  std::vector<std::unique_ptr<ml::ast::Conditional>> cases;
  while (!this->isEof() && !this->checkValue("}")) {
    if (this->matchValue("default")) {
      auto defaultBlock = this->parseBlock();
      cases.push_back(std::make_unique<ml::ast::Conditional>(
          defaultBlock->start, defaultBlock->end, nullptr,
          std::move(defaultBlock)));
      continue;
    }
    this->expectValue("case", "to start switch case");
    auto caseExpression = this->parseExpression();
    auto caseBlock = this->parseBlock();
    cases.push_back(std::make_unique<ml::ast::Conditional>(
        caseExpression->start, caseBlock->end, std::move(caseExpression),
        std::move(caseBlock)));
  }
  this->expectValue("}", "to end switch conditional");
  return std::make_unique<ml::ast::SwitchConditional>(
      switchExpression->start, cases.back()->end, std::move(switchExpression),
      std::move(cases));
}

std::unique_ptr<ml::ast::WhileConditional> Parser::parseWhile() {
  this->expectValue("while", "to start while conditional");
  auto condition = this->parseExpression();
  auto body = this->parseBlock();
  return std::make_unique<ml::ast::WhileConditional>(
      condition->start, body->end, std::move(condition), std::move(body));
}

std::unique_ptr<ml::ast::ForConditional> Parser::parseFor() {
  this->expectValue("for", "to start for conditional");
  this->expectValue("(", "after 'for' in for conditional");

  if (this->checkValue("let")) {
    auto initializer = this->parseVariable(false, true);

    std::unique_ptr<ml::ast::Expression> condition = this->parseExpression();
    this->expectValue(";", "after for loop condition");

    std::unique_ptr<ml::ast::Expression> increment = nullptr;
    if (!this->matchValue(")")) {
      increment = this->parseExpression();
      this->expectValue(")", "after for loop increment");
    }

    auto body = this->parseBlock();
    return std::make_unique<ml::ast::ForConditional>(
        initializer->start, body->end, std::move(initializer),
        std::move(condition), std::move(increment), std::move(body));
  } else {
    if (this->checkToken(ml::lexer::TokenKind::Identifier) &&
        (this->look(1)->value == ":")) {
      auto initializer = this->parseVariable(false, false);
      this->expectValue("in", "after for-each variable declaration");
      auto iterable = this->parseExpression();
      this->expectValue(")", "after for-each iterable expression");
      auto body = this->parseBlock();
      return std::make_unique<ml::ast::ForConditional>(
          initializer->start, body->end, std::move(initializer), nullptr,
          std::move(iterable), std::move(body));
    } else {
      auto condition = this->parseExpression();
      this->expectValue(")", "after for-range condition");
      auto body = this->parseBlock();
      return std::make_unique<ml::ast::ForConditional>(
          condition->start, body->end, nullptr, std::move(condition), nullptr,
          std::move(body));
    }
  }
}

std::unique_ptr<ml::ast::Expression> Parser::parseExpression() {
  return this->parseAssignment();
}

std::unique_ptr<ml::ast::Expression> Parser::parseAssignment() {
  auto expr = this->parseLogicalOr();
  if (this->matchValue("=")) {
    auto right = this->parseExpression();
    return std::make_unique<ml::ast::BinaryExpression>(
        expr->start, right->end, std::move(expr), "=", std::move(right));
  }
  return expr;
}

std::unique_ptr<ml::ast::Expression> Parser::parseLogicalOr() {
  auto expr = this->parseLogicalAnd();
  while (this->matchValue("||")) {
    auto opToken = this->tokens_[this->index_ - 1].get();
    auto right = this->parseLogicalAnd();
    expr = std::make_unique<ml::ast::BinaryExpression>(
        expr->start, right->end, std::move(expr), opToken->value,
        std::move(right));
  }
  return expr;
}

std::unique_ptr<ml::ast::Expression> Parser::parseLogicalAnd() {
  auto expr = this->parseEquality();
  while (this->matchValue("&&")) {
    auto opToken = this->tokens_[this->index_ - 1].get();
    auto right = this->parseEquality();
    expr = std::make_unique<ml::ast::BinaryExpression>(
        expr->start, right->end, std::move(expr), opToken->value,
        std::move(right));
  }
  return expr;
}

std::unique_ptr<ml::ast::Expression> Parser::parseEquality() {
  auto expr = this->parseComparison();
  while (this->matchValue("==") || this->matchValue("!=")) {
    auto opToken = this->tokens_[this->index_ - 1].get();
    auto right = this->parseComparison();
    expr = std::make_unique<ml::ast::BinaryExpression>(
        expr->start, right->end, std::move(expr), opToken->value,
        std::move(right));
  }
  return expr;
}

std::unique_ptr<ml::ast::Expression> Parser::parseComparison() {
  auto expr = this->parseTerm();
  while (this->matchValue("<") || this->matchValue(">") ||
         this->matchValue("<=") || this->matchValue(">=") ||
         this->matchValue("..") || this->matchValue("...")) {
    auto opToken = this->tokens_[this->index_ - 1].get();
    auto right = this->parseTerm();
    expr = std::make_unique<ml::ast::BinaryExpression>(
        expr->start, right->end, std::move(expr), opToken->value,
        std::move(right));
  }
  return expr;
}

std::unique_ptr<ml::ast::Expression> Parser::parseTerm() {
  auto expr = this->parseFactor();
  while (this->matchValue("+") || this->matchValue("-")) {
    auto opToken = this->tokens_[this->index_ - 1].get();
    auto right = this->parseFactor();
    expr = std::make_unique<ml::ast::BinaryExpression>(
        expr->start, right->end, std::move(expr), opToken->value,
        std::move(right));
  }
  return expr;
}

std::unique_ptr<ml::ast::Expression> Parser::parseFactor() {
  auto expr = this->parseUnary();
  while (this->matchValue("*") || this->matchValue("/") ||
         this->matchValue("%")) {
    auto opToken = this->tokens_[this->index_ - 1].get();
    auto right = this->parseUnary();
    expr = std::make_unique<ml::ast::BinaryExpression>(
        expr->start, right->end, std::move(expr), opToken->value,
        std::move(right));
  }
  return expr;
}

std::unique_ptr<ml::ast::Expression> Parser::parseUnary() {
  if (this->matchValue("!") || this->matchValue("-")) {
    auto opToken = this->tokens_[this->index_ - 1].get();
    auto right = this->parseUnary();
    return std::make_unique<ml::ast::UnaryExpression>(
        opToken->start, right->end, opToken->value, std::move(right));
  }
  return this->parsePostfix();
}

std::unique_ptr<ml::ast::Expression> Parser::parsePostfix() {
  auto expr = this->parsePrimary();

  while (true) {
    if (this->matchValue("(")) {
      std::vector<std::unique_ptr<ml::ast::Expression>> args;
      if (!this->checkValue(")")) {
        do {
          auto arg = this->parseExpression();
          args.push_back(std::move(arg));
        } while (this->matchValue(","));
      }
      auto *rightParen =
          this->expectValue(")", "after function call arguments");
      expr = std::make_unique<ml::ast::CallExpression>(
          expr->start, rightParen->end, std::move(expr), std::move(args));
    } else if (this->matchValue("++") || this->matchValue("--")) {
      auto opToken = this->tokens_[this->index_ - 1].get();
      expr = std::make_unique<ml::ast::UnaryExpression>(
          expr->start, opToken->end, opToken->value, std::move(expr));
    } else if (this->matchValue(".")) {
      auto attribute = this->parseExpression();
      expr = std::make_unique<ml::ast::AttributeExpression>(
          expr->start, attribute->end, std::move(expr), std::move(attribute));
    } else if (this->matchValue("[")) {
      auto index = this->parseExpression();
      this->expectValue("]", "after index expression");
      expr = std::make_unique<ml::ast::IndexExpression>(
          expr->start, index->end, std::move(expr), std::move(index));
    } else {
      break;
    }
  }
  return expr;
}

std::unique_ptr<ml::ast::Expression> Parser::parsePrimary() {
  if (this->matchValue("true")) {
    auto *token = this->tokens_[this->index_ - 1].get();
    return std::make_unique<ml::ast::LiteralExpression>(
        token->start, token->end, "true", ast::LiteralType::Boolean);
  }
  if (this->matchValue("false")) {
    auto *token = this->tokens_[this->index_ - 1].get();
    return std::make_unique<ml::ast::LiteralExpression>(
        token->start, token->end, "false", ast::LiteralType::Boolean);
  }
  if (this->matchValue("this")) {
    auto *token = this->tokens_[this->index_ - 1].get();
    return std::make_unique<ml::ast::IdentifierExpression>(
        token->start, token->end, token->value);
  }
  if (this->matchToken(ml::lexer::TokenKind::Integer)) {
    auto *token = this->tokens_[this->index_ - 1].get();
    return std::make_unique<ml::ast::LiteralExpression>(
        token->start, token->end, token->value, ast::LiteralType::Integer);
  }
  if (this->matchToken(ml::lexer::TokenKind::Float)) {
    auto *token = this->tokens_[this->index_ - 1].get();
    return std::make_unique<ml::ast::LiteralExpression>(
        token->start, token->end, token->value, ast::LiteralType::Float);
  }
  if (this->matchToken(ml::lexer::TokenKind::String)) {
    auto *token = this->tokens_[this->index_ - 1].get();
    return std::make_unique<ml::ast::LiteralExpression>(
        token->start, token->end, token->value, ast::LiteralType::String);
  }
  if (this->matchToken(ml::lexer::TokenKind::Character)) {
    auto *token = this->tokens_[this->index_ - 1].get();
    return std::make_unique<ml::ast::LiteralExpression>(
        token->start, token->end, token->value, ast::LiteralType::Character);
  }
  if (this->matchToken(ml::lexer::TokenKind::Identifier)) {
    auto *token = this->tokens_[this->index_ - 1].get();
    return std::make_unique<ml::ast::IdentifierExpression>(
        token->start, token->end, token->value);
  }
  if (this->matchValue("(")) {
    auto expr = this->parseExpression();
    this->expectValue(")", "after expression");
    return expr;
  }
  if (this->matchValue("[")) {
    std::vector<std::unique_ptr<ml::ast::Expression>> elements;
    if (!this->checkValue("]")) {
      do {
        auto element = this->parseExpression();
        elements.push_back(std::move(element));
      } while (this->matchValue(","));
    }
    auto *rightBracket = this->expectValue("]", "after array elements");
    return std::make_unique<ml::ast::ArrayExpression>(
        this->last_token_.start, rightBracket->end, std::move(elements));
  }

  if (this->isEof() || (this->peek() && this->peek()->value.empty())) {
    return nullptr;
  }

  basic::Error err(basic::ErrorLevel::Error, "Unexpected token",
                   "Expected primary expression",
                   this->peek() ? this->peek()->start : basic::Locus(0, 0),
                   this->peek() ? this->peek()->end : basic::Locus(0, 0),
                   "<input>", this->lexer_.source(), 0);
  err.log();

  this->advance();
  return nullptr;
}

std::unique_ptr<ml::ast::Program> Parser::parse(const std::string &source) {
  this->lexer_ = ml::lexer::Lexer(source);
  this->tokens_ = this->lexer_.lex(source);
  this->index_ = 0;

  for (const auto &token : this->tokens_) {
    std::cout << (std::string)*token << std::endl;
  }

  auto result = this->parseProgram();
  return result;
}

} // namespace ml::parser
