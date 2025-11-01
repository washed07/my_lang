#include "ml/ast/ast.h"
#include "ml/basic/error.h"
#include "ml/parser/parser.h"
#include <gtest/gtest.h>
#include <memory>
#include <sstream>

using namespace ml::parser;
using namespace ml::ast;

class ParserTest : public ::testing::Test {
protected:
  void SetUp() override {}
  void TearDown() override {}

  // Helper function to parse source code and return the program
  std::unique_ptr<Program> parseSource(const std::string &source) {
    Parser parser;
    return parser.parse(source);
  }

  // Helper function to parse and check for nullptr (parse failure)
  void expectParseFailure(const std::string &source) {
    auto program = parseSource(source);
    EXPECT_EQ(program, nullptr);
  }

  // Helper function to parse and check for success
  void expectParseSuccess(const std::string &source) {
    auto program = parseSource(source);
    EXPECT_NE(program, nullptr);
  }
};

// Basic parsing tests
TEST_F(ParserTest, EmptySource) {
  auto program = parseSource("");
  EXPECT_NE(program, nullptr);
  EXPECT_TRUE(program->statements.empty());
}

TEST_F(ParserTest, WhitespaceOnlySource) {
  auto program = parseSource("   \n\t   ");
  EXPECT_NE(program, nullptr);
  EXPECT_TRUE(program->statements.empty());
}

// Variable declaration tests
TEST_F(ParserTest, SimpleVariableDeclaration) {
  auto program = parseSource("let x: int = 5;");
  EXPECT_NE(program, nullptr);
  ASSERT_EQ(program->statements.size(), 1);

  auto *varDecl =
      dynamic_cast<VariableDeclaration *>(program->statements[0].get());
  ASSERT_NE(varDecl, nullptr);
  EXPECT_EQ(varDecl->identifier->name, "x");
}

TEST_F(ParserTest, VariableDeclarationWithoutInitializer) {
  auto program = parseSource("let x: int;");
  EXPECT_NE(program, nullptr);
  ASSERT_EQ(program->statements.size(), 1);

  auto *varDecl =
      dynamic_cast<VariableDeclaration *>(program->statements[0].get());
  ASSERT_NE(varDecl, nullptr);
  EXPECT_EQ(varDecl->identifier->name, "x");
  EXPECT_EQ(varDecl->initializer, nullptr);
}

TEST_F(ParserTest, VariableDeclarationWithFloatType) {
  auto program = parseSource("let y: float = 3.14;");
  EXPECT_NE(program, nullptr);
  ASSERT_EQ(program->statements.size(), 1);

  auto *varDecl =
      dynamic_cast<VariableDeclaration *>(program->statements[0].get());
  ASSERT_NE(varDecl, nullptr);
  EXPECT_EQ(varDecl->identifier->name, "y");
}

TEST_F(ParserTest, VariableDeclarationWithStringInitializer) {
  auto program = parseSource("let name: string = \"hello\";");
  EXPECT_NE(program, nullptr);
  ASSERT_EQ(program->statements.size(), 1);

  auto *varDecl =
      dynamic_cast<VariableDeclaration *>(program->statements[0].get());
  ASSERT_NE(varDecl, nullptr);
  EXPECT_EQ(varDecl->identifier->name, "name");
}

TEST_F(ParserTest, ArrayVariableDeclaration) {
  auto program = parseSource("let arr: int[10];");
  EXPECT_NE(program, nullptr);
  ASSERT_EQ(program->statements.size(), 1);

  auto *varDecl =
      dynamic_cast<VariableDeclaration *>(program->statements[0].get());
  ASSERT_NE(varDecl, nullptr);
  EXPECT_EQ(varDecl->identifier->name, "arr");

  auto *arrayType =
      dynamic_cast<ArrayIdentifierExpression *>(varDecl->type.get());
  ASSERT_NE(arrayType, nullptr);
  EXPECT_EQ(arrayType->name, "int");
}

