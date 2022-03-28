#include "soldier.h"

int Soldier::GetHitPoints() {
  return hit_points_;
}

QColor Soldier::GetColor() {
  return color_;
}

void Soldier::MoveSoldier() {
  if (position_.x()) {
    MoveLeft();
  } else {
    SetRandomPosition();
  }
}
