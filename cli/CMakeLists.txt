cmake_minimum_required(VERSION 3.16)

set(INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include/ml/basic)
set(SOURCE_DIR ${CMAKE_SOURCE_DIR}/src/compiler)

include_directories(${INCLUDE_DIR})

add_executable(my_lang
  my_lang.cpp
)

target_link_libraries(my_lang
  ml_compiler
)

set_target_properties(
  my_lang
  PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)