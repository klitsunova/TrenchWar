#include "controller.h"

#include <memory>

Controller::Controller() {
  pixmap_loader_ = std::make_shared<PixmapLoader>();
  world_ = std::make_shared<World>(kWorldSize, pixmap_loader_);
  view_ = std::make_unique<View>(world_);
  timer_ = std::make_unique<QBasicTimer>();
  for (int i = 0; i < 10; ++i) {
    world_->AddSoldier();
  }
  world_->AddTerrainObject();
  timer_->start(kTimerInterval, this);
}

void Controller::paintEvent(QPaintEvent*) {
  QPainter qp(this);
  view_->Update(&qp, world_->GetGameObjects());
}

void Controller::timerEvent(QTimerEvent*) {
  for (const auto& soldier : world_->GetSoldiers()) {
    soldier->MoveSoldier(world_->GetSize());
  }
  update();
}
