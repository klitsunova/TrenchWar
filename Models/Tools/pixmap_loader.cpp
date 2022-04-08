#include "pixmap_loader.h"

PixmapLoader::PixmapLoader() {
  soldier_path_ = ":././Resources/Images/Soldier1.png";
  LoadPixmap(soldier_path_);
  // left_soldier_path_ = ":././Resources/Images/Soldier2.png";
  // LoadPixmap(left_soldier_path_);
  right_soldier_path_ = ":././Resources/Images/Soldier1.png";
  LoadPixmap(right_soldier_path_);
  terrain_object_path_ = ":././Resources/Images/Tower1.png";
  LoadPixmap(terrain_object_path_);
}

void PixmapLoader::LoadPixmap(const QString& path) {
  images_[path] = (std::make_shared<QPixmap>(QPixmap(path)));
  assert(!images_[path]->isNull());
}

const std::shared_ptr<QPixmap>& PixmapLoader::GetPixmap(const QString& path) {
  if (images_.find(path) == images_.end()) {
    images_[path] = (std::make_shared<QPixmap>(QPixmap(path)));
    assert(!images_[path]->isNull());
  }
  return images_[path];
}

const std::shared_ptr<QPixmap>& PixmapLoader::GetSoldier() {
  return GetPixmap(soldier_path_);
}

const std::shared_ptr<QPixmap>& PixmapLoader::GetLSoldier() {
  return GetPixmap(left_soldier_path_);
}

const std::shared_ptr<QPixmap>& PixmapLoader::GetRSoldier() {
  return GetPixmap(right_soldier_path_);
}

const std::shared_ptr<QPixmap>& PixmapLoader::GetTerrainObject() {
  return GetPixmap(terrain_object_path_);
}
