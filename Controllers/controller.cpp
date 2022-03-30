#include "controller.h"

#include <memory>

Controller::Controller() : view_(std::make_unique<View>()),
                           world_(std::make_unique<World>(200, 50)),
                           timer_(new QBasicTimer) {
  for (int i = 0; i < 100; ++i) {
    world_->AddSoldier();
  }
  world_->AddTerraintbject();
  timer_->start(50, this);
}

void Controller::paintEvent(QPaintEvent* event) {
  QPainter qp(this);
  world_->DrawMap(&qp);
}

void Controller::timerEvent(QTimerEvent* event) {
  Q_UNUSED(event);
  for (const auto& soldier: world_->GetSoldiers()) {
    soldier->MoveSoldier(world_->GetWidth(), world_->GetHeight());
  }
  world_->UpdateSoldiersInCells();
  update();
}
