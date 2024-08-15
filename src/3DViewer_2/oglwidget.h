#ifndef THREE_D_VIEWER_OGLWIDGET_H
#define THREE_D_VIEWER_OGLWIDGET_H

// QT includes
#include <QMatrix4x4>
#include <QOpenGLWidget>
// OpenGL imports
#include <QOpenGLBuffer>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>
// QT imports
#include <QColor>
#include <QDebug>
#include <QFile>
#include <QMessageBox>
#include <filesystem>
#include <fstream>
#include <iostream>

#include "mainwindow.h"

namespace viewer {
struct StyleData {
  int v_style;
  int v_size;
  int e_style;
  int e_size;
  QColor v_color;
  QColor e_color;
  QColor bg_color;
};

typedef enum { PERSPECTIVE, ORTHO } projection_t;

constexpr double DEFAULT_ASPECT_RATIO = 1.865889;

class OGLWidget : public QOpenGLWidget, protected QOpenGLFunctions {
  Q_OBJECT

 public:
  OGLWidget(QWidget* parent = nullptr);
  ~OGLWidget();
  void setWidgetState(ViewerSettings& uiState);
  void setNewGeometry(ModelObj&& other);
  int getNVerticies();
  int getNIndicies();
  std::string getFilePath();

 protected:
  void initializeGL() override;
  void paintGL() override;
  void resizeGL(int width, int height) override;
  void initShaders();
  void loadGeometry();
  void cleanUp();

 private:
  void CalculateProjection();
  void SetTransformations(ViewerSettings& uiState);
  void SetStyle(ViewerSettings& uiState);
  std::string file_path_;
  float aspect = DEFAULT_ASPECT_RATIO;
  ModelObj model_obj_;
  TransformData transformations_;
  StyleData style_;
  QOpenGLShaderProgram program;
  QOpenGLShaderProgram program_P;
  QOpenGLShaderProgram program_dash;
  QMatrix4x4 projection_;
  Matrix4X4 affine_transformation_matrix_{};
  QOpenGLBuffer array_buf_;
  QOpenGLBuffer index_buf_;
};
};      // namespace viewer
#endif  // THREE_D_VIEWER_OGLWIDGET_H
