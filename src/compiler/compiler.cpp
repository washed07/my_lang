/**
 * @file compiler.cpp
 * @brief Compiler source code for My Language.
 * @copyright Copyright (c) 2025 Karson P. Califf
 */

#include "ml/compiler/compiler.h"

namespace ml::compiler {

std::string Compiler::readFile(const std::string &file_path) {
  std::ifstream file_stream(file_path);
  if (!file_stream.is_open()) {
    throw std::runtime_error("Failed to open file: " + file_path);
  }

  std::stringstream buffer;
  buffer << file_stream.rdbuf();
  return buffer.str();
}

std::unique_ptr<ast::Program>
Compiler::compileSource(const std::string &source,
                        const Configuration &config) {
  this->parser_ = parser::Parser();
  auto program = this->parser_.parse(source);
  if (config.debug) {
    std::cout << "Compilation finished." << std::endl;
    ast::NodePrinter printer;
    program->accept(printer);
  }
  return program;
}

std::unique_ptr<ast::Program>
Compiler::compileFile(const std::string &file_path,
                      const Configuration &config) {
  std::string source = readFile(file_path);
  return this->compileSource(source, config);
}

} // namespace ml::compiler
