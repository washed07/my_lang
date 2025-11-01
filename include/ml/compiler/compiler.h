/**
 * @file compiler.h
 * @brief Compiler definitions for My Language.
 * @details Defines the Compiler class and related functions for compiling
 * source code from My Language.
 * @copyright Copyright (c) 2025 Karson P. Califf
 */

#pragma once

#include "ml/ast/ast.h"
#include "ml/parser/parser.h"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>

namespace ml::compiler {

/**
 * @struct Configuration compiler.h
 * @brief Compiler configuration options.
 */
struct Configuration {
  bool debug = false; // Enable debug information
};

/**
 * @class Compiler compiler.h
 * @brief Compiler for My Language.
 * @details Provides methods to compile source code from My Language.
 */
class Compiler {
private:
  parser::Parser parser_; // The parser instance for parsing source code

  static std::string readFile(const std::string &file_path);

public:
  /**
   * @brief Compiles the given source code.
   * @param source The source code to compile.
   * @return A unique pointer to the AST root node.
   */
  uint64_t compileSource(const std::string &source,
                         const Configuration &config = Configuration());

  /**
   * @brief Compiles the source code from the given file.
   * @param file_path The path to the source file.
   * @return A unique pointer to the AST root node.
   */
  uint64_t compileFile(const std::string &file_path,
                       const Configuration &config = Configuration());
};

} // namespace ml::compiler
