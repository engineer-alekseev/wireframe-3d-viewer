#ifndef THREE_D_VIEWER_CONTROLLER_H_
#define THREE_D_VIEWER_CONTROLLER_H_

#include "imagesaver.h"
#include "parser/parser.h"

namespace viewer {
class Controller {
 public:
  Controller() = default;
  ~Controller() = default;

  viewer::ModelObj StartParsingFile(const std::string& filename) {
    viewer::ModelObj result;
    try {
      output_ = "";
      result = parser_.Parse(filename);
    } catch (std::exception& e) {
      output_ = e.what();
    }
    return result;
  }
  std::string GetOutput() const { return output_; }

  void SaveImage(QMainWindow* w, QOpenGLWidget* ogl) {
    saver_.SetOGLImage(ogl);
    saver_.SaveImage(w);
  }

  void SaveGif(QMainWindow* w, QOpenGLWidget* ogl) {
    saver_.SetOGLImage(ogl);
    saver_.SaveGif(w);
  }

 private:
  viewer::ObjParser parser_;
  std::string output_{};
  viewer::ImageSaver saver_;
};
};  // namespace viewer

#endif  // THREE_D_VIEWER_CONTROLLER_H_
