#define GL_SILENCE_DEPRECATION
#include "oglwidget.h"

viewer::OGLWidget::OGLWidget(QWidget* parent)
    : QOpenGLWidget(parent), index_buf_(QOpenGLBuffer::IndexBuffer) {}

viewer::OGLWidget::~OGLWidget() { cleanUp(); }

void viewer::OGLWidget::cleanUp() {
  array_buf_.destroy();
  index_buf_.destroy();
}

void viewer::OGLWidget::initializeGL() {
  initializeOpenGLFunctions();
  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  glEnable(GL_PROGRAM_POINT_SIZE);
  initShaders();
  loadGeometry();
}

void viewer::OGLWidget::paintGL() {
  CalculateProjection();
  glClearColor(style_.bg_color.red() / 255., style_.bg_color.green() / 255.,
               style_.bg_color.blue() / 255., 1);
  glClear(GL_COLOR_BUFFER_BIT);

  affine_transformation_matrix_.MakeMovement(transformations_);

  QMatrix4x4 matrix(affine_transformation_matrix_.CreateOneRowMatrix());

  program.bind();
  program_P.bind();

  program.setUniformValue("mvp_matrix", projection_ * matrix);
  program.setUniformValue("line_color", style_.e_color.red() / 255.,
                          style_.e_color.green() / 255.,
                          style_.e_color.blue() / 255., 1.0);

  array_buf_.bind();
  index_buf_.bind();

  quintptr offset = 0;

  int vertexLocation = program.attributeLocation("a_position");
  program.enableAttributeArray(vertexLocation);
  program_P.enableAttributeArray(vertexLocation);

  program.setAttributeBuffer(vertexLocation, GL_FLOAT, offset, 3, 0);
  program_P.setAttributeBuffer(vertexLocation, GL_FLOAT, offset, 3, 0);

  glLineWidth(style_.e_size);
  if (style_.e_style == 2) {
    glLineStipple(4, 0x0C0F);
    glEnable(GL_LINE_STIPPLE);
  } else {
    glDisable(GL_LINE_STIPPLE);
  }

  if (style_.e_style != 0) {
    glDrawElements(GL_TRIANGLES, model_obj_.faces.size(), GL_UNSIGNED_INT,
                   nullptr);
  }

  float pointSize = (style_.v_size != 0) ? style_.v_size : 0;
  program_P.setUniformValue("point_size", pointSize);

  if (style_.v_style != 0) {
    if (style_.v_style == 1) {
      glEnable(GL_POINT_SMOOTH);
    } else {
      glDisable(GL_POINT_SMOOTH);
    }
    program_P.setUniformValue("mvp_matrix", projection_ * matrix);
    program_P.setUniformValue("dot_color", style_.v_color.red() / 255.,
                              style_.v_color.green() / 255.,
                              style_.v_color.blue() / 255., 1.0);
    glDrawElements(GL_POINTS, model_obj_.faces.size(), GL_UNSIGNED_INT,
                   nullptr);
  }

  program.release();
  program_P.release();
}

void viewer::OGLWidget::resizeGL(int w, int h) {
  aspect = w / float(h ? h : 1);
  CalculateProjection();
}

void viewer::OGLWidget::CalculateProjection() {
  projection_.setToIdentity();
  if (transformations_.perspective_ortho == false) {
    projection_.perspective(22.5, aspect, 0.1, 10.0);
  } else {
    double scaler = 1.;
    projection_.ortho(-scaler * aspect, scaler * aspect, -scaler, scaler, 0.1,
                      10.0);
  }
  // в вызове perspective:
  // 1 - угол наклона камеры
  // 2 - aspect ratio (соотношение сторон)
  // 3, 4 - передняя и задняя плоскости отсечения
  // в вызове ortho:
  // 1, 2 - левая и правая границы
  // 3, 4 - нижняя и верхняя границы
  // 5, 6 - передняя и задняя плоскости отсечения
}

void viewer::OGLWidget::initShaders() {
  // Compile vertex shader
  if (!program.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/vshader.glsl"))
    close();
  if (!program_P.addShaderFromSourceFile(QOpenGLShader::Vertex,
                                         ":/vshader.glsl"))
    // Compile geometry shader
    if (!program_P.addShaderFromSourceFile(QOpenGLShader::Geometry,
                                           ":/gshader.glsl"))
      close();
  // Compile fragment shader
  if (!program.addShaderFromSourceFile(QOpenGLShader::Fragment,
                                       ":/fshader.glsl"))
    close();
  // Compile second fragment shader for vertex color handling
  if (!program_P.addShaderFromSourceFile(QOpenGLShader::Fragment,
                                         ":/fshaderP.glsl"))
    close();
  // Link shader pipeline
  if (!program.link()) close();
  if (!program_P.link()) close();
  // Bind shader pipeline for use
  if (!program.bind()) close();
  if (!program_P.bind()) close();
  program.release();
  program_P.release();
}

void viewer::OGLWidget::loadGeometry() {
  transformations_.model_scaler = model_obj_.scaler;

  array_buf_.create();
  array_buf_.bind();

  float* p_to_data = model_obj_.vertices.data();

  array_buf_.allocate(p_to_data, model_obj_.vertices.size() * sizeof(float));
  array_buf_.release();
  index_buf_.create();
  index_buf_.bind();

  index_buf_.allocate(model_obj_.faces.data(),
                      model_obj_.faces.size() * sizeof(unsigned int));

  index_buf_.release();
}

void viewer::OGLWidget::setNewGeometry(ModelObj&& other) {
  model_obj_ = std::move(other);
  loadGeometry();
  update();
}

void viewer::OGLWidget::setWidgetState(ViewerSettings& uiState) {
  file_path_ = uiState.GetUiState().filePath;
  SetTransformations(uiState);
  SetStyle(uiState);
  update();
}

void viewer::OGLWidget::SetTransformations(ViewerSettings& uiState) {
  transformations_.x_rotation_deg = uiState.GetUiState().x_rotation_deg;
  transformations_.y_rotation_deg = uiState.GetUiState().y_rotation_deg;
  transformations_.z_rotation_deg = uiState.GetUiState().z_rotation_deg;
  transformations_.x_shift = (uiState.GetUiState().x_shift / 50.0) * aspect;
  transformations_.y_shift = uiState.GetUiState().y_shift / 50.0;
  transformations_.z_shift = uiState.GetUiState().z_shift / 50.0;
  transformations_.user_scaler = 1 + uiState.GetUiState().user_scaler / 101.0;
  transformations_.perspective_ortho = uiState.GetUiState().perspective;
};

void viewer::OGLWidget::SetStyle(ViewerSettings& uiState) {
  style_.bg_color = uiState.GetUiState().bg_color;
  style_.e_color = uiState.GetUiState().e_color;
  style_.v_color = uiState.GetUiState().v_color;
  style_.e_style = uiState.GetUiState().e_style;
  style_.e_size = uiState.GetUiState().e_size;
  style_.v_style = uiState.GetUiState().v_style;
  style_.v_size = uiState.GetUiState().v_size;
};

int viewer::OGLWidget::getNVerticies() { return model_obj_.vertices.size(); }

int viewer::OGLWidget::getNIndicies() { return model_obj_.faces.size(); }

std::string viewer::OGLWidget::getFilePath() {
  using namespace std;
  string model_file_path = "Model file: ";
  model_file_path += file_path_;
  return model_file_path;
}
