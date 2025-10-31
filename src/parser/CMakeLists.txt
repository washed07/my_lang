cmake_minimum_required(VERSION 3.16)

set(INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include/ml/parser)

set(ML_PARSER_HEADERS
  ${INCLUDE_DIR}/parser.h
)

set(ML_PARSER_SOURCES
  parser.cpp
)

add_library(
  ml_parser
  STATIC
    ${ML_PARSER_HEADERS}
    ${ML_PARSER_SOURCES}
)

target_include_directories(
  ml_parser
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(
  ml_parser
  PUBLIC
    ML::Basic
    ML::Ast
    ML::Lexer
)

set_target_properties(
  ml_parser
    PROPERTIES
      OUTPUT_NAME "ml_parser"
      ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
)

add_library(
  ML::Parser
  ALIAS
  ml_parser
)


