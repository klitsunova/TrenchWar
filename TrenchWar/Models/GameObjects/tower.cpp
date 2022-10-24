#include "tower.h"
#include "helpers/weapons.h"

Tower::Tower(const QPoint& point)
    : GameObject(point) {
  picture_ = PixmapLoader::GetTower();
}

void Tower::TakeDamage(int damage) {
  health = std::max(0, health - damage);
}

bool Tower::IsDestroyed() const {
  return health == 0;
}
