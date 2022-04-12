#include "soldier.h"

Soldier::Soldier()
    : GameObject() {
  picture_ = PixmapLoader::GetSoldier();
}

Soldier::Soldier(const QPoint& point)
    : GameObject(point) {
  picture_ = PixmapLoader::GetSoldier();
}

Soldier::Health Soldier::GetHitPoints() const {
  return hit_points_;
}

void Soldier::MoveSoldier(QSize size) {
  // TODO(klitsunova): soldier's movement along the route
  if (position_.x() - kStep > 0) {
    MoveLeft();
    picture_ = PixmapLoader::GetLSoldier();
  } else {
    SetRandomPosition(size);
  }
}
