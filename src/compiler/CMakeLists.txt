cmake_minimum_required(VERSION 3.16)

set(COMPILER_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include/ml/compiler)

add_library(
  ml_compiler
  compiler.cpp
)

target_include_directories(
  ml_compiler
  PUBLIC
    ${COMPILER_INCLUDE_DIR}
)

target_link_libraries(
  ml_compiler
  PUBLIC
    ML::Basic
    ML::Lexer
    ML::Parser
    ML::Ast
    ML::Sema
)

set_target_properties(
  ml_compiler
    PROPERTIES
      OUTPUT_NAME "compiler"
      ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
)

add_library(
  ML::Compiler
  ALIAS
  ml_compiler
)
