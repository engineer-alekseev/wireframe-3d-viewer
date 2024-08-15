#include <gtest/gtest.h>

#include <exception>

#include "../parser/parser.h"

constexpr float ACCURACY = 1e-05;

TEST(test_parser, test1) {
  viewer::ObjParser parser;
  std::string file = "notrealfile.obj";
  try {
    parser.Parse(file);
  } catch (std::exception &e) {
    std::string result = e.what();
    EXPECT_EQ("Could not open file", result);
  }
}

TEST(test_parser, test2) {
  viewer::ObjParser parser;
  std::string file = "viewer_test/bad_file.obj";
  try {
    parser.Parse(file);
  } catch (std::exception &e) {
    std::string result = e.what();
    EXPECT_EQ("Could not open file: invalid vertex", result);
  }
}

TEST(test_parser, test3) {
  viewer::ObjParser parser;
  std::string file = "viewer_test/bad_file2.obj";
  try {
    parser.Parse(file);
  } catch (std::exception &e) {
    std::string result = e.what();
    EXPECT_EQ("Could not open file: invalid face", result);
  }
}

TEST(test_parser, test4) {
  viewer::ObjParser parser;
  std::string file = "viewer_test/bad_file3.obj";
  try {
    parser.Parse(file);
  } catch (std::exception &e) {
    std::string result = e.what();
    EXPECT_EQ("Could not open file: no vertices", result);
  }
}

TEST(test_parser, test5) {
  viewer::ModelObj model;
  viewer::ObjParser parser;
  std::string file = "viewer_test/examplecube.obj";

  model = parser.Parse(file);

  EXPECT_EQ(model.faces.size(), 72);
  EXPECT_EQ(model.vertices.size(), 24);
  EXPECT_EQ(model.scaler, 2.0);
}