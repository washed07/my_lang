/**
 * @file expr.h
 * @brief Abstract Syntax Tree (AST) node definitions for expression statements.
 * @details Includes all necessary AST node headers for expression statements.
 * @copyright Copyright (c) 2025 Karson P. Califf
 */

#pragma once

#include "ml/basic/accessor.h"
#include "ml/basic/modifier.h"
#include "node.h"
#include <memory>
#include <vector>

namespace ml::ast {

/**
 * @struct Expression expr.h
 * @brief Base class for all expression nodes in the AST.
 * @details Inherits from Node and provides a common interface for all
 * expression types.
 */
struct Expression : public Node, public basic::Visitable<Expression> {
  Expression(const basic::Locus start, const basic::Locus end)
      : Node(start, end, NodeKind::Expression) {}

  ENABLE_VISITORS(Expression)

  virtual ~Expression() = default;
};

/**
 * @struct BinaryExpression expr.h
 * @brief Represents a binary expression in the AST.
 * @details Inherits from Expression and contains left and right operands
 * and an operator.
 */
struct BinaryExpression : public Expression,
                          public basic::Visitable<BinaryExpression> {
  /**
   * @var left
   * @brief The left operand of the binary expression.
   */
  std::unique_ptr<Expression> left;

  /**
   * @var op
   * @brief The operator of the binary expression.
   */
  std::string op;

  /**
   * @var right
   * @brief The right operand of the binary expression.
   */
  std::unique_ptr<Expression> right;

  BinaryExpression(const basic::Locus start, const basic::Locus end,
                   std::unique_ptr<Expression> left, std::string op,
                   std::unique_ptr<Expression> right)
      : Expression(start, end), left(std::move(left)), op(op),
        right(std::move(right)) {}

  ENABLE_VISITORS(BinaryExpression)
};

/**
 * @struct UnaryExpression expr.h
 * @brief Represents a unary expression in the AST.
 * @details Inherits from Expression and contains an operand and an operator.
 */
struct UnaryExpression : public Expression,
                         public basic::Visitable<UnaryExpression> {

  /**
   * @var op
   * @brief The operator of the unary expression.
   */
  std::string op;

  /**
   * @var operand
   * @brief The operand of the unary expression.
   */
  std::unique_ptr<Expression> operand;

  UnaryExpression(const basic::Locus start, const basic::Locus end,
                  std::string op, std::unique_ptr<Expression> operand)
      : Expression(start, end), op(op), operand(std::move(operand)) {}

  ENABLE_VISITORS(UnaryExpression)
};

enum class LiteralType { Integer, Float, String, Character, Boolean, Null };

/**
 * @struct LiteralExpression expr.h
 * @brief Represents a literal expression in the AST.
 * @details Inherits from Expression and contains a literal value.
 */
struct LiteralExpression : public Expression,
                           public basic::Visitable<LiteralExpression> {

  /**
   * @var value
   * @brief The literal value of the expression.
   */
  std::string value;
  LiteralType type;

  LiteralExpression(const basic::Locus start, const basic::Locus end,
                    std::string value, LiteralType type)
      : Expression(start, end), value(value), type(type) {}
  ENABLE_VISITORS(LiteralExpression)
};

/**
 * @struct IdentifierExpression expr.h
 * @brief Represents an identifier expression in the AST.
 * @details Inherits from Expression and contains the identifier name.
 */
struct IdentifierExpression : public Expression,
                              public basic::Visitable<IdentifierExpression> {

  /**
   * @var name
   * @brief The name of the identifier.
   */
  std::string name;

  IdentifierExpression(const basic::Locus start, const basic::Locus end,
                       std::string name)
      : Expression(start, end), name(name) {}

  ENABLE_VISITORS(IdentifierExpression)
};

/**
 * @struct ArrayIdentifierExpression expr.h
 * @brief Represents an array identifier expression in the AST.
 * @details Inherits from IdentifierExpression and contains array size
 * information.
 */
struct ArrayIdentifierExpression
    : public IdentifierExpression,
      public basic::Visitable<ArrayIdentifierExpression> {

  /**
   * @var size
   * @brief The size expression for the array.
   */
  std::unique_ptr<Expression> size;

  ArrayIdentifierExpression(const basic::Locus start, const basic::Locus end,
                            std::string name, std::unique_ptr<Expression> size)
      : IdentifierExpression(start, end, name), size(std::move(size)) {}

  ENABLE_VISITORS(ArrayIdentifierExpression)
};

/**
 * @struct IndexExpression expr.h
 * @brief Represents an array indexing expression in the AST.
 * @details Inherits from Expression and contains array and index expressions.
 */
struct IndexExpression : public Expression,
                         public basic::Visitable<IndexExpression> {

  /**
   * @var array
   * @brief The array expression to be indexed.
   */
  std::unique_ptr<Expression> array;

  /**
   * @var index
   * @brief The index expression for array access.
   */
  std::unique_ptr<Expression> index;

  IndexExpression(const basic::Locus start, const basic::Locus end,
                  std::unique_ptr<Expression> array,
                  std::unique_ptr<Expression> index)
      : Expression(start, end), array(std::move(array)),
        index(std::move(index)) {}

  ENABLE_VISITORS(IndexExpression)
};

/**
 * @struct CallExpression expr.h
 * @brief Represents a function call expression in the AST.
 * @details Inherits from Expression and contains callee and argument
 * expressions.
 */
struct CallExpression : public Expression,
                        public basic::Visitable<CallExpression> {

  /**
   * @var callee
   * @brief The expression representing the function to be called.
   */
  std::unique_ptr<Expression> callee;

  /**
   * @var arguments
   * @brief The list of argument expressions for the function call.
   */
  std::vector<std::unique_ptr<Expression>> arguments;

  CallExpression(const basic::Locus start, const basic::Locus end,
                 std::unique_ptr<Expression> callee,
                 std::vector<std::unique_ptr<Expression>> arguments)
      : Expression(start, end), callee(std::move(callee)),
        arguments(std::move(arguments)) {}

  ENABLE_VISITORS(CallExpression)
};

/**
 * @struct AttributeExpression expr.h
 * @brief Represents a member access expression in the AST.
 * @details Inherits from Expression and contains object and attribute
 * expressions.
 */
struct AttributeExpression : public Expression,
                             public basic::Visitable<AttributeExpression> {

  /**
   * @var object
   * @brief The object expression to access the attribute from.
   */
  std::unique_ptr<Expression> object;

  /**
   * @var attribute
   * @brief The attribute expression to access.
   */
  std::unique_ptr<Expression> attribute;

  AttributeExpression(const basic::Locus start, const basic::Locus end,
                      std::unique_ptr<Expression> object,
                      std::unique_ptr<Expression> attribute)
      : Expression(start, end), object(std::move(object)),
        attribute(std::move(attribute)) {}

  ENABLE_VISITORS(AttributeExpression)
};

/**
 * @struct ArrayExpression expr.h
 * @brief Represents an array literal expression in the AST.
 * @details Inherits from Expression and contains a list of element expressions.
 */
struct ArrayExpression : public Expression,
                         public basic::Visitable<ArrayExpression> {

  /**
   * @var elements
   * @brief The list of element expressions in the array literal.
   */
  std::vector<std::unique_ptr<Expression>> elements;

  ArrayExpression(const basic::Locus start, const basic::Locus end,
                  std::vector<std::unique_ptr<Expression>> elements)
      : Expression(start, end), elements(std::move(elements)) {}

  ENABLE_VISITORS(ArrayExpression)
};

} // namespace ml::ast