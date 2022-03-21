#include <random>

#include "game_object.h"

GameObject::GameObject() : position_(std::make_pair(0, 0)) {
}

GameObject::GameObject(const GameObject::Point& position) {
  position_.first = position.first;
  position_.second = position.second;
}

GameObject::Point GameObject::GetPosition() const {
  return position_;
}

void GameObject::SetPosition(const GameObject::Point& position) {
  position_.first = position.first;
  position_.second = position.second;
}

int GetRandomNumber() {
  std::random_device rd;
  std::mt19937 mt(rd());
  std::uniform_real_distribution<double> dist(0.0, 100.0);
  return dist(mt);
}

void GameObject::SetRandomPosition() {
  position_.first = GetRandomNumber();
  position_.second = GetRandomNumber();
}

void GameObject::MoveLeft() {
  --position_.first;
}

void GameObject::MoveRight() {
  ++position_.first;
}

void GameObject::MoveUp() {
  ++position_.second;
}

void GameObject::MoveDown() {
  --position_.second;
}

QColor GameObject::GetColor() {
  return color_;
}
