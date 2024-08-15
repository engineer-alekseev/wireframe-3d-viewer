#ifndef THREE_D_VIEWER_IMAGESAVER_H
#define THREE_D_VIEWER_IMAGESAVER_H

#include <QBuffer>
#include <QFileDialog>
#include <QMainWindow>
#include <QMessageBox>
#include <QOpenGLWidget>
#include <QTimer>

#include "gif_image/gifimage/qgifimage.h"

namespace viewer {
class ImageSaver : public QMainWindow {
  Q_OBJECT
 public:
  ImageSaver() = default;
  ~ImageSaver() = default;

  void SaveImage(QWidget* w);
  void SaveGif(QWidget* w);
  void SetOGLImage(QOpenGLWidget* image);

 private slots:
  void RecordGif();

 private:
  void StartTimerForGif(QWidget* w);
  QOpenGLWidget* image_;
  QString image_file_name_;
  QTimer* timer_;
  int frame_;
  QGifImage* gif_;
  QFile* ptr_save_file_;
};
};  // namespace viewer

#endif  // THREE_D_VIEWER_IMAGESAVER_H
