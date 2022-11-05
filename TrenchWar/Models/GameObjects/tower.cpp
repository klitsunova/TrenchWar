#include "tower.h"
#include "helpers/weapons.h"

Tower::Tower(const QPoint& point)
    : GameObject(point) {
  picture_ = PixmapLoader::GetTower();
}
