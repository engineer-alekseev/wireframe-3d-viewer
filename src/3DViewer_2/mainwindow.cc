#include "mainwindow.h"

#include "./ui_mainwindow.h"

viewer::MainWindow::MainWindow(viewer::Controller* controller, QWidget* parent)
    : controller_(controller), QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);
  FindOutBasePath();
  SetState(viewer_conf_);
  ShowFileInfo();
  MinimizeUi();
  connect(ui->group_colors, &QButtonGroup::buttonClicked, this,
          &MainWindow::colorSettings);
  connect(ui->userScalerSlider, &QSlider::valueChanged, this,
          &MainWindow::changeSliders);
  connect(ui->xRotationSlider, &QSlider::valueChanged, this,
          &MainWindow::changeSliders);
  connect(ui->yRotationSlider, &QSlider::valueChanged, this,
          &MainWindow::changeSliders);
  connect(ui->zRotationSlider, &QSlider::valueChanged, this,
          &MainWindow::changeSliders);
  connect(ui->xShiftSlider, &QSlider::valueChanged, this,
          &MainWindow::changeSliders);
  connect(ui->yShiftSlider, &QSlider::valueChanged, this,
          &MainWindow::changeSliders);
  connect(ui->zShiftSlider, &QSlider::valueChanged, this,
          &MainWindow::changeSliders);
  connect(ui->userScalerSpinBox, &QDoubleSpinBox::valueChanged, this,
          &MainWindow::changeSpinBoxes);
  connect(ui->xRotationSpinBox, &QDoubleSpinBox::valueChanged, this,
          &MainWindow::changeSpinBoxes);
  connect(ui->yRotationSpinBox, &QDoubleSpinBox::valueChanged, this,
          &MainWindow::changeSpinBoxes);
  connect(ui->zRotationSpinBox, &QDoubleSpinBox::valueChanged, this,
          &MainWindow::changeSpinBoxes);
  connect(ui->xShiftSpinBox, &QDoubleSpinBox::valueChanged, this,
          &MainWindow::changeSpinBoxes);
  connect(ui->yShiftSpinBox, &QDoubleSpinBox::valueChanged, this,
          &MainWindow::changeSpinBoxes);
  connect(ui->zShiftSpinBox, &QDoubleSpinBox::valueChanged, this,
          &MainWindow::changeSpinBoxes);
  connect(ui->edgesSizeSpinBox, &QDoubleSpinBox::valueChanged, this,
          &MainWindow::EdgesAndVerticiesSizeChange);
  connect(ui->edgesSizeSlider, &QSlider::valueChanged, this,
          &MainWindow::EdgesAndVerticiesSizeChange);
  connect(ui->vertexSizeSpinBox, &QDoubleSpinBox::valueChanged, this,
          &MainWindow::EdgesAndVerticiesSizeChange);
  connect(ui->verticiesSizeSlider, &QSlider::valueChanged, this,
          &MainWindow::EdgesAndVerticiesSizeChange);
}

viewer::MainWindow::~MainWindow() {
  viewer_conf_.SaveConf(base_path_, viewer_conf_);
  delete ui;
}

void viewer::MainWindow::FindOutBasePath() {
  using namespace std;
  using namespace filesystem;
  base_path_ = QCoreApplication::applicationDirPath().toStdString();
}

void viewer::MainWindow::SetState(ViewerSettings& uiState) {
  using namespace std;
  using namespace filesystem;
  string confPath = base_path_ + "/session.conf";
  ifstream fileStream;
  fileStream.open(confPath);
  if (fileStream.good()) {
    fileStream.close();
    viewer_conf_.LoadConf(confPath);
  } else {
    SetDefaults();
    viewer_conf_.LoadConf(confPath);
    string file_path = base_path_ + "/logo.obj";
    viewer_conf_.SetPath(file_path);
  }
  SyncUi();
  ui->widget->setWidgetState(uiState);
  ui->widget->setNewGeometry(
      controller_->StartParsingFile(viewer_conf_.GetUiState().filePath));
  if (controller_->GetOutput() != "") {
    QMessageBox::critical(this, "Error",
                          QString::fromStdString(controller_->GetOutput()));
  }
}