TEST_F(ParserTest, NullableVariableDeclaration) {
  auto program = parseSource("let opt: int? = null;");
  EXPECT_NE(program, nullptr);
  ASSERT_EQ(program->statements.size(), 1);

  auto *varDecl =
      dynamic_cast<VariableDeclaration *>(program->statements[0].get());
  ASSERT_NE(varDecl, nullptr);
  EXPECT_EQ(varDecl->identifier->name, "opt");
}

// Function declaration tests
TEST_F(ParserTest, SimpleFunctionDeclaration) {
  auto program = parseSource("fn add(a: int, b: int) { return a + b; }");
  EXPECT_NE(program, nullptr);
  ASSERT_EQ(program->statements.size(), 1);

  auto *funcDecl =
      dynamic_cast<FunctionDeclaration *>(program->statements[0].get());
  ASSERT_NE(funcDecl, nullptr);
  EXPECT_EQ(funcDecl->identifier->name, "add");
  EXPECT_EQ(funcDecl->parameters.size(), 2);
}

TEST_F(ParserTest, FunctionDeclarationWithReturnType) {
  auto program = parseSource("fn getValue() int { return 42; }");
  EXPECT_NE(program, nullptr);
  ASSERT_EQ(program->statements.size(), 1);

  auto *funcDecl =
      dynamic_cast<FunctionDeclaration *>(program->statements[0].get());
  ASSERT_NE(funcDecl, nullptr);
  EXPECT_EQ(funcDecl->identifier->name, "getValue");
  EXPECT_EQ(funcDecl->parameters.size(), 0);
}

TEST_F(ParserTest, FunctionDeclarationNoParameters) {
  auto program = parseSource("fn main() { }");
  EXPECT_NE(program, nullptr);
  ASSERT_EQ(program->statements.size(), 1);

  auto *funcDecl =
      dynamic_cast<FunctionDeclaration *>(program->statements[0].get());
  ASSERT_NE(funcDecl, nullptr);
  EXPECT_EQ(funcDecl->identifier->name, "main");
  EXPECT_EQ(funcDecl->parameters.size(), 0);
}

TEST_F(ParserTest, FunctionDeclarationWithModifiers) {
  auto program = parseSource("fn publicFunction(): void { }");
  EXPECT_NE(program, nullptr);
  ASSERT_EQ(program->statements.size(), 1);

  auto *funcDecl =
      dynamic_cast<FunctionDeclaration *>(program->statements[0].get());
  ASSERT_NE(funcDecl, nullptr);
  EXPECT_EQ(funcDecl->identifier->name, "publicFunction");
}

// Record declaration tests
TEST_F(ParserTest, SimpleRecordDeclaration) {
  auto program = parseSource("rec Person { name: string; age: int; }");
  EXPECT_NE(program, nullptr);
  ASSERT_EQ(program->statements.size(), 1);

  auto *recDecl =
      dynamic_cast<RecordDeclaration *>(program->statements[0].get());
  ASSERT_NE(recDecl, nullptr);
  EXPECT_EQ(recDecl->identifier->name, "Person");
  EXPECT_EQ(recDecl->fields.size(), 2);
}

TEST_F(ParserTest, EmptyRecordDeclaration) {
  auto program = parseSource("rec Empty { }");
  EXPECT_NE(program, nullptr);
  ASSERT_EQ(program->statements.size(), 1);

  auto *recDecl =
      dynamic_cast<RecordDeclaration *>(program->statements[0].get());
  ASSERT_NE(recDecl, nullptr);
  EXPECT_EQ(recDecl->identifier->name, "Empty");
  EXPECT_EQ(recDecl->fields.size(), 0);
}

// Expression tests
TEST_F(ParserTest, SimpleExpression) {
  auto program = parseSource("5 + 3;");
  EXPECT_NE(program, nullptr);
  ASSERT_EQ(program->statements.size(), 1);

  auto *exprStmt =
      dynamic_cast<ExpressionStatement *>(program->statements[0].get());
  ASSERT_NE(exprStmt, nullptr);

  auto *binExpr = dynamic_cast<BinaryExpression *>(exprStmt->expression.get());
  ASSERT_NE(binExpr, nullptr);
  EXPECT_EQ(binExpr->op, "+");
}

