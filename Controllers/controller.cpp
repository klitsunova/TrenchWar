#include "controller.h"

#include <memory>

Controller::Controller() : timer_(new QBasicTimer),
                           view_(std::make_unique<View>()),
                           world_(std::make_unique<World>()) {
  timer_->start(15, this);
  world_->AddSoldier();
  world_->AddTerraintbject();
}

void Controller::paintEvent(QPaintEvent* event) {
  QPainter qp(this);
  view_->Update(&qp, world_->GetObjects());
}

void Controller::timerEvent(QTimerEvent*) {
  for (const auto& soldier : world_->GetSoldiers()) {
    soldier->MoveSoldier();
  }
  update();
}
