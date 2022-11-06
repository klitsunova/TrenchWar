#include "MovingObject.h"

MovingObject::MovingObject(Side side, const QPoint& position)
    : GameObject(side, position) {}

int MovingObject::GetTimeLag() const {
  return time_lag_;
}

void MovingObject::AddTimeLag(int lag) {
  assert(lag >= 0);
  time_lag_ += lag;
}

void MovingObject::MakeTick() {
  time_lag_ = std::max(0, time_lag_ - 1);
}
