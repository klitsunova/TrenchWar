#include "tower.h"
#include "helpers/weapons.h"

Tower::Tower(const QPoint& point, Side side)
    : GameObject(side, point) {
  picture_ = PixmapLoader::GetTower();
}
