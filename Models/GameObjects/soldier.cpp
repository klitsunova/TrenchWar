#include "soldier.h"

Soldier::Soldier(const std::shared_ptr<PixmapLoader>& pixmap_loader)
// : GameObject(":Resources/Images/Soldier1.png") {
    : GameObject(pixmap_loader) {
  picture_ = pixmap_loader->GetSoldier();
}

Soldier::Soldier(const QPoint& point,
                 const std::shared_ptr<PixmapLoader>& pixmap_loader)
// : GameObject(point, ":Resources/Images/Soldier1.png") {
    : GameObject(point, pixmap_loader) {
  picture_ = pixmap_loader->GetSoldier();
}

Soldier::Health Soldier::GetHitPoints() const {
  return hit_points_;
}

void Soldier::MoveSoldier(QSize size) {
  // TODO(klitsunova): soldier's movement along the route
  if (position_.x() - kStep > 0) {
    MoveLeft();
  } else {
    SetRandomPosition(size);
  }
}