void viewer::MainWindow::SetDefaults() {
  QString defaultConfPath =
      QString::fromStdString(base_path_) + "/session.conf";
  QString defaultModelPath = QString::fromStdString(base_path_) + "/logo.obj";
  QFile::copy(":/defaults/session.conf", defaultConfPath);
  QFile::setPermissions(defaultConfPath,
                        QFileDevice::ReadOwner | QFileDevice::WriteOwner);
  QFile::copy(":/defaults/logo.obj", defaultModelPath);
  QFile::setPermissions(defaultModelPath,
                        QFileDevice::ReadOwner | QFileDevice::WriteOwner);
}

void viewer::MainWindow::UpdateUiState(int value, std::string& value_type) {
  viewer_conf_.UpdateState(value, value_type);
  ui->widget->setWidgetState(viewer_conf_);
  SyncUi();
}

void viewer::MainWindow::SyncUi() {
  ui->xRotationSlider->setValue(viewer_conf_.GetUiState().x_rotation_deg);
  ui->xRotationSpinBox->setValue(viewer_conf_.GetUiState().x_rotation_deg);
  ui->yRotationSlider->setValue(viewer_conf_.GetUiState().y_rotation_deg);
  ui->yRotationSpinBox->setValue(viewer_conf_.GetUiState().y_rotation_deg);
  ui->zRotationSlider->setValue(viewer_conf_.GetUiState().z_rotation_deg);
  ui->zRotationSpinBox->setValue(viewer_conf_.GetUiState().z_rotation_deg);
  ui->xShiftSlider->setValue(viewer_conf_.GetUiState().x_shift);
  ui->xShiftSpinBox->setValue(viewer_conf_.GetUiState().x_shift);
  ui->yShiftSlider->setValue(viewer_conf_.GetUiState().y_shift);
  ui->yShiftSpinBox->setValue(viewer_conf_.GetUiState().y_shift);
  ui->zShiftSlider->setValue(viewer_conf_.GetUiState().z_shift);
  ui->zShiftSpinBox->setValue(viewer_conf_.GetUiState().z_shift);
  ui->userScalerSlider->setValue(viewer_conf_.GetUiState().user_scaler);
  ui->userScalerSpinBox->setValue(viewer_conf_.GetUiState().user_scaler);
  ui->persperctiveComboBox->setCurrentIndex(
      viewer_conf_.GetUiState().perspective);
  ui->verticiesTypeComboBox->setCurrentIndex(viewer_conf_.GetUiState().v_style);
  ui->verticiesSizeSlider->setValue(viewer_conf_.GetUiState().v_size);
  ui->vertexSizeSpinBox->setValue(viewer_conf_.GetUiState().v_size);
  ui->edgesTypeComboBox->setCurrentIndex(viewer_conf_.GetUiState().e_style);
  ui->edgesSizeSlider->setValue(viewer_conf_.GetUiState().e_size);
  ui->edgesSizeSpinBox->setValue(viewer_conf_.GetUiState().e_size);
}

void viewer::MainWindow::on_uiShowButton_clicked() {
  if (ui->uiFrame->size().height() == FRAME_H_MIN) {
    MaximizeUi();
  } else {
    MinimizeUi();
  }
}

void viewer::MainWindow::on_uiCloseButton_clicked() {
  ui->uiFrame->setVisible(false);
  ui->actionShowUi->setChecked(false);
}

void viewer::MainWindow::on_showTransformsButton_clicked() {
  int currentUiH = ui->uiFrame->size().height();
  if (ui->uiTransformsFrame->size().height() == FRAME_H_MIN) {
    MaximizeUiTransforms();
    ui->uiFrame->setFixedSize(FRAME_W, currentUiH + FRAME_H_TR_CTRLS);
  } else {
    MinimizeUiTransforms();
    ui->uiFrame->setFixedSize(FRAME_W, currentUiH - FRAME_H_TR_CTRLS);
  }
}

