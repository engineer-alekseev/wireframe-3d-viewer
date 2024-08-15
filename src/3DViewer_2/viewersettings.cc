#include "viewersettings.h"

#include <filesystem>
#include <fstream>
#include <iostream>

namespace viewer {
ViewerSettings::ViewerSettings() {}

void ViewerSettings::LoadConf(std::string& confPath) {
  using namespace std;
  using namespace filesystem;
  ifstream fileStream;
  fileStream.open(confPath);
  string line, key, value;
  int separatorIdx;
  map<string, function<void(const string&)>> key_value_pairs = {
      {"filePath", [&](const string& val) { conf_settings_.filePath = val; }},
      {"x_rotation_deg",
       [&](const string& val) { conf_settings_.x_rotation_deg = stoi(val); }},
      {"y_rotation_deg",
       [&](const std::string& val) {
         conf_settings_.y_rotation_deg = stoi(val);
       }},
      {"z_rotation_deg",
       [&](const string& val) { conf_settings_.z_rotation_deg = stoi(val); }},
      {"x_shift",
       [&](const string& val) { conf_settings_.x_shift = stoi(val); }},
      {"y_shift",
       [&](const string& val) { conf_settings_.y_shift = stoi(val); }},
      {"z_shift",
       [&](const string& val) { conf_settings_.z_shift = stoi(val); }},
      {"user_scaler",
       [&](const string& val) { conf_settings_.user_scaler = stoi(val); }},
      {"perspective",
       [&](const string& val) { conf_settings_.perspective = stoi(val); }},
      {"e_style",
       [&](const string& val) { conf_settings_.e_style = stoi(val); }},
      {"e_size", [&](const string& val) { conf_settings_.e_size = stoi(val); }},
      {"v_style",
       [&](const string& val) { conf_settings_.v_style = stoi(val); }},
      {"v_size", [&](const string& val) { conf_settings_.v_size = stoi(val); }},
      {"n_verticies",
       [&](const string& val) { conf_settings_.n_verticies = stoi(val); }},
      {"n_indices",
       [&](const string& val) { conf_settings_.n_indices = stoi(val); }},
      {"bg_color_red",
       [&](const string& val) { conf_settings_.bg_color.setRed(stoi(value)); }},
      {"bg_color_green",
       [&](const string& val) {
         conf_settings_.bg_color.setGreen(stoi(value));
       }},
      {"bg_color_blue",
       [&](const string& val) {
         conf_settings_.bg_color.setBlue(stoi(value));
       }},
      {"v_color_red",
       [&](const string& val) { conf_settings_.v_color.setRed(stoi(value)); }},
      {"v_color_green",
       [&](const string& val) {
         conf_settings_.v_color.setGreen(stoi(value));
       }},
      {"v_color_blue",
       [&](const string& val) { conf_settings_.v_color.setBlue(stoi(value)); }},
      {"e_color_red",
       [&](const string& val) { conf_settings_.e_color.setRed(stoi(value)); }},
      {"e_color_green",
       [&](const string& val) {
         conf_settings_.e_color.setGreen(stoi(value));
       }},
      {"e_color_blue", [&](const string& val) {
         conf_settings_.e_color.setBlue(stoi(value));
       }}};
  while (getline(fileStream, line)) {
    separatorIdx = line.find("=");
    key = line.substr(0, separatorIdx);
    value = line.substr(separatorIdx + 1, line.length());
    key_value_pairs.find(key)->second(value);
  }
  fileStream.close();
}

void ViewerSettings::SaveConf(std::string& basePath, ViewerSettings& uiState) {
  using namespace std;
  using namespace filesystem;
  conf_settings_ = uiState.conf_settings_;
  string confPath = basePath + "/session.conf";
  ofstream fileStream;
  fileStream.open(confPath);
  fileStream << "filePath=" << conf_settings_.filePath << endl;
  fileStream << "n_verticies=" << conf_settings_.n_verticies << endl;
  fileStream << "n_indices=" << conf_settings_.n_indices << endl;
  fileStream << "x_rotation_deg=" << conf_settings_.x_rotation_deg << endl;
  fileStream << "y_rotation_deg=" << conf_settings_.y_rotation_deg << endl;
  fileStream << "z_rotation_deg=" << conf_settings_.z_rotation_deg << endl;
  fileStream << "x_shift=" << conf_settings_.x_shift << endl;
  fileStream << "y_shift=" << conf_settings_.y_shift << endl;
  fileStream << "z_shift=" << conf_settings_.z_shift << endl;
  fileStream << "user_scaler=" << conf_settings_.user_scaler << endl;
  fileStream << "perspective=" << conf_settings_.perspective << endl;
  fileStream << "bg_color_red=" << conf_settings_.bg_color.red() << endl;
  fileStream << "bg_color_green=" << conf_settings_.bg_color.green() << endl;
  fileStream << "bg_color_blue=" << conf_settings_.bg_color.blue() << endl;
  fileStream << "v_color_red=" << conf_settings_.v_color.red() << endl;
  fileStream << "v_color_green=" << conf_settings_.v_color.green() << endl;
  fileStream << "v_color_blue=" << conf_settings_.v_color.blue() << endl;
  fileStream << "e_color_red=" << conf_settings_.e_color.red() << endl;
  fileStream << "e_color_green=" << conf_settings_.e_color.green() << endl;
  fileStream << "e_color_blue=" << conf_settings_.e_color.blue() << endl;
  fileStream << "e_style=" << conf_settings_.e_style << endl;
  fileStream << "e_size=" << conf_settings_.e_size << endl;
  fileStream << "v_style=" << conf_settings_.v_style << endl;
  fileStream << "v_size=" << conf_settings_.v_size << endl;
  fileStream.close();
}

void ViewerSettings::UpdateState(int value, const std::string& value_type) {
  using namespace std;
  map<string, function<void(const int&)>> conf_state = {
      {"x_rotation_deg",
       [&](const int& val) { conf_settings_.x_rotation_deg = val; }},
      {"y_rotation_deg",
       [&](const int& val) { conf_settings_.y_rotation_deg = val; }},
      {"z_rotation_deg",
       [&](const int& val) { conf_settings_.z_rotation_deg = val; }},
      {"x_shift", [&](const int& val) { conf_settings_.x_shift = val; }},
      {"y_shift", [&](const int& val) { conf_settings_.y_shift = val; }},
      {"z_shift", [&](const int& val) { conf_settings_.z_shift = val; }},
      {"user_scaler",
       [&](const int& val) { conf_settings_.user_scaler = val; }},
      {"perspective",
       [&](const int& val) { conf_settings_.perspective = val; }},
  };
  conf_state.find(value_type)->second(value);
}

void ViewerSettings::SetDefaultTransforms() {
  conf_settings_.x_rotation_deg = 0;
  conf_settings_.y_rotation_deg = 0;
  conf_settings_.z_rotation_deg = 0;
  conf_settings_.x_shift = 0;
  conf_settings_.y_shift = 0;
  conf_settings_.z_shift = 0;
  conf_settings_.user_scaler = 1;
  conf_settings_.perspective = false;
}

void ViewerSettings::SetDefaultStyle() {
  conf_settings_.e_style = 1;
  conf_settings_.e_size = 1;
  conf_settings_.v_style = 0;
  conf_settings_.v_size = 0;
  conf_settings_.bg_color = QColor(25, 39, 52);
  conf_settings_.e_color = QColor(136, 153, 166);
  conf_settings_.v_color = QColor(255, 255, 255);
}

void ViewerSettings::SetPath(std::string& path) {
  conf_settings_.filePath = path;
}

};  // namespace viewer
