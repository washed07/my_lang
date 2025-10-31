cmake_minimum_required(VERSION 3.16)

set(INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include/ml/basic)

set(ML_BASIC_HEADERS
  ${INCLUDE_DIR}/flags.h
  ${INCLUDE_DIR}/locus.h
  ${INCLUDE_DIR}/syntax.h
  ${INCLUDE_DIR}/error.h
  ${INCLUDE_DIR}/visitor.h
  ${INCLUDE_DIR}/accessor.h
  ${INCLUDE_DIR}/modifier.h
)

set(ML_BASIC_SOURCES
  error.cpp
)

add_library(
  ml_basic
  STATIC
    ${ML_BASIC_HEADERS}
    ${ML_BASIC_SOURCES}
)

target_include_directories(
  ml_basic
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
    
    $<INSTALL_INTERFACE:include>
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(
  ml_basic
    PROPERTIES
      OUTPUT_NAME "ml_basic"
      ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
)

add_library(
  ML::Basic
  ALIAS
  ml_basic
)
