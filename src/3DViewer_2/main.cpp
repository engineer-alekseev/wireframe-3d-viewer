#include <QApplication>

#include "mainwindow.h"

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);
  QApplication::setStyle("fusion");
  viewer::Controller controller;
  viewer::MainWindow w(&controller);
  w.show();
  return a.exec();
}
