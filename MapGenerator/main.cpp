#include <iostream>

#include <QApplication>
#include <QPainter>

#include "map_generator.h"

int main(int argc, char* argv[]) {
  QApplication app(argc, argv);
  MapGenerator generator;
  generator.show();
  generator.resize(900, 500);
  return QApplication::exec();
}
