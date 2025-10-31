/**
 * @file stmt.h
 * @brief Abstract Syntax Tree (AST) node definitions for statement statements.
 * @details Defines AST node structures for various statement types.
 * @copyright Copyright (c) 2025 Karson P. Califf
 */

#pragma once

#include "expr.h"
#include "node.h"
#include <memory>
#include <vector>

namespace ml::ast {

/**
 * @struct Statement stmt.h
 * @brief Base class for all statement nodes in the AST.
 * @details Inherits from Node and provides a common interface for all
 * statement types.
 */
struct Statement : public Node, public basic::Visitable<Statement> {
  Statement(const basic::Locus start, const basic::Locus end)
      : Node(start, end, NodeKind::Statement) {}

  ENABLE_VISITORS(Statement)

  virtual ~Statement() = default;
};

/**
 * @struct ReturnStatement stmt.h
 * @brief Represents a return statement in the AST.
 * @details Inherits from Statement and contains an optional expression
 * representing the return value.
 */
struct ReturnStatement : public Statement,
                         public basic::Visitable<ReturnStatement> {

  /**
   * @var expression
   * @brief The expression to be returned by the return statement.
   */
  std::unique_ptr<Expression> expression;

  ReturnStatement(const basic::Locus start, const basic::Locus end,
                  std::unique_ptr<Expression> expression)
      : Statement(start, end), expression(std::move(expression)) {}

  ENABLE_VISITORS(ReturnStatement)
};

/**
 * @struct BreakStatement stmt.h
 * @brief Represents a break statement in the AST.
 * @details Inherits from Statement and indicates a break in control flow.
 */
struct BreakStatement : public Statement,
                        public basic::Visitable<BreakStatement> {
  BreakStatement(const basic::Locus start, const basic::Locus end)
      : Statement(start, end) {}

  ENABLE_VISITORS(BreakStatement)
};

/**
 * @struct ContinueStatement stmt.h
 * @brief Represents a continue statement in the AST.
 * @details Inherits from Statement and indicates continuation in loops.
 */
struct ContinueStatement : public Statement,
                           public basic::Visitable<ContinueStatement> {
  ContinueStatement(const basic::Locus start, const basic::Locus end)
      : Statement(start, end) {}

  ENABLE_VISITORS(ContinueStatement)
};

/**
 * @struct ExpressionStatement stmt.h
 * @brief Represents an expression statement in the AST.
 * @details Inherits from Statement and contains an expression.
 */
struct ExpressionStatement : public Statement,
                             public basic::Visitable<ExpressionStatement> {

  /**
   * @var expression
   * @brief The expression contained in the expression statement.
   */
  std::unique_ptr<Expression> expression;

  ExpressionStatement(const basic::Locus start, const basic::Locus end,
                      std::unique_ptr<Expression> expression)
      : Statement(start, end), expression(std::move(expression)) {}

  ENABLE_VISITORS(ExpressionStatement)
};

/**
 * @struct BlockStatement stmt.h
 * @brief Represents a block statement in the AST.
 * @details Inherits from Statement and contains a list of statements
 */
struct BlockStatement : public Statement,
                        public basic::Visitable<BlockStatement> {

  /**
   * @var statements
   * @brief The list of statements contained in the block statement.
   */
  std::vector<std::unique_ptr<Statement>> statements;

  BlockStatement(const basic::Locus start, const basic::Locus end,
                 std::vector<std::unique_ptr<Statement>> statements)
      : Statement(start, end), statements(std::move(statements)) {}

  ENABLE_VISITORS(BlockStatement)
};

/**
 * @struct ModifierStatement stmt.h
 * @brief Represents a modifier statement in the AST.
 * @details Inherits from Statement and contains accessor and modifier info.
 */
struct ModifierStatement : public Statement,
                           public basic::Visitable<ModifierStatement> {

  /**
   * @var accessor
   * @brief The accessor type for the modifier statement.
   * @details Default is Private.
   */
  ml::basic::Accessor accessor = ml::basic::Accessor::Private;

  /**
   * @var modifier
   * @brief The modifier flags for the modifier statement.
   * @details Default is None.
   */
  ml::basic::Modifier modifier = ml::basic::Modifier::None;

  ModifierStatement(const basic::Locus start, const basic::Locus end)
      : Statement(start, end) {}

  ModifierStatement(const basic::Locus start, const basic::Locus end,
                    ml::basic::Accessor accessor, ml::basic::Modifier modifier)
      : Statement(start, end), accessor(accessor), modifier(modifier) {}

  ENABLE_VISITORS(ModifierStatement)
};

/**
 * @struct Program stmt.h
 * @brief Represents the root program node in the AST.
 * @details Inherits from Node and contains a list of top-level statements.
 */
struct Program : public Node, public basic::Visitable<Program> {

  /**
   * @var statements
   * @brief The list of top-level statements in the program.
   */
  std::vector<std::unique_ptr<Statement>> statements;

  Program(const basic::Locus start, const basic::Locus end,
          std::vector<std::unique_ptr<Statement>> statements)
      : Node(start, end), statements(std::move(statements)) {}

  ENABLE_VISITORS(Program)
};

} // namespace ml::ast