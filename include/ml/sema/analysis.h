#pragma once

#include "ml/ast/ast.h"
#include "ml/basic/error.h"
#include "ml/basic/visitor.h"
#include "ml/sema/cls.h"
#include "ml/sema/func.h"
#include "ml/sema/rec.h"
#include "ml/sema/scope.h"
#include "ml/sema/type.h"
#include "ml/sema/var.h"
#include <iostream>
#include <string>

namespace ml::sema {

struct Analyzer {
private:
  std::shared_ptr<Scope> current_scope = nullptr;
  std::vector<basic::Error> errors;

  void enterScope(const std::string &name, const ScopeKind kind);
  void exitScope();

  Type inferExpression(const ast::Expression &expr);
  Type inferBinary(const ast::BinaryExpression &expr);
  Type inferUnary(const ast::UnaryExpression &expr);
  Type inferLiteral(const ast::LiteralExpression &expr);
  Type inferIdentifier(const ast::IdentifierExpression &expr);
  Type inferArrayIdentifier(const ast::ArrayIdentifierExpression &expr);
  Type inferIndex(const ast::IndexExpression &expr);
  Type inferArray(const ast::ArrayExpression &expr);
  Type inferCall(const ast::CallExpression &expr);
  Type inferAttribute(const ast::AttributeExpression &expr);

  Variable declareVariable(const ast::VariableDeclaration &decl);
  Function declareFunction(const ast::FunctionDeclaration &decl);
  Record declareRecord(const ast::RecordDeclaration &decl);
  Class declareClass(const ast::ClassDeclaration &decl);

public:
  Analyzer();

  void analyze(ast::Program &program);
  void analyzeStatement(const ast::Statement &stmt);
  void analyzeDeclaration(const ast::Declaration &decl);
  void analyzeFunctionDeclaration(const ast::FunctionDeclaration &func_decl);
  void analyzeVariableDeclaration(const ast::VariableDeclaration &var_decl);
  void analyzeClassDeclaration(const ast::ClassDeclaration &class_decl);
  void analyzeRecordDeclaration(const ast::RecordDeclaration &record_decl);
  void analyzeModifierStatement(const ast::ModifierStatement &mod_stmt);
  void analyzeBlockStatement(const ast::BlockStatement &block_stmt);
  void analyzeExpressionStatement(const ast::ExpressionStatement &expr_stmt);
  void analyzeConditional(const ast::Conditional &cond);
  void analyzeIfConditional(const ast::IfConditional &if_cond);
  void analyzeSwitchConditional(const ast::SwitchConditional &switch_cond);
  void analyzeWhileConditional(const ast::WhileConditional &while_cond);
  void analyzeForConditional(const ast::ForConditional &for_cond);
  void analyzeReturnStatement(const ast::ReturnStatement &ret_stmt);
  void analyzeBreakStatement(const ast::BreakStatement &break_stmt);
  void analyzeContinueStatement(const ast::ContinueStatement &cont_stmt);

  inline bool hasErrors() const { return !this->errors.empty(); }
  inline const std::vector<basic::Error> &getErrors() const {
    return this->errors;
  }
};
} // namespace ml::sema
