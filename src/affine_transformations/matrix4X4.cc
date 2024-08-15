#include "matrix4X4.h"

namespace viewer {
Matrix4X4::Matrix4X4() : rows_(4), cols_(4) {
  MemoryAllocate();
  InitializeIdentityMatrix();
}

Matrix4X4::Matrix4X4(const Matrix4X4& other) { *this = other; }

Matrix4X4::~Matrix4X4() {
  if (matrix_) MemoryFree();
  matrix_ = nullptr;
  rows_ = cols_ = 0;
  if (matrix_one_row_ != nullptr) delete[] matrix_one_row_;
}

void Matrix4X4::MulMatrix(const Matrix4X4& other) { *this *= other; }

Matrix4X4& Matrix4X4::operator=(const Matrix4X4& other) noexcept {
  if (matrix_ == nullptr) {
    rows_ = other.rows_;
    cols_ = other.cols_;
    MemoryAllocate();
  }
  for (int i = 0; i < rows_; ++i) {
    for (int j = 0; j < cols_; ++j) {
      matrix_[i][j] = other.matrix_[i][j];
    }
  }
  return *this;
}

Matrix4X4& Matrix4X4::operator*=(const Matrix4X4& other) noexcept {
  Matrix4X4 result_matrix;
  result_matrix.SetNullMatrix();
  for (int i = 0; i < rows_; ++i) {
    for (int j = 0; j < cols_; ++j) {
      for (int k = 0; k < cols_; ++k) {
        result_matrix.matrix_[i][j] += matrix_[i][k] * other.matrix_[k][j];
      }
    }
  }
  *this = result_matrix;
  return *this;
}

void Matrix4X4::MulNumber(const double& num) noexcept {
  for (int i = 0; i < rows_; ++i) {
    for (int j = 0; j < cols_; ++j) {
      matrix_[i][j] *= num;
    }
  }
}

float** Matrix4X4::GetMatrix() const noexcept { return matrix_; }

void Matrix4X4::SetData(const TransformData& other) {
  data_.x_rotation_deg = other.x_rotation_deg;
  data_.y_rotation_deg = other.y_rotation_deg;
  data_.z_rotation_deg = other.z_rotation_deg;
  data_.x_shift = other.x_shift;
  data_.y_shift = other.y_shift;
  data_.z_shift = other.z_shift;
  data_.user_scaler = other.user_scaler;
  data_.model_scaler = other.model_scaler;
  data_.perspective_ortho = other.perspective_ortho;
}

float* Matrix4X4::CreateOneRowMatrix() noexcept {
  if (matrix_one_row_ == nullptr) matrix_one_row_ = new float[rows_ * cols_];
  for (int i = 0; i < rows_; ++i) {
    for (int j = 0; j < cols_; ++j) {
      matrix_one_row_[i * cols_ + j] = matrix_[i][j];
    }
  }
  return matrix_one_row_;
}

void Matrix4X4::MemoryAllocate() {
  matrix_ = new float*[rows_];
  for (int i = 0; i < rows_; ++i) {
    matrix_[i] = new float[cols_]{};
  }
}

void Matrix4X4::MemoryFree() {
  for (int i = 0; i < rows_; ++i) {
    delete[] matrix_[i];
  }
  delete[] matrix_;
}

void Matrix4X4::InitializeIdentityMatrix() {
  for (int i = 0; i < rows_; ++i) {
    for (int j = 0; j < cols_; ++j) {
      if (i == j) {
        matrix_[i][j] = 1;
      } else {
        matrix_[i][j] = 0;
      }
    }
  }
}
};  // namespace viewer
