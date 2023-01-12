#include "MathHelper.h"

int64_t MathHelper::SquareDistance(const QPoint& from, const QPoint& to) {
  return static_cast<int64_t>(from.x() - to.x()) * (from.x() - to.x())
      + static_cast<int64_t>(from.y() - to.y()) * (from.y() - to.y());
}
