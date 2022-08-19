#pragma once

#include <QPoint>
#include <QRect>

#include <random>

namespace utils {

class RandomGenerator {
 public:
  RandomGenerator() = delete;

  static QPoint GetRandomPoint(const QRect&);
};

}  // namespace utils
