#include "soldier.h"

Soldier::Soldier() : GameObject() {
  picture_ = QPixmap(":Resources/Images/Soldier1.png");
}

Soldier::Soldier(const QPoint& point)
    : GameObject(point) {
  picture_ = QPixmap(":Resources/Images/Soldier1.png");
}

Soldier::health_t Soldier::GetHitPoints() const {
  return hit_points_;
}

void Soldier::MoveSoldier(QSize size) {
  // TODO(klitsunova): soldier's movement along the route
  if (position_.x() - step_ > 0) {
    MoveLeft();
  } else {
    SetRandomPosition(size);
  }
}