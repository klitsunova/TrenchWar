#include "controller.h"

Controller::Controller() : timer_(new QBasicTimer) {
  view_ = new View();
  soldier_ = new Soldier();
  terrain_object_ = new TerrainObject();
  soldier_->SetRandomPosition();
  terrain_object_->SetRandomPosition();
  objects_.push_back(soldier_);
  objects_.push_back(terrain_object_);
  timer_->start(15, this);
}

Controller::~Controller() {
  delete view_;
  delete soldier_;
  delete terrain_object_;
}

void Controller::paintEvent(QPaintEvent* event) {
  QPainter qp(this);
  view_->Update(&qp, objects_);
}

std::vector<GameObject*> Controller::GetObjects() const {
  return objects_;
}

void Controller::timerEvent(QTimerEvent*) {
  soldier_->MoveSoldier();
  repaint();
}
