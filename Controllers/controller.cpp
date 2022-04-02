#include "controller.h"

#include <memory>

Controller::Controller() : view_(std::make_unique<View>()),
                           world_(std::make_unique<World>(world_size_)),
                           timer_(new QBasicTimer) {
  for (int i = 0; i < 10; ++i) {
    world_->AddSoldier();
  }
  world_->AddTerrainObject();
  timer_->start(timer_interval_, this);
}

void Controller::paintEvent(QPaintEvent*) {
  QPainter qp(this);
  view_->Update(&qp, world_->GetGameObjects(), *world_);
}

void Controller::timerEvent(QTimerEvent*) {
  for (const auto& soldier : world_->GetSoldiers()) {
    soldier->MoveSoldier(world_->GetSize());
  }
  update();
}
