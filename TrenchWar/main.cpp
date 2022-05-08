#include <QApplication>

#include "Controllers/main_controller.h"

#include <iostream>

int main(int argc, char* argv[]) {
  QApplication app(argc, argv);
  MainController controller;
  return QApplication::exec();
}
