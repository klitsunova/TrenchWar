#include "soldier.h"

Soldier::health_t Soldier::GetHitPoints() const {
  return hit_points_;
}

QColor Soldier::GetColor() {
  return color_;
}

void Soldier::MoveSoldier() {
  //  TODO: soldier's movement along the route
  if (position_.x()) {
    MoveLeft();
  } else {
    SetRandomPosition();
  }
}
