#include <gtest/gtest.h>

#include "../affine_transformations/matrix4X4.h"

constexpr float ACCURACY = 1e-05;

TEST(affine_transformations_test_shift, test_1) {
  viewer::Matrix4X4 m;
  viewer::TransformData transformations;
  transformations.x_shift = -12.67;
  transformations.y_shift = 34.21;
  transformations.z_shift = 10.01;
  m.MakeMovement(transformations);
  float result[][4] = {{1, 0, 0, transformations.x_shift},
                       {0, 1, 0, transformations.y_shift},
                       {0, 0, 1, -5 + transformations.z_shift},
                       {0, 0, 0, 1}};
  float **result_matrix = m.GetMatrix();
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; ++j) {
      EXPECT_NEAR(result_matrix[i][j], result[i][j], ACCURACY);
    }
  }
}

TEST(affine_transformations_test_scaler, test_1) {
  viewer::Matrix4X4 m;
  viewer::TransformData transformations;
  transformations.user_scaler = 123.45;
  m.MakeMovement(transformations);
  float result[][4] = {{transformations.user_scaler, 0, 0, 0},
                       {0, transformations.user_scaler, 0, 0},
                       {0, 0, transformations.user_scaler, -5},
                       {0, 0, 0, 1}};
  float **result_matrix = m.GetMatrix();
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; ++j)
      EXPECT_NEAR(result_matrix[i][j], result[i][j], ACCURACY);
  }
}

TEST(affine_transformations_test_rotation, test_1) {
  viewer::Matrix4X4 m;
  viewer::TransformData transformations;
  transformations.x_rotation_deg = 45;
  transformations.y_rotation_deg = 60;
  transformations.z_rotation_deg = 35;
  m.MakeMovement(transformations);
  float result[][4] = {{0.409576, -0.286788, 0.866025, 0},
                       {0.907206, 0.227986, -0.353553, 0},
                       {-0.0960463, 0.93047, 0.353553, -5},
                       {0, 0, 0, 1}};
  float **result_matrix = m.GetMatrix();
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; ++j)
      EXPECT_NEAR(result_matrix[i][j], result[i][j], ACCURACY);
  }
}

TEST(affine_transformations_test_all, test_1) {
  viewer::Matrix4X4 m;
  viewer::TransformData transformations;
  transformations.x_shift = -26.67;
  transformations.y_shift = 37.89;
  transformations.z_shift = -10.54;
  transformations.user_scaler = 50;
  transformations.x_rotation_deg = 45;
  transformations.y_rotation_deg = 60;
  transformations.z_rotation_deg = 35;
  m.MakeMovement(transformations);
  float result[][4] = {{20.4788, -14.3394109, 43.30127018, -26.67},
                       {45.3603, 11.39927829, -17.6776695, 37.89},
                       {-4.80232, 46.52351823, 17.6776695, -15.54},
                       {0, 0, 0, 1}};
  float **result_matrix = m.GetMatrix();
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; ++j)
      EXPECT_NEAR(result_matrix[i][j], result[i][j], ACCURACY);
  }
}

TEST(one_row_matrix, test_1) {
  viewer::Matrix4X4 m;
  viewer::TransformData transformations;
  transformations.x_shift = -26.67;
  transformations.y_shift = 37.89;
  transformations.z_shift = -10.54;
  transformations.user_scaler = 50;
  transformations.x_rotation_deg = 45;
  transformations.y_rotation_deg = 60;
  transformations.z_rotation_deg = 35;
  m.MakeMovement(transformations);

  float result[16] = {20.4788,  -14.3394109, 43.30127018, -26.67,
                      45.3603,  11.39927829, -17.6776695, 37.89,
                      -4.80232, 46.52351823, 17.6776695,  -15.54,
                      0,        0,           0,           1};
  float *result_matrix = m.CreateOneRowMatrix();
  for (int i = 0; i < 16; i++) {
    EXPECT_NEAR(result_matrix[i], result[i], ACCURACY);
  }
}

TEST(copy_constructor, test_1) {
  viewer::Matrix4X4 m;
  viewer::TransformData transformations;
  transformations.x_shift = -26.67;
  transformations.y_shift = 37.89;
  transformations.z_shift = -10.54;
  transformations.user_scaler = 50;
  transformations.x_rotation_deg = 45;
  transformations.y_rotation_deg = 60;
  transformations.z_rotation_deg = 35;
  m.MakeMovement(transformations);
  float result[][4] = {{20.4788, -14.3394109, 43.30127018, -26.67},
                       {45.3603, 11.39927829, -17.6776695, 37.89},
                       {-4.80232, 46.52351823, 17.6776695, -15.54},
                       {0, 0, 0, 1}};
  viewer::Matrix4X4 copy(m);
  float **result_matrix = copy.GetMatrix();
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; ++j)
      EXPECT_NEAR(result_matrix[i][j], result[i][j], ACCURACY);
  }
}
