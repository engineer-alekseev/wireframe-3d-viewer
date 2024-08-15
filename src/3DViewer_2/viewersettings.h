#ifndef THREE_D_VIEWER_VIEWERSETTINGS_H
#define THREE_D_VIEWER_VIEWERSETTINGS_H

#include <QColor>
#include <QColorDialog>
#include <functional>
#include <map>
#include <string>

#include "../affine_transformations/matrix4X4.h"
#include "QtWidgets/qapplication.h"

namespace viewer {
struct UiState {
  UiState()
      : filePath(""),
        n_verticies(0),
        n_indices(0),
        x_rotation_deg(0),
        y_rotation_deg(0),
        z_rotation_deg(0),
        x_shift(0),
        y_shift(0),
        z_shift(0),
        user_scaler(1),
        perspective(false),
        v_style(0),
        v_size(0),
        e_style(1),
        e_size(0),
        v_color(QColor(255, 255, 255)),
        e_color(QColor(136, 153, 166)),
        bg_color(QColor(25, 39, 52)) {}
  UiState& operator=(const UiState& other) {
    filePath = other.filePath;
    n_verticies = other.n_verticies;
    n_indices = other.n_indices;
    x_rotation_deg = other.x_rotation_deg;
    y_rotation_deg = other.y_rotation_deg;
    z_rotation_deg = other.z_rotation_deg;
    x_shift = other.x_shift;
    y_shift = other.y_shift;
    z_shift = other.z_shift;
    user_scaler = other.user_scaler;
    perspective = other.perspective;
    v_style = other.v_style;
    v_size = other.v_size;
    e_style = other.e_style;
    e_size = other.e_size;
    v_color = other.v_color;
    e_color = other.e_color;
    bg_color = other.bg_color;
    return *this;
  }
  std::string filePath;
  int n_verticies;
  int n_indices;
  int x_rotation_deg;
  int y_rotation_deg;
  int z_rotation_deg;
  int x_shift;
  int y_shift;
  int z_shift;
  int user_scaler;
  bool perspective;
  int v_style;
  int v_size;
  int e_style;
  int e_size;
  QColor v_color;
  QColor e_color;
  QColor bg_color;
};

class ViewerSettings {
 public:
  ViewerSettings();
  void SaveConf(std::string& basePath, ViewerSettings& uiState);
  void LoadConf(std::string& confPath);
  void UpdateState(int value, const std::string& value_type);
  void SetDefaultTransforms();
  void SetDefaultStyle();
  void SetPath(std::string& path);
  ViewerSettings& operator=(ViewerSettings& other) {
    conf_settings_ = other.conf_settings_;
    return *this;
  }
  UiState& GetUiState() { return conf_settings_; }

 private:
  viewer::TransformData GetTransformations(viewer::ViewerSettings& s);
  UiState conf_settings_{};
};

};      // namespace viewer
#endif  // THREE_D_VIEWER_VIEWERSETTINGS_H