void viewer::MainWindow::on_showStyleButton_clicked() {
  int currentUiH = ui->uiFrame->size().height();
  if (ui->uiStyleFrame->size().height() == FRAME_H_MIN) {
    MaximizeUiStyle();
    ui->uiFrame->setFixedSize(FRAME_W, currentUiH + FRAME_H_ST_CTRLS);
  } else {
    MinimizeUiStyle();
    ui->uiFrame->setFixedSize(FRAME_W, currentUiH - FRAME_H_ST_CTRLS);
  }
}

void viewer::MainWindow::MinimizeUi() {
  ui->uiFrame->setFixedSize(FRAME_W, FRAME_H_MIN);
  ui->uiTransformsFrame->setVisible(false);
  ui->uiStyleFrame->setVisible(false);
}

void viewer::MainWindow::MaximizeUi() {
  ui->uiFrame->setFixedSize(FRAME_W, FRAME_H_MIN * 3);
  MinimizeUiTransforms();
  ui->uiTransformsFrame->setVisible(true);
  MinimizeUiStyle();
  ui->uiStyleFrame->setVisible(true);
}

void viewer::MainWindow::MinimizeUiTransforms() {
  ui->uiTransformsFrame->setFixedSize(FRAME_W, FRAME_H_MIN);
  ui->uiTransformsCtrlFrame->setVisible(false);
}

void viewer::MainWindow::MaximizeUiTransforms() {
  ui->uiTransformsFrame->setFixedSize(FRAME_W, FRAME_H_MIN + FRAME_H_TR_CTRLS);
  ui->uiTransformsCtrlFrame->setVisible(true);
}

void viewer::MainWindow::MinimizeUiStyle() {
  ui->uiStyleFrame->setFixedSize(FRAME_W, FRAME_H_MIN);
  ui->uiStyleCtrlFrame->setVisible(false);
}

void viewer::MainWindow::MaximizeUiStyle() {
  ui->uiStyleFrame->setFixedSize(FRAME_W, FRAME_H_MIN + FRAME_H_ST_CTRLS);
  ui->uiStyleCtrlFrame->setVisible(true);
}

void viewer::MainWindow::ShowFileInfo() {
  using namespace std;
  ui->widget->getNIndicies();
  ui->widget->getNVerticies();
  string fileInfo = ui->widget->getFilePath();
  fileInfo += " N indicies ";
  fileInfo += to_string(viewer_conf_.GetUiState().n_indices);
  fileInfo += " N edges ";
  fileInfo += to_string((viewer_conf_.GetUiState().n_indices / 3) +
                        viewer_conf_.GetUiState().n_verticies - 2);
  QString message = QString::fromStdString(fileInfo);
  ui->statusBar->showMessage(message);
}

void viewer::MainWindow::on_actionShowUi_triggered() {
  if (ui->uiFrame->isVisible()) {
    ui->uiFrame->setVisible(false);
  } else {
    ui->uiFrame->setVisible(true);
  }
}

void viewer::MainWindow::changeSliders(int value) {
  std::string src;
  using namespace std::string_literals;
  if (ui->userScalerSlider->hasFocus()) {
    src = "user_scaler";
    UpdateUiState(value, src);
  }
  if (ui->xRotationSlider->hasFocus()) {
    src = "x_rotation_deg";
    UpdateUiState(value, src);
  }
  if (ui->yRotationSlider->hasFocus()) {
    src = "y_rotation_deg";
    UpdateUiState(value, src);
  }
  if (ui->zRotationSlider->hasFocus()) {
    src = "z_rotation_deg"s;
    UpdateUiState(value, src);
  }
  if (ui->xShiftSlider->hasFocus()) {
    src = "x_shift";
    UpdateUiState(value, src);
  }
  if (ui->yShiftSlider->hasFocus()) {
    src = "y_shift";
    UpdateUiState(value, src);
  }
  if (ui->zShiftSlider->hasFocus()) {
    src = "z_shift";
    UpdateUiState(value, src);
  }
}

