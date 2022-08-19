#include "random_generator.h"

namespace utils {
QPoint RandomGenerator::GetRandomPoint(const QRect& field) {
  std::random_device rd;
  std::uniform_int_distribution<int>
      width_distribution(field.left(), field.right());
  std::uniform_int_distribution<int>
      height_distribution(field.top(), field.bottom());
  return {width_distribution(rd), height_distribution(rd)};
}
}  // namespace utils
