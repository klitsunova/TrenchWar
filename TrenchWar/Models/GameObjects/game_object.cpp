#include "game_object.h"

#include <random>

namespace {

int GetRandomNumber() {
  static std::random_device rd;
  static std::mt19937 mt(rd());
  static std::uniform_real_distribution<double> dist(0.0, 100.0);
  return dist(mt);
}

} // namespace

GameObject::GameObject() {}

GameObject::GameObject(const QPoint& position)
    : position_(position) {}

const QPoint& GameObject::GetPosition() const {
  return position_;
}

void GameObject::SetPosition(const QPoint& position) {
  position_ = position;
}

void GameObject::SetRandomPosition(const QSize& size) {
  // temporary code
  position_ = QPoint(std::rand() % size.width(),
                     std::rand() % size.height());
}

void GameObject::MoveLeft() {
  position_ -= QPoint(kStep, 0);
}

void GameObject::MoveRight() {
  position_ += QPoint(kStep, 0);
}

void GameObject::MoveUp() {
  position_ -= QPoint(0, kStep);
}

void GameObject::MoveDown() {
  position_ += QPoint(0, kStep);
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
