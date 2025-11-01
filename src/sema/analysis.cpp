#include "ml/sema/analysis.h"

namespace ml::sema {
Analyzer::Analyzer() : current_scope(nullptr) {}

void Analyzer::enterScope(const std::string &name, ScopeKind kind) {
  if (this->current_scope) {
    // Combine with parent scope kind using bitwise OR
    this->current_scope = std::make_shared<Scope>(
        name, this->current_scope->kind | kind, this->current_scope);
  } else {
    // First scope (global), no parent
    this->current_scope = std::make_shared<Scope>(name, kind, nullptr);
  }
}

void Analyzer::exitScope() {
  if (this->current_scope) {
    this->current_scope = this->current_scope->getParent();
  }
}

Type Analyzer::inferExpression(const ast::Expression &expr) {
  // Dispatch based on the actual type of expression
  if (auto *binary_expr = dynamic_cast<const ast::BinaryExpression *>(&expr)) {
    return this->inferBinary(*binary_expr);
  } else if (auto *unary_expr =
                 dynamic_cast<const ast::UnaryExpression *>(&expr)) {
    return this->inferUnary(*unary_expr);
  } else if (auto *literal_expr =
                 dynamic_cast<const ast::LiteralExpression *>(&expr)) {
    return this->inferLiteral(*literal_expr);
  } else if (auto *identifier_expr =
                 dynamic_cast<const ast::IdentifierExpression *>(&expr)) {
    return this->inferIdentifier(*identifier_expr);
  } else if (auto *array_id_expr =
                 dynamic_cast<const ast::ArrayIdentifierExpression *>(&expr)) {
    return this->inferArrayIdentifier(*array_id_expr);
  } else if (auto *index_expr =
                 dynamic_cast<const ast::IndexExpression *>(&expr)) {
    return this->inferIndex(*index_expr);
  } else if (auto *array_expr =
                 dynamic_cast<const ast::ArrayExpression *>(&expr)) {
    return this->inferArray(*array_expr);
  } else if (auto *call_expr =
                 dynamic_cast<const ast::CallExpression *>(&expr)) {
    return this->inferCall(*call_expr);
  } else if (auto *attr_expr =
                 dynamic_cast<const ast::AttributeExpression *>(&expr)) {
    return this->inferAttribute(*attr_expr);
  } else {
    this->errors.push_back(basic::Error(
        basic::ErrorLevel::Error, "Unknown expression type.",
        "Ensure the expression is valid.", expr.start, expr.end, "", ""));
    return none_ty;
  }
}

Type Analyzer::inferBinary(const ast::BinaryExpression &expr) {
  Type left_type = this->inferExpression(*expr.left);
  Type right_type = this->inferExpression(*expr.right);
  return promoteTypes(left_type, right_type);
}

Type Analyzer::inferUnary(const ast::UnaryExpression &expr) {
  return this->inferExpression(*expr.operand);
}

Type Analyzer::inferLiteral(const ast::LiteralExpression &expr) {
  switch (expr.type) {
  case ast::LiteralType::Integer:
    return i64_ty;
  case ast::LiteralType::Float:
    return f64_ty;
  case ast::LiteralType::String:
    return string_ty;
  case ast::LiteralType::Character:
    return char_ty;
  case ast::LiteralType::Boolean:
    return bool_ty;
  case ast::LiteralType::Null:
    return null_ty;
  default:
    return none_ty;
  }
}

Type Analyzer::inferIdentifier(const ast::IdentifierExpression &expr) {
  if (this->current_scope->hasVariable(expr.name)) {
    Variable var = this->current_scope->getVariable(expr.name);
    return var.type;
  } else if (this->current_scope->hasFunction(expr.name)) {
    Function func = this->current_scope->getFunction(expr.name);
    return func;
  } else if (this->current_scope->hasClass(expr.name)) {
    Class cls = this->current_scope->getClass(expr.name);
    return cls;
  } else if (this->current_scope->hasRecord(expr.name)) {
    Record rec = this->current_scope->getRecord(expr.name);
    return rec;
  } else if (this->current_scope->hasType(expr.name)) {
    Type type = this->current_scope->getType(expr.name);
    return type;
  }

  // Variable/identifier not found - report error
  this->errors.push_back(basic::Error(
      basic::ErrorLevel::Error, "Undeclared identifier: " + expr.name,
      "Ensure the identifier is declared before use.", expr.start, expr.end, "",
      ""));
  return none_ty;
}

Type Analyzer::inferArrayIdentifier(
    const ast::ArrayIdentifierExpression &expr) {
  if (this->current_scope && this->current_scope->hasVariable(expr.name)) {
    Variable var = this->current_scope->getVariable(expr.name);
    return var.type;
  }
  return none_ty;
}

Type Analyzer::inferIndex(const ast::IndexExpression &expr) {
  Type array_type = this->inferExpression(*expr.array);
  Type index_type = this->inferExpression(*expr.index);
  if (array_type.kind == TypeKind::Array && index_type.isInteger()) {
    return array_type; // Simplified for demonstration
  }
  return none_ty;
}

Type Analyzer::inferArray(const ast::ArrayExpression &expr) {
  if (expr.elements.empty()) {
    return none_ty;
  }
  Type element_type = this->inferExpression(*expr.elements[0]);
  return Type(TypeKind::Array, "array" + element_type.name);
}

Type Analyzer::inferCall(const ast::CallExpression &expr) {

  Type callee_type = this->inferExpression(*expr.callee);
  if (callee_type.kind == TypeKind::Function) {
    std::vector<Type> param_types = {};
    for (const auto &arg : expr.arguments) {
      param_types.push_back(this->inferExpression(*arg));
    }
    Function func = static_cast<Function>(callee_type);
    if (func.isValidArguments(param_types)) {
      return func.return_type;
    }
    this->errors.push_back(basic::Error(
        basic::ErrorLevel::Error, "Function called with invalid arguments.",
        "Ensure the arguments match the function parameters.", expr.start,
        expr.end, "", ""));
  } else if (callee_type.kind == TypeKind::Class) {
    // Handle constructor calls
    Class cls = this->current_scope->getClass(callee_type.name);
    std::vector<Type> param_types = {};
    for (const auto &arg : expr.arguments) {
      param_types.push_back(this->inferExpression(*arg));
    }

    // First check if constructor exists at all
    if (!cls.hasMethod("init", basic::Accessor::Public)) {
      this->errors.push_back(
          basic::Error(basic::ErrorLevel::Error,
                       "Class has no accessible constructor: " + cls.name,
                       "Ensure the class has a public constructor defined.",
                       expr.start, expr.end, "", ""));
      return none_ty;
    }

    auto constructor = cls.getMethod("init", basic::Accessor::Public);
    if (!constructor.isValid()) {
      this->errors.push_back(basic::Error(
          basic::ErrorLevel::Error,
          "Class constructor is not accessible: " + cls.name,
          "Ensure the constructor is public.", expr.start, expr.end, "", ""));
      return none_ty;
    }
    if (constructor.isValidArguments(param_types)) {
      return cls;
    }
    this->errors.push_back(
        basic::Error(basic::ErrorLevel::Error,
                     "Invalid constructor arguments for class: " + cls.name,
                     "Ensure the arguments match the constructor parameters.",
                     expr.start, expr.end, "", ""));
    return none_ty;
  }
  this->errors.push_back(
      basic::Error(basic::ErrorLevel::Error, "Called function does not exist.",
                   "Ensure the function is declared before use.", expr.start,
                   expr.end, "", ""));
  return none_ty;
}

Type Analyzer::inferAttribute(const ast::AttributeExpression &expr) {
  Type object_type = this->inferExpression(*expr.object);
  if (object_type.kind == TypeKind::Class) {
    // Get the actual class from scope instead of casting
    Class cls = this->current_scope->getClass(object_type.name);
    if (auto attr_expr =
            dynamic_cast<ast::IdentifierExpression *>(expr.attribute.get())) {
      if (cls.hasField(attr_expr->name, basic::Accessor::Public)) {
        Variable field = cls.getField(attr_expr->name, basic::Accessor::Public);
        return field.type;
      }
      this->errors.push_back(basic::Error(
          basic::ErrorLevel::Error, "Unknown attribute: " + attr_expr->name,
          "Ensure the attribute is valid.", expr.start, expr.end, "", ""));
    } else if (auto call_expr =
                   dynamic_cast<ast::CallExpression *>(expr.attribute.get())) {
      if (auto method_expr = dynamic_cast<ast::IdentifierExpression *>(
              call_expr->callee.get())) {
        if (cls.hasMethod(method_expr->name, basic::Accessor::Public)) {
          Function method =
              cls.getMethod(method_expr->name, basic::Accessor::Public);
          std::vector<Type> arg_types = {};
          for (const auto &arg : call_expr->arguments) {
            Type arg_type = this->inferExpression(*arg);
            arg_types.push_back(arg_type);
          }
          if (method.isValidArguments(arg_types)) {
            return method.return_type;
          }
          this->errors.push_back(basic::Error(
              basic::ErrorLevel::Error, "Method called with invalid arguments.",
              "Ensure the arguments match the method parameters.", expr.start,
              expr.end, "", ""));
          return none_ty; // Return here instead of falling through
        } else {
          this->errors.push_back(basic::Error(
              basic::ErrorLevel::Error, "Unknown method: " + method_expr->name,
              "Ensure the method exists and is accessible.", expr.start,
              expr.end, "", ""));
          return none_ty; // Return here instead of falling through
        }
      }
      this->errors.push_back(basic::Error(
          basic::ErrorLevel::Error, "Unknown method attribute expression.",
          "Ensure the attribute expression is valid.", expr.start, expr.end, "",
          ""));
      return none_ty; // Return here instead of falling through
    } else {
      this->errors.push_back(basic::Error(
          basic::ErrorLevel::Error, "Unknown attribute type",
          "Ensure the attribute is valid.", expr.start, expr.end, "", ""));
      return none_ty;
    }
  } else if (object_type.kind == TypeKind::Record) {
    // Get the actual record from scope instead of casting
    Record rec = this->current_scope->getRecord(object_type.name);
    if (auto attr_expr =
            dynamic_cast<ast::IdentifierExpression *>(expr.attribute.get())) {
      if (rec.hasField(attr_expr->name, basic::Accessor::Public)) {
        Variable field = rec.getField(attr_expr->name, basic::Accessor::Public);
        return field.type;
      }
      this->errors.push_back(basic::Error(
          basic::ErrorLevel::Error, "Unknown attribute: " + attr_expr->name,
          "Ensure the attribute is valid.", expr.start, expr.end, "", ""));
    }
    this->errors.push_back(basic::Error(
        basic::ErrorLevel::Error, "Record has no accessible attribute.",
        "Ensure the attribute exists and is accessible.", expr.start, expr.end,
        "", ""));
  }
  return none_ty;
}

Variable Analyzer::declareVariable(const ast::VariableDeclaration &decl) {
  Type var_type = this->inferExpression(*decl.type);
  if (!var_type.isValid()) {
    this->errors.push_back(basic::Error(
        basic::ErrorLevel::Error,
        "Invalid type for variable declaration: " + decl.identifier->name,
        "Ensure the type is defined before use.", decl.start, decl.end, "",
        ""));
  }
  this->current_scope->addVariable(Variable(decl.identifier->name, var_type,
                                            decl.modifier.get()->accessor,
                                            decl.modifier.get()->modifier));
  return this->current_scope->getVariable(decl.identifier->name);
}

Function Analyzer::declareFunction(const ast::FunctionDeclaration &decl) {
  Type return_type = this->inferExpression(*decl.type);
  if (!return_type.isValid()) {
    this->errors.push_back(
        basic::Error(basic::ErrorLevel::Error,
                     "Invalid return type for function declaration: " +
                         decl.identifier->name,
                     "Ensure the return type is defined before use.",
                     decl.start, decl.end, "", ""));
  }
  std::vector<Variable> parameters = {};
  for (const auto &param_decl : decl.parameters) {
    Type param_type = this->inferExpression(*param_decl->type);
    if (!param_type.isValid()) {
      this->errors.push_back(
          basic::Error(basic::ErrorLevel::Error,
                       "Invalid type for function parameter: " +
                           param_decl->identifier->name,
                       "Ensure the parameter type is defined before use.",
                       param_decl->start, param_decl->end, "", ""));
    }
    parameters.push_back(Variable(param_decl->identifier->name, param_type,
                                  param_decl->modifier->accessor,
                                  param_decl->modifier->modifier));
  }
  this->current_scope->addFunction(Function(decl.identifier->name, return_type,
                                            parameters, decl.modifier->accessor,
                                            decl.modifier->modifier));
  return this->current_scope->getFunction(decl.identifier->name);
}

Class Analyzer::declareClass(const ast::ClassDeclaration &decl) {
  std::vector<Variable> fields = {};
  for (const auto &field_decl : decl.fields) {
    Type field_type = this->inferExpression(*field_decl->type);
    if (!field_type.isValid()) {
      this->errors.push_back(basic::Error(
          basic::ErrorLevel::Error,
          "Invalid type for class field: " + field_decl->identifier->name,
          "Ensure the field type is defined before use.", field_decl->start,
          field_decl->end, "", ""));
    }
    fields.push_back(Variable(field_decl->identifier->name, field_type,
                              field_decl->modifier->accessor,
                              field_decl->modifier->modifier));
  }
  std::vector<Function> methods = {};
  for (const auto &method_decl : decl.methods) {
    Type return_type = this->inferExpression(*method_decl->type);
    if (!return_type.isValid()) {
      this->errors.push_back(
          basic::Error(basic::ErrorLevel::Error,
                       "Invalid return type for class method: " +
                           method_decl->identifier->name,
                       "Ensure the return type is defined before use.",
                       method_decl->start, method_decl->end, "", ""));
    }
    std::vector<Variable> parameters = {};
    for (const auto &param_decl : method_decl->parameters) {
      Type param_type = this->inferExpression(*param_decl->type);
      if (!param_type.isValid()) {
        this->errors.push_back(
            basic::Error(basic::ErrorLevel::Error,
                         "Invalid type for method parameter: " +
                             param_decl->identifier->name,
                         "Ensure the parameter type is defined before use.",
                         param_decl->start, param_decl->end, "", ""));
      }
      parameters.push_back(Variable(param_decl->identifier->name, param_type,
                                    param_decl->modifier->accessor,
                                    param_decl->modifier->modifier));
    }
    methods.push_back(Function(method_decl->identifier->name, return_type,
                               parameters, method_decl->modifier->accessor,
                               method_decl->modifier->modifier));
  }
  this->current_scope->addClass(Class(decl.identifier->name, fields, methods));
  return this->current_scope->getClass(decl.identifier->name);
}

Record Analyzer::declareRecord(const ast::RecordDeclaration &decl) {
  std::vector<Variable> fields = {};
  for (const auto &field_decl : decl.fields) {
    Type field_type = this->inferExpression(*field_decl->type);
    if (!field_type.isValid()) {
      this->errors.push_back(basic::Error(
          basic::ErrorLevel::Error,
          "Invalid type for record field: " + field_decl->identifier->name,
          "Ensure the field type is defined before use.", field_decl->start,
          field_decl->end, "", ""));
    }
    fields.push_back(Variable(field_decl->identifier->name, field_type,
                              field_decl->modifier->accessor,
                              field_decl->modifier->modifier));
  }
  this->current_scope->addRecord(Record(decl.identifier->name, fields));
  return this->current_scope->getRecord(decl.identifier->name);
}

void Analyzer::analyze(ast::Program &program) {
  this->enterScope("global", ScopeKind::Global);
  for (const auto &stmt : program.statements) {
    this->analyzeStatement(*stmt);
  }
  this->exitScope();
}

void Analyzer::analyzeStatement(const ast::Statement &stmt) {
  // Dispatch based on the specific type of statement
  if (auto decl = dynamic_cast<const ast::Declaration *>(&stmt)) {
    this->analyzeDeclaration(*decl);
  } else if (auto mod_stmt =
                 dynamic_cast<const ast::ModifierStatement *>(&stmt)) {
    this->analyzeModifierStatement(*mod_stmt);
  } else if (auto block_stmt =
                 dynamic_cast<const ast::BlockStatement *>(&stmt)) {
    this->analyzeBlockStatement(*block_stmt);
  } else if (auto expr_stmt =
                 dynamic_cast<const ast::ExpressionStatement *>(&stmt)) {
    this->analyzeExpressionStatement(*expr_stmt);
  } else if (auto cond = dynamic_cast<const ast::Conditional *>(&stmt)) {
    this->analyzeConditional(*cond);
  } else if (auto ret_stmt =
                 dynamic_cast<const ast::ReturnStatement *>(&stmt)) {
    this->analyzeReturnStatement(*ret_stmt);
  } else if (auto break_stmt =
                 dynamic_cast<const ast::BreakStatement *>(&stmt)) {
    this->analyzeBreakStatement(*break_stmt);
  } else if (auto cont_stmt =
                 dynamic_cast<const ast::ContinueStatement *>(&stmt)) {
    this->analyzeContinueStatement(*cont_stmt);
  }
}

void Analyzer::analyzeDeclaration(const ast::Declaration &decl) {
  // Dispatch based on the specific type of declaration
  if (auto func_decl = dynamic_cast<const ast::FunctionDeclaration *>(&decl)) {
    this->analyzeFunctionDeclaration(*func_decl);
  } else if (auto var_decl =
                 dynamic_cast<const ast::VariableDeclaration *>(&decl)) {
    this->analyzeVariableDeclaration(*var_decl);
  } else if (auto class_decl =
                 dynamic_cast<const ast::ClassDeclaration *>(&decl)) {
    this->analyzeClassDeclaration(*class_decl);
  } else if (auto record_decl =
                 dynamic_cast<const ast::RecordDeclaration *>(&decl)) {
    this->analyzeRecordDeclaration(*record_decl);
  }
}

void Analyzer::analyzeFunctionDeclaration(
    const ast::FunctionDeclaration &func_decl) {
  auto func = this->declareFunction(func_decl);
  if (!func.isValid()) {
    this->errors.push_back(basic::Error(
        basic::ErrorLevel::Error,
        "Unable to declare function: " + func_decl.identifier->name,
        "Ensure the function is declared correctly.", func_decl.start,
        func_decl.end, "", ""));
    return;
  }
  this->enterScope(func_decl.identifier->name, ScopeKind::Function);
  for (const auto &param_decl : func_decl.parameters) {
    auto param = this->declareVariable(*param_decl);
    if (!param.isValid()) {
      this->errors.push_back(
          basic::Error(basic::ErrorLevel::Error,
                       "Unable to declare function parameter: " +
                           param_decl->identifier->name,
                       "Ensure the parameter is declared correctly.",
                       param_decl->start, param_decl->end, "", ""));
    }
  }
  this->analyzeBlockStatement(*func_decl.body);
  this->exitScope();
}

void Analyzer::analyzeVariableDeclaration(
    const ast::VariableDeclaration &var_decl) {
  auto var = this->declareVariable(var_decl);
  if (!var.isValid()) {
    this->errors.push_back(
        basic::Error(basic::ErrorLevel::Error,
                     "Unable to declare variable: " + var_decl.identifier->name,
                     "Ensure the variable is not already declared.",
                     var_decl.start, var_decl.end, "", ""));
  }
  if (var_decl.initializer) {
    Type init_type = this->inferExpression(*var_decl.initializer);
    if (!init_type.isValid()) {
      this->errors.push_back(basic::Error(
          basic::ErrorLevel::Error,
          "Invalid type for variable initializer: " + var_decl.identifier->name,
          "Ensure the initializer expression is valid.",
          var_decl.initializer->start, var_decl.initializer->end, "", ""));
    } else if (!canAssignType(init_type, var.type)) {
      this->errors.push_back(basic::Error(
          basic::ErrorLevel::Error,
          "Type mismatch in variable initializer: " + var_decl.identifier->name,
          "Ensure the initializer type matches the variable type.",
          var_decl.initializer->start, var_decl.initializer->end, "", ""));
    }
  }
}

void Analyzer::analyzeClassDeclaration(
    const ast::ClassDeclaration &class_decl) {
  auto cls = this->declareClass(class_decl);
  if (!cls.isValid()) {
    this->errors.push_back(
        basic::Error(basic::ErrorLevel::Error,
                     "Unable to declare class: " + class_decl.identifier->name,
                     "Ensure the class is not already declared.",
                     class_decl.start, class_decl.end, "", ""));
    return;
  }

  // Enter class scope and analyze methods
  this->enterScope(class_decl.identifier->name, ScopeKind::Class);
  for (const auto &method : class_decl.methods) {
    this->analyzeFunctionDeclaration(*method);
  }
  this->exitScope();
}

void Analyzer::analyzeRecordDeclaration(
    const ast::RecordDeclaration &record_decl) {
  auto record = this->declareRecord(record_decl);
  if (!record.isValid()) {
    this->errors.push_back(basic::Error(
        basic::ErrorLevel::Error,
        "Unable to declare record: " + record_decl.identifier->name,
        "Ensure the record is not already declared.", record_decl.start,
        record_decl.end, "", ""));
  }
}

void Analyzer::analyzeBlockStatement(const ast::BlockStatement &block_stmt) {
  this->enterScope("block", ScopeKind::Block);
  for (const auto &stmt : block_stmt.statements) {
    this->analyzeStatement(*stmt);
  }
  this->exitScope();
}

void Analyzer::analyzeExpressionStatement(
    const ast::ExpressionStatement &expr_stmt) {
  auto expr_type = this->inferExpression(*expr_stmt.expression);
  if (!expr_type.isValid()) {
    this->errors.push_back(basic::Error(
        basic::ErrorLevel::Error, "Invalid expression in expression statement.",
        "Ensure the expression is valid.", expr_stmt.start, expr_stmt.end, "",
        ""));
  }
}

void Analyzer::analyzeConditional(const ast::Conditional &cond) {
  // Dispatch based on the specific type of conditional
  if (auto if_cond = dynamic_cast<const ast::IfConditional *>(&cond)) {
    this->analyzeIfConditional(*if_cond);
  } else if (auto switch_cond =
                 dynamic_cast<const ast::SwitchConditional *>(&cond)) {
    this->analyzeSwitchConditional(*switch_cond);
  } else if (auto while_cond =
                 dynamic_cast<const ast::WhileConditional *>(&cond)) {
    this->analyzeWhileConditional(*while_cond);
  } else if (auto for_cond = dynamic_cast<const ast::ForConditional *>(&cond)) {
    this->analyzeForConditional(*for_cond);
  }
}

void Analyzer::analyzeIfConditional(const ast::IfConditional &if_cond) {
  auto cond_type = this->inferExpression(*if_cond.condition);
  if (!cond_type.isValid()) {
    this->errors.push_back(basic::Error(
        basic::ErrorLevel::Error, "Invalid type for condition expression.",
        "Ensure the condition expression is valid.", if_cond.condition->start,
        if_cond.condition->end, "", ""));
  } else if (!cond_type.isTruthy()) {
    this->errors.push_back(
        basic::Error(basic::ErrorLevel::Error,
                     "Condition expression must be of a truthy type.",
                     "Ensure the condition evaluates to a boolean value.",
                     if_cond.condition->start, if_cond.condition->end, "", ""));
  }
  this->analyzeBlockStatement(*if_cond.then_branch);
  for (const auto &elif_branch : if_cond.elif_branches) {
    this->analyzeIfConditional(*elif_branch);
  }
  if (if_cond.else_branch) {
    this->analyzeBlockStatement(*if_cond.else_branch);
  }
}

void Analyzer::analyzeModifierStatement(
    const ast::ModifierStatement &mod_stmt) {
  if (!hasFlag(this->current_scope->kind, ScopeKind::Class) &&
      (mod_stmt.accessor != basic::Accessor::Public ||
       mod_stmt.modifier != basic::Modifier::None)) {
    this->errors.push_back(
        basic::Error(basic::ErrorLevel::Error,
                     "Modifiers can only be used within class scopes.",
                     "Ensure modifiers are used inside classes.",
                     mod_stmt.start, mod_stmt.end, "", ""));
  }
}

void Analyzer::analyzeSwitchConditional(
    const ast::SwitchConditional &switch_cond) {
  auto switch_type = this->inferExpression(*switch_cond.switch_expression);
  if (!switch_type.isValid()) {
    this->errors.push_back(basic::Error(
        basic::ErrorLevel::Error, "Invalid type for switch expression.",
        "Ensure the switch expression is valid.",
        switch_cond.switch_expression->start,
        switch_cond.switch_expression->end, "", ""));
  }
  for (const auto &case_branch : switch_cond.case_branches) {
    this->analyzeBlockStatement(*case_branch->then_branch);
  }
}

void Analyzer::analyzeWhileConditional(
    const ast::WhileConditional &while_cond) {
  this->enterScope("while", ScopeKind::Loop);
  auto cond_type = this->inferExpression(*while_cond.condition);
  if (!cond_type.isValid()) {
    this->errors.push_back(basic::Error(
        basic::ErrorLevel::Error, "Invalid type for condition expression.",
        "Ensure the condition expression is valid.",
        while_cond.condition->start, while_cond.condition->end, "", ""));
  } else if (!cond_type.isTruthy()) {
    this->errors.push_back(basic::Error(
        basic::ErrorLevel::Error,
        "Condition expression must be of a truthy type.",
        "Ensure the condition evaluates to a boolean value.",
        while_cond.condition->start, while_cond.condition->end, "", ""));
  }
  this->analyzeBlockStatement(*while_cond.then_branch);
  this->exitScope();
}

void Analyzer::analyzeForConditional(const ast::ForConditional &for_cond) {
  this->enterScope("for", ScopeKind::Loop);
  if (for_cond.initializer) {
    this->analyzeDeclaration(*for_cond.initializer);
  }
  if (for_cond.condition) {
    auto cond_type = this->inferExpression(*for_cond.condition);
    if (!cond_type.isValid()) {
      this->errors.push_back(basic::Error(
          basic::ErrorLevel::Error,
          "Condition expression must be of a valid type.",
          "Ensure the condition expression is valid.",
          for_cond.condition->start, for_cond.condition->end, "", ""));
    } else if (!cond_type.isTruthy()) {
      this->errors.push_back(basic::Error(
          basic::ErrorLevel::Error,
          "Condition expression must be of a truthy type.",
          "Ensure the condition evaluates to a boolean value.",
          for_cond.condition->start, for_cond.condition->end, "", ""));
    }
  }
  if (for_cond.increment) {
    auto inc_type = this->inferExpression(*for_cond.increment);
    if (!inc_type.isValid()) {
      basic::Error err(basic::ErrorLevel::Error,
                       "Increment expression must be of a valid type.",
                       "Ensure the increment expression is valid.",
                       for_cond.increment->start, for_cond.increment->end, "",
                       "");
      err.log();
    } else if (!inc_type.isValid()) {
      basic::Error err(basic::ErrorLevel::Error,
                       "Increment expression must be of a valid type.",
                       "Ensure the increment expression is valid.",
                       for_cond.increment->start, for_cond.increment->end, "",
                       "");
      err.log();
    }
  }
  this->analyzeBlockStatement(*for_cond.then_branch);
  this->exitScope();
}

void Analyzer::analyzeReturnStatement(const ast::ReturnStatement &ret_stmt) {
  if (ret_stmt.expression) {
    this->inferExpression(*ret_stmt.expression);
  }
  if (!hasFlag(this->current_scope->kind, ScopeKind::Function)) {
    this->errors.push_back(
        basic::Error(basic::ErrorLevel::Error,
                     "Return statement not within a function scope.",
                     "Ensure return statements are inside functions.",
                     ret_stmt.start, ret_stmt.end, "", ""));
  }
}

void Analyzer::analyzeBreakStatement(const ast::BreakStatement &break_stmt) {
  if (!hasFlag(this->current_scope->kind, ScopeKind::Loop)) {
    this->errors.push_back(basic::Error(
        basic::ErrorLevel::Error, "Break statement not within a loop scope.",
        "Ensure break statements are inside loops.", break_stmt.start,
        break_stmt.end, "", ""));
  }
}

void Analyzer::analyzeContinueStatement(
    const ast::ContinueStatement &cont_stmt) {
  if (!hasFlag(this->current_scope->kind, ScopeKind::Loop)) {
    this->errors.push_back(basic::Error(
        basic::ErrorLevel::Error, "Continue statement not within a loop scope.",
        "Ensure continue statements are inside loops.", cont_stmt.start,
        cont_stmt.end, "", ""));
  }
}

} // namespace ml::sema