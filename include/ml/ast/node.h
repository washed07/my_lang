/**
 * @file node.h
 * @brief Abstract Syntax Tree (AST) node definition.
 * @details Defines the base AST node structure and related utilities.
 * @copyright Copyright (c) 2025 Karson P. Califf
 */

#pragma once

#include "ml/basic/locus.h"
#include "ml/basic/visitor.h"
#include <iostream>
#include <string>

namespace ml::ast {

enum class NodeKind;
struct Node;

enum class NodeKind {
  None,
  Expression,
  Statement,
  Declaration,
  Conditional,
};

/**
 * @struct Node node.h
 * @brief Base class for all AST nodes.
 * @details Contains common properties such as source location and node kind.
 */
struct Node : public basic::Visitable<Node> {
  /**
   * @brief The starting and ending locations of the node in the source code.
   */
  const basic::Locus start, end;
  const NodeKind kind = NodeKind::None;

  explicit Node(const basic::Locus start, const basic::Locus end,
                const NodeKind kind)
      : start(start), end(end), kind(kind) {}

  explicit Node(const basic::Locus start, const basic::Locus end)
      : start(start), end(end) {}

  ENABLE_VISITORS(Node)

  virtual ~Node() = default;
};

/**
 * @brief Utility function to check if a node is an expression.
 * @param node The AST node to check.
 * @return True if the node is an expression, false otherwise.
 */
inline bool isexpr(const Node node) {
  return node.kind == NodeKind::Expression;
}

/**
 * @brief Utility function to check if a node is a statement.
 * @param node The AST node to check.
 * @return True if the node is a statement, false otherwise.
 */
inline bool isstmt(const Node node) {
  return node.kind == NodeKind::Statement ||
         node.kind == NodeKind::Declaration ||
         node.kind == NodeKind::Conditional;
}

/**
 * @brief Utility function to check if a node is a declaration.
 * @param node The AST node to check.
 * @return True if the node is a declaration, false otherwise.
 */
inline bool isdecl(const Node node) {
  return node.kind == NodeKind::Declaration;
}

/**
 * @brief Utility function to check if a node is a conditional.
 * @param node The AST node to check.
 * @return True if the node is a conditional, false otherwise.
 */
inline bool iscond(const Node node) {
  return node.kind == NodeKind::Conditional;
}

} // namespace ml::ast