TEST_F(ParserTest, ComplexExpression) {
  auto program = parseSource("a * b + c / d;");
  EXPECT_NE(program, nullptr);
  ASSERT_EQ(program->statements.size(), 1);

  auto *exprStmt =
      dynamic_cast<ExpressionStatement *>(program->statements[0].get());
  ASSERT_NE(exprStmt, nullptr);

  auto *binExpr = dynamic_cast<BinaryExpression *>(exprStmt->expression.get());
  ASSERT_NE(binExpr, nullptr);
  EXPECT_EQ(binExpr->op, "+");
}

TEST_F(ParserTest, ParenthesizedExpression) {
  auto program = parseSource("(a + b) * c;");
  EXPECT_NE(program, nullptr);
  ASSERT_EQ(program->statements.size(), 1);

  auto *exprStmt =
      dynamic_cast<ExpressionStatement *>(program->statements[0].get());
  ASSERT_NE(exprStmt, nullptr);

  auto *binExpr = dynamic_cast<BinaryExpression *>(exprStmt->expression.get());
  ASSERT_NE(binExpr, nullptr);
  EXPECT_EQ(binExpr->op, "*");
}

TEST_F(ParserTest, UnaryExpression) {
  auto program = parseSource("-x;");
  EXPECT_NE(program, nullptr);
  ASSERT_EQ(program->statements.size(), 1);

  auto *exprStmt =
      dynamic_cast<ExpressionStatement *>(program->statements[0].get());
  ASSERT_NE(exprStmt, nullptr);

  auto *unaryExpr = dynamic_cast<UnaryExpression *>(exprStmt->expression.get());
  ASSERT_NE(unaryExpr, nullptr);
  EXPECT_EQ(unaryExpr->op, "-");
}

TEST_F(ParserTest, AssignmentExpression) {
  auto program = parseSource("x = 5;");
  EXPECT_NE(program, nullptr);
  ASSERT_EQ(program->statements.size(), 1);

  auto *exprStmt =
      dynamic_cast<ExpressionStatement *>(program->statements[0].get());
  ASSERT_NE(exprStmt, nullptr);

  auto *binExpr = dynamic_cast<BinaryExpression *>(exprStmt->expression.get());
  ASSERT_NE(binExpr, nullptr);
  EXPECT_EQ(binExpr->op, "=");
}

// Function call tests
TEST_F(ParserTest, SimpleFunctionCall) {
  auto program = parseSource("foo();");
  EXPECT_NE(program, nullptr);
  ASSERT_EQ(program->statements.size(), 1);

  auto *exprStmt =
      dynamic_cast<ExpressionStatement *>(program->statements[0].get());
  ASSERT_NE(exprStmt, nullptr);

  auto *callExpr = dynamic_cast<CallExpression *>(exprStmt->expression.get());
  ASSERT_NE(callExpr, nullptr);
  EXPECT_EQ(callExpr->arguments.size(), 0);
}

TEST_F(ParserTest, FunctionCallWithArguments) {
  auto program = parseSource("add(1, 2);");
  EXPECT_NE(program, nullptr);
  ASSERT_EQ(program->statements.size(), 1);

  auto *exprStmt =
      dynamic_cast<ExpressionStatement *>(program->statements[0].get());
  ASSERT_NE(exprStmt, nullptr);

  auto *callExpr = dynamic_cast<CallExpression *>(exprStmt->expression.get());
  ASSERT_NE(callExpr, nullptr);
  EXPECT_EQ(callExpr->arguments.size(), 2);
}

