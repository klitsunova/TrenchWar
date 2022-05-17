#include "bullet.h"

Bullet::Bullet(const QPoint& from, const QPoint& to, int damage)
    : GameObject(from),
      from_(from),
      to_(to),
      damage_(damage) {
  size_ = QSize(7, 7);
  picture_ = PixmapLoader::GetBullet();
}

const QPoint& Bullet::GetPosition() const {
  return position_;
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

  if ((position_.x() > to_.x()) || (position_.y() > to_.y())) {
    --moving_progress;
  } else {
    ++moving_progress;
  }
  int x = from_.x()
      + moving_progress
          * sqrt(square_dx / static_cast<double>(square_dx + square_dy));
  int y = from_.y()
      + moving_progress
          * sqrt(square_dy / static_cast<double>(square_dx + square_dy));
  position_ = QPoint(x, y);
}
