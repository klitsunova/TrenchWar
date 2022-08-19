#include "game_object.h"

#include <random>

GameObject::GameObject(const QPoint& position)
    : position_(position) {}

const QPoint& GameObject::GetPosition() const {
  return position_;
}

void GameObject::SetPosition(const QPoint& position) {
  position_ = position;
}

void GameObject::SetRandomPosition(const QRect& field) {
  position_ = RandomGenerator::GetRandomPoint(field);
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

int GameObject::GetTimeLag() const {
  return time_lag_;
}

void GameObject::SetTimeLag(int lag) {
  assert(lag >= 0);
  time_lag_ = lag;
}

void GameObject::MakeTick() {
  time_lag_ = std::max(0, time_lag_ - 1);
}
