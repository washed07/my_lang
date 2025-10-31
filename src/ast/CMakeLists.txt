cmake_minimum_required(VERSION 3.15)

set(INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include/ml/ast)

set(ML_AST_HEADERS
  ${INCLUDE_DIR}/ast.h  
  ${INCLUDE_DIR}/node.h
  ${INCLUDE_DIR}/expr.h
  ${INCLUDE_DIR}/stmt.h
  ${INCLUDE_DIR}/decl.h
  ${INCLUDE_DIR}/cond.h
  ${INCLUDE_DIR}/node_printer.h
)

set(ML_AST_SOURCES
  node_printer.cpp
)

add_library(
  ml_ast
  STATIC
    ${ML_AST_HEADERS}
    ${ML_AST_SOURCES}
)

target_include_directories(
  ml_ast
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(
  ml_ast
  PUBLIC
    ML::Basic
)

set_target_properties(
  ml_ast
    PROPERTIES
      OUTPUT_NAME "ml_ast"
      ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
)

add_library(
  ML::Ast
  ALIAS
  ml_ast
)