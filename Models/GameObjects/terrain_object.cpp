#include "terrain_object.h"

TerrainObject::TerrainObject(const std::shared_ptr<PixmapLoader>& pixmap_loader)
    : GameObject(pixmap_loader) {
  picture_ = pixmap_loader->GetTerrainObject();
}

TerrainObject::TerrainObject(const QPoint& point,
                             const std::shared_ptr<PixmapLoader>& pixmap_loader)
    : GameObject(point, pixmap_loader) {
  picture_ = pixmap_loader->GetTerrainObject();
}
