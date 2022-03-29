#include "Controllers/controller.h"

#include <QApplication>
#include <QPushButton>

int main(int argc, char* argv[]) {
  QApplication app(argc, argv);
  Controller controller;
  controller.show();
  return QApplication::exec();
}
