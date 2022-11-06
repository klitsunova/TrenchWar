#include "AirMovingObject.h"
AirMovingObject::AirMovingObject(Side side,
                                 const QPoint& from,
                                 const QPoint& to)
    : MovingObject(side, from), from_(from), to_(to) {}

void AirMovingObject::Move() {
  assert(!IsUsed());

  int64_t square_dx = (to_.x() - from_.x()) * (to_.x() - from_.x());
  int64_t square_dy = (to_.y() - from_.y()) * (to_.y() - from_.y());

  ++moving_progress_;

  int x, y;
  int pr_x = moving_progress_ *
      sqrt(square_dx / static_cast<double>(square_dx + square_dy));
  int pr_y = moving_progress_ *
      sqrt(square_dy / static_cast<double>(square_dx + square_dy));
  if (to_.x() > from_.x()) {
    x = from_.x() + pr_x;
  } else {
    x = from_.x() - pr_x;
  }
  if (to_.y() > from_.y()) {
    y = from_.y() + pr_y;
  } else {
    y = from_.y() - pr_y;
  }

  position_ = QPoint(x, y);
  if (to_ == position_) {
    is_used_ = true;
  }
}

bool AirMovingObject::IsUsed() const {
  return is_used_;
}
void AirMovingObject::MakeUsed() {
  is_used_ = true;
}
