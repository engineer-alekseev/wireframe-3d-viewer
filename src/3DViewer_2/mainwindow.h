#ifndef THREE_D_VIEWER_MAINWINDOW_H
#define THREE_D_VIEWER_MAINWINDOW_H

#include <QAbstractButton>
#include <QColor>
#include <QColorDialog>
#include <QFileDialog>
#include <QMainWindow>

#include "affine_transformations/matrix4X4.h"
#include "imagesaver.h"
#include "parser/parser.h"
#include "controller.h"
#include "viewersettings.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

namespace viewer {
constexpr int FRAME_W = 256;
constexpr int FRAME_H_MIN = 30;
constexpr int FRAME_H_TR_CTRLS = 284;
constexpr int FRAME_H_ST_CTRLS = 284;

class MainWindow : public QMainWindow {
  Q_OBJECT
 public:
  MainWindow(viewer::Controller* controller, QWidget* parent = nullptr);
  ~MainWindow();

 private slots:
  void on_uiShowButton_clicked();
  void on_showTransformsButton_clicked();
  void on_showStyleButton_clicked();
  void on_uiCloseButton_clicked();
  void on_actionShowUi_triggered();
  void changeSliders(int value);
  void changeSpinBoxes(double arg);
  void on_setDefaultTransformsButton_clicked();
  void on_actionOpen_File_triggered();

  void on_persperctiveComboBox_activated(int index);
  void on_verticiesTypeComboBox_activated(int index);
  void on_edgesTypeComboBox_activated(int index);

  void EdgesAndVerticiesSizeChange(double value);
  void on_setDefaultStyleButton_clicked();
  void colorSettings(QAbstractButton* button);
  void on_Save_image_triggered();
  void on_Save_gif_triggered();

 private:
  void FindOutBasePath();
  void SetState(ViewerSettings& uiState);
  void SetDefaults();
  void UpdateUiState(int value, std::string& value_type);
  void SyncUi();
  void MinimizeUi();
  void MaximizeUi();
  void MinimizeUiTransforms();
  void MaximizeUiTransforms();
  void MinimizeUiStyle();
  void MaximizeUiStyle();
  void ShowFileInfo();
  Ui::MainWindow* ui;
  viewer::Controller* controller_;
  std::string base_path_;
  ViewerSettings viewer_conf_;
  viewer::ImageSaver image_saver_;
};
};      // namespace viewer
#endif  // THREE_D_VIEWER_MAINWINDOW_H
