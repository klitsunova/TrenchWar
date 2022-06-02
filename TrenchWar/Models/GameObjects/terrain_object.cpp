#include "terrain_object.h"

TerrainObject::TerrainObject()
    : GameObject() {
  picture_ = PixmapLoader::GetTower();
}

TerrainObject::TerrainObject(const QPoint& point)
    : GameObject(point) {
  picture_ = PixmapLoader::GetTower();
}
