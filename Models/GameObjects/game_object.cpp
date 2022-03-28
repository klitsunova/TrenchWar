#include "game_object.h"

#include <random>

namespace {
  int GetRandomNumber() {
    static std::random_device rd;
    static std::mt19937 mt(rd());
    static std::uniform_real_distribution<double> dist(0.0, 100.0);
    return dist(mt);
  }
}

GameObject::GameObject() {
  SetRandomPosition();
}

GameObject::GameObject(const QPoint& position) : position_(position) {
}

QPoint GameObject::GetPosition() const {
  return position_;
}

void GameObject::SetColor(QColor color) {
  color_ = color;
}

void GameObject::SetPosition(const QPoint& position) {
  position_ = position;
}

void GameObject::SetRandomPosition() {
  position_.setX(GetRandomNumber());
  position_.setY(GetRandomNumber());
}

void GameObject::MoveLeft() {
  position_ -= QPoint(1, 0);
}

void GameObject::MoveRight() {
  position_ += QPoint(1, 0);
}

void GameObject::MoveUp() {
  position_ += QPoint(0, 1);
}

void GameObject::MoveDown() {
  position_ -= QPoint(0, 1);
}
