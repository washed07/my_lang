/**
 * @file cond.h
 * @brief Abstract Syntax Tree (AST) node definitions for conditional
 * statements.
 * @details Defines AST node structures for various conditional statements.
 * @copyright Copyright (c) 2025 Karson P. Califf
 */

#pragma once

#include "decl.h"
#include "expr.h"
#include "node.h"
#include "stmt.h"
#include <memory>

namespace ml::ast {

/**
 * @struct Conditional cond.h
 * @brief Base class for all conditional statement nodes in the AST.
 * @details Inherits from Statement and provides a common interface for all
 * conditional statement types.
 */
struct Conditional : public Statement, public basic::Visitable<Conditional> {

  /**
   * @var condition
   * @brief The condition expression to be evaluated.
   */
  std::unique_ptr<Expression> condition;

  /**
   * @var then_branch
   * @brief The block statement to execute when condition is true.
   */
  std::unique_ptr<BlockStatement> then_branch;

  Conditional(const basic::Locus start, const basic::Locus end,
              std::unique_ptr<Expression> condition,
              std::unique_ptr<BlockStatement> then_branch)
      : Statement(start, end), condition(std::move(condition)),
        then_branch(std::move(then_branch)) {}

  ENABLE_VISITORS(Conditional)

  virtual ~Conditional() = default;
};

/**
 * @struct IfConditional cond.h
 * @brief Represents an if-elif-else conditional statement in the AST.
 * @details Inherits from Conditional and contains optional elif and else
 * branches.
 */
struct IfConditional : public Conditional,
                       public basic::Visitable<IfConditional> {

  /**
   * @var elif_branches
   * @brief The list of elif branches for the if statement.
   */
  std::vector<std::unique_ptr<IfConditional>> elif_branches;

  /**
   * @var else_branch
   * @brief The optional else branch for the if statement.
   */
  std::unique_ptr<BlockStatement> else_branch;

  IfConditional(const basic::Locus start, const basic::Locus end,
                std::unique_ptr<Expression> condition,
                std::unique_ptr<BlockStatement> then_branch,
                std::vector<std::unique_ptr<IfConditional>> elif_branches,
                std::unique_ptr<BlockStatement> else_branch)
      : Conditional(start, end, std::move(condition), std::move(then_branch)),
        elif_branches(std::move(elif_branches)),
        else_branch(std::move(else_branch)) {}

  ENABLE_VISITORS(IfConditional)
};

/**
 * @struct SwitchConditional cond.h
 * @brief Represents a switch statement in the AST.
 * @details Inherits from Conditional and contains switch expression and case
 * branches.
 */
struct SwitchConditional : public Conditional,
                           public basic::Visitable<SwitchConditional> {

  /**
   * @var switch_expression
   * @brief The expression to switch on.
   */
  std::unique_ptr<Expression> switch_expression;

  /**
   * @var case_branches
   * @brief The list of case branches for the switch statement.
   */
  std::vector<std::unique_ptr<Conditional>> case_branches;

  SwitchConditional(const basic::Locus start, const basic::Locus end,
                    std::unique_ptr<Expression> switch_expression,
                    std::vector<std::unique_ptr<Conditional>> case_branches)
      : Conditional(start, end, nullptr, nullptr),
        switch_expression(std::move(switch_expression)),
        case_branches(std::move(case_branches)) {}

  ENABLE_VISITORS(SwitchConditional)
};

/**
 * @struct WhileConditional cond.h
 * @brief Represents a while loop statement in the AST.
 * @details Inherits from Conditional and executes the body while condition is
 * true.
 */
struct WhileConditional : public Conditional,
                          public basic::Visitable<WhileConditional> {

  WhileConditional(const basic::Locus start, const basic::Locus end,
                   std::unique_ptr<Expression> condition,
                   std::unique_ptr<BlockStatement> then_branch)
      : Conditional(start, end, std::move(condition), std::move(then_branch)) {}

  ENABLE_VISITORS(WhileConditional)
};

/**
 * @struct ForConditional cond.h
 * @brief Represents a for loop statement in the AST.
 * @details Inherits from Conditional and contains optional initializer and
 * increment expressions.
 */
struct ForConditional : public Conditional,
                        public basic::Visitable<ForConditional> {

  /**
   * @var initializer
   * @brief The optional initializer declaration for the for loop.
   */
  std::unique_ptr<Declaration> initializer;

  /**
   * @var increment
   * @brief The optional increment expression for the for loop.
   */
  std::unique_ptr<Expression> increment;

  ForConditional(const basic::Locus start, const basic::Locus end,
                 std::unique_ptr<Declaration> initializer,
                 std::unique_ptr<Expression> condition,
                 std::unique_ptr<Expression> increment,
                 std::unique_ptr<BlockStatement> then_branch)
      : Conditional(start, end, std::move(condition), std::move(then_branch)),
        initializer(std::move(initializer)), increment(std::move(increment)) {}

  ENABLE_VISITORS(ForConditional)
};

} // namespace ml::ast