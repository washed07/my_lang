/**
 * @file node_printer.h
 * @brief Abstract Syntax Tree (AST) node printer visitor.
 * @details Defines a visitor that prints the structure of AST nodes.
 * @copyright Copyright (c) 2025 Karson P. Califf
 */

#pragma once

#include "cond.h"
#include "decl.h"
#include "expr.h"
#include "ml/basic/visitor.h"
#include "node.h"
#include "stmt.h"

namespace ml::ast {

struct NodePrinter : public basic::Visitor,
                     public basic::Visiting<Node>,
                     public basic::Visiting<Program>,
                     public basic::Visiting<Expression>,
                     public basic::Visiting<BinaryExpression>,
                     public basic::Visiting<UnaryExpression>,
                     public basic::Visiting<LiteralExpression>,
                     public basic::Visiting<IdentifierExpression>,
                     public basic::Visiting<ArrayIdentifierExpression>,
                     public basic::Visiting<IndexExpression>,
                     public basic::Visiting<ArrayExpression>,
                     public basic::Visiting<CallExpression>,
                     public basic::Visiting<AttributeExpression>,
                     public basic::Visiting<Statement>,
                     public basic::Visiting<ReturnStatement>,
                     public basic::Visiting<BreakStatement>,
                     public basic::Visiting<ContinueStatement>,
                     public basic::Visiting<ExpressionStatement>,
                     public basic::Visiting<BlockStatement>,
                     public basic::Visiting<ModifierStatement>,
                     public basic::Visiting<Declaration>,
                     public basic::Visiting<VariableDeclaration>,
                     public basic::Visiting<FunctionDeclaration>,
                     public basic::Visiting<RecordDeclaration>,
                     public basic::Visiting<ClassDeclaration>,
                     public basic::Visiting<Conditional>,
                     public basic::Visiting<IfConditional>,
                     public basic::Visiting<SwitchConditional>,
                     public basic::Visiting<WhileConditional>,
                     public basic::Visiting<ForConditional> {
public:
  uint64_t current_indent = 0;

  void indent() const;
  void unindent();

  void location(const Node &v) const;

  template <typename T> void print_node(T &v, bool is_last = false) {
    static_cast<basic::Visitable<T> &>(v).accept(*this);
  }

  void print_str(std::string string) { std::cout << string << std::endl; }

  void print_line(std::string content) {
    indent();
    std::cout << content << std::endl;
  }

  void enter_node() { current_indent++; }

  void exit_node() {
    if (current_indent > 0) {
      current_indent--;
    }
  }

  void visit(Node &v) override;

  void visit(Program &v) override;

  void visit(Expression &v) override;
  void visit(BinaryExpression &v) override;
  void visit(UnaryExpression &v) override;
  void visit(LiteralExpression &v) override;
  void visit(IdentifierExpression &v) override;
  void visit(ArrayIdentifierExpression &v) override;
  void visit(IndexExpression &v) override;
  void visit(ArrayExpression &v) override;
  void visit(CallExpression &v) override;
  void visit(AttributeExpression &v) override;

  void visit(Statement &v) override;
  void visit(ReturnStatement &v) override;
  void visit(BreakStatement &v) override;
  void visit(ContinueStatement &v) override;
  void visit(ExpressionStatement &v) override;
  void visit(BlockStatement &v) override;
  void visit(ModifierStatement &v) override;

  void visit(Declaration &v) override;
  void visit(VariableDeclaration &v) override;
  void visit(FunctionDeclaration &v) override;
  void visit(RecordDeclaration &v) override;
  void visit(ClassDeclaration &v) override;

  void visit(Conditional &v) override;
  void visit(IfConditional &v) override;
  void visit(SwitchConditional &v) override;
  void visit(WhileConditional &v) override;
  void visit(ForConditional &v) override;
};

} // namespace ml::ast