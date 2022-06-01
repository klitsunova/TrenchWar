#include "bullet.h"

Bullet::Bullet(const QPoint& from, const QPoint& to,
               Side type, int damage)
    : GameObject(from),
      from_(from),
      to_(to),
      side_(type),
      damage_(damage) {
  size_ = image_sizes::kBulletImage;
  picture_ = PixmapLoader::GetBullet();
}

const QPoint& Bullet::GetPosition() const {
  return position_;
}

const QPoint& Bullet::GetFromPosition() const {
  return from_;
}

const QPoint& Bullet::GetToPosition() const {
  return to_;
}

int Bullet::GetDamage() const {
  return damage_;
}

Side Bullet::GetSide() const {
  return side_;
}

void Bullet::Move() {
  assert(!IsUsed());

  int64_t square_dx = (to_.x() - from_.x()) * (to_.x() - from_.x());
  int64_t square_dy = (to_.y() - from_.y()) * (to_.y() - from_.y());

  ++moving_progress_;

  int x, y;
  int rx = moving_progress_ *
      sqrt(square_dx / static_cast<double>(square_dx + square_dy));
  int ry = moving_progress_ *
      sqrt(square_dy / static_cast<double>(square_dx + square_dy));
  if (to_.x() > from_.x()) {
    x = from_.x() + rx;
  } else {
    x = from_.x() - rx;
  }
  if(to_.y() > from_.y()){
    y = from_.y() + ry;
  } else {
    y = from_.y() - ry;
  }

  position_ = QPoint(x, y);
  if (to_ == position_) {
    is_used_ = true;
  }
}

bool Bullet::IsUsed() const {
  return is_used_;
}

void Bullet::MakeUsed() {
  is_used_ = true;
}
