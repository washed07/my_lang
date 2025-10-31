cmake_minimum_required(VERSION 3.16)

# Try to find Google Test first
find_package(GTest QUIET)

if(NOT GTest_FOUND)
    # If not found, fetch Google Test
    include(FetchContent)
    FetchContent_Declare(
      googletest
      URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
      DOWNLOAD_EXTRACT_TIMESTAMP true
    )
    # For Windows: Prevent overriding the parent project's compiler/linker settings
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)
    
    # Use the fetched targets
    set(GTEST_LIBRARIES gtest_main)
    set(GTEST_INCLUDE_DIRS)
else()
    # Use system installation
    set(GTEST_LIBRARIES GTest::gtest_main)
    set(GTEST_INCLUDE_DIRS)
endif()

# Enable testing
include(GoogleTest)

# Test executables
add_executable(test_lexer test_lexer.cpp)
add_executable(test_core test_core.cpp)
add_executable(test_parser test_parser.cpp)

# Link against our libraries and Google Test
target_link_libraries(test_lexer PRIVATE ML::Lexer ML::Basic ${GTEST_LIBRARIES})
target_link_libraries(test_core PRIVATE ML::Basic ${GTEST_LIBRARIES})
target_link_libraries(test_parser PRIVATE ML::Parser ML::Ast ML::Lexer ML::Basic ${GTEST_LIBRARIES})

# Include directories
target_include_directories(test_lexer PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_include_directories(test_core PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_include_directories(test_parser PRIVATE ${CMAKE_SOURCE_DIR}/include)

# Discover tests automatically
gtest_discover_tests(test_lexer)
gtest_discover_tests(test_core)
gtest_discover_tests(test_parser)