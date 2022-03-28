#include "controller.h"

#include <memory>

Controller::Controller() : timer_(new QBasicTimer),
                           view_(std::make_unique<View>()),
                           map_(std::make_unique<Map>()) {
  timer_->start(15, this);
  map_->AddSoldier();
  map_->AddTerraintbject();
}

void Controller::paintEvent(QPaintEvent* event) {
  QPainter qp(this);
  view_->Update(&qp, map_->GetObjects());
}

void Controller::timerEvent(QTimerEvent*) {
  for (const auto& soldier: map_->GetSoldiers()) {
    soldier->MoveSoldier();
  }
  repaint();
}
