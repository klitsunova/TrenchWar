#include "terrain_object.h"

TerrainObject::TerrainObject() : GameObject() {
  picture_ = QPixmap(":Resources/Images/Tower1.png");
}

TerrainObject::TerrainObject(const QPoint& point)
    : GameObject(point) {
  picture_ = QPixmap(":Resources/Images/Tower1.png");
}
