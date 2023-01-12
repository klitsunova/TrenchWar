#include "bullet.h"

Bullet::Bullet(const QPoint& from, const QPoint& to,
               Side side, int damage)
    : AirMovingObject(side, from, to),
      damage_(damage) {
  size_ = image_sizes::kBulletImage;
  picture_ = PixmapLoader::GetBullet();
}

int Bullet::GetDamage() const {
  return damage_;
}
