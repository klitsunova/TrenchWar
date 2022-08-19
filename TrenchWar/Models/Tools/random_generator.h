#pragma once

#include <QPoint>
#include <QRect>

#include <random>

class RandomGenerator {
 public:
  RandomGenerator() = delete;

  static QPoint GetRandomPoint(const QRect&);
};