// Attribute access tests
TEST_F(ParserTest, AttributeAccess) {
  auto program = parseSource("this.name;");
  EXPECT_NE(program, nullptr);
  ASSERT_EQ(program->statements.size(), 1);

  auto *exprStmt =
      dynamic_cast<ExpressionStatement *>(program->statements[0].get());
  ASSERT_NE(exprStmt, nullptr);

  auto *attrExpr =
      dynamic_cast<AttributeExpression *>(exprStmt->expression.get());
  ASSERT_NE(attrExpr, nullptr);
}

TEST_F(ParserTest, ChainedAttributeAccess) {
  auto program = parseSource("obj.field.subfield;");
  EXPECT_NE(program, nullptr);
  ASSERT_EQ(program->statements.size(), 1);

  auto *exprStmt =
      dynamic_cast<ExpressionStatement *>(program->statements[0].get());
  ASSERT_NE(exprStmt, nullptr);

  auto *attrExpr =
      dynamic_cast<AttributeExpression *>(exprStmt->expression.get());
  ASSERT_NE(attrExpr, nullptr);
}

// Array access tests
TEST_F(ParserTest, ArrayIndexing) {
  auto program = parseSource("arr[0];");
  EXPECT_NE(program, nullptr);
  ASSERT_EQ(program->statements.size(), 1);

  auto *exprStmt =
      dynamic_cast<ExpressionStatement *>(program->statements[0].get());
  ASSERT_NE(exprStmt, nullptr);

  auto *indexExpr = dynamic_cast<IndexExpression *>(exprStmt->expression.get());
  ASSERT_NE(indexExpr, nullptr);
}

TEST_F(ParserTest, MultiDimensionalArrayIndexing) {
  auto program = parseSource("matrix[i][j];");
  EXPECT_NE(program, nullptr);
  ASSERT_EQ(program->statements.size(), 1);

  auto *exprStmt =
      dynamic_cast<ExpressionStatement *>(program->statements[0].get());
  ASSERT_NE(exprStmt, nullptr);

  auto *indexExpr = dynamic_cast<IndexExpression *>(exprStmt->expression.get());
  ASSERT_NE(indexExpr, nullptr);
}

// Control flow tests
TEST_F(ParserTest, IfStatement) {
  auto program = parseSource("if (x > 0) { return x; }");
  EXPECT_NE(program, nullptr);
  ASSERT_EQ(program->statements.size(), 1);

  auto *ifCond = dynamic_cast<IfConditional *>(program->statements[0].get());
  ASSERT_NE(ifCond, nullptr);
  ASSERT_NE(ifCond->condition, nullptr);
  ASSERT_NE(ifCond->then_branch, nullptr);
}

TEST_F(ParserTest, IfElseStatement) {
  auto program = parseSource("if (x > 0) { return x; } else { return -x; }");
  EXPECT_NE(program, nullptr);
  ASSERT_EQ(program->statements.size(), 1);

  auto *ifCond = dynamic_cast<IfConditional *>(program->statements[0].get());
  ASSERT_NE(ifCond, nullptr);
  ASSERT_NE(ifCond->condition, nullptr);
  ASSERT_NE(ifCond->then_branch, nullptr);
  ASSERT_NE(ifCond->else_branch, nullptr);
}

TEST_F(ParserTest, IfElifElseStatement) {
  auto program = parseSource("if (x > 0) { return 1; } elif (x < 0) { return "
                             "-1; } else { return 0; }");
  EXPECT_NE(program, nullptr);
  ASSERT_EQ(program->statements.size(), 1);

  auto *ifCond = dynamic_cast<IfConditional *>(program->statements[0].get());
  ASSERT_NE(ifCond, nullptr);
  ASSERT_NE(ifCond->condition, nullptr);
  ASSERT_NE(ifCond->then_branch, nullptr);
  EXPECT_EQ(ifCond->elif_branches.size(), 1);
  ASSERT_NE(ifCond->else_branch, nullptr);
}

TEST_F(ParserTest, WhileLoop) {
  auto program = parseSource("while (i < 10) { i = i + 1; }");
  EXPECT_NE(program, nullptr);
  ASSERT_EQ(program->statements.size(), 1);

  auto *whileCond =
      dynamic_cast<WhileConditional *>(program->statements[0].get());
  ASSERT_NE(whileCond, nullptr);
  ASSERT_NE(whileCond->condition, nullptr);
  ASSERT_NE(whileCond->then_branch, nullptr);
}

