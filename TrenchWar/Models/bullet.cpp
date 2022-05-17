#include "bullet.h"

Bullet::Bullet(const QPoint& from, const QPoint& to, int damage)
    : from_(from),
      to_(to),
      damage_(damage),
      position_(from) {
  picture_ = PixmapLoader::GetBullet();
}

const QPoint& Bullet::GetPosition() const {
  return position_;
}

Equipment::EquipmentType Bullet::GetType() const {
  return Equipment::EquipmentType::Weapon;
}

void Bullet::SetPosition(const QPoint& position) {
  position_ = position;
}

const QPoint& Bullet::GetFromPosition() const {
  return from_;
}

QPoint& Bullet::GetFromPosition() {
  return from_;
}

const QPoint& Bullet::GetToPosition() const {
  return to_;
}

QPoint& Bullet::GetToPosition() {
  return to_;
}

int Bullet::GetDamage() {
  return damage_;
}

const QPixmap& Bullet::GetPixmap() const {
  return *picture_;
}

void Bullet::Move() {
  if (to_ == position_) {
    return;
  }

  int square_dx = (to_.x() - from_.x()) * (to_.x() - from_.x());
  int square_dy = (to_.y() - from_.y()) * (to_.y() - from_.y());
  int t = 1;
  if ((position_.x() > to_.x()) || (position_.y() > to_.y())) {
    t *= -1;
  }
  moving_progress += t;
  int x = from_.x()
      + moving_progress
          * sqrt(square_dx / static_cast<double>(square_dx + square_dy));
  int y = from_.y()
      + moving_progress
          * sqrt(square_dy / static_cast<double>(square_dx + square_dy));
  position_ = QPoint(x, y);
}
