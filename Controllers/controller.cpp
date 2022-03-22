#include "controller.h"

Controller::Controller() {
  view_ = new View();
  soldier_ = new Soldier();
  terrain_object_ = new TerrainObject();
  soldier_->SetRandomPosition();
  terrain_object_->SetRandomPosition();
  objects_.push_back(soldier_);
  objects_.push_back(terrain_object_);
}

Controller::~Controller() {
  delete view_;
  delete soldier_;
  delete terrain_object_;
}

void Controller::Tick() {
  soldier_->MoveSoldier();
}

void Controller::paintEvent(QPaintEvent* event) {
  QPainter qp(this);
  view_->Update(&qp, objects_);
}

std::vector<GameObject*> Controller::GetObjects() const {
  return objects_;
}