void viewer::MainWindow::changeSpinBoxes(double arg) {
  std::string src;
  if (ui->userScalerSpinBox->hasFocus()) {
    src = "user_scaler";
    UpdateUiState(arg, src);
  }
  if (ui->xRotationSpinBox->hasFocus()) {
    src = "x_rotation_deg";
    UpdateUiState(arg, src);
  }
  if (ui->yRotationSpinBox->hasFocus()) {
    src = "y_rotation_deg";
    UpdateUiState(arg, src);
  }
  if (ui->zRotationSpinBox->hasFocus()) {
    src = "z_rotation_deg";
    UpdateUiState(arg, src);
  }
  if (ui->xShiftSpinBox->hasFocus()) {
    src = "x_shift";
    UpdateUiState(arg, src);
  }
  if (ui->yShiftSpinBox->hasFocus()) {
    src = "y_shift";
    UpdateUiState(arg, src);
  }
  if (ui->zShiftSpinBox->hasFocus()) {
    src = "z_shift";
    UpdateUiState(arg, src);
  }
}

void viewer::MainWindow::on_setDefaultTransformsButton_clicked() {
  viewer_conf_.SetDefaultTransforms();
  SyncUi();
  ui->widget->setWidgetState(viewer_conf_);
}

void viewer::MainWindow::on_persperctiveComboBox_activated(int index) {
  std::string valueType = "perspective";
  UpdateUiState(index, valueType);
}

void viewer::MainWindow::on_actionOpen_File_triggered() {
  using namespace std::string_literals;
  QString filter = "Obj File (*.obj)";
  QString inFileName = QFileDialog::getOpenFileName(
      this, "Open file", QString::fromStdString(base_path_), filter);
  if (inFileName.isNull() == false) {
    viewer_conf_.GetUiState().filePath = inFileName.toStdString();
    SyncUi();
    ui->widget->setWidgetState(viewer_conf_);
    ui->widget->setNewGeometry(
        controller_->StartParsingFile(viewer_conf_.GetUiState().filePath));
    if (controller_->GetOutput() != "") {
      QMessageBox::critical(this, "Error",
                            QString::fromStdString(controller_->GetOutput()));
    }
    viewer_conf_.GetUiState().n_indices = ui->widget->getNIndicies();
    viewer_conf_.GetUiState().n_verticies = ui->widget->getNVerticies();
    ShowFileInfo();
  }
}

void viewer::MainWindow::colorSettings(QAbstractButton* button) {
  QColor color = QColorDialog::getColor(Qt::white, this, "Choose color");
  if (color.isValid()) {
    std::map<std::string, std::function<void(QColor&)>> set_colors = {
        {"verticies",
         [&](QColor& colour) { viewer_conf_.GetUiState().v_color = colour; }},
        {"edges",
         [&](QColor& colour) { viewer_conf_.GetUiState().e_color = colour; }},
        {"BG",
         [&](QColor& colour) { viewer_conf_.GetUiState().bg_color = colour; }}};
    set_colors.find(button->text().toStdString())->second(color);
  }
  ui->widget->setWidgetState(viewer_conf_);
}

void viewer::MainWindow::on_verticiesTypeComboBox_activated(int index) {
  viewer_conf_.GetUiState().v_style = index;
  ui->widget->setWidgetState(viewer_conf_);
}

void viewer::MainWindow::on_edgesTypeComboBox_activated(int index) {
  viewer_conf_.GetUiState().e_style = index;
  ui->widget->setWidgetState(viewer_conf_);
}

void viewer::MainWindow::EdgesAndVerticiesSizeChange(double value) {
  if (ui->edgesSizeSpinBox->hasFocus() || ui->edgesSizeSlider->hasFocus())
    viewer_conf_.GetUiState().e_size = value;
  if (ui->vertexSizeSpinBox->hasFocus() || ui->verticiesSizeSlider->hasFocus())
    viewer_conf_.GetUiState().v_size = value;
  ui->widget->setWidgetState(viewer_conf_);
  SyncUi();
}

void viewer::MainWindow::on_setDefaultStyleButton_clicked() {
  viewer_conf_.SetDefaultStyle();
  ui->widget->setWidgetState(viewer_conf_);
  SyncUi();
}

void viewer::MainWindow::on_Save_image_triggered() {
  controller_->SaveImage(this, ui->widget);
}

void viewer::MainWindow::on_Save_gif_triggered() {
  controller_->SaveGif(this, ui->widget);
}
