#include "bullet.h"

Bullet::Bullet(const QPoint& from, const QPoint& to,
               Soldier::Type type, int damage)
    : GameObject(from),
      from_(from),
      to_(to),
      type_(type),
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

Soldier::Type Bullet::GetType() const {
  return type_;
}

void Bullet::Move() {
  assert(!IsUsed());

  int square_dx = (to_.x() - from_.x()) * (to_.x() - from_.x());
  int square_dy = (to_.y() - from_.y()) * (to_.y() - from_.y());

  moving_progress_ = abs(moving_progress_) + 1;
  if ((position_.x() > to_.x()) || (position_.y() > to_.y())) {
    moving_progress_ *= -1;
  }
  int x = from_.x()
      + moving_progress_
          * sqrt(square_dx / static_cast<double>(square_dx + square_dy));
  int y = from_.y()
      + moving_progress_
          * sqrt(square_dy / static_cast<double>(square_dx + square_dy));
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
