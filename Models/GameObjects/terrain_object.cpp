#include "terrain_object.h"

TerrainObject::TerrainObject()
    : GameObject() {
  picture_ = PixmapLoader::GetTerrainObject();
}

TerrainObject::TerrainObject(const QPoint& point)
    : GameObject(point) {
  picture_ = PixmapLoader::GetTerrainObject();
}
