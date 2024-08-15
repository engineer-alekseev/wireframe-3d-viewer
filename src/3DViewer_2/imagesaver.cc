#include "imagesaver.h"

namespace viewer {

void ImageSaver::SaveImage(QWidget* w) {
  QString filter = "*.bmp;; *.jpeg";
  image_file_name_ = "/0";
  image_file_name_ = QFileDialog::getSaveFileName(w, "Сохранить файл",
                                                  QDir::homePath(), filter);
  image_->grabFramebuffer().save(image_file_name_);
  if (image_file_name_.isNull() == false) {
    QMessageBox::about(w, "Статус", "Изображение сохранено");
  }
}

void ImageSaver::SaveGif(QWidget* w) {
  QString filter = "*.gif";
  image_file_name_ = "/0";
  image_file_name_ = QFileDialog::getSaveFileName(w, "Сохранить GIF",
                                                  QDir::homePath(), filter);

  ptr_save_file_ = new QFile(image_file_name_);
  if (!ptr_save_file_->open(QIODevice::WriteOnly)) {
    QMessageBox::critical(w, "Warning", "Не удалось записать файл");
  } else {
    StartTimerForGif(w);
  }
}

void ImageSaver::SetOGLImage(QOpenGLWidget* image) { image_ = image; }

void ImageSaver::StartTimerForGif(QWidget* w) {
  gif_ = new QGifImage(QSize(640, 480));
  timer_ = new QTimer(w);
  connect(timer_, &QTimer::timeout, this, &ImageSaver::RecordGif);
  timer_->start(100);
  frame_ = 0;
}

void ImageSaver::RecordGif() {
  frame_++;
  gif_->addFrame(image_->grabFramebuffer(), 100);
  if (frame_ == 50) {
    timer_->stop();
    gif_->save(ptr_save_file_);
    ptr_save_file_->close();
    delete ptr_save_file_;
    ptr_save_file_ = nullptr;
    delete gif_;
    gif_ = nullptr;
    QMessageBox::about(this, "Статус", "GIF сохранена");
  }
}

};  // namespace viewer
