#include "pixmap_loader.h"

const QString PixmapLoader::soldier_path =
    ":Resources/Images/Soldier1.png";
const QString PixmapLoader::left_soldier_path =
    ":Resources/Images/Soldier2.png";
const QString PixmapLoader::right_soldier_path =
    ":Resources/Images/Soldier1.png";
const QString PixmapLoader::tower_path =
    ":Resources/Images/Tower2.png";
const QString PixmapLoader::bullet_path =
    ":Resources/Images/Bullet1.png";
const QString PixmapLoader::menu_background_path =
    ":Resources/Images/MenuBackground.jpg";
const QString PixmapLoader::menu_title_path =
    ":Resources/Images/MenuTitle.png";
const QString PixmapLoader::dollar_path =
    ":Resources/Images/Dollar.png";

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

const std::shared_ptr<QPixmap>& PixmapLoader::GetTower() {
  return GetPixmap(tower_path);
}

const std::shared_ptr<QPixmap>& PixmapLoader::GetBullet() {
  return GetPixmap(bullet_path);
}

const std::shared_ptr<QPixmap>& PixmapLoader::GetMenuBackground() {
  return GetPixmap(menu_background_path);
}

const std::shared_ptr<QPixmap>& PixmapLoader::GetMenuTitle() {
  return GetPixmap(menu_title_path);
}

const std::shared_ptr<QPixmap>& PixmapLoader::GetDollar() {
  return GetPixmap(dollar_path);
}
