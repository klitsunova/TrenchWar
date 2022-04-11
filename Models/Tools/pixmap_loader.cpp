#include "pixmap_loader.h"

const QString PixmapLoader::soldier_path =
    ":././Resources/Images/Soldier1.png";
const QString PixmapLoader::left_soldier_path =
    ":././Resources/Images/Soldier2.png";
const QString PixmapLoader::right_soldier_path =
    ":././Resources/Images/Soldier1.png";
const QString PixmapLoader::terrain_object_path =
    ":././Resources/Images/Tower2.png";
std::map<QString, std::shared_ptr<QPixmap>> PixmapLoader::images;

void PixmapLoader::LoadPixmap(const QString& path) {
  images[path] = (std::make_shared<QPixmap>(QPixmap(path)));
  assert(!images[path]->isNull());
}

const std::shared_ptr<QPixmap>& PixmapLoader::GetPixmap(const QString& path) {
  if (images.find(path) == images.end()) {
    images[path] = (std::make_shared<QPixmap>(QPixmap(path)));
    assert(!images[path]->isNull());
  }
  return images[path];
}

const std::shared_ptr<QPixmap>& PixmapLoader::GetSoldier() {
  return GetPixmap(soldier_path);
}

const std::shared_ptr<QPixmap>& PixmapLoader::GetLSoldier() {
  return GetPixmap(left_soldier_path);
}

const std::shared_ptr<QPixmap>& PixmapLoader::GetRSoldier() {
  return GetPixmap(right_soldier_path);
}

const std::shared_ptr<QPixmap>& PixmapLoader::GetTerrainObject() {
  return GetPixmap(terrain_object_path);
}
