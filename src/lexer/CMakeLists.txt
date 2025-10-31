cmake_minimum_required(VERSION 3.16)

set(INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include/ml/lexer)

set(ML_LEXER_HEADERS
  ${INCLUDE_DIR}/lexer.h
  ${INCLUDE_DIR}/token.h
)

set(ML_LEXER_SOURCES
  lexer.cpp
)

add_library(
  ml_lexer
  STATIC
    ${ML_LEXER_HEADERS}
    ${ML_LEXER_SOURCES}
)

target_include_directories(
  ml_lexer
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(
  ml_lexer
    PROPERTIES
      OUTPUT_NAME "ml_lexer"
      ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
)

add_library(
  ML::Lexer
  ALIAS
  ml_lexer
)
