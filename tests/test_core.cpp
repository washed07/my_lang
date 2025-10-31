#include "ml/basic/error.h"
#include "ml/basic/locus.h"
#include <gtest/gtest.h>
#include <sstream>

using namespace ml::basic;

// Locus Tests
class LocusTest : public ::testing::Test {
protected:
  void SetUp() override {}
  void TearDown() override {}
};

TEST_F(LocusTest, DefaultConstruction) {
  Locus loc;
  EXPECT_EQ(loc.line, 0);
  EXPECT_EQ(loc.column, 0);
}

TEST_F(LocusTest, ParameterizedConstruction) {
  Locus loc(5, 10);
  EXPECT_EQ(loc.line, 5);
  EXPECT_EQ(loc.column, 10);
}

TEST_F(LocusTest, StringConversion) {
  Locus loc(3, 7);
  std::string str = loc;
  EXPECT_EQ(str, "3:7");
}

TEST_F(LocusTest, ZeroValues) {
  Locus loc(0, 0);
  std::string str = loc;
  EXPECT_EQ(str, "0:0");
}

// Error Tests
class ErrorTest : public ::testing::Test {
protected:
  void SetUp() override {}
  void TearDown() override {}
};

TEST_F(ErrorTest, BasicConstruction) {
  Locus start_loc(1, 1);
  Locus end_loc(1, 10);
  Error err(ErrorLevel::Error, "Test error", "Test help", start_loc, end_loc,
            "test.txt", "source code");

  EXPECT_STREQ(err.what(), "Test error");
  EXPECT_STREQ(err.why(), "Test help");
  EXPECT_STREQ(err.how(), "Test error"); // how() returns desc like what()
}

TEST_F(ErrorTest, LocationMethods) {
  Locus start_loc(1, 6, 5);
  Locus end_loc(1, 16, 15);
  Error err(ErrorLevel::Error, "Test error", "Test help", start_loc, end_loc,
            "test.txt", "0123456789012345");

  EXPECT_EQ(err.from(), 5);
  EXPECT_EQ(err.to(), 15);
  EXPECT_EQ(err.where(), "[1:1]");
}

TEST_F(ErrorTest, SnippetExtraction) {
  std::string source = "hello world test";
  Locus start_loc(1, 7, 6);
  Locus end_loc(1, 12, 11);
  Error err(ErrorLevel::Error, "Test error", "Test help", start_loc, end_loc,
            "test.txt", source);

  std::string snippet = err.snippet();
  EXPECT_EQ(snippet, "world");
}

TEST_F(ErrorTest, ContextRetrieval) {
  std::string source = "test source code";
  Locus start_loc(1, 1, 0);
  Locus end_loc(1, 5, 4);
  Error err(ErrorLevel::Error, "Test error", "Test help", start_loc, end_loc,
            "test.txt", source);

  EXPECT_EQ(err.context(), source);
}

TEST_F(ErrorTest, HighlightGeneration) {
  std::string source = "hello";
  Locus start_loc(1, 2, 1);
  Locus end_loc(1, 4, 3);
  Error err(ErrorLevel::Error, "Test error", "Test help", start_loc, end_loc,
            "test.txt", source);

  std::string highlight = err.highlight();
  // Should contain the snippet and carets
  EXPECT_NE(highlight.find("el"), std::string::npos);
  EXPECT_NE(highlight.find("^^"), std::string::npos);
}

TEST_F(ErrorTest, FormatGeneration) {
  Locus start_loc(1, 1, 0);
  Locus end_loc(1, 6, 5);
  Error err(ErrorLevel::Error, "Test error", "Test help", start_loc, end_loc,
            "test.txt", "hello");

  std::string formatted = err.format();
  EXPECT_NE(formatted.find("Test error"), std::string::npos);
  EXPECT_NE(formatted.find("[1:1]"), std::string::npos);
}

TEST_F(ErrorTest, EmptySource) {
  Locus start_loc(1, 1, 0);
  Locus end_loc(1, 1, 0);
  Error err(ErrorLevel::Error, "Test error", "Test help", start_loc, end_loc,
            "test.txt", "");

  EXPECT_EQ(err.snippet(), "");
  EXPECT_EQ(err.context(), "");
}

TEST_F(ErrorTest, SingleCharacterSnippet) {
  std::string source = "a";
  Locus start_loc(1, 1, 0);
  Locus end_loc(1, 2, 1);
  Error err(ErrorLevel::Error, "Test error", "Test help", start_loc, end_loc,
            "test.txt", source);

  EXPECT_EQ(err.snippet(), "a");
}

TEST_F(ErrorTest, BoundaryConditions) {
  std::string source = "test";

  // Start at beginning
  Locus start_loc1(1, 1, 0);
  Locus end_loc1(1, 3, 2);
  Error err1(ErrorLevel::Error, "Test error", "Test help", start_loc1, end_loc1,
             "test.txt", source);
  EXPECT_EQ(err1.snippet(), "te");

  // End at end
  Locus start_loc2(1, 3, 2);
  Locus end_loc2(1, 5, 4);
  Error err2(ErrorLevel::Error, "Test error", "Test help", start_loc2, end_loc2,
             "test.txt", source);
  EXPECT_EQ(err2.snippet(), "st");

  // Full string
  Locus start_loc3(1, 1, 0);
  Locus end_loc3(1, 5, 4);
  Error err3(ErrorLevel::Error, "Test error", "Test help", start_loc3, end_loc3,
             "test.txt", source);
  EXPECT_EQ(err3.snippet(), "test");
}