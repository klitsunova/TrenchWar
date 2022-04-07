#include "terrain_object.h"

TerrainObject::TerrainObject() : GameObject(":Resources/Images/Tower1.png") {
}

TerrainObject::TerrainObject(const QPoint& point)
    : GameObject(point, ":Resources/Images/Tower1.png") {
}
