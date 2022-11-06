#include "game_object.h"

#include <random>

GameObject::GameObject(Side side, const QPoint& position)
    : position_(position), side_(side) {}

const QPoint& GameObject::GetPosition() const {
  return position_;
}

void GameObject::SetPosition(const QPoint& position) {
  position_ = position;
}

void GameObject::SetRandomPosition(const QRect& field) {
  position_ = utils::RandomGenerator::GetRandomPoint(field);
}

const QPixmap& GameObject::GetPixmap() const {
  return *picture_;
}

QSize GameObject::GetSize() const {
  return size_;
}

void GameObject::SetSize(const QSize& size) {
  size_ = size;
}

Side GameObject::GetSide() const {
  return side_;
}

void GameObject::TakeDamage(int damage) {
  health_ = std::max(0, health_ - damage);
}

bool GameObject::IsDead() const {
  assert(health_ >= 0);
  return health_ == 0;
}

int GameObject::GetHealth() const {
  return health_;
}
