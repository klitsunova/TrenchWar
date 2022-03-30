#include "game_object.h"

#include <random>

namespace {

int GetRandomNumber() {
  static std::random_device rd;
  static std::mt19937 mt(rd());
  static std::uniform_real_distribution<double> dist(0.0, 100.0);
  return dist(mt);
}

}  //  namespace

GameObject::GameObject() {}
GameObject::GameObject(int x_position, int y_position)
    : x_position_(x_position),
      y_position_(y_position) {}
int GameObject::GetXPosition() const {
  return x_position_;
}
int GameObject::GetYPosition() const {
  return y_position_;
}

void GameObject::SetColor(QColor color) {
  color_ = color;
}

void GameObject::SetPosition(int x, int y) {
  x_position_ = x;
  y_position_ = y;
}

void GameObject::SetRandomPosition(int x_max, int y_max) {
  x_position_ = GetRandomNumber() % x_max;
  y_position_ = GetRandomNumber() % y_max;
}

void GameObject::MoveLeft() {
  --x_position_;
}

void GameObject::MoveRight() {
  ++x_position_;
}

void GameObject::MoveUp() {
  ++y_position_;
}

void GameObject::MoveDown() {
  --y_position_;
}
