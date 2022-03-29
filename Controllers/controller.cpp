#include "controller.h"

#include <memory>

Controller::Controller() :
                           view_(std::make_unique<View>()),
                           world_(std::make_unique<World>(50, 40)),
                           timer_(new QBasicTimer) {
  world_->AddSoldier();
  timer_->start(100, this);
  // map_->AddTerraintbject();
}

void Controller::paintEvent(QPaintEvent* event) {
  QPainter qp(this);
  world_->DrawMap(&qp);
  // view_->Update(&qp, map_->GetObjects());
}

void Controller::timerEvent(QTimerEvent* event) {
  Q_UNUSED(event);
  for (const auto& soldier: world_->GetSoldiers()) {
    soldier->MoveSoldier(world_->GetWidth(), world_->GetHeight());
  }
  world_->UpdateSoldiersInCells();
  update();
}
