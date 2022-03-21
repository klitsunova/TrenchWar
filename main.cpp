#include <QApplication>
#include <QPushButton>
#include "Controllers/controller.h"


int main(int argc, char* argv[]) {
  QApplication app(argc, argv);
  Controller controller;
  controller.show();
  return QApplication::exec();
}
