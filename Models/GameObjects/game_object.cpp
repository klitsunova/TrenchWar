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

GameObject::GameObject(const QString& image_path)
    : picture_(image_path) {}

GameObject::GameObject(const QPoint& position, const QString& image_path)
    : position_(position),
      picture_(image_path) {}

const QPoint& GameObject::GetPosition() const {
  return position_;
}

void GameObject::SetPosition(const QPoint& position) {
  position_ = position;
}

void GameObject::SetRandomPosition(const QSize& size) {
  position_ = QPoint(std::rand() % size.width(),
                     std::rand() % size.height());
  // temporary code
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
QSize GameObject::GetSize() const {
  return size_;
}
void GameObject::SetSize(const QSize& size) {
  size_ = size;
}
