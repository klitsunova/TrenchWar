#include "helpers/weapons.h"
#include "tower.h"

Tower::Tower()
    : GameObject() {
  picture_ = PixmapLoader::GetTower();
}

Tower::Tower(const QPoint& point)
    : GameObject(point) {
  picture_ = PixmapLoader::GetTower();
}

void Tower::TakeDamage() {
  health -= weapons::kTowerDamage;
  if (health < 0) {
    health = 0;
  }
}

bool Tower::IsDestroyed() const {
  return health == 0;
}
