/**
 * @file node_printer.h
 * @brief NodePrinter definitions for My Language.
 * @details Defines the NodePrinter class and related functions for printing
 * Abstract Syntax Tree (AST) nodes.
 * @copyright Copyright (c) 2025 Karson P. Califf
 */

#include "ml/ast/node_printer.h"
#include "ml/basic/flags.h"

namespace ml::ast {

void NodePrinter::indent() const {
  for (uint64_t i = 0; i < this->current_indent; i++) {
    std::cout << "  ";
  }
}

void NodePrinter::unindent() {
  if (this->current_indent > 0) {
    this->current_indent--;
  }
}

void NodePrinter::location(const Node &v) const {
  std::cout << " [" << static_cast<std::string>(v.start) << " - "
            << static_cast<std::string>(v.end) << "] ";
}

void NodePrinter::visit(Node &v) { print_line("Node"); }

void NodePrinter::visit(Program &v) {
  print_line("Program");
  enter_node();
  for (auto &stmt : v.statements) {
    print_node(*stmt);
  }
  exit_node();
}

void NodePrinter::visit(Expression &v) { print_line("Expression"); }

void NodePrinter::visit(BinaryExpression &v) {
  print_line("BinaryExpression");
  enter_node();

  print_line("Left:");
  enter_node();
  print_node(*v.left);
  exit_node();

  print_line("Operator: " + v.op);

  print_line("Right:");
  enter_node();
  print_node(*v.right);
  exit_node();

  exit_node();
}

void NodePrinter::visit(UnaryExpression &v) {
  print_line("UnaryExpression");
  enter_node();

  print_line("Operator: " + v.op);

  print_line("Operand:");
  enter_node();
  print_node(*v.operand);
  exit_node();

  exit_node();
}

void NodePrinter::visit(LiteralExpression &v) {
  print_line("Literal: \"" + v.value + "\"");
}

void NodePrinter::visit(IdentifierExpression &v) {
  print_line("Identifier: " + v.name);
}

void NodePrinter::visit(ArrayIdentifierExpression &v) {
  print_line("ArrayIdentifierExpression");
  enter_node();

  print_line("Name: " + v.name);

  print_line("Size:");
  enter_node();
  print_node(*v.size);
  exit_node();

  exit_node();
}

void NodePrinter::visit(IndexExpression &v) {
  print_line("IndexExpression");
  enter_node();

  print_line("Array:");
  enter_node();
  print_node(*v.array);
  exit_node();

  print_line("Index:");
  enter_node();
  print_node(*v.index);
  exit_node();

  exit_node();
}

void NodePrinter::visit(ArrayExpression &v) {
  print_line("ArrayExpression");
  enter_node();

  if (v.elements.empty()) {
    print_line("(empty)");
  } else {
    for (auto &element : v.elements) {
      print_node(*element);
    }
  }

  exit_node();
}

void NodePrinter::visit(CallExpression &v) {
  print_line("CallExpression");
  enter_node();

  print_line("Callee:");
  enter_node();
  print_node(*v.callee);
  exit_node();

  print_line("Arguments:");
  enter_node();
  if (v.arguments.empty()) {
    print_line("(empty)");
  } else {
    for (auto &arg : v.arguments) {
      print_node(*arg);
    }
  }
  exit_node();

  exit_node();
}

void NodePrinter::visit(AttributeExpression &v) {
  print_line("AttributeExpression");
  enter_node();

  print_line("Object:");
  enter_node();
  print_node(*v.object);
  exit_node();

  print_line("Attribute:");
  enter_node();
  print_node(*v.attribute);
  exit_node();

  exit_node();
}

void NodePrinter::visit(Statement &v) { print_line("Statement"); }

void NodePrinter::visit(ReturnStatement &v) {
  print_line("ReturnStatement");
  enter_node();
  if (v.expression) {
    print_node(*v.expression);
  } else {
    print_line("(void)");
  }
  exit_node();
}

void NodePrinter::visit(BreakStatement &v) { print_line("BreakStatement"); }

void NodePrinter::visit(ContinueStatement &v) {
  print_line("ContinueStatement");
}

void NodePrinter::visit(ExpressionStatement &v) {
  print_line("ExpressionStatement");
  enter_node();
  print_node(*v.expression);
  exit_node();
}

void NodePrinter::visit(BlockStatement &v) {
  print_line("BlockStatement");
  enter_node();
  if (v.statements.empty()) {
    print_line("(empty)");
  } else {
    for (auto &stmt : v.statements) {
      print_node(*stmt);
    }
  }
  exit_node();
}

void NodePrinter::visit(ModifierStatement &v) {
  print_line("ModifierStatement");
  enter_node();

  print_line("Accessor: " + basic::accstr(v.accessor));

  print_line("Modifiers:");
  enter_node();

  bool has_any = false;
  if (hasFlag(v.modifier, basic::Modifier::Constant)) {
    print_line("Const");
    has_any = true;
  }
  if (hasFlag(v.modifier, basic::Modifier::Static)) {
    print_line("Static");
    has_any = true;
  }
  if (hasFlag(v.modifier, basic::Modifier::Init)) {
    print_line("InitOnly");
    has_any = true;
  }
  if (hasFlag(v.modifier, basic::Modifier::Nullable)) {
    print_line("Nullable");
    has_any = true;
  }
  if (hasFlag(v.modifier, basic::Modifier::None) || !has_any) {
    print_line("None");
  }

  exit_node();
  exit_node();
}

void NodePrinter::visit(Declaration &v) { print_line("Declaration"); }

void NodePrinter::visit(VariableDeclaration &v) {
  print_line("VariableDeclaration");
  enter_node();

  print_node(*v.identifier);

  print_line("Type:");
  enter_node();
  print_node(*v.type);
  exit_node();

  print_line("Modifiers:");
  enter_node();
  print_node(*v.modifier);
  exit_node();

  if (v.initializer) {
    print_line("Initializer:");
    enter_node();
    print_node(*v.initializer);
    exit_node();
  }

  exit_node();
}

void NodePrinter::visit(FunctionDeclaration &v) {
  print_line("FunctionDeclaration");
  enter_node();

  print_node(*v.identifier);

  print_line("Type:");
  enter_node();
  print_node(*v.type);
  exit_node();

  print_line("Modifiers:");
  enter_node();
  print_node(*v.modifier);
  exit_node();

  print_line("Parameters:");
  enter_node();
  if (v.parameters.empty()) {
    print_line("(empty)");
  } else {
    for (auto &param : v.parameters) {
      print_node(*param);
    }
  }
  exit_node();

  if (v.body) {
    print_line("Body:");
    enter_node();
    print_node(*v.body);
    exit_node();
  }

  exit_node();
}

void NodePrinter::visit(RecordDeclaration &v) {
  print_line("RecordDeclaration");
  enter_node();

  print_node(*v.identifier);

  print_line("Modifiers:");
  enter_node();
  print_node(*v.modifier);
  exit_node();

  print_line("Fields:");
  enter_node();
  if (v.fields.empty()) {
    print_line("(empty)");
  } else {
    for (auto &field : v.fields) {
      print_node(*field);
    }
  }
  exit_node();

  exit_node();
}

void NodePrinter::visit(ClassDeclaration &v) {
  print_line("ClassDeclaration");
  enter_node();

  print_node(*v.identifier);

  print_line("Type:");
  enter_node();
  print_node(*v.type);
  exit_node();

  print_line("Modifiers:");
  enter_node();
  print_node(*v.modifier);
  exit_node();

  print_line("Fields:");
  enter_node();
  if (v.fields.empty()) {
    print_line("(empty)");
  } else {
    for (auto &field : v.fields) {
      print_node(*field);
    }
  }
  exit_node();

  print_line("Methods:");
  enter_node();
  if (v.methods.empty()) {
    print_line("(empty)");
  } else {
    for (auto &method : v.methods) {
      print_node(*method);
    }
  }
  exit_node();

  exit_node();
}

void NodePrinter::visit(Conditional &v) {
  print_line("Conditional");
  enter_node();

  if (v.condition) {
    print_line("Condition:");
    enter_node();
    print_node(*v.condition);
    exit_node();
  }

  if (v.then_branch) {
    print_line("ThenBranch:");
    enter_node();
    print_node(*v.then_branch);
    exit_node();
  }

  exit_node();
}

void NodePrinter::visit(IfConditional &v) {
  print_line("IfConditional");
  enter_node();

  print_line("Condition:");
  enter_node();
  print_node(*v.condition);
  exit_node();

  print_line("ThenBranch:");
  enter_node();
  print_node(*v.then_branch);
  exit_node();

  for (auto &elif_branch : v.elif_branches) {
    print_line("ElifBranch:");
    enter_node();
    print_node(*elif_branch);
    exit_node();
  }

  if (v.else_branch) {
    print_line("ElseBranch:");
    enter_node();
    print_node(*v.else_branch);
    exit_node();
  }

  exit_node();
}

void NodePrinter::visit(SwitchConditional &v) {
  print_line("SwitchConditional");
  enter_node();

  print_line("SwitchExpression:");
  enter_node();
  print_node(*v.switch_expression);
  exit_node();

  print_line("CaseBranches:");
  enter_node();
  if (v.case_branches.empty()) {
    print_line("(empty)");
  } else {
    for (auto &case_branch : v.case_branches) {
      print_node(*case_branch);
    }
  }
  exit_node();

  exit_node();
}

void NodePrinter::visit(WhileConditional &v) {
  print_line("WhileConditional");
  enter_node();

  print_line("Condition:");
  enter_node();
  print_node(*v.condition);
  exit_node();

  print_line("ThenBranch:");
  enter_node();
  print_node(*v.then_branch);
  exit_node();

  exit_node();
}

void NodePrinter::visit(ForConditional &v) {
  print_line("ForConditional");
  enter_node();

  if (v.initializer) {
    print_line("Initializer:");
    enter_node();
    print_node(*v.initializer);
    exit_node();
  }

  if (v.condition) {
    print_line("Condition:");
    enter_node();
    print_node(*v.condition);
    exit_node();
  }

  if (v.increment) {
    print_line("Increment:");
    enter_node();
    print_node(*v.increment);
    exit_node();
  }

  print_line("ThenBranch:");
  enter_node();
  print_node(*v.then_branch);
  exit_node();

  exit_node();
}

} // namespace ml::ast