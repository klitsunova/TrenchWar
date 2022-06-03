#include "tower.h"
#include "helpers/weapons.h"

Tower::Tower()
    : GameObject() {
  picture_ = PixmapLoader::GetTower();
  size_ = kStandardSize * 1.5;
}

Tower::Tower(const QPoint& point)
    : GameObject(point) {
  picture_ = PixmapLoader::GetTower();
}

void Tower::TakeDamage(int multiply) {
  health -= weapons::kTowerDamage * multiply;
  if (health < 0) {
    health = 0;
  }
}

bool Tower::IsDestroyed() const {
  return health == 0;
}
