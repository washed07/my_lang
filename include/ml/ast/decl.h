/**
 * @file decl.h
 * @brief Abstract Syntax Tree (AST) node definitions for declaration
 * statements.
 * @details Defines AST node structures for various declaration statements.
 * @author Karson Califf
 * @date 2025-10-30
 * @copyright Copyright (c) 2025 Karson Califf
 */

#pragma once

#include "expr.h"
#include "node.h"
#include "stmt.h"
#include <memory>
#include <string>

namespace ml::ast {

/**
 * @struct Declaration decl.h
 * @brief Base class for all declaration nodes in the AST.
 * @details Inherits from Statement and provides a common interface for all
 * declaration types.
 */
struct Declaration : public Statement, public basic::Visitable<Declaration> {

  /**
   * @var identifier
   * @brief The identifier name of the declaration.
   */
  std::unique_ptr<IdentifierExpression> identifier;

  /**
   * @var type
   * @brief The type expression of the declaration.
   */
  std::unique_ptr<Expression> type;

  /**
   * @var modifier
   * @brief The modifier statement containing access and modifier information.
   */
  std::unique_ptr<ModifierStatement> modifier;

  Declaration(const basic::Locus start, const basic::Locus end,
              std::unique_ptr<IdentifierExpression> identifier,
              std::unique_ptr<Expression> type,
              std::unique_ptr<ModifierStatement> modifier)
      : Statement(start, end), identifier(std::move(identifier)),
        type(std::move(type)), modifier(std::move(modifier)) {}

  ENABLE_VISITORS(Declaration)
};

/**
 * @struct VariableDeclaration decl.h
 * @brief Represents a variable declaration in the AST.
 * @details Inherits from Declaration and contains an optional initializer
 * expression.
 */
struct VariableDeclaration : public Declaration,
                             public basic::Visitable<VariableDeclaration> {

  /**
   * @var initializer
   * @brief The optional initializer expression for the variable.
   */
  std::unique_ptr<Expression> initializer;

  VariableDeclaration(const basic::Locus start, const basic::Locus end,
                      std::unique_ptr<IdentifierExpression> identifier,
                      std::unique_ptr<Expression> type,
                      std::unique_ptr<ModifierStatement> modifier,
                      std::unique_ptr<Expression> initializer)
      : Declaration(start, end, std::move(identifier), std::move(type),
                    std::move(modifier)),
        initializer(std::move(initializer)) {}

  ENABLE_VISITORS(VariableDeclaration)
};

/**
 * @struct FunctionDeclaration decl.h
 * @brief Represents a function declaration in the AST.
 * @details Inherits from Declaration and contains parameters and function body.
 */
struct FunctionDeclaration : public Declaration,
                             public basic::Visitable<FunctionDeclaration> {

  /**
   * @var parameters
   * @brief The list of parameter declarations for the function.
   */
  std::vector<std::unique_ptr<VariableDeclaration>> parameters;

  /**
   * @var body
   * @brief The block statement containing the function body.
   */
  std::unique_ptr<BlockStatement> body;

  FunctionDeclaration(
      const basic::Locus start, const basic::Locus end,
      std::unique_ptr<IdentifierExpression> identifier,
      std::unique_ptr<Expression> type,
      std::unique_ptr<ModifierStatement> modifier,
      std::vector<std::unique_ptr<VariableDeclaration>> parameters,
      std::unique_ptr<BlockStatement> body)
      : Declaration(start, end, std::move(identifier), std::move(type),
                    std::move(modifier)),
        parameters(std::move(parameters)), body(std::move(body)) {}

  ENABLE_VISITORS(FunctionDeclaration)
};

/**
 * @struct ClassDeclaration decl.h
 * @brief Represents a class declaration in the AST.
 * @details Inherits from Declaration and contains fields and methods.
 */
struct ClassDeclaration : public Declaration,
                          public basic::Visitable<ClassDeclaration> {

  /**
   * @var fields
   * @brief The list of field variable declarations for the class.
   */
  std::vector<std::unique_ptr<VariableDeclaration>> fields;

  /**
   * @var methods
   * @brief The list of method function declarations for the class.
   */
  std::vector<std::unique_ptr<FunctionDeclaration>> methods;

  ClassDeclaration(const basic::Locus start, const basic::Locus end,
                   std::unique_ptr<IdentifierExpression> identifier,
                   std::unique_ptr<Expression> type,
                   std::unique_ptr<ModifierStatement> modifier,
                   std::vector<std::unique_ptr<VariableDeclaration>> fields,
                   std::vector<std::unique_ptr<FunctionDeclaration>> methods)
      : Declaration(start, end, std::move(identifier), std::move(type),
                    std::move(modifier)),
        fields(std::move(fields)), methods(std::move(methods)) {}

  ENABLE_VISITORS(ClassDeclaration)
};

/**
 * @struct RecordDeclaration decl.h
 * @brief Represents a record declaration in the AST.
 * @details Inherits from Declaration and contains field declarations.
 */
struct RecordDeclaration : public Declaration,
                           public basic::Visitable<RecordDeclaration> {

  /**
   * @var fields
   * @brief The list of field variable declarations for the record.
   */
  std::vector<std::unique_ptr<VariableDeclaration>> fields;

  RecordDeclaration(const basic::Locus start, const basic::Locus end,
                    std::unique_ptr<IdentifierExpression> identifier,
                    std::unique_ptr<Expression> type,
                    std::unique_ptr<ModifierStatement> modifier,
                    std::vector<std::unique_ptr<VariableDeclaration>> fields)
      : Declaration(start, end, std::move(identifier), std::move(type),
                    std::move(modifier)),
        fields(std::move(fields)) {}

  ENABLE_VISITORS(RecordDeclaration)
};

} // namespace ml::ast