TEST_F(ParserTest, ForLoopWithInitializer) {
  auto program = parseSource("for (let i: int = 0; i < 10; i = i + 1) { }");
  EXPECT_NE(program, nullptr);
  ASSERT_EQ(program->statements.size(), 1);

  auto *forCond = dynamic_cast<ForConditional *>(program->statements[0].get());
  ASSERT_NE(forCond, nullptr);
  ASSERT_NE(forCond->initializer, nullptr);
  ASSERT_NE(forCond->condition, nullptr);
  ASSERT_NE(forCond->increment, nullptr);
  ASSERT_NE(forCond->then_branch, nullptr);
}

TEST_F(ParserTest, ForRangeLoop) {
  auto program = parseSource("for (0..10) { }");
  EXPECT_NE(program, nullptr);
  ASSERT_EQ(program->statements.size(), 1);

  auto *forCond = dynamic_cast<ForConditional *>(program->statements[0].get());
  ASSERT_NE(forCond, nullptr);
  EXPECT_EQ(forCond->initializer, nullptr);
  ASSERT_NE(forCond->condition, nullptr);
  EXPECT_EQ(forCond->increment, nullptr);

  // Check that condition is a range expression
  auto *rangeExpr = dynamic_cast<BinaryExpression *>(forCond->condition.get());
  ASSERT_NE(rangeExpr, nullptr);
  EXPECT_EQ(rangeExpr->op, "..");
}

// Return statement tests
TEST_F(ParserTest, SimpleReturnStatement) {
  auto program = parseSource("return 42;");
  EXPECT_NE(program, nullptr);
  ASSERT_EQ(program->statements.size(), 1);

  auto *retStmt = dynamic_cast<ReturnStatement *>(program->statements[0].get());
  ASSERT_NE(retStmt, nullptr);
  ASSERT_NE(retStmt->expression, nullptr);
}

TEST_F(ParserTest, EmptyReturnStatement) {
  auto program = parseSource("return;");
  EXPECT_NE(program, nullptr);
  ASSERT_EQ(program->statements.size(), 1);

  auto *retStmt = dynamic_cast<ReturnStatement *>(program->statements[0].get());
  ASSERT_NE(retStmt, nullptr);
  EXPECT_EQ(retStmt->expression, nullptr);
}

// Break and continue tests
TEST_F(ParserTest, BreakStatement) {
  auto program = parseSource("break;");
  EXPECT_NE(program, nullptr);
  ASSERT_EQ(program->statements.size(), 1);

  auto *breakStmt =
      dynamic_cast<BreakStatement *>(program->statements[0].get());
  ASSERT_NE(breakStmt, nullptr);
}

TEST_F(ParserTest, ContinueStatement) {
  auto program = parseSource("continue;");
  EXPECT_NE(program, nullptr);
  ASSERT_EQ(program->statements.size(), 1);

  auto *contStmt =
      dynamic_cast<ContinueStatement *>(program->statements[0].get());
  ASSERT_NE(contStmt, nullptr);
}

// Complex program tests
TEST_F(ParserTest, CompleteFunction) {
  std::string source = R"(
    fn factorial(n: int): int {
      if (n <= 1) {
        return 1;
      } else {
        return n * factorial(n - 1);
      }
    }
  )";

  auto program = parseSource(source);
  EXPECT_NE(program, nullptr);
  ASSERT_EQ(program->statements.size(), 1);

  auto *funcDecl =
      dynamic_cast<FunctionDeclaration *>(program->statements[0].get());
  ASSERT_NE(funcDecl, nullptr);
  EXPECT_EQ(funcDecl->identifier->name, "factorial");
  EXPECT_EQ(funcDecl->parameters.size(), 1);
}

