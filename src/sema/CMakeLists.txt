cmake_minimum_required(VERSION 3.16)

set(INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include/ml/sema)

set(ML_SEMA_HEADERS
  ${INCLUDE_DIR}/analysis.h
  ${INCLUDE_DIR}/scope.h
  ${INCLUDE_DIR}/cls.h
  ${INCLUDE_DIR}/rec.h
  ${INCLUDE_DIR}/func.h
  ${INCLUDE_DIR}/var.h
  ${INCLUDE_DIR}/type.h
)

set(ML_SEMA_SOURCES
  analysis.cpp
)

add_library(
  ml_sema
  STATIC
    ${ML_SEMA_HEADERS}
    ${ML_SEMA_SOURCES}
)

target_include_directories(
  ml_sema
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(
  ml_sema
  PUBLIC
    ML::Basic
    ML::Ast
)

set_target_properties(
  ml_sema
    PROPERTIES
      OUTPUT_NAME "ml_sema"
      ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
)

add_library(
  ML::Sema
  ALIAS
  ml_sema
)


