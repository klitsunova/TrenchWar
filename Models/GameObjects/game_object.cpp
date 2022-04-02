#include "game_object.h"

#include <random>

namespace {

int GetRandomNumber() {
  static std::random_device rd;
  static std::mt19937 mt(rd());
  static std::uniform_real_distribution<double> dist(0.0, 100.0);
  return dist(mt);
}

}  // namespace

GameObject::GameObject() {}

GameObject::GameObject(const QPoint& position)
    : position_(position) {}

int GameObject::GetXPosition() const {
  return position_.x();
}

int GameObject::GetYPosition() const {
  return position_.y();
}

QPoint GameObject::GetPosition() const {
  return position_;
}

void GameObject::SetPosition(const QPoint& position) {
  position_ = position;
}

void GameObject::SetRandomPosition(const QSize& size) {
  position_ = QPoint(std::rand() % size.width(),
                     std::rand() % size.height());
}

void GameObject::MoveLeft() {
  position_ -= QPoint(step_, 0);
}

void GameObject::MoveRight() {
  position_ += QPoint(step_, 0);
}

void GameObject::MoveUp() {
  position_ -= QPoint(0, step_);
}

void GameObject::MoveDown() {
  position_ += QPoint(0, step_);
}

QPixmap GameObject::GetPixmap() const {
  return picture_;
}