TEST_F(ParserTest, RecordWithMethods) {
  std::string source = R"(
    rec Point {
      x: float;
      y: float;
    }
    
    fn distance(p1: Point, p2: Point): float {
      let dx: float = p1.x - p2.x;
      let dy: float = p1.y - p2.y;
      return dx * dx + dy * dy;
    }
  )";

  auto program = parseSource(source);
  EXPECT_NE(program, nullptr);
  ASSERT_EQ(program->statements.size(), 2);

  auto *recDecl =
      dynamic_cast<RecordDeclaration *>(program->statements[0].get());
  ASSERT_NE(recDecl, nullptr);
  EXPECT_EQ(recDecl->identifier->name, "Point");

  auto *funcDecl =
      dynamic_cast<FunctionDeclaration *>(program->statements[1].get());
  ASSERT_NE(funcDecl, nullptr);
  EXPECT_EQ(funcDecl->identifier->name, "distance");
}

// Error handling tests
TEST_F(ParserTest, InvalidSyntax) {
  testing::internal::CaptureStderr();
  auto program = parseSource("let x = ;");
  std::string stderr_output = testing::internal::GetCapturedStderr();

  // Parser should report errors to stderr but may still return a program
  EXPECT_GT(stderr_output.length(), 0);
}

// Edge cases
TEST_F(ParserTest, NestedBlocks) {
  std::string source = R"(
    fn test() {
      {
        let x: int = 5;
        {
          let y: int = x + 1;
        }
      }
    }
  )";

  expectParseSuccess(source);
}

TEST_F(ParserTest, ComplexExpressionPrecedence) {
  auto program = parseSource("a + b * c - d / e;");
  EXPECT_NE(program, nullptr);
  ASSERT_EQ(program->statements.size(), 1);

  auto *exprStmt =
      dynamic_cast<ExpressionStatement *>(program->statements[0].get());
  ASSERT_NE(exprStmt, nullptr);

  // Should parse as: (a + (b * c)) - (d / e)
  auto *outerExpr =
      dynamic_cast<BinaryExpression *>(exprStmt->expression.get());
  ASSERT_NE(outerExpr, nullptr);
  EXPECT_EQ(outerExpr->op, "-");
}

TEST_F(ParserTest, ChainedComparisons) {
  auto program = parseSource("a < b && b <= c;");
  EXPECT_NE(program, nullptr);
  ASSERT_EQ(program->statements.size(), 1);

  auto *exprStmt =
      dynamic_cast<ExpressionStatement *>(program->statements[0].get());
  ASSERT_NE(exprStmt, nullptr);

  auto *binExpr = dynamic_cast<BinaryExpression *>(exprStmt->expression.get());
  ASSERT_NE(binExpr, nullptr);
  EXPECT_EQ(binExpr->op, "&&");
}

TEST_F(ParserTest, ArrayLiteral) {
  auto program = parseSource("let arr: int[] = [1, 2, 3];");
  EXPECT_NE(program, nullptr);
  ASSERT_EQ(program->statements.size(), 1);

  auto *varDecl =
      dynamic_cast<VariableDeclaration *>(program->statements[0].get());
  ASSERT_NE(varDecl, nullptr);

  auto *arrayExpr = dynamic_cast<ArrayExpression *>(varDecl->initializer.get());
  ASSERT_NE(arrayExpr, nullptr);
  EXPECT_EQ(arrayExpr->elements.size(), 3);
}

TEST_F(ParserTest, EmptyArrayLiteral) {
  auto program = parseSource("let arr: int[] = [];");
  EXPECT_NE(program, nullptr);
  ASSERT_EQ(program->statements.size(), 1);

  auto *varDecl =
      dynamic_cast<VariableDeclaration *>(program->statements[0].get());
  ASSERT_NE(varDecl, nullptr);

  auto *arrayExpr = dynamic_cast<ArrayExpression *>(varDecl->initializer.get());
  ASSERT_NE(arrayExpr, nullptr);
  EXPECT_EQ(arrayExpr->elements.size(), 0);